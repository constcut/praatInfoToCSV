#ifndef WATCHDOG_HPP
#define WATCHDOG_HPP

#include <QObject>
#include <QTimer>
#include <QTextStream>

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
            saveFilesConfig();
        }

        Q_INVOKABLE void setInfoDirectory(QString dir) {
            _infoDirectory = dir;
            saveFilesConfig();
        }

        Q_INVOKABLE int getBreakBorder() {
            return _breakFieldBorder;
        }

        Q_INVOKABLE QString getInfoDir() {
            return _infoDirectory;
        }

        Q_INVOKABLE QString getCsvFile() {
            return _csvFilename;
        }

    public slots:

        void explore();

    signals:

        void notifyFilled(QStringList list);
        void notifyLastFilled(QString last);

    private:

        void saveFilesConfig();

        void checkFile(const QString& filename);
        void checkLine(const QString& line);

        void resetStored();

        void dumpToFile(const QString& field) const;
        void sendToQML();

        void fillHeader(int from, int to, QTextStream &outStream) const;
        void fillValues(size_t from, size_t to, QTextStream &outStream) const;

        QTimer _timer;

        const int _timerInterval = 500;

        const int _breakFieldBorder = 3;

        QStringList _starters;
        QStringList _enders;
        QStringList _csvNames;

        QString _singleFieldCut = " dB";

        std::vector<QString> _storedValues;

        QString _name = "Not set";
        QString _date = "Not set";
        QString _dayTime = "Not set";

        QString _csvFilename = "export.csv";
        QString _infoDirectory = "praatInfo";
    };


}

#endif // WATCHDOG_HPP
