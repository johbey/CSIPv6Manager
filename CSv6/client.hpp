
#ifndef CSV6_CLIENT_HPP
#define CSV6_CLIENT_HPP

#include <cstdint>
#include <memory>
#include <QObject>
#include <QHostAddress>
#include <QUdpSocket>
#include <QHash>

class Client
	: public QObject
{
	Q_OBJECT

public:
	Client(
		const QHostAddress& serverAddressIPv6
		, std::uint16_t portIPv6
		, std::uint16_t portIPv4
		, QObject* parent = nullptr);
	~Client();

private:
	struct ClientMapping;

private:
	ClientMapping* mapClient(
		std::uint16_t portIPv4);
	void readFromIPv6Server(
		ClientMapping* mapping);
	void readFromIPv4Client();

private:
	const QHostAddress m_serverAddressIPv6;
	const std::uint16_t m_portIPv6;
	QUdpSocket m_socketIPv4;
	QHash<
		std::uint16_t // IPv4 port
		, std::shared_ptr<ClientMapping>
	> m_clientMap;
	std::uint16_t m_dynamicPortIPv6;
};

#endif // CSV6_CLIENT_HPP
