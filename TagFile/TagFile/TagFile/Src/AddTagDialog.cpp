/*!
 * \file AddTagDialog.cpp
 * \date 2022/11/20 11:29
 *
 * \author ffzzc7
 *
 * \brief 添加标签对话框
 *
 * TODO: 
 *
 * \note
*/

#include "AddTagDialog.h"
#include <Windows.h>
#include <ObjIdl.h>
#include <atlbase.h>
#include <atlcom.h>

CAddTagDialog::CAddTagDialog(QWidget *parent, bool bModify)
    : QDialog(parent)
{
    ui.setupUi(this);
    connect(ui.pushButton, SIGNAL(clicked()), this, SLOT(onConfiremed()));
}

CAddTagDialog::~CAddTagDialog()
{
}

// 确认后写入
void CAddTagDialog::onConfiremed()
{
    QString strTag = ui.lineEdit->text();
    /* 创建一个stg文件 */
    CComPtr<IStorage> pStorage;
    CComPtr<IPropertySetStorage> pPropertySetStorage;
    HRESULT  hr = StgOpenStorageEx(m_strFileName.c_str(),STGM_DIRECT | STGM_READWRITE | STGM_SHARE_EXCLUSIVE,
        STGFMT_ANY, 0, 0, 0, IID_IPropertySetStorage, (void**)&pPropertySetStorage);

    /* 打开Summary属性 */
    CComPtr<IPropertyStorage> pPropertyStorage;
    hr = pPropertySetStorage->Create(FMTID_SummaryInformation, NULL, PROPSETFLAG_DEFAULT, STGM_CREATE | STGM_READWRITE | STGM_SHARE_EXCLUSIVE,&pPropertyStorage);

    /* 写入属性 */
    PROPSPEC ps;
    ps.ulKind = PRSPEC_PROPID;
    ps.propid = PIDSI_KEYWORDS;
    PROPVARIANT pv;
    pv.vt = VT_LPSTR;
    std::string strTemp = strTag.toStdString();
    char *temp1 = (LPSTR)strTemp.c_str();
    pv.pszVal = temp1;
    hr = pPropertyStorage->WriteMultiple(1, &ps, &pv, PID_FIRST_USABLE);
    if (hr ==S_OK)
    {
        m_strTag = strTag;
        accept();
    }
}

// 收到添加标签命令（文件管理器添加或者界面上选择修改标签），显示原有标签
void CAddTagDialog::onRecv(const QString& str)
{
    setVisible(true);
    m_strFileName = str.toStdWString();
    ui.m_pLabelFileName->setText(str);
    CComPtr<IPropertySetStorage> pPropertySetStorage;
    HRESULT  hr = StgOpenStorageEx(m_strFileName.c_str(), STGM_DIRECT | STGM_READWRITE | STGM_SHARE_EXCLUSIVE, STGFMT_ANY, 0, 0, 0, IID_IPropertySetStorage, (void**)&pPropertySetStorage);
    /* 获取属性集 */
    if (hr == S_OK)
    {
        //pPropertySetStorage = pStorage;
        /* 打开Summary属性 */
        CComPtr<IPropertyStorage> pPropertyStorage;
        hr = pPropertySetStorage->Open(FMTID_SummaryInformation, STGM_READ | STGM_SHARE_EXCLUSIVE, &pPropertyStorage);
        if (hr == S_OK)
        {
            /* 写入属性 */
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
                    ui.lineEdit->setText(QString::fromStdString(strTag));
                }
            }
        }
    }
}

