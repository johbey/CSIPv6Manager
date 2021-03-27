#include "backend.h"

#include "client.hpp"
#include "server.hpp"

#include <QNetworkAccessManager>
#include <QEventLoop>
#include <QNetworkReply>

BackEnd::BackEnd(QObject *parent) :
    QObject(parent),
    m_ipv4Port(27015),
    m_ipv6Port(37015)
{
}

BackEnd::~BackEnd()
{
}

int BackEnd::ipv4Port()
{
    return m_ipv4Port;
}

void BackEnd::setIpv4Port(int port)
{
    if (port == m_ipv4Port)
    {
        return;
    }

    m_ipv4Port = port;
    emit ipv4PortChanged();
}

int BackEnd::ipv6Port()
{
    return m_ipv6Port;
}

void BackEnd::setIpv6Port(int port)
{
    if (port == m_ipv6Port)
    {
        return;
    }

    m_ipv6Port = port;
    emit ipv6PortChanged();
}

QStringList BackEnd::clients()
{
    return m_connectedClients;
}

void BackEnd::setClients(const QStringList& list)
{
    if (list == m_connectedClients)
    {
        return;
    }

    m_connectedClients = list;
    emit clientsChanged();
}

void BackEnd::addClient(const QString& client)
{
    if (m_connectedClients.contains(client)
        || client.isEmpty())
    {
        return;
    }

    auto tempClients = m_connectedClients;
    tempClients.append(client);
    setClients(tempClients);
}

QString BackEnd::ipv6Address()
{
    return m_ipv6Address;
}

void BackEnd::setIpv6Address(const QString& address)
{
    if (address == m_ipv6Address)
    {
        return;
    }

    m_ipv6Address = address;
    emit ipv6AddressChanged();
}

QString BackEnd::serverError()
{
    return m_serverError;
}

void BackEnd::setServerError(const QString& message)
{
    if (message == m_serverError)
    {
        return;
    }

    if (!message.isEmpty())
    {
        stopServer();
    }

    m_serverError = message;
    emit serverErrorChanged();
}

QString BackEnd::clientError()
{
    return m_clientError;
}

void BackEnd::setClientError(const QString& message)
{
    if (message == m_clientError)
    {
        return;
    }

    if (!message.isEmpty())
    {
        disconnectClient();
    }

    m_clientError = message;
    emit clientErrorChanged();
}


void BackEnd::updateIpv6Address()
{
    QNetworkAccessManager manager;
    QNetworkReply *response = manager.get(QNetworkRequest(QUrl("http://v6.ident.me/")));
    QEventLoop event;
    connect(response,SIGNAL(finished()),&event,SLOT(quit()));
    event.exec();
    QString ipv6 = response->readAll();

    setIpv6Address(ipv6);
}

void BackEnd::connectClient(const QString address)
{
    const QHostAddress serverAddress(address);
    m_client.reset(new Client(
                serverAddress,
                static_cast<std::uint16_t>(ipv6Port()),
                static_cast<std::uint16_t>(ipv4Port())));
    connect(m_client.get(), &Client::errorMessage, this, &BackEnd::setClientError);
    setClientError({});
    emit clientConnected();
}

void BackEnd::disconnectClient()
{
    m_client.reset(nullptr);
    emit clientDisconnected();
}

void BackEnd::toggleClient(const QString address)
{
    if (m_client != nullptr)
    {
        disconnectClient();
    }
    else
    {
        connectClient(address);
    }
}

void BackEnd::startServer()
{
    updateIpv6Address();
    m_server.reset(new Server(
                static_cast<std::uint16_t>(ipv6Port()),
                static_cast<std::uint16_t>(ipv4Port())));
    connect(m_server.get(), &Server::connectionMapped, this, &BackEnd::addClient);
    connect(m_server.get(), &Server::errorMessage, this, &BackEnd::setServerError);
    setServerError({});
    emit serverStarted();
}

void BackEnd::stopServer()
{
    m_server.reset(nullptr);
    setClients({});
    setIpv6Address({});
    emit serverStopped();
}

void BackEnd::toggleServer()
{
    if (m_server != nullptr)
    {
        stopServer();
    }
    else
    {
        startServer();
    }
}
