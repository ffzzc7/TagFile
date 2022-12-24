/********************************************************************************
** Form generated from reading UI file 'AddTagDialog.ui'
**
** Created by: Qt User Interface Compiler version 5.14.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_ADDTAGDIALOG_H
#define UI_ADDTAGDIALOG_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>

QT_BEGIN_NAMESPACE

class Ui_CAddTagDialog
{
public:
    QGridLayout *gridLayout_2;
    QSpacerItem *horizontalSpacer;
    QPushButton *pushButton;
    QGridLayout *gridLayout;
    QLabel *label_2;
    QLabel *m_pLabelFileName;
    QLabel *label;
    QLineEdit *lineEdit;

    void setupUi(QDialog *CAddTagDialog)
    {
        if (CAddTagDialog->objectName().isEmpty())
            CAddTagDialog->setObjectName(QString::fromUtf8("CAddTagDialog"));
        CAddTagDialog->resize(400, 130);
        gridLayout_2 = new QGridLayout(CAddTagDialog);
        gridLayout_2->setSpacing(6);
        gridLayout_2->setContentsMargins(11, 11, 11, 11);
        gridLayout_2->setObjectName(QString::fromUtf8("gridLayout_2"));
        horizontalSpacer = new QSpacerItem(298, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout_2->addItem(horizontalSpacer, 1, 0, 1, 1);

        pushButton = new QPushButton(CAddTagDialog);
        pushButton->setObjectName(QString::fromUtf8("pushButton"));

        gridLayout_2->addWidget(pushButton, 1, 1, 1, 1);

        gridLayout = new QGridLayout();
        gridLayout->setSpacing(6);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        label_2 = new QLabel(CAddTagDialog);
        label_2->setObjectName(QString::fromUtf8("label_2"));

        gridLayout->addWidget(label_2, 0, 0, 1, 1);

        m_pLabelFileName = new QLabel(CAddTagDialog);
        m_pLabelFileName->setObjectName(QString::fromUtf8("m_pLabelFileName"));

        gridLayout->addWidget(m_pLabelFileName, 0, 1, 1, 1);

        label = new QLabel(CAddTagDialog);
        label->setObjectName(QString::fromUtf8("label"));

        gridLayout->addWidget(label, 1, 0, 1, 1);

        lineEdit = new QLineEdit(CAddTagDialog);
        lineEdit->setObjectName(QString::fromUtf8("lineEdit"));

        gridLayout->addWidget(lineEdit, 1, 1, 1, 1);


        gridLayout_2->addLayout(gridLayout, 0, 0, 1, 2);


        retranslateUi(CAddTagDialog);

        QMetaObject::connectSlotsByName(CAddTagDialog);
    } // setupUi

    void retranslateUi(QDialog *CAddTagDialog)
    {
        CAddTagDialog->setWindowTitle(QCoreApplication::translate("CAddTagDialog", "\346\267\273\345\212\240\346\240\207\347\255\276", nullptr));
        pushButton->setText(QCoreApplication::translate("CAddTagDialog", "\347\241\256\345\256\232", nullptr));
        label_2->setText(QCoreApplication::translate("CAddTagDialog", "\346\226\207\344\273\266", nullptr));
        m_pLabelFileName->setText(QString());
        label->setText(QCoreApplication::translate("CAddTagDialog", "\346\240\207\347\255\276", nullptr));
    } // retranslateUi

};

namespace Ui {
    class CAddTagDialog: public Ui_CAddTagDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_ADDTAGDIALOG_H
