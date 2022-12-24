/*!
 * file MainWindow.cpp
 * date 2022/11/20 11:45
 *
 * author ffzzc7
 *
 * brief 主界面
 *
 * TODO:
 *
 * note
*/
#include "MainWindow.h"
#include "AddTagDialog.h"
#include "SearchThread.h"
#include <QDebug>
#include <QProcess>
#include <QCloseEvent>


CMainWindow::CMainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    ui.setupUi(this);
    connect(ui.searchBtn, SIGNAL(clicked()), this, SLOT(onSearch()));
    ui.searchBtn->setEnabled(false);

    // 表格
    QStringList strs = { tr("fileName"),tr( "Tags") };
    m_model.setHorizontalHeaderLabels(strs);
    m_filterModel.setSourceModel(&m_model);
    m_filterModel.setFilterKeyColumn(1);
    ui.resultTable->verticalHeader()->setHidden(true);
    ui.resultTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui.resultTable->setModel(&m_filterModel);
    ui.resultTable->setContextMenuPolicy(Qt::CustomContextMenu);
    ui.progressBar->setRange(0, 100);
    connect(&m_model, SIGNAL(itemChanged(QStandardItem*)), this, SLOT(onItemChanged(QStandardItem*)));
    
    // 右键菜单 
    m_pMenu = new QMenu(ui.resultTable);
    QAction *actionOpen = new QAction(ui.resultTable);
    QAction *actionOpenDir = new QAction(ui.resultTable);
    actionOpen->setText(QString("Open"));
    actionOpenDir->setText(QString("Open Dir"));
    m_pMenu->addAction(actionOpen);
    m_pMenu->addAction(actionOpenDir);
    connect(ui.resultTable, SIGNAL(customContextMenuRequested(QPoint)), this, SLOT(slotContextMenu(QPoint)));
    connect(actionOpen, SIGNAL(triggered()), this, SLOT(onActionOpen()));
    connect(actionOpenDir, SIGNAL(triggered()), this, SLOT(onActionOpenDir()));

    // 初始化线程，搜索所有有标签的文件，并通知进度
    CSearchThread *pWorkerThread = new CSearchThread(this);
    qRegisterMetaType<std::vector<std::wstring>>("std::vector<std::wstring>");
    qRegisterMetaType<map<wstring, string>>("map<wstring, string>");
    connect(pWorkerThread, &CSearchThread::progress, this, &CMainWindow::onIniProgressNotify);
    connect(pWorkerThread, &CSearchThread::resultReady, this, &CMainWindow::handleIniResults);
    connect(pWorkerThread, &CSearchThread::finished, pWorkerThread, &QObject::deleteLater);
    pWorkerThread->start();

    // 托盘
    m_pTrayIcon = new QSystemTrayIcon(this);
    QIcon icon = QIcon(":/CMainWindow/icon(1).png");
    m_pTrayIcon->setToolTip(tr("Tag File"));
    m_pTrayIcon->setIcon(icon);
    connect(m_pTrayIcon, SIGNAL(activated(QSystemTrayIcon::ActivationReason)), this, SLOT(onTrayActivated(QSystemTrayIcon::ActivationReason)));

    QAction *pShowAction = new QAction(tr("show"), this);
    connect(pShowAction, SIGNAL(triggered()), this, SLOT(showNormal()));
    QAction *pQuitAction = new QAction(tr("quit"), this);
    connect(pQuitAction, SIGNAL(triggered()), qApp, SLOT(quit()));

    QMenu *pTrayMenu = new QMenu(this);
    pTrayMenu->addAction(pShowAction);
    pTrayMenu->addAction(pQuitAction);
    m_pTrayIcon->setContextMenu(pTrayMenu);
    m_pTrayIcon->show();
}

// 开始搜索
void CMainWindow::onSearch()
{
    QString strKey = ui.searchLineEdit->text();
    m_filterModel.setFilterFixedString(strKey);
}

// shell消息接收
void CMainWindow::onRecv(const QString &str)
{
    CAddTagDialog *pDialog = new CAddTagDialog(this);
    pDialog->onRecv(str);
    if (pDialog->exec() == QDialog::Accepted)
    {
        bool bFound{ false };
        int row = m_model.rowCount();
        QString strTag = pDialog->getTag();
        for (int i=0;i<row;i++)
        {
            QStandardItem *item = m_model.item(i, 0);
            if (item!=nullptr && item->data(Qt::DisplayRole).toString()==str)
            {
                QStandardItem *tagItem = m_model.item(i, 1);
                if (strTag.isEmpty())
                {
                    m_model.removeRow(i);
                }
                else
                {
                    tagItem->setData(strTag, Qt::DisplayRole);
                }
                bFound = true;
                break;
            }
        }
        if (!bFound && !str.isEmpty())
        {
            QStandardItem *item = new QStandardItem(str);
            QStandardItem *item2 = new QStandardItem(strTag);
            item->setEditable(false);
            QList<QStandardItem*>itemList;
            itemList.push_back(item);
            itemList.push_back(item2);
            m_model.appendRow(itemList);
        }
    }
}


