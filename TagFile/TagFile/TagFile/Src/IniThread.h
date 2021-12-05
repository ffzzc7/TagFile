#pragma once

#include <QThread>
#include "Volume.h"

class CIniThread : public QThread
{
    Q_OBJECT

public:
    CIniThread(QObject *parent);
    ~CIniThread();

protected:
    void run();

signals:
    void resultReady();

private:

    InitData m_initdata;
};
