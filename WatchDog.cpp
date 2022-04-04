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
            << "%" << "%" << "%";
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

        QString line;
        int lineCount = 0;

        while (textStream.readLineInto(&line))
        {
            ++lineCount;
            checkLine(line);
            //Search for lines
        }

        qDebug() << "Total lines count: " << lineCount;
    }
}




void WatchDog::checkLine(const QString &line)
{
    for (int i = 0; i < _starters.size(); ++i)
    {
        const auto& start = _starters[i];
        const auto& end = _enders[i];

        int index = line.indexOf(start);
        if (index != -1)
        {
            const int posAfterStarter = index + start.size();

            QString found;
            if (end.isEmpty() == false)
            {
                int separator = line.indexOf(end);
                int len = separator - posAfterStarter;
                found = line.mid(posAfterStarter, len);

            }
            else
                found = line.mid(posAfterStarter);

            //Если % нужен вконце можем добавлять его тут
            qDebug() << i << " found " << found;
            return;
        }
    }
}
