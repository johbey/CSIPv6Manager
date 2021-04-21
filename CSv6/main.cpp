
#include <iostream>
#include <optional>
#ifdef __linux__
#include <signal.h>
#include <sys/prctl.h>
#endif
#include <QCoreApplication>
#include <QCommandLineParser>
#include <QCommandLineOption>
#include <QHostAddress>
#include "client.hpp"
#include "server.hpp"
#include "config.hpp"
#include "utility.hpp"

/*!	general notes
 *	(1) the CS1.6 client uses at least 2 UDP client ports to communicate with the server:
 *	"query" and "play".
 *	(2) workaround: \a m_dynamicPort<protocol>
 *	random port from OS seems not to work correctly here (?).
 *	it may select a (maybe even shared with IPv4/6) port below 1024 (?!?)
 *	--> error "bound address already in use" or "address is protected".
 */

int main(int argc, char *argv[])
{

#ifdef __linux__
	// enable quit on CTRL-C when running in selinux sandbox.
	// by default CTRL-C will quit the sandbox process only
	// and leave \a this child process running.
	prctl(PR_SET_PDEATHSIG, SIGINT);
#endif

	QCoreApplication app{ argc, argv };
	QCommandLineParser parser;

	parser.setApplicationDescription(
		"CSv6 - enable CS1.6 internet server with DS-Lite"
		"\n#todo DOS protection"
		"\n#todo RCON support (TCP)");
	parser.addOption({ { "s", "server" }, "Server mode (default: client mode)." });
	parser.addOption({ { "p", "ipv6port" }, "The IPv6 port to use (default: 37015).", "port", "37015" });
	parser.addOption({ { "q", "ipv4port" }, "The IPv4 port to use (default: 27015).", "port", "27015" });
	parser.addHelpOption();
	parser.addPositionalArgument(
		"serverAddress"
		, "The IPv6 server address to connect to. Mandatory in client mode, ignored in server mode."
		, "[serverAddress]");
	parser.parse(QCoreApplication::arguments());
	if (parser.isSet("h"))
	{
		parser.showHelp(); // noreturn
	}

	std::optional<Client> client;
	std::optional<Server> server;
	const auto portIPv6 = static_cast<std::uint16_t>(parser.value("p").toUInt());
	const auto portIPv4 = static_cast<std::uint16_t>(parser.value("q").toUInt());

	if (parser.isSet("s"))
	{
		server.emplace(portIPv6, portIPv4);
	}
	else
	{
		const auto positionalArguments = parser.positionalArguments();

		if (positionalArguments.size() != 1)
		{
			parser.showHelp(1); // invalid command line // noreturn
		}
		client.emplace(QHostAddress{ positionalArguments[0] }, portIPv6, portIPv4);
	}	
	return app.exec();
}
