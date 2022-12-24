/*!
 * file SearchThread.cpp
 * date 2022/11/20 11:52
 *
 * author ffzzc7
 *
 * brief ��ʼ����ǩ�߳�
 *
 * TODO:
 *
 * note
*/


#include "SearchThread.h"
/*Ԥ��������Լ�

FMTID_SummaryInformation ժҪ��Ϣ���Լ�
FMTID_DocSummaryInformation 
FMTID_UserDefinedProperties
*/


/*����
ժҪ��Ϣ���Լ�
#define PIDSI_TITLE               0x00000002L  // VT_LPSTR
#define PIDSI_SUBJECT             0x00000003L  // VT_LPSTR
#define PIDSI_AUTHOR              0x00000004L  // VT_LPSTR
#define PIDSI_KEYWORDS            0x00000005L  // VT_LPSTR
#define PIDSI_COMMENTS            0x00000006L  // VT_LPSTR
#define PIDSI_TEMPLATE            0x00000007L  // VT_LPSTR
#define PIDSI_LASTAUTHOR          0x00000008L  // VT_LPSTR
#define PIDSI_REVNUMBER           0x00000009L  // VT_LPSTR
#define PIDSI_EDITTIME            0x0000000aL  // VT_FILETIME (UTC)
#define PIDSI_LASTPRINTED         0x0000000bL  // VT_FILETIME (UTC)
#define PIDSI_CREATE_DTM          0x0000000cL  // VT_FILETIME (UTC)
#define PIDSI_LASTSAVE_DTM        0x0000000dL  // VT_FILETIME (UTC)
#define PIDSI_PAGECOUNT           0x0000000eL  // VT_I4
#define PIDSI_WORDCOUNT           0x0000000fL  // VT_I4
#define PIDSI_CHARCOUNT           0x00000010L  // VT_I4
#define PIDSI_THUMBNAIL           0x00000011L  // VT_CF
#define PIDSI_APPNAME             0x00000012L  // VT_LPSTR
#define PIDSI_DOC_SECURITY        0x00000013L  // VT_I4
*/
CSearchThread::CSearchThread(QObject *parent)
    : QThread(parent)
{
}

CSearchThread::~CSearchThread()
{
}

void CSearchThread::run()
{
    // ��ʼ�������ļ���Ϣ
    int num = iniData.init(NULL);
    char *pvol = iniData.getVol();
    for (int i = 0; i < num; ++i)
    {
        iniData.initvolumelist((LPVOID)pvol[i]);
    }
    // ���������ļ���
    map<wstring, string> result;
    vector<wstring>allFile;
    for (list<Volume>::iterator lvolit = iniData.volumelist.begin(); lvolit != iniData.volumelist.end(); ++lvolit)
    {
        vector<wstring> temp = lvolit->getAllFile();
        allFile.insert(allFile.end(), temp.begin(), temp.end());
    }
    int fileCount = allFile.size();
    int newCount = 0;
    int oldCount = 0;
    for (auto it : allFile)
    {
        newCount++;
        if (newCount - oldCount > (fileCount / 100))
        {
            oldCount = newCount;
            emit progress(newCount * 100 / fileCount);
        }
        CComPtr<IPropertySetStorage> pPropertySetStorage;
        HRESULT  hr = StgOpenStorageEx(it.c_str(), STGM_DIRECT | STGM_READWRITE | STGM_SHARE_EXCLUSIVE, STGFMT_ANY, 0, 0, 0, IID_IPropertySetStorage, (void**)&pPropertySetStorage);
        /* ��ȡ���Լ� */
        if (hr == S_OK)
        {
            //pPropertySetStorage = pStorage;
            /* ��Summary���� */
            CComPtr<IPropertyStorage> pPropertyStorage;
            hr = pPropertySetStorage->Open(FMTID_SummaryInformation, STGM_READ | STGM_SHARE_EXCLUSIVE, &pPropertyStorage);
            if (hr == S_OK)
            {
                /* д������ */
                PROPSPEC ps;
                ps.ulKind = PRSPEC_PROPID;
                ps.propid = PIDSI_KEYWORDS;

                PROPVARIANT pv;
                hr = pPropertyStorage->ReadMultiple(1, &ps, &pv);
                if (hr == S_OK)
                {
                    std::string strTag = pv.pszVal;
                    if (!strTag.empty())
                    {
                        result.insert(make_pair(it, strTag));
                    }
                }
            }
        }
    }
    emit resultReady(result);
}
