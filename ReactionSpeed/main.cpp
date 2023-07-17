#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <mainlogic.h>
#include <QQmlContext>

int main(int argc, char *argv[])
{

    QGuiApplication app(argc, argv);

    MainLogic mainLogic;

    QQmlApplicationEngine engine;
    engine.rootContext()->setContextProperty("mainLogic", &mainLogic);

    const QUrl url(u"qrc:/ReactionSpeed/Main.qml"_qs);
    QObject::connect(&engine, &QQmlApplicationEngine::objectCreationFailed,
        &app, []() { QCoreApplication::exit(-1); },
        Qt::QueuedConnection);
    engine.load(url);

    return app.exec();
}
