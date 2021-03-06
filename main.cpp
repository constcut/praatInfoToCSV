#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>

#include <QFont>
#include <QTextCodec>
#include <QDir>

#include "WatchDog.hpp"


int main(int argc, char *argv[])
{

#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
#endif

    QGuiApplication app(argc, argv);

    app.setApplicationName("PraatInfoToCSV");
    app.setOrganizationName("a");
    app.setOrganizationDomain("a.a");

    auto font = app.font();
    font.setPixelSize(14);
    app.setFont(font);
    QTextCodec::setCodecForLocale(QTextCodec::codecForName("UTF-8"));

    QQmlApplicationEngine engine;

    QDir dir;
    if (dir.exists("praatInfo") == false) {
        if (dir.mkdir("praatInfo") == false)
            qDebug() << "Failed to create records directory";
    }

    pitc::WatchDog watchDog;
    watchDog.loadFilesConfig();
    watchDog.loadAdditionalFields();

    engine.rootContext()->setContextProperty("watchDog", &watchDog);

    watchDog.startTimer();

    const QUrl url(QStringLiteral("qrc:/main.qml"));

    QObject::connect(&engine, &QQmlApplicationEngine::objectCreated,
                     &app, [url](QObject *obj, const QUrl &objUrl) {
        if (!obj && url == objUrl)
            QCoreApplication::exit(-1);
    }, Qt::QueuedConnection);


    engine.load(url);


    return app.exec();
}
