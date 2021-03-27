
#include <iostream>
#include <QNetworkDatagram>
#include <QTimer>
#include "client.hpp"
#include "config.hpp"
#include "utility.hpp"

/*! \class Client
 *	\brief forward local CS IPv4 client <--> CSv6 IPv6 internet server
 */

struct Client::ClientMapping
{
	QUdpSocket socketIPv6;
	std::uint16_t portIPv4;
	explicit ClientMapping(
		const std::uint16_t portIPv4)
		: portIPv4(portIPv4)
	{
	}
};

Client::Client(
	const QHostAddress& serverAddressIPv6
	, const std::uint16_t portIPv6
	, const std::uint16_t portIPv4
	, QObject* const parent /* = nullptr */)
	: QObject(parent)
	, m_serverAddressIPv6(serverAddressIPv6)
	, m_portIPv6(portIPv6)
	, m_dynamicPortIPv6(portIPv6) // to be incremented, see general note (2)
{
    QTimer::singleShot(100, this, [=]
    {
        initialize(portIPv4);
    });
}

Client::~Client() = default;

void Client::initialize(const std::uint16_t portIPv4)
{
    if (m_serverAddressIPv6.isNull())
    {
        emit errorMessage("No IPv6 Address");
        return;
    }

    std::cout
        << "[client] server: " << m_serverAddressIPv6 << ":" << m_portIPv6
        << std::endl;

    { // setup local IPv4 server
    if (m_socketIPv4.bind(LocalHostIPv4, portIPv4, BindMode))
    {
        connect(&m_socketIPv4, &QIODevice::readyRead, this, &Client::readFromIPv4Client);
        std::cout
            << "[client] listening on: " << Endpoint{ LocalHostIPv4, portIPv4 }
            << std::endl;
    }
    else
    {
        const auto error = SocketError{ "[client] IPv4", m_socketIPv4 };
        std::cout << error << std::endl;
        emit errorMessage(error.toString());
    }
    }
}

/*!	\note we assume \a localhost only
 *	\note see general note (1)
 */
Client::ClientMapping* Client::mapClient(
	const std::uint16_t portIPv4)
{
	auto& mapping = m_clientMap[portIPv4];

	if (Q_UNLIKELY(!mapping))
	{
		std::cout
			<< "[client] connection from: " << Endpoint{ LocalHostIPv4, portIPv4 }
			<< std::endl;
		mapping = std::make_shared<ClientMapping>(portIPv4);

		auto& socketIPv6 = mapping->socketIPv6;
		bool ok = false;

		for (int i = 0; i != MaxBindRetryCount; ++i)
		{
			if (socketIPv6.bind(
				AnyIPv6
				, ++m_dynamicPortIPv6 // see general note (2)
				, BindMode))
			{
				ok = true;
				break;
			}
            const auto error = SocketError{ "[client] IPv6", socketIPv6 };
            std::cout << error << std::endl;
            socketIPv6.close(); // else may be "half open" (?)
		}
		if (ok)
        {
			connect(&socketIPv6, &QIODevice::readyRead, this, [this, mapping]
			{
				readFromIPv6Server(&*mapping);
            });
		}
		else
		{	// maybe retry later
			mapping = {};
            emit errorMessage("mapping error, maybe retry later");
		}
	}
	return &*mapping;
}

/*!	forward to local IPv4 client
 */
void Client::readFromIPv6Server(
	ClientMapping* const mapping)
{
	auto& socketIPv6 = mapping->socketIPv6;
	const auto portIPv4 = mapping->portIPv4;

    while (socketIPv6.state() == QAbstractSocket::BoundState
        && socketIPv6.hasPendingDatagrams())
	{
		const auto datagram = socketIPv6.receiveDatagram();
		LOG_RECEIVED_DGRAM("[client]", datagram);
		const auto n = m_socketIPv4.writeDatagram(
			datagram.data()
			, LocalHostIPv4
			, portIPv4);

		if (n == -1)
		{
            const auto error = SocketError{ "[client] IPv4", m_socketIPv4 };
            std::cout << error << std::endl;
            emit errorMessage(error.toString());
		}
    }
}

/*!	forward to IPv6 server
 */
void Client::readFromIPv4Client()
{
    while (m_socketIPv4.state() == QAbstractSocket::BoundState
        && m_socketIPv4.hasPendingDatagrams())
	{
		const auto datagram = m_socketIPv4.receiveDatagram();
		LOG_RECEIVED_DGRAM("[client]", datagram);
		const auto portIPv4 = static_cast<std::uint16_t>(datagram.senderPort());
		auto* mapping = mapClient(portIPv4);

        if (mapping)
		{
			auto& socketIPv6 = mapping->socketIPv6;
			const auto n = socketIPv6.writeDatagram(
				datagram.data()
				, m_serverAddressIPv6
				, m_portIPv6);

			if (n == -1)
			{
                const auto error = SocketError{ "[client] IPv6", socketIPv6 };
                std::cout << error << std::endl;
                emit errorMessage(error.toString());
			}
        }
        else
        {
            return;
        }
	}
    if (m_socketIPv4.state() != QAbstractSocket::BoundState)
    {
        emit errorMessage("IPv4 connection lost");
    }
}
