#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_MainWindow.h"
#include "SearchThread.h"

class CMainWindow : public QMainWindow
{
    Q_OBJECT

public:
    CMainWindow(QWidget *parent = Q_NULLPTR);

public slots:
    void onRecv(const QString &str);

private slots:
    void onSearch();

    void handleResults(std::vector<std::wstring> result);

    void handleIniResults();

private:
    Ui::CMainWindowClass ui;
};
