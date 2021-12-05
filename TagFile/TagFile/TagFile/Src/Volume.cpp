#include "Volume.h"

InitData iniData;

wstring Volume::getPath(DWORDLONG frn, wstring &path)
{
    QTextCodec *codec = QTextCodec::codecForName("GBK");
    QTextCodec::setCodecForLocale(codec);
    // 查找2
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

    // 遍历 VecNameCur
    // 通过一个匹配函数， 得到符合的 filename
    // 传入 frnPfrnNameMap，递归得到路径
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
        // 还原 路径
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
        /* 获取属性集 */
        if (hr == S_OK)
        {
            //qDebug() << "StgOpenStorageEx ok Count:" << ++StgOpenStorageExokCount<< "         path:"<<QString::fromStdWString(path);
            //pPropertySetStorage = pStorage;
            /* 打开Summary属性 */
            CComPtr<IPropertyStorage> pPropertyStorage;
            hr = pPropertySetStorage->Open(FMTID_SummaryInformation, STGM_READ | STGM_SHARE_EXCLUSIVE, &pPropertyStorage);
            if (hr == S_OK)
            {
                /* 写入属性 */
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
                        //// 还原 路径
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
    // TODO 加入别的匹配函数

    //int pos = 0;
    //int end = str.GetLength(); 
    //while ( pos < end ) {
    //	// 对于str，取得 每个空格分开为的关键词
    //	pos = str.Find( _T(' ') );

    //	CString strtmp;
    //	if ( pos == -1 ) {
    //		// 无空格
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
    //	// 大小写敏感
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
    //	// 无顺序
    //	filename.Delete(0, pos+1);
    //}

    return true;
}

bool Volume::getHandle() {
    // 为\\.\C:的形式
    wstring lpFileName(_T("\\\\.\\c:"));
    lpFileName[4] = vol;

    hVol = CreateFile(lpFileName.c_str(), GENERIC_READ | GENERIC_WRITE, // 可以为0
        FILE_SHARE_READ | FILE_SHARE_WRITE, // 必须包含有FILE_SHARE_WRITE
        NULL,
        OPEN_EXISTING, // 必须包含OPEN_EXISTING, CREATE_ALWAYS可能会导致错误
        FILE_ATTRIBUTE_READONLY, // FILE_ATTRIBUTE_NORMAL可能会导致错误
        NULL);


    if (INVALID_HANDLE_VALUE != hVol)
    {
        return true;
    }
    else
    {
        return false;
        //		exit(1);
        MessageBox(NULL, _T("USN错误"), _T("错误"), MB_OK);
    }
}

bool Volume::createUSN()
{
    cujd.MaximumSize = 0; // 0表示使用默认值  
    cujd.AllocationDelta = 0; // 0表示使用默认值

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

    // 根目录
    wstring tmp(_T("C:"));
    tmp[0] = vol;
    frnPfrnNameMap[0x5000000000005].filename = tmp;
    frnPfrnNameMap[0x5000000000005].pfrn = 0;

#define BUF_LEN 0x10000	// 尽可能地大，提高效率

    CHAR Buffer[BUF_LEN];
    DWORD usnDataSize;
    PUSN_RECORD UsnRecord;
    int USN_counter = 0;

    // 统计文件中...

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
        // 找到第一个 USN 记录  
        UsnRecord = (PUSN_RECORD)(((PCHAR)Buffer) + sizeof(USN));

        while (dwRetBytes > 0) {
            // 获取到的信息  	
            wstring CfileName(UsnRecord->FileName);
            CfileName.resize(UsnRecord->FileNameLength / 2);/*, UsnRecord->FileNameLength / 2)*/;

            pfrnName.filename = nameCur.filename = CfileName;
            pfrnName.pfrn = nameCur.pfrn = UsnRecord->ParentFileReferenceNumber;

            // Vector
            VecNameCur.push_back(nameCur);

            // 构建hash...
            frnPfrnNameMap[UsnRecord->FileReferenceNumber] = pfrnName;
            // 获取下一个记录  
            DWORD recordLen = UsnRecord->RecordLength;
            dwRetBytes -= recordLen;
            UsnRecord = (PUSN_RECORD)(((PCHAR)UsnRecord) + recordLen);

        }
        // 获取下一页数据 
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