#include "WatchDog.hpp"

#include <QFile>
#include <QDebug>


using namespace pitc;


WatchDog::WatchDog()
{
    _timer.setInterval(1000);
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
    _timer.stop();

    QString filename = "C:\\Users\\constcut\\Desktop\\tests\\1\\info";
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

        //qDebug() << "Total lines count: " << linesCount;
        //qDebug() << "Last line: " << prevLine;

        if (linesCount == 1)
        {
            QString intesivity = prevLine.mid(0, prevLine.indexOf(" dB"));
            qDebug() << "Intensivity: " << intesivity;
        }
        else
        {
            qDebug() << "After explore: ";
            for (int i = 0; i < _starters.size(); ++i)
                qDebug() << _starters[i] << " " << _storedValues[i] << " " << i;
        }

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

            //Если % нужен вконце можем добавлять его тут
            qDebug() << i << " found " << found << " for " << start;
            return;
        }
    }
}