/*!
 * file SearchThread.h
 * date 2022/11/20 11:54
 *
 * author ffzzc7
 *
 * brief ��ʼ���߳�
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
    // ��ʼ�����
    void resultReady(map<wstring, string> result);
    // ֪ͨ����
    void progress(int percent);
};
