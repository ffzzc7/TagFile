#ifndef _WIN32_WINNT		// 允许使用特定于 Windows XP 或更高版本的功能。
#define _WIN32_WINNT 0x0501	// 将此值更改为相应的值，以适用于 Windows 的其他版本。
#endif	

#include "MainWindow.h"
#include <QTextCodec>
#include <QtWidgets/QApplication>
#include <QDebug>
#include <QLabel>
#include <../Src/qtsingleapplication/qtsingleapplication.h>


int main(int argc, char *argv[])
{
    QtSingleApplication app(argc, argv);
    CMainWindow w;
    if (app.isRunning())
    {
        if (argc>1)
        {
            qDebug() << argv[1];
            app.sendMessage(QString::fromLocal8Bit(argv[1]));

        }
        return EXIT_SUCCESS;
    }
    QObject::connect(&app, SIGNAL(messageReceived(const QString&)), &w, SLOT(onRecv(const QString&)));
    w.show();
    if (argc > 1)
    {
        w.onRecv(QString::fromLocal8Bit(argv[1]));
    }
    return app.exec();
}
