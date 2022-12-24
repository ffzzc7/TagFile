/*!
 * \file MainWindow.h
 * \date 2022/11/20 11:42
 *
 * \author ffzzc7
 *
 * \brief ������
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
    // shell��Ϣ����
    void onRecv(const QString &str);

protected:
    void closeEvent(QCloseEvent *event);

private slots:
    // ��ʼ����
    void onSearch();

    // ��ɳ�ʼ��
    void handleIniResults(std::map<std::wstring, std::string>);

    // �Ҽ��˵�
    void slotContextMenu(QPoint pos);

    // ���ļ�
    void onActionOpen();

    // �������ļ���
    void onActionOpenDir();

    // ��ʾ��ʼ������
    void onIniProgressNotify(int percent);

    // ֱ��˫���޸ı�ǩ
    void onItemChanged(QStandardItem* pItem);

    // ˫����ʾ������
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
    QModelIndex m_rightIndex;   // �Ҽ�λ��
    QSystemTrayIcon *m_pTrayIcon{ nullptr };
};
