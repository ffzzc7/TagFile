#include "IniThread.h"

CIniThread::CIniThread(QObject *parent)
    : QThread(parent)
{
}

CIniThread::~CIniThread()
{
}


void CIniThread::run()
{
    int num = iniData.init(NULL);
    char *pvol = iniData.getVol();
    for (int i = 0; i<num; ++i)
    {
        iniData.initvolumelist((LPVOID)pvol[i]);
    }
    emit resultReady();
}
