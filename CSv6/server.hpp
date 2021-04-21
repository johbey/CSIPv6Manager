
#ifndef CSV6_SERVER_HPP
#define CSV6_SERVER_HPP

#include <cstdint>
#include <memory>
#include <QObject>
#include <QUdpSocket>
#include <QHash>

class Server
	: public QObject
{
	Q_OBJECT

public:
	Server(
		std::uint16_t portIPv6
		, std::uint16_t portIPv4
		, QObject *parent = nullptr);
	~Server();

private:
	struct ClientMapping;

private:
	ClientMapping* mapClient(
		const QHostAddress& addressIPv6
		, std::uint16_t portIPv6);
	void readFromIPv6Client();
	void readFromIPv4Server(
		ClientMapping* mapping);

private:
	QUdpSocket m_socketIPv6;
	QHash<
		QPair<QHostAddress, std::uint16_t> // IPv6
		, std::shared_ptr<ClientMapping>
	> m_clientMap;
	const std::uint16_t m_portIPv4;
	std::uint16_t m_dynamicPortIPv4;
};

#endif // CSV6_SERVER_HPP
