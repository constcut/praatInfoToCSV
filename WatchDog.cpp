#include "WatchDog.hpp"

#include <QFile>
#include <QDebug>


using namespace pitc;


WatchDog::WatchDog()
{
    _timer.setInterval(1000);
    connect(&_timer, &QTimer::timeout, this, &WatchDog::explore);
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
        }

        qDebug() << "Total lines count: " << lineCount;
    }
}
