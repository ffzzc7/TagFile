/*!
* \file main.cpp
* \date 2022/11/20 11:29
*
* \author ffzzc7
*
* \brief main函数
*
* TODO:
*
* \note
*/


#ifndef _WIN32_WINNT		// 允许使用特定于 Windows XP 或更高版本的功能。
#define _WIN32_WINNT 0x0501	// 将此值更改为相应的值，以适用于 Windows 的其他版本。
#endif	

#include <QTextCodec>
#include <QtWidgets/QApplication>
#include <QDebug>
#include "MainWindow.h"
#include "qtsingleapplication/qtsingleapplication.h"


int main(int argc, char *argv[])
{
    QtSingleApplication app(argc, argv);
    CMainWindow w;

    // 已打开软件
    if (app.isRunning())
    {
        if (argc>1)
        {
            qDebug() <<"receive message"<< argv[1];
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
