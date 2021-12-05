#include "Volume.h"

InitData iniData;

wstring Volume::getPath(DWORDLONG frn, wstring &path)
{
    QTextCodec *codec = QTextCodec::codecForName("GBK");
    QTextCodec::setCodecForLocale(codec);
    // ����2
    Frn_Pfrn_Name_Map::iterator it = frnPfrnNameMap.find(frn);
    if (it != frnPfrnNameMap.end())
    {
        if (0 != it->second.pfrn)
        {
            getPath(it->second.pfrn, path);
        }
        path += it->second.filename;
        path += (_T("\\"));
    }
    return path;
}

vector<wstring> Volume::findFile(string str) {

    // ���� VecNameCur
    // ͨ��һ��ƥ�亯���� �õ����ϵ� filename
    // ���� frnPfrnNameMap���ݹ�õ�·��
    qDebug() << "begin" << QDateTime::currentDateTime();
    vector<wstring>pignorelist;
    pignorelist.push_back(_T("C:\\Program Files"));
    pignorelist.push_back(_T("C:\\$WinREAgent"));
    pignorelist.push_back(_T("C:\\Ksoftware"));
    pignorelist.push_back(_T("C:\\OneDriveTemp"));
    pignorelist.push_back(_T("C:\\Program Files (x86)"));
    pignorelist.push_back(_T("C:\\ProgramData"));
    pignorelist.push_back(_T("C:\\Qt"));
    pignorelist.push_back(_T("C:\\Windows"));
    pignorelist.push_back(_T("C:\\Users"));
    pignorelist.push_back(_T("C:\\Code"));
    int notIngoreFileCount = 0;
    int StgOpenStorageExokCount = 0;
    int OpenokCount = 0;
    int ReadOkCount = 0;
    qDebug() << "VecNameCur size" << VecNameCur.size();
    for (vector<Name_Cur>::const_iterator cit = VecNameCur.begin(); cit != VecNameCur.end(); ++cit)
    {
        path.clear();
        // ��ԭ ·��
        // vol:\  path \ cit->filename
        getPath(cit->pfrn, path);
        path += cit->filename;


        if (isIgnore(pignorelist))
        {
            continue;
        }
        //qDebug() << "not Ingore File Count" << ++notIngoreFileCount;
        CComPtr<IPropertySetStorage> pPropertySetStorage;
        HRESULT  hr = StgOpenStorageEx(path.c_str(), STGM_DIRECT | STGM_READWRITE | STGM_SHARE_EXCLUSIVE, STGFMT_ANY, 0, 0, 0, IID_IPropertySetStorage, (void**)&pPropertySetStorage);
        /* ��ȡ���Լ� */
        if (hr == S_OK)
        {
            //qDebug() << "StgOpenStorageEx ok Count:" << ++StgOpenStorageExokCount<< "         path:"<<QString::fromStdWString(path);
            //pPropertySetStorage = pStorage;
            /* ��Summary���� */
            CComPtr<IPropertyStorage> pPropertyStorage;
            hr = pPropertySetStorage->Open(FMTID_SummaryInformation, STGM_READ | STGM_SHARE_EXCLUSIVE, &pPropertyStorage);
            if (hr == S_OK)
            {
                /* д������ */
                //qDebug() << "Open ok Count" << ++OpenokCount << "         path:" << QString::fromStdWString(path);
                PROPSPEC ps;
                ps.ulKind = PRSPEC_PROPID;
                ps.propid = PIDSI_TITLE;

                PROPVARIANT pv;
                hr = pPropertyStorage->ReadMultiple(1, &ps, &pv);
                if (hr == S_OK)
                {
                    std::string strTag = pv.pszVal;
                    // qDebug() << "Read ok Count" << ++ReadOkCount << "       path:" << QString::fromStdWString(path) << "    value" << QString::fromLocal8Bit(pv.pszVal);
                    if (strTag.find(str)!= string::npos)
                    {
                        //path.clear();
                        //// ��ԭ ·��
                        //// vol:\  path \ cit->filename
                        //getPath(cit->pfrn, path);
                        //path += cit->filename;

                        rightFile.push_back(path);
                    }
                }
            }
        }
    }
    qDebug() << "end" << QDateTime::currentDateTime();
    return rightFile;
}

bool cmpStrStr::cmpStrFilename(wstring str, wstring filename) {
    // TODO ������ƥ�亯��

    //int pos = 0;
    //int end = str.GetLength(); 
    //while ( pos < end ) {
    //	// ����str��ȡ�� ÿ���ո�ֿ�Ϊ�Ĺؼ���
    //	pos = str.Find( _T(' ') );

    //	CString strtmp;
    //	if ( pos == -1 ) {
    //		// �޿ո�
    //		strtmp = str;
    //		pos = end;
    //	} else {
    //		strtmp = str.Mid(0, pos-0);
    //	}

    //	if ( !infilename(strtmp, filename) ) {
    //		return false;
    //	}
    //	str.Delete(0, pos);
    //	str.TrimLeft(' ');
    //}

    return true;
}

bool cmpStrStr::infilename(wstring &strtmp, wstring &filename) {
    //string filenametmp(filename);
    //int pos;
    //if ( !uplow ) {
    //	// ��Сд����
    //	filenametmp.MakeLower();
    //	pos = filenametmp.Find(strtmp.MakeLower());
    //} else {
    //	pos = filenametmp.Find(strtmp);
    //}
    //
    //if ( -1 == pos ) {
    //	return false;
    //}
    //if ( !isOrder ) {
    //	// ��˳��
    //	filename.Delete(0, pos+1);
    //}

    return true;
}

