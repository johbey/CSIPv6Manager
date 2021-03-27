
#include <iostream>
#include <QNetworkDatagram>
#include <QTimer>
#include "server.hpp"
#include "config.hpp"
#include "utility.hpp"

/*! \class Server
 *	\brief forward local CS IPv4 server <--> CSv6 IPv6 internet client
 */

struct Server::ClientMapping
{
	QUdpSocket socketIPv4;
	QHostAddress addressIPv6;
	std::uint16_t portIPv6;
	ClientMapping(
		const QHostAddress& addressIPv6
		, const std::uint16_t portIPv6)
		: addressIPv6(addressIPv6)
		, portIPv6(portIPv6)
	{
	}
};

Server::Server(
	const std::uint16_t portIPv6
	, const std::uint16_t portIPv4
	, QObject* const parent /* = nullptr */)
	: QObject(parent)
	, m_portIPv4(portIPv4)
	, m_dynamicPortIPv4(portIPv6) // to be incremented, see general note (2)
{

    QTimer::singleShot(100, this, [=]
    {
        initialize(portIPv6);
    });

}

Server::~Server()
{
}

void Server::initialize(const std::uint16_t portIPv6)
{
    { // setup IPv6 server
    if (m_socketIPv6.bind(AnyIPv6, portIPv6, BindMode))
    {
        connect(&m_socketIPv6,
                &QIODevice::readyRead,
                this,
                &Server::readFromIPv6Client);
        std::cout
            << "[server] listening on " << Endpoint{ AnyIPv6, portIPv6 }
            << std::endl;
    }
    else
    {
        const auto error = SocketError{ "[server] IPv6", m_socketIPv6 };
        std::cout << error << std::endl;
        emit errorMessage(error.toString());
    }
    }
}

/*!	forward to local IPv4 server
 */
void Server::readFromIPv6Client()
{
    while (m_socketIPv6.state() == QAbstractSocket::BoundState
        && m_socketIPv6.hasPendingDatagrams())
	{
		const auto datagram = m_socketIPv6.receiveDatagram();
		const auto addressIPv6 = datagram.senderAddress();
		const auto portIPv6 = static_cast<std::uint16_t>(datagram.senderPort());
		LOG_RECEIVED_DGRAM("[server]", datagram);
		auto* mapping = mapClient(addressIPv6, portIPv6);

        if (mapping != nullptr)
		{
			auto& socketIPv4 = mapping->socketIPv4;
			const auto n = socketIPv4.writeDatagram(
				datagram.data()
				, LocalHostIPv4
				, m_portIPv4);

			if (n == -1)
			{
                const auto error = SocketError{ "[server] IPv4", socketIPv4 };
                std::cout << error << std::endl;
                emit errorMessage(error.toString());
			}
        }
    }
}

/*!	forward to IPv6 client
 */
void Server::readFromIPv4Server(
	ClientMapping* const mapping)
{
	auto& socketIPv4 = mapping->socketIPv4;
	const auto& addressIPv6 = mapping->addressIPv6;
	const auto portIPv6 = mapping->portIPv6;

    while (m_socketIPv6.state() == QAbstractSocket::BoundState
        && socketIPv4.hasPendingDatagrams())
	{
		const auto datagram = socketIPv4.receiveDatagram();
		LOG_RECEIVED_DGRAM("[server]", datagram);
		const auto n = m_socketIPv6.writeDatagram(
			datagram.data()
			, addressIPv6
			, portIPv6);

		if (n == -1)
		{
            const auto error = SocketError{ "[server] IPv6", m_socketIPv6 };
            std::cout << error << std::endl;
            emit errorMessage(error.toString());
		}
	}
}

/*!	\note see general note (1)
 */
Server::ClientMapping* Server::mapClient(
	const QHostAddress& addressIPv6
	, const std::uint16_t portIPv6)
{
	if (m_clientMap.size() == MaxConnectionCount)
	{
        const QString error("[server] error: max connection count reached");
		std::cout
            << error
			<< std::endl;
        emit errorMessage(error);
		return nullptr;
	}

	auto& mapping = m_clientMap[{ addressIPv6, portIPv6 }];

	if (Q_UNLIKELY(!mapping))
	{
		std::cout
			<< "[server] connection from: " << Endpoint{ addressIPv6, portIPv6 }
			<< std::endl;
		mapping = std::make_shared<ClientMapping>(addressIPv6, portIPv6);

        emit connectionMapped(addressIPv6.toString());

		auto& socketIPv4 = mapping->socketIPv4;

		if (socketIPv4.bind(
			LocalHostIPv4
			, ++m_dynamicPortIPv4 // see general note (2)
			, BindMode))
		{
			connect(&socketIPv4, &QIODevice::readyRead, this, [this, mapping]
			{
				readFromIPv4Server(&*mapping);
			});
		}
		else
		{	// maybe retry later
            const auto error = SocketError{ "[server] IPv4", socketIPv4 };
            std::cout << error << std::endl;
            emit errorMessage(error.toString());
			mapping = {};
		}
	}
	return &*mapping;
}
