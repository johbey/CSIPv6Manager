
#ifndef CSV6_UTILITY_HPP
#define CSV6_UTILITY_HPP

#include <cstdint>
#include <iostream>
#include <QString>
#include <QHostAddress>
#include <QNetworkDatagram>
#include "config.hpp"

extern const QHostAddress LocalHostIPv4;
extern const QHostAddress LocalHostIPv6;
extern const QHostAddress AnyIPv6;

inline std::ostream& operator<<(std::ostream& os, const QString& s)
{
	return os << s.toStdString();
}

inline std::ostream& operator<<(std::ostream& os, const QHostAddress& a)
{
	return os << a.toString();
}

struct SocketError
{
	const char* const source;
	const QAbstractSocket& socket;
};

inline std::ostream& operator<<(std::ostream& os, const SocketError& e)
{
	const auto& s = e.socket;

	return os
		<< e.source
		<< " socket error: " << s.errorString()
		<< " (" << s.error() << ")";
}

struct Endpoint
{
	const QHostAddress& address;
	const std::uint16_t port;
};

inline std::ostream& operator<<(std::ostream& os, const Endpoint& ep)
{
	return os << ep.address << ":" << ep.port;
}

#ifdef LOG_RECEIVED_DGRAMS
inline void LOG_RECEIVED_DGRAM(
	const char* source
	, const QNetworkDatagram& dgram)
{
	const auto data = dgram.data();

	std::cout
		<< source
		<< " dgram from: " << Endpoint{
		   dgram.senderAddress()
		   , static_cast<std::uint16_t>(dgram.senderPort())
		}
		<< " size: " << data.size()
		<< std::endl;
}
#else
template <class ...Args> // noop
void LOG_RECEIVED_DGRAM(Args&& ...) {}
#endif

#endif // CSV6_UTILITY_HPP
