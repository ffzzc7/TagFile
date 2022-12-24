/*!
* \file AddTagDialog.cpp
* \date 2022/11/20 11:29
*
* \author ffzzc7
*
* \brief ��ӱ�ǩ�Ի���
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

    //��ȡ��ǩ
    QString getTag() { return m_strTag; }

public slots:
    // �յ���ӱ�ǩ����ļ���������ӻ��߽�����ѡ���޸ı�ǩ������ʾԭ�б�ǩ
    void onRecv(const QString& str);

private slots:
    // ȷ�Ϻ�д��
    void onConfiremed();

private:
    Ui::CAddTagDialog ui;
    std::wstring m_strFileName;   // �ļ���
    QString m_strTag;             // ��ǩ
};
