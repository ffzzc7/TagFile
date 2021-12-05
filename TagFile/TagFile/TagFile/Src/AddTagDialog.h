#pragma once

#include <QDialog>
#include "ui_AddTagDialog.h"

class CAddTagDialog : public QDialog
{
    Q_OBJECT

public:
    CAddTagDialog(QWidget *parent = Q_NULLPTR );
    ~CAddTagDialog();

public slots:
    void onRecv(const QString& str);

private slots:
    void onConfiremed();

private:
    Ui::CAddTagDialog ui;
    std::wstring m_strFileName;
};
