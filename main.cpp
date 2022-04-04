#include <QGuiApplication>
#include <QQmlApplicationEngine>

#include <QFont>
#include <QTextCodec>


int main(int argc, char *argv[])
{

#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
#endif

    QGuiApplication app(argc, argv);

    auto font = app.font();
    font.setPixelSize(14);
    app.setFont(font);
    QTextCodec::setCodecForLocale(QTextCodec::codecForName("UTF-8"));

    QQmlApplicationEngine engine;

    //engine.rootContext()->setContextProperty("watchDog", &watchDog);

    const QUrl url(QStringLiteral("qrc:/main.qml"));

    QObject::connect(&engine, &QQmlApplicationEngine::objectCreated,
                     &app, [url](QObject *obj, const QUrl &objUrl) {
        if (!obj && url == objUrl)
            QCoreApplication::exit(-1);
    }, Qt::QueuedConnection);


    engine.load(url);


    return app.exec();
}
