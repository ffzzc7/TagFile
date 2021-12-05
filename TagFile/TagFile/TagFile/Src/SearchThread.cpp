#include "SearchThread.h"



CSearchThread::CSearchThread(QObject *parent)
    : QThread(parent)
{
}

CSearchThread::~CSearchThread()
{
}


void CSearchThread::run()
{
    vector<wstring> result;
    for (list<Volume>::iterator lvolit = iniData.volumelist.begin(); lvolit != iniData.volumelist.end(); ++lvolit)
    {
        result= lvolit->findFile(m_strKey.toStdString());
    }
    emit resultReady(result);
}
