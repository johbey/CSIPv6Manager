#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <optional>

#include "backend.h"

int main(int argc, char *argv[])
{
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);

    QGuiApplication app(argc, argv);

    BackEnd backEnd;

    QQmlApplicationEngine engine;
    engine.rootContext()->setContextProperty("backEnd", &backEnd);
    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));
    if (engine.rootObjects().isEmpty())
    {
        return -1;
    }

    return app.exec();
}