// 完成初始化，将结果显示到表格
void CMainWindow::handleIniResults(std::map<std::wstring, std::string>mapFileTag)
{
    // 第一列为文件名，第二列为标签
    for (auto it: mapFileTag)
    {
        QStandardItem *item = new QStandardItem(QString::fromStdWString(it.first));
        QStandardItem *item2 = new QStandardItem(QString::fromStdString(it.second));
        item->setEditable(false);
        QList<QStandardItem*>itemList;
        itemList.push_back(item);
        itemList.push_back(item2);
        m_model.appendRow(itemList);
    }
    ui.searchBtn->setEnabled(true);
    ui.resultTable->setColumnWidth(0, 800);
    ui.progressBar->setVisible(false);
}

// 右键菜单
void CMainWindow::slotContextMenu(QPoint pos)
{
    m_rightIndex = ui.resultTable->indexAt(pos);
    if (m_rightIndex.isValid())
    {
        if (m_pMenu != nullptr)
        {
            m_pMenu->exec(QCursor::pos()); // 菜单出现的位置为当前鼠标的位置
        }
    }
}

// 打开文件
void CMainWindow::onActionOpen()
{
    QString strFileName = getCurFileName();
    std::wstring stdStr = strFileName.toStdWString();
    ShellExecute(GetDesktopWindow(), NULL, (LPCWSTR)(stdStr.c_str()), NULL, NULL, SW_SHOWNORMAL);
}

// 打开文件夹
void CMainWindow::onActionOpenDir()
{
    QString strFileName = getCurFileName();
    QString strDir = filename2Dir(strFileName);
    std::wstring stdStr = strDir.toStdWString();
    HINSTANCE ret = ShellExecute(GetDesktopWindow(), NULL, (LPCWSTR)(stdStr.c_str()), NULL, NULL, SW_SHOWNORMAL);
}

QString CMainWindow::getCurFileName()
{
    QModelIndex nameIndex = m_rightIndex.siblingAtColumn(0);
    return m_filterModel.data(nameIndex).toString();
}

QString CMainWindow::getCurTag()
{
    QModelIndex tagIndex = m_rightIndex.siblingAtColumn(1);
    return m_filterModel.data(tagIndex).toString();
}

QString CMainWindow::filename2Dir(QString strFileName)
{  
    int temp = strFileName.lastIndexOf("\\");
    QString j = strFileName.right(strFileName.length() - temp - 1);
    QString k = strFileName.left(temp + 1);
    return k;
}

// 显示初始化进度
void CMainWindow::onIniProgressNotify(int percent)
{
    ui.progressBar->setValue(percent);
}

// 双击修改标签
void CMainWindow::onItemChanged(QStandardItem* pItem)
{
    if (pItem == nullptr)
    {
        return;
    }
    int row =pItem->row();
    QStandardItem* pNameItem = m_model.item(row, 0);
    QString strFile = pNameItem->data(Qt::DisplayRole).toString();
    QString strTag =pItem->data(Qt::DisplayRole).toString();
    CComPtr<IStorage> pStorage;
    CComPtr<IPropertySetStorage> pPropertySetStorage;
    HRESULT  hr = StgOpenStorageEx(strFile.toStdWString().c_str(), STGM_DIRECT | STGM_READWRITE | STGM_SHARE_EXCLUSIVE,
        STGFMT_ANY, 0, 0, 0, IID_IPropertySetStorage, (void**)&pPropertySetStorage);

    CComPtr<IPropertyStorage> pPropertyStorage;
    hr = pPropertySetStorage->Create(FMTID_SummaryInformation, NULL, PROPSETFLAG_DEFAULT, STGM_CREATE | STGM_READWRITE | STGM_SHARE_EXCLUSIVE, &pPropertyStorage);


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
}


void CMainWindow::onTrayActivated(QSystemTrayIcon::ActivationReason reanson)
{
    switch (reanson)
    {
    case QSystemTrayIcon::DoubleClick:
        showNormal();
    default:
        break;
    }

}

void CMainWindow::closeEvent(QCloseEvent *pevent)
{
    if (m_pTrayIcon->isVisible())
    {
        hide();
        pevent->ignore();
    }
}