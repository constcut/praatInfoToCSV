#ifndef WATCHDOG_HPP
#define WATCHDOG_HPP

#include <QObject>
#include <QFile>
#include <QTimer>


namespace pitc {


    class WatchDog : public QObject
    {
        Q_OBJECT

    public:
        WatchDog() = default;


    };


}

#endif // WATCHDOG_HPP
