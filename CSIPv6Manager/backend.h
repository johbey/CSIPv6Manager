#ifndef BACKEND_H
#define BACKEND_H

#include <QObject>
#include <memory>

class QProcess;

struct QObjectDeleteLater {
    void operator()(QObject *o) {
        o->deleteLater();
    }
};

class BackEnd : public QObject
{
    Q_OBJECT
    Q_PROPERTY(int ipv4Port READ ipv4Port WRITE setIpv4Port NOTIFY ipv4PortChanged)
    Q_PROPERTY(int ipv6Port READ ipv6Port WRITE setIpv6Port NOTIFY ipv6PortChanged)
    Q_PROPERTY(QStringList clients READ clients WRITE setClients NOTIFY clientsChanged)
    Q_PROPERTY(QString ipv6Address READ ipv6Address WRITE setIpv6Address NOTIFY ipv6AddressChanged)
    Q_PROPERTY(QString serverError READ serverError WRITE setServerMessage NOTIFY serverErrorChanged)
    Q_PROPERTY(QString clientError READ clientError WRITE setClientMessage NOTIFY clientErrorChanged)

public:
    explicit BackEnd(QObject *parent = nullptr);
    ~BackEnd();

    int ipv4Port();
    void setIpv4Port(int port);
    int ipv6Port();
    void setIpv6Port(int port);
    QStringList clients();
    void setClients(const QStringList& list);
    void addClient(const QString& client);
    QString ipv6Address();
    void setIpv6Address(const QString& address);
    QString serverError();
    void setServerMessage(const QString& message);
    void readServerMessage();
    QString clientError();
    void setClientMessage(const QString& message);
    void readClientMessage();

    void updateIpv6Address();
    QStringList portArguments();

public slots:
    void connectClient(QString address);
    void disconnectClient();
    void toggleClient(QString address);

    void startServer();
    void stopServer();
    void toggleServer();

signals:
    void ipv4PortChanged();
    void clientsChanged();
    void ipv6PortChanged();
    void ipv6AddressChanged();
    void serverErrorChanged();
    void clientErrorChanged();

    void clientConnected();
    void clientDisconnected();

    void serverStarted();
    void serverStopped();

private:
    int m_ipv4Port;
    int m_ipv6Port;
    QStringList m_connectedClients;
    QString m_ipv6Address;
    QString m_serverError;
    QString m_clientError;

    std::unique_ptr<QProcess, QObjectDeleteLater> m_client;
    std::unique_ptr<QProcess, QObjectDeleteLater> m_server;
};

#endif // BACKEND_H
