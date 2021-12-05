#pragma once

#include <vector>
#include <map>
//#include <Winioctl.h>
//#include <Windows.h>
//#include <ObjIdl.h>
#include <atlbase.h>
#include <atlcom.h>
#include <list>
#include <iostream>
#include <fstream>
#include <string>
#include <QDebug>
#include <QTextCodec>
#include <QDateTime>

#define MAXVOL 3

using namespace std;

typedef struct _name_cur
{
    wstring filename;
    DWORDLONG pfrn;   //?
}Name_Cur;

typedef struct _pfrn_name
{
    DWORDLONG pfrn;
    wstring filename;
}Pfrn_Name;

typedef map<DWORDLONG, Pfrn_Name> Frn_Pfrn_Name_Map;

class cmpStrStr
{
public:
    cmpStrStr(bool uplow, bool inorder)
    {
        this->uplow = uplow;
        this->isOrder = inorder;
    }
    ~cmpStrStr() {};
    bool cmpStrFilename(wstring str, wstring filename);
    bool infilename(wstring &strtmp, wstring &filenametmp);
private:
    bool uplow;
    bool isOrder;
};


class Volume
{
public:
    Volume(char vol)
    {
        this->vol = vol;
        hVol = NULL;
        path = _T("");
    }
    ~Volume() 
    {
        //		CloseHandle(hVol);
    }

    bool initVolume()
    {
        if (
            // 2.获取驱动盘句柄
            getHandle() &&
            // 3.创建USN日志
            createUSN() &&
            // 4.获取USN日志信息
            getUSNInfo() &&
            // 5.获取 USN Journal 文件的基本信息
            getUSNJournal() &&
            // 06. 删除 USN 日志文件 ( 也可以不删除 ) 
            deleteUSN())
        {
            return true;
        }
        else
        {
            return false;
        }
    }

    bool isIgnore(vector<wstring> pignorelist)
    {
        wstring tmp = path;
        for (vector<wstring>::iterator it = pignorelist.begin(); it != pignorelist.end(); ++it)
        {
            size_t i = it->length();
            if (!tmp.compare(0, i, *it, 0, i))
            {
                return true;
            }
        }
        return false;
    }

    bool getHandle();
    bool createUSN();
    bool getUSNInfo();
    bool getUSNJournal();
    bool deleteUSN();

    vector<wstring> findFile(string str);

    wstring getPath(DWORDLONG frn, wstring &path);

    vector<wstring> rightFile;	 // 结果

private:
    char vol;
    HANDLE hVol;
    vector<Name_Cur> VecNameCur;		// 查找1
    Name_Cur nameCur;
    Pfrn_Name pfrnName;
    Frn_Pfrn_Name_Map frnPfrnNameMap;	// 查找2
    wstring path;
    USN_JOURNAL_DATA ujd;
    CREATE_USN_JOURNAL_DATA cujd;

    
};


/*
*	计算一共有几个盘符是NTFS格式
*/
class InitData {
public:

    bool isNTFS(char c);

    list<Volume> volumelist;
    UINT initvolumelist(LPVOID vol)
    {
        char c = (char)vol;
        Volume volume(c);
        volume.initVolume();
        volumelist.push_back(volume);

        return 1;
    }
    /*
    static UINT initThread(LPVOID pParam) {
    InitData * pObj = (InitData*)pParam;
    if ( pObj ) {
    return pObj->init(NULL);
    }
    return false;
    }
    */
    UINT init(LPVOID lp) {

#ifdef TEST
        for (i = j = 0; i < MAXVOL; ++i) {
#else
        for (i = j = 0; i < 26; ++i) {
#endif
            cvol = i + 'A';
            if (isNTFS(cvol)) {
                vol[j++] = cvol;
            }
        }
        /*
        CString showpro(_T("正在统计"));
        for ( i=0; i<j; ++i ) {
        initvolumelist((LPVOID)vol[i]);
        GetDlgItem(IDC_SHOWPRO)->SetWindowText(showpro + _T(vol[i]));
        }
        */
        return true;
        }

    int getJ() {
        return j;
    }
    char * getVol() {
        return vol;
    }

    vector<string>* getIgnorePath()
    {
        ignorepath.clear();
        ifstream fin("config.ini");
        string tmp;
        while (getline(fin, tmp))
        {
            ignorepath.push_back(tmp);
        }
        return &ignorepath;
    }

private:
    char vol[26];
    char cvol;
    int i, j;

    vector<string> ignorepath;
    };

extern InitData iniData;