#ifndef WATCHDOG_HPP
#define WATCHDOG_HPP

#include <QObject>
#include <QTimer>

#include <vector>


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

        void notifyFilled(QStringList list);

    private:

        void checkLine(const QString& line);

        void resetStored();

        void dumpToFile(const QString& field);

        void sendToQML();

        QTimer _timer;

        QStringList _starters;
        QStringList _enders;
        QStringList _csvNames;

        QString _singleFieldCut = " dB";

        std::vector<QString> _storedValues;
    };


}

#endif // WATCHDOG_HPP
