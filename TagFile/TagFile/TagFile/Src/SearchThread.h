#pragma once

#include <QThread>
#include "Volume.h"

class CSearchThread : public QThread
{
    Q_OBJECT

public:
    CSearchThread(QObject *parent);
    ~CSearchThread();

    void setKey(QString strKey) { m_strKey = strKey; }

    void setIniData(InitData m_initdata) { m_initdata = m_initdata; }

protected:
    void run();

signals:
    void resultReady(std::vector<std::wstring> result);

private:
    QString m_strKey;
    InitData m_initdata;
};
