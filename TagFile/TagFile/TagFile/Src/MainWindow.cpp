#include "MainWindow.h"
#include "AddTagDialog.h"
#include "IniThread.h"
#include <QDebug>

CMainWindow::CMainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    ui.setupUi(this);
    connect(ui.pushButton, SIGNAL(clicked()), this, SLOT(onSearch()));
    ui.pushButton->setEnabled(false);
    ui.tableWidget->horizontalHeader()->setHidden(true);
    ui.tableWidget->setColumnWidth(0,2000);
    CIniThread *iniThread = new CIniThread(this);
    connect(iniThread, &CIniThread::resultReady, this, &CMainWindow::handleIniResults);
    connect(iniThread, &CIniThread::finished, iniThread, &QObject::deleteLater);
    iniThread->start();
}


void CMainWindow::onSearch()
{
    ui.pushButton->setEnabled(false);
    QString strKey = ui.lineEdit->text();
    
    CSearchThread *workerThread = new CSearchThread(this);
    workerThread->setKey(strKey);
    qRegisterMetaType<std::vector<std::wstring>>("std::vector<std::wstring>");
    connect(workerThread, &CSearchThread::resultReady, this, &CMainWindow::handleResults);
    connect(workerThread, &CSearchThread::finished, workerThread, &QObject::deleteLater);
    workerThread->start();
}

void CMainWindow::onRecv(const QString &str)
{
    CAddTagDialog *pDialog = new CAddTagDialog;
    pDialog->onRecv(str);
}

void CMainWindow::handleResults(vector<wstring> result)
{
    ui.pushButton->setEnabled(true);
    ui.tableWidget->setRowCount(result.size());
    int i = 0;
    for (auto it : result)
    {
        QTableWidgetItem *item = new QTableWidgetItem(QString::fromStdWString(it));
        ui.tableWidget->setItem(i, 0, item);
        i++;
    }
}

void CMainWindow::handleIniResults()
{
     ui.pushButton->setEnabled(true);
}