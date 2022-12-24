/*!
 * file SearchThread.h
 * date 2022/11/20 11:54
 *
 * author ffzzc7
 *
 * brief 初始化线程
 *
 * TODO:
 *
 * note
*/
#pragma once

#include <QThread>
#include "fake_everything/Volume.h"

class CSearchThread : public QThread
{
    Q_OBJECT

public:
    CSearchThread(QObject *parent);
    ~CSearchThread();

protected:
    void run();

signals:
    // 初始化完成
    void resultReady(map<wstring, string> result);
    // 通知进度
    void progress(int percent);
};
