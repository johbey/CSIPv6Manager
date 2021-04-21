#include "backend.h"

#include <QNetworkAccessManager>
#include <QEventLoop>
#include <QNetworkReply>
#include <QHostAddress>
#include <QProcess>

BackEnd::BackEnd(QObject *parent) :
    QObject(parent),
    m_ipv4Port(27015),
    m_ipv6Port(37015)
{
}

BackEnd::~BackEnd()
{
    disconnectClient();
    stopServer();
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

void BackEnd::setServerMessage(const QString& message)
{
    m_serverError = message;
    emit serverErrorChanged();
}
void BackEnd::readServerMessage()
{
    QByteArray error = m_server.get()->readAllStandardError();
    QByteArray output = m_server.get()->readAllStandardOutput();

    const int connectionIndex = output.indexOf("connection from: ") + 17;
    if (connectionIndex != 16)
    {
        const int portIndex = output.lastIndexOf(":", connectionIndex);
        addClient(output.mid(connectionIndex, portIndex-connectionIndex));
        return;
    }

    const int errorIndex = output.indexOf("error");
    if (errorIndex != -1
            || !error.isEmpty())
    {
        stopServer();
        setServerMessage(output+error);
    }
}

QString BackEnd::clientError()
{
    return m_clientError;
}

void BackEnd::setClientMessage(const QString& message)
{
    m_clientError = message;
    emit clientErrorChanged();
}

void BackEnd::readClientMessage()
{
    QByteArray error = m_client.get()->readAllStandardError();
    QByteArray output = m_client.get()->readAllStandardOutput();

    const int errorIndex = output.indexOf("error");
    if (errorIndex != -1
            || !error.isEmpty())
    {
        disconnectClient();
        setClientMessage(output + error);
    }


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

QStringList BackEnd::portArguments()
{
    QStringList result;
    result << "-p" << QString::number(ipv6Port());
    result << "-q" << QString::number(ipv4Port());
    return result;
}

void BackEnd::connectClient(const QString address)
{
    if (address.isEmpty())
    {
        setClientMessage("no ipv6 address!");
        return;
    }

    const QHostAddress serverAddress(address);
    m_client.reset(new QProcess());
    m_client.get()->start(
                "CSv6.exe",
                { "-p", QString::number(ipv6Port()), "-q", QString::number(ipv4Port()), serverAddress.toString() });
    connect(m_client.get(), &QProcess::readyReadStandardOutput, this, &BackEnd::readClientMessage);
    connect(m_client.get(), &QProcess::readyReadStandardError, this, &BackEnd::readClientMessage);

    m_clientError.clear();
    emit clientErrorChanged();
    emit clientConnected();
}

void BackEnd::disconnectClient()
{
    if (m_client != nullptr)
    {
        m_client->kill();
    }

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

    m_server.reset(new QProcess());
    m_server.get()->start(
                "CSv6.exe",
                { "-s", "-p", QString::number(ipv6Port()), "-q", QString::number(ipv4Port()) });
    connect(m_server.get(), &QProcess::readyReadStandardOutput, this, &BackEnd::readServerMessage);
    connect(m_server.get(), &QProcess::readyReadStandardError, this, &BackEnd::readServerMessage);

    m_serverError.clear();
    emit serverErrorChanged();
    emit serverStarted();
}

void BackEnd::stopServer()
{
    if (m_server != nullptr)
    {
        m_server->kill();
    }
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