bool Volume::getHandle() {
    // Ϊ\\.\C:����ʽ
    wstring lpFileName(_T("\\\\.\\c:"));
    lpFileName[4] = vol;

    hVol = CreateFile(lpFileName.c_str(), GENERIC_READ | GENERIC_WRITE, // ����Ϊ0
        FILE_SHARE_READ | FILE_SHARE_WRITE, // ���������FILE_SHARE_WRITE
        NULL,
        OPEN_EXISTING, // �������OPEN_EXISTING, CREATE_ALWAYS���ܻᵼ�´���
        FILE_ATTRIBUTE_READONLY, // FILE_ATTRIBUTE_NORMAL���ܻᵼ�´���
        NULL);


    if (INVALID_HANDLE_VALUE != hVol)
    {
        return true;
    }
    else
    {
        return false;
        //		exit(1);
        MessageBox(NULL, _T("USN����"), _T("����"), MB_OK);
    }
}

bool Volume::createUSN()
{
    cujd.MaximumSize = 0; // 0��ʾʹ��Ĭ��ֵ  
    cujd.AllocationDelta = 0; // 0��ʾʹ��Ĭ��ֵ

    DWORD br;
    if (
        DeviceIoControl(hVol,// handle to volume
            FSCTL_CREATE_USN_JOURNAL,      // dwIoControlCode
            &cujd,           // input buffer
            sizeof(cujd),         // size of input buffer
            NULL,                          // lpOutBuffer
            0,                             // nOutBufferSize
            &br,     // number of bytes returned
            NULL) // OVERLAPPED structure	
        )
    {
        return true;
    }
    else
    {
        return false;
    }
}


bool Volume::getUSNInfo()
{
    DWORD br;
    if (
        DeviceIoControl(hVol, // handle to volume
            FSCTL_QUERY_USN_JOURNAL,// dwIoControlCode
            NULL,            // lpInBuffer
            0,               // nInBufferSize
            &ujd,     // output buffer
            sizeof(ujd),  // size of output buffer
            &br, // number of bytes returned
            NULL) // OVERLAPPED structure
        )
    {
        return true;
    }
    else
    {
        return false;
    }
}

bool Volume::getUSNJournal()
{
    MFT_ENUM_DATA_V0 med;
    med.StartFileReferenceNumber = 0;
    med.LowUsn = ujd.FirstUsn;
    med.HighUsn = ujd.NextUsn;

    // ��Ŀ¼
    wstring tmp(_T("C:"));
    tmp[0] = vol;
    frnPfrnNameMap[0x5000000000005].filename = tmp;
    frnPfrnNameMap[0x5000000000005].pfrn = 0;

#define BUF_LEN 0x10000	// �����ܵش����Ч��

    CHAR Buffer[BUF_LEN];
    DWORD usnDataSize;
    PUSN_RECORD UsnRecord;
    int USN_counter = 0;

    // ͳ���ļ���...

    while (0 != DeviceIoControl(hVol,
        FSCTL_ENUM_USN_DATA,
        &med,
        sizeof(med),
        Buffer,
        BUF_LEN,
        &usnDataSize,
        NULL))
    {

        DWORD dwRetBytes = usnDataSize - sizeof(USN);
        // �ҵ���һ�� USN ��¼  
        UsnRecord = (PUSN_RECORD)(((PCHAR)Buffer) + sizeof(USN));

        while (dwRetBytes > 0) {
            // ��ȡ������Ϣ  	
            wstring CfileName(UsnRecord->FileName);
            CfileName.resize(UsnRecord->FileNameLength / 2);/*, UsnRecord->FileNameLength / 2)*/;

            pfrnName.filename = nameCur.filename = CfileName;
            pfrnName.pfrn = nameCur.pfrn = UsnRecord->ParentFileReferenceNumber;

            // Vector
            VecNameCur.push_back(nameCur);

            // ����hash...
            frnPfrnNameMap[UsnRecord->FileReferenceNumber] = pfrnName;
            // ��ȡ��һ����¼  
            DWORD recordLen = UsnRecord->RecordLength;
            dwRetBytes -= recordLen;
            UsnRecord = (PUSN_RECORD)(((PCHAR)UsnRecord) + recordLen);

        }
        // ��ȡ��һҳ���� 
        med.StartFileReferenceNumber = *(USN *)&Buffer;
    }
    int i = GetLastError();
    return true;
}

bool Volume::deleteUSN()
{
    DELETE_USN_JOURNAL_DATA dujd;
    dujd.UsnJournalID = ujd.UsnJournalID;
    dujd.DeleteFlags = USN_DELETE_FLAG_DELETE;
    DWORD br;

    if (DeviceIoControl(hVol, FSCTL_DELETE_USN_JOURNAL, &dujd, sizeof(dujd), NULL, 0, &br, NULL))
    {
        CloseHandle(hVol);
        return true;
    }
    else
    {
        CloseHandle(hVol);
        return false;
    }
}

bool InitData::isNTFS(char c)
{
    char lpRootPathName[] = ("c:\\");
    lpRootPathName[0] = c;
    char lpVolumeNameBuffer[MAX_PATH];
    DWORD lpVolumeSerialNumber;
    DWORD lpMaximumComponentLength;
    DWORD lpFileSystemFlags;
    char lpFileSystemNameBuffer[MAX_PATH];

    if (GetVolumeInformationA(lpRootPathName, lpVolumeNameBuffer, MAX_PATH, &lpVolumeSerialNumber, &lpMaximumComponentLength, &lpFileSystemFlags, lpFileSystemNameBuffer, MAX_PATH))
    {
        if (!strcmp(lpFileSystemNameBuffer, "NTFS"))
        {
            return true;
        }
    }
    return false;
}