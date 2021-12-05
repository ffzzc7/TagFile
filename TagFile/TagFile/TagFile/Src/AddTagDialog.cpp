#include "AddTagDialog.h"
#include <Windows.h>
#include <ObjIdl.h>
#include <atlbase.h>
#include <atlcom.h>

CAddTagDialog::CAddTagDialog(QWidget *parent)
    : QDialog(parent)
{
    ui.setupUi(this);
    connect(ui.pushButton, SIGNAL(clicked()), this, SLOT(onConfiremed()));
}

CAddTagDialog::~CAddTagDialog()
{
}


void CAddTagDialog::onConfiremed()
{
    QString strTag = ui.lineEdit->text();
    /* 创建一个stg文件 */
    CComPtr<IStorage> pStorage;
    CComPtr<IPropertySetStorage> pPropertySetStorage;
    HRESULT  hr = StgOpenStorageEx(m_strFileName.c_str(),STGM_DIRECT | STGM_READWRITE | STGM_SHARE_EXCLUSIVE,
        STGFMT_ANY, 0, 0, 0, IID_IPropertySetStorage, (void**)&pPropertySetStorage);
    /* 获取属性集 */

    //pPropertySetStorage = pStorage;
    /* 打开Summary属性 */
    CComPtr<IPropertyStorage> pPropertyStorage;
    hr = pPropertySetStorage->Create(FMTID_SummaryInformation, NULL, PROPSETFLAG_DEFAULT, STGM_CREATE | STGM_READWRITE | STGM_SHARE_EXCLUSIVE,&pPropertyStorage);

    PROPSPEC psOld;
    psOld.ulKind = PRSPEC_PROPID;
    psOld.propid = PIDSI_TITLE;
    std::string s;
    PROPVARIANT pvOld;
    hr = pPropertyStorage->ReadMultiple(1, &psOld, &pvOld);
    if (hr == S_OK)
    {
        s = pvOld.pszVal;
    }

    /* 写入属性 */
    PROPSPEC ps;
    ps.ulKind = PRSPEC_PROPID;
    ps.propid = PIDSI_TITLE;

    PROPVARIANT pv;
    pv.vt = VT_LPSTR;
    s.append(",");
    s.append(strTag.toStdString());
    char *temp1 = (LPSTR)s.c_str();
    pv.pszVal = temp1;
    hr = pPropertyStorage->WriteMultiple(1, &ps, &pv, PID_FIRST_USABLE);
    accept();
}


void CAddTagDialog::onRecv(const QString& str)
{
    setVisible(true);
    m_strFileName = str.toStdWString();
    ui.m_pLabelFileName->setText(str);
}