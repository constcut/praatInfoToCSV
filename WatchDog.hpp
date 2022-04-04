#ifndef WATCHDOG_HPP
#define WATCHDOG_HPP

#include <QObject>

#include <QTimer>


namespace pitc {


    class WatchDog : public QObject
    {
        Q_OBJECT

    public:
        WatchDog();

        void startTimer();

        //load configuration

    public slots:

        void explore();

    signals:

    private:

        void checkLine(const QString& line);

        QTimer _timer;

        QStringList _starters;
        QStringList _enders;
        QStringList _csvNames;
    };


}

#endif // WATCHDOG_HPP
