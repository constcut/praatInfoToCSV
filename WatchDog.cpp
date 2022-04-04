#include "WatchDog.hpp"

#include <QFile>
#include <QDebug>


using namespace pitc;


WatchDog::WatchDog()
{
    _timer.setInterval(_timerInterval);
    connect(&_timer, &QTimer::timeout, this, &WatchDog::explore);

    _starters << "duration:" << "Median pitch:" << "Mean pitch:" << "Number of pulses:"
              << "Fraction of locally unvoiced frames:" << "Jitter (local):" << "Shimmer (local):";

    _enders << "seconds" << "Hz" << "Hz" << ""
            << "%" << "%" << "%"; //Другой вариант до следующего пробела, если его нет - до конца

    resetStored();
}


void WatchDog::resetStored() {
    _storedValues = std::vector<QString>(_starters.size());
}


void WatchDog::startTimer()
{
    _timer.start();
}


void WatchDog::explore()
{
    QString filename = _infoDirectory + "/info";
    bool exists = QFile::exists(filename);

    if (exists)
    {
        QFile file(filename);
        file.open(QFile::ReadOnly | QFile::Text);
        QTextStream textStream(&file);

        QString line, prevLine;
        int linesCount = 0;

        while (textStream.readLineInto(&line))
        {
            ++linesCount;
            prevLine = line;
            checkLine(line);
        }

        if (linesCount == 1)
        {
            QString intensity = prevLine.mid(0, prevLine.indexOf(" dB"));
            dumpToFile(intensity);

            emit notifyLastFilled(intensity);
        }
        else {
            sendToQML();
        }

        file.close();
        QFile::remove(filename);
    }
}


void WatchDog::sendToQML()
{
    QStringList valuesToSend;
    for (const auto& s: _storedValues)
        valuesToSend << s;

    emit notifyFilled(valuesToSend);
}



void WatchDog::dumpToFile(const QString& field) const
{
    QString filename = _csvFilename;

    bool existed = QFile::exists(filename);

    QFile file(filename);
    file.open(QFile::Append | QFile::Text);

    QTextStream outStream(&file);

    if (existed == false)
    {
        outStream << "Name, Date, DayTime, Intensity, ";
        for (int i = 0; i < _starters.size(); ++i)
        {
            QString part = _starters[i].mid(0, _starters[i].size() - 1);
            outStream << part;
            if (i != _starters.size() - 1)
                outStream << ", ";
            else
                outStream << "\n";
        }
    }

    outStream << _name << ", " << _date << ", "
              << _dayTime << ", " << field << ", ";

    for (size_t i = 0; i < _storedValues.size(); ++i)
    {
        outStream << _storedValues[i];

        if (i != _storedValues.size() - 1)
            outStream << ", ";
        else
            outStream << "\n";
    }

}


void WatchDog::checkLine(const QString &line)
{
    for (int i = 0; i < _starters.size(); ++i)
    {
        const auto& start = _starters[i];
        const auto& end = _enders[i];

        int startIndex = line.indexOf(start);
        if (startIndex != -1)
        {
            const int posAfterStarter = startIndex + start.size();

            QString found;
            if (end.isEmpty() == false)
            {
                int separator = line.indexOf(end, startIndex);
                int len = separator - posAfterStarter;
                found = line.mid(posAfterStarter, len);
            }
            else
                found = line.mid(posAfterStarter);

            found = found.trimmed();
            _storedValues[i] = found;
            return;
        }
    }
}


void WatchDog::setName(QString name)
{
    _name = name;
}

void WatchDog::setDate(QString date)
{
    _date = date;
}

void WatchDog::setDayTime(QString dayTime)
{
    _dayTime = dayTime;
}
