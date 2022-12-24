/*!
 * \file MainWindow.h
 * \date 2022/11/20 11:42
 *
 * \author ffzzc7
 *
 * \brief 主界面
 *
 * TODO: long description
 *
 * \note
*/
#pragma once

#include <QtWidgets/QMainWindow>
#include <QStandardItemModel>
#include <QSortFilterProxyModel>
#include <QSystemTrayIcon>
#include "ui_MainWindow.h"


class CMainWindow : public QMainWindow
{
    Q_OBJECT

public:
    CMainWindow(QWidget *parent = Q_NULLPTR);

public slots:
    // shell消息接收
    void onRecv(const QString &str);

protected:
    void closeEvent(QCloseEvent *event);

private slots:
    // 开始搜索
    void onSearch();

    // 完成初始化
    void handleIniResults(std::map<std::wstring, std::string>);

    // 右键菜单
    void slotContextMenu(QPoint pos);

    // 打开文件
    void onActionOpen();

    // 打开所在文件夹
    void onActionOpenDir();

    // 显示初始化进度
    void onIniProgressNotify(int percent);

    // 直接双击修改标签
    void onItemChanged(QStandardItem* pItem);

    // 双击显示主界面
    void onTrayActivated(QSystemTrayIcon::ActivationReason reanson);

private:
    QString getCurFileName();

    QString getCurTag();

    QString filename2Dir(QString strFileName);

private:
    Ui::CMainWindowClass ui;
    QStandardItemModel m_model;
    QSortFilterProxyModel m_filterModel;
    QMenu *m_pMenu{ nullptr };
    QModelIndex m_rightIndex;   // 右键位置
    QSystemTrayIcon *m_pTrayIcon{ nullptr };
};
