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

        Q_INVOKABLE void setName(QString name);
        Q_INVOKABLE void setDate(QString date);
        Q_INVOKABLE void setDayTime(QString dayTime);

        Q_INVOKABLE int totalFields() const {
            return _starters.size() + 1;
        }

        Q_INVOKABLE void setCSVfilename(QString fn) {
            _csvFilename = fn;
        }

        Q_INVOKABLE void setInfoDirectory(QString dir) {
            _infoDirectory = dir;
        }

    public slots:

        void explore();

    signals:

        void notifyFilled(QStringList list);
        void notifyLastFilled(QString last);

    private:

        void checkLine(const QString& line);

        void resetStored();

        void dumpToFile(const QString& field) const;
        void sendToQML();

        QTimer _timer;

        const int _timerInterval = 500;

        QStringList _starters;
        QStringList _enders;
        QStringList _csvNames;

        QString _singleFieldCut = " dB";

        std::vector<QString> _storedValues;

        QString _name;
        QString _date;
        QString _dayTime;

        QString _csvFilename;
        QString _infoDirectory;
    };


}

#endif // WATCHDOG_HPP
