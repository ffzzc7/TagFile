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

#pragma once

#include <QDialog>
#include "ui_AddTagDialog.h"

class CAddTagDialog : public QDialog
{
    Q_OBJECT

public:
    CAddTagDialog(QWidget *parent = Q_NULLPTR, bool bModify=false );
    ~CAddTagDialog();

    //获取标签
    QString getTag() { return m_strTag; }

public slots:
    // 收到添加标签命令（文件管理器添加或者界面上选择修改标签），显示原有标签
    void onRecv(const QString& str);

private slots:
    // 确认后写入
    void onConfiremed();

private:
    Ui::CAddTagDialog ui;
    std::wstring m_strFileName;   // 文件名
    QString m_strTag;             // 标签
};
