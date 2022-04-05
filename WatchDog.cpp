#include "WatchDog.hpp"

#include <QFile>
#include <QDebug>


using namespace pitc;


WatchDog::WatchDog()
{
    _timer.setInterval(_timerInterval);
    connect(&_timer, &QTimer::timeout, this, &WatchDog::explore);

    _starters << "duration:" << "Mean pitch:" << "Median pitch:" << "Number of pulses:"
              << "Fraction of locally unvoiced frames:" << "Jitter (local):" << "Shimmer (local):"
              << "Mean harmonics-to-noise ratio:";

    _enders << "seconds" << "Hz" << "Hz" << ""
            << "%" << "%" << "%" << "dB"; //Другой вариант до следующего пробела, если его нет - до конца

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
    QString filenameTxt = _infoDirectory + "/info.txt";
    checkFile(filename);
    checkFile(filenameTxt);
}


void WatchDog::checkFile(const QString& filename)
{
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

        if (linesCount <= 2)
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
        outStream << "Name, Date, DayTime, ";
        fillHeader(0, _breakFieldBorder, outStream);

        outStream << "Intensity, ";
        fillHeader(_breakFieldBorder, _starters.size(), outStream);
    }

    outStream << _name << ", " << _date << ", "
              << _dayTime << ", ";

    fillHeader(0, _breakFieldBorder, outStream);
    outStream << field << ", ";
    fillHeader(_breakFieldBorder, _storedValues.size(), outStream);
}


void WatchDog::fillHeader(int from, int to, QTextStream& outStream) const
{
    for (int i = from; i < to; ++i)
    {
        QString part = _starters[i].mid(0, _starters[i].size() - 1);
        outStream << part;
        if (i != _starters.size() - 1)
            outStream << ", ";
        else
            outStream << "\n";
    }
}


void WatchDog::fillValues(size_t from, size_t to, QTextStream &outStream) const
{
    for (size_t i = from; i < to; ++i)
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
