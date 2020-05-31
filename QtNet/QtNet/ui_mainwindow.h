/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 5.14.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QRadioButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QSpinBox>
#include <QtWidgets/QTextEdit>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralWidget;
    QGroupBox *groupBox_2;
    QGroupBox *groupBox_4;
    QHBoxLayout *horizontalLayout_4;
    QRadioButton *radioServer;
    QRadioButton *radioClient;
    QSpacerItem *horizontalSpacer_2;
    QPushButton *BtnConn;
    QLabel *labelAddr;
    QLabel *label_2;
    QLineEdit *EditIP;
    QLineEdit *EditPort;
    QGroupBox *groupBox;
    QHBoxLayout *horizontalLayout_2;
    QTextEdit *EditRecv;
    QVBoxLayout *verticalLayout_2;
    QCheckBox *chkHexDisp;
    QPushButton *BtnClearRecv;
    QSpacerItem *verticalSpacer;
    QGroupBox *groupBox_3;
    QHBoxLayout *horizontalLayout_3;
    QTextEdit *EditSend;
    QVBoxLayout *verticalLayout_3;
    QCheckBox *chkHexSend;
    QPushButton *BtnSend;
    QSpacerItem *verticalSpacer_2;
    QComboBox *cbLstClients;
    QPushButton *cmd2;
    QPushButton *cmd1;
    QTextEdit *textRecv;
    QLineEdit *textSend;
    QSpinBox *maxlight;
    QSpinBox *frq;
    QWidget *layoutWidget;
    QHBoxLayout *horizontalLayout_6;
    QLabel *label_3;
    QComboBox *comboBox;
    QLabel *label_4;
    QComboBox *comboBox_2;
    QLabel *label_5;
    QPushButton *btnOpen;
    QPushButton *btnSend;
    QPushButton *btnClear;
    QLabel *label_6;
    QLabel *label_7;
    QLabel *label_8;
    QLabel *label_9;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QString::fromUtf8("MainWindow"));
        MainWindow->resize(1215, 473);
        centralWidget = new QWidget(MainWindow);
        centralWidget->setObjectName(QString::fromUtf8("centralWidget"));
        groupBox_2 = new QGroupBox(centralWidget);
        groupBox_2->setObjectName(QString::fromUtf8("groupBox_2"));
        groupBox_2->setGeometry(QRect(9, 33, 551, 131));
        QSizePolicy sizePolicy(QSizePolicy::Preferred, QSizePolicy::Minimum);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(groupBox_2->sizePolicy().hasHeightForWidth());
        groupBox_2->setSizePolicy(sizePolicy);
        groupBox_2->setMaximumSize(QSize(16777215, 350));
        groupBox_4 = new QGroupBox(groupBox_2);
        groupBox_4->setObjectName(QString::fromUtf8("groupBox_4"));
        groupBox_4->setGeometry(QRect(11, 23, 501, 60));
        sizePolicy.setHeightForWidth(groupBox_4->sizePolicy().hasHeightForWidth());
        groupBox_4->setSizePolicy(sizePolicy);
        groupBox_4->setMinimumSize(QSize(0, 60));
        horizontalLayout_4 = new QHBoxLayout(groupBox_4);
        horizontalLayout_4->setSpacing(6);
        horizontalLayout_4->setContentsMargins(11, 11, 11, 11);
        horizontalLayout_4->setObjectName(QString::fromUtf8("horizontalLayout_4"));
        radioServer = new QRadioButton(groupBox_4);
        radioServer->setObjectName(QString::fromUtf8("radioServer"));

        horizontalLayout_4->addWidget(radioServer);

        radioClient = new QRadioButton(groupBox_4);
        radioClient->setObjectName(QString::fromUtf8("radioClient"));

        horizontalLayout_4->addWidget(radioClient);

        horizontalSpacer_2 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_4->addItem(horizontalSpacer_2);

        BtnConn = new QPushButton(groupBox_4);
        BtnConn->setObjectName(QString::fromUtf8("BtnConn"));
        QSizePolicy sizePolicy1(QSizePolicy::Minimum, QSizePolicy::Minimum);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(BtnConn->sizePolicy().hasHeightForWidth());
        BtnConn->setSizePolicy(sizePolicy1);
        BtnConn->setMinimumSize(QSize(120, 0));
        BtnConn->setMaximumSize(QSize(10000, 16777215));

        horizontalLayout_4->addWidget(BtnConn);

        labelAddr = new QLabel(groupBox_2);
        labelAddr->setObjectName(QString::fromUtf8("labelAddr"));
        labelAddr->setGeometry(QRect(30, 90, 72, 16));
        label_2 = new QLabel(groupBox_2);
        label_2->setObjectName(QString::fromUtf8("label_2"));
        label_2->setGeometry(QRect(351, 90, 36, 16));
        EditIP = new QLineEdit(groupBox_2);
        EditIP->setObjectName(QString::fromUtf8("EditIP"));
        EditIP->setGeometry(QRect(30, 110, 300, 20));
        EditIP->setMinimumSize(QSize(300, 20));
        EditPort = new QLineEdit(groupBox_2);
        EditPort->setObjectName(QString::fromUtf8("EditPort"));
        EditPort->setGeometry(QRect(350, 110, 140, 20));
        QSizePolicy sizePolicy2(QSizePolicy::Minimum, QSizePolicy::Fixed);
        sizePolicy2.setHorizontalStretch(0);
        sizePolicy2.setVerticalStretch(0);
        sizePolicy2.setHeightForWidth(EditPort->sizePolicy().hasHeightForWidth());
        EditPort->setSizePolicy(sizePolicy2);
        EditPort->setMinimumSize(QSize(140, 20));
        EditPort->setMaximumSize(QSize(150, 16777215));
        groupBox = new QGroupBox(centralWidget);
        groupBox->setObjectName(QString::fromUtf8("groupBox"));
        groupBox->setGeometry(QRect(10, 170, 551, 171));
        QSizePolicy sizePolicy3(QSizePolicy::Preferred, QSizePolicy::Preferred);
        sizePolicy3.setHorizontalStretch(0);
        sizePolicy3.setVerticalStretch(0);
        sizePolicy3.setHeightForWidth(groupBox->sizePolicy().hasHeightForWidth());
        groupBox->setSizePolicy(sizePolicy3);
        horizontalLayout_2 = new QHBoxLayout(groupBox);
        horizontalLayout_2->setSpacing(6);
        horizontalLayout_2->setContentsMargins(11, 11, 11, 11);
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        EditRecv = new QTextEdit(groupBox);
        EditRecv->setObjectName(QString::fromUtf8("EditRecv"));
        EditRecv->setReadOnly(true);
        EditRecv->setTextInteractionFlags(Qt::TextSelectableByKeyboard|Qt::TextSelectableByMouse);

        horizontalLayout_2->addWidget(EditRecv);

        verticalLayout_2 = new QVBoxLayout();
        verticalLayout_2->setSpacing(6);
        verticalLayout_2->setObjectName(QString::fromUtf8("verticalLayout_2"));
        chkHexDisp = new QCheckBox(groupBox);
        chkHexDisp->setObjectName(QString::fromUtf8("chkHexDisp"));

        verticalLayout_2->addWidget(chkHexDisp);

        BtnClearRecv = new QPushButton(groupBox);
        BtnClearRecv->setObjectName(QString::fromUtf8("BtnClearRecv"));
        BtnClearRecv->setMinimumSize(QSize(120, 0));

        verticalLayout_2->addWidget(BtnClearRecv);

        verticalSpacer = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout_2->addItem(verticalSpacer);


        horizontalLayout_2->addLayout(verticalLayout_2);

        groupBox_3 = new QGroupBox(centralWidget);
        groupBox_3->setObjectName(QString::fromUtf8("groupBox_3"));
        groupBox_3->setGeometry(QRect(10, 340, 551, 131));
        groupBox_3->setMaximumSize(QSize(16777215, 150));
        horizontalLayout_3 = new QHBoxLayout(groupBox_3);
        horizontalLayout_3->setSpacing(6);
        horizontalLayout_3->setContentsMargins(11, 11, 11, 11);
        horizontalLayout_3->setObjectName(QString::fromUtf8("horizontalLayout_3"));
        EditSend = new QTextEdit(groupBox_3);
        EditSend->setObjectName(QString::fromUtf8("EditSend"));

        horizontalLayout_3->addWidget(EditSend);

        verticalLayout_3 = new QVBoxLayout();
        verticalLayout_3->setSpacing(6);
        verticalLayout_3->setObjectName(QString::fromUtf8("verticalLayout_3"));
        chkHexSend = new QCheckBox(groupBox_3);
        chkHexSend->setObjectName(QString::fromUtf8("chkHexSend"));

        verticalLayout_3->addWidget(chkHexSend);

        BtnSend = new QPushButton(groupBox_3);
        BtnSend->setObjectName(QString::fromUtf8("BtnSend"));
        BtnSend->setMinimumSize(QSize(120, 0));

        verticalLayout_3->addWidget(BtnSend);

        verticalSpacer_2 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout_3->addItem(verticalSpacer_2);

        cbLstClients = new QComboBox(groupBox_3);
        cbLstClients->setObjectName(QString::fromUtf8("cbLstClients"));
        cbLstClients->setMinimumSize(QSize(120, 0));

        verticalLayout_3->addWidget(cbLstClients);


        horizontalLayout_3->addLayout(verticalLayout_3);

        cmd2 = new QPushButton(centralWidget);
        cmd2->setObjectName(QString::fromUtf8("cmd2"));
        cmd2->setGeometry(QRect(1120, 220, 75, 23));
        cmd1 = new QPushButton(centralWidget);
        cmd1->setObjectName(QString::fromUtf8("cmd1"));
        cmd1->setGeometry(QRect(1120, 130, 75, 23));
        textRecv = new QTextEdit(centralWidget);
        textRecv->setObjectName(QString::fromUtf8("textRecv"));
        textRecv->setGeometry(QRect(580, 80, 451, 171));
        textRecv->setReadOnly(true);
        textSend = new QLineEdit(centralWidget);
        textSend->setObjectName(QString::fromUtf8("textSend"));
        textSend->setGeometry(QRect(580, 320, 451, 20));
        maxlight = new QSpinBox(centralWidget);
        maxlight->setObjectName(QString::fromUtf8("maxlight"));
        maxlight->setGeometry(QRect(1060, 220, 42, 22));
        maxlight->setMaximum(9);
        frq = new QSpinBox(centralWidget);
        frq->setObjectName(QString::fromUtf8("frq"));
        frq->setGeometry(QRect(1060, 130, 42, 22));
        frq->setMaximum(9);
        layoutWidget = new QWidget(centralWidget);
        layoutWidget->setObjectName(QString::fromUtf8("layoutWidget"));
        layoutWidget->setGeometry(QRect(580, 360, 601, 61));
        horizontalLayout_6 = new QHBoxLayout(layoutWidget);
        horizontalLayout_6->setSpacing(6);
        horizontalLayout_6->setContentsMargins(11, 11, 11, 11);
        horizontalLayout_6->setObjectName(QString::fromUtf8("horizontalLayout_6"));
        horizontalLayout_6->setContentsMargins(0, 0, 0, 0);
        label_3 = new QLabel(layoutWidget);
        label_3->setObjectName(QString::fromUtf8("label_3"));
        label_3->setMaximumSize(QSize(50, 16777215));

        horizontalLayout_6->addWidget(label_3);

        comboBox = new QComboBox(layoutWidget);
        comboBox->addItem(QString());
        comboBox->addItem(QString());
        comboBox->addItem(QString());
        comboBox->addItem(QString());
        comboBox->setObjectName(QString::fromUtf8("comboBox"));
        comboBox->setMaximumSize(QSize(70, 16777215));

        horizontalLayout_6->addWidget(comboBox);

        label_4 = new QLabel(layoutWidget);
        label_4->setObjectName(QString::fromUtf8("label_4"));
        label_4->setMaximumSize(QSize(50, 16777215));

        horizontalLayout_6->addWidget(label_4);

        comboBox_2 = new QComboBox(layoutWidget);
        comboBox_2->addItem(QString());
        comboBox_2->addItem(QString());
        comboBox_2->addItem(QString());
        comboBox_2->addItem(QString());
        comboBox_2->addItem(QString());
        comboBox_2->setObjectName(QString::fromUtf8("comboBox_2"));
        comboBox_2->setMaximumSize(QSize(70, 16777215));

        horizontalLayout_6->addWidget(comboBox_2);

        label_5 = new QLabel(layoutWidget);
        label_5->setObjectName(QString::fromUtf8("label_5"));
        label_5->setMaximumSize(QSize(20, 16777215));

        horizontalLayout_6->addWidget(label_5);

        btnOpen = new QPushButton(layoutWidget);
        btnOpen->setObjectName(QString::fromUtf8("btnOpen"));

        horizontalLayout_6->addWidget(btnOpen);

        btnSend = new QPushButton(layoutWidget);
        btnSend->setObjectName(QString::fromUtf8("btnSend"));

        horizontalLayout_6->addWidget(btnSend);

        btnClear = new QPushButton(layoutWidget);
        btnClear->setObjectName(QString::fromUtf8("btnClear"));

        horizontalLayout_6->addWidget(btnClear);

        label_6 = new QLabel(centralWidget);
        label_6->setObjectName(QString::fromUtf8("label_6"));
        label_6->setGeometry(QRect(580, 280, 111, 16));
        label_7 = new QLabel(centralWidget);
        label_7->setObjectName(QString::fromUtf8("label_7"));
        label_7->setGeometry(QRect(1060, 180, 101, 16));
        label_8 = new QLabel(centralWidget);
        label_8->setObjectName(QString::fromUtf8("label_8"));
        label_8->setGeometry(QRect(1060, 90, 101, 16));
        label_9 = new QLabel(centralWidget);
        label_9->setObjectName(QString::fromUtf8("label_9"));
        label_9->setGeometry(QRect(590, 40, 101, 16));
        MainWindow->setCentralWidget(centralWidget);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QCoreApplication::translate("MainWindow", "MainWindow", nullptr));
        groupBox_2->setTitle(QCoreApplication::translate("MainWindow", "\347\275\221\347\273\234\350\256\276\347\275\256", nullptr));
        groupBox_4->setTitle(QCoreApplication::translate("MainWindow", "\346\250\241\345\274\217\351\200\211\346\213\251", nullptr));
        radioServer->setText(QCoreApplication::translate("MainWindow", "TCP\346\234\215\345\212\241\345\231\250", nullptr));
        radioClient->setText(QCoreApplication::translate("MainWindow", "TCP\345\256\242\346\210\267\347\253\257", nullptr));
        BtnConn->setText(QCoreApplication::translate("MainWindow", "\350\277\236\346\216\245\346\234\215\345\212\241\345\231\250", nullptr));
        labelAddr->setText(QCoreApplication::translate("MainWindow", "\346\234\215\345\212\241\345\231\250\345\234\260\345\235\200\357\274\232", nullptr));
        label_2->setText(QCoreApplication::translate("MainWindow", "\347\253\257\345\217\243\357\274\232", nullptr));
        EditIP->setText(QCoreApplication::translate("MainWindow", "192.168.", nullptr));
        EditPort->setText(QCoreApplication::translate("MainWindow", "4321", nullptr));
        groupBox->setTitle(QCoreApplication::translate("MainWindow", "\346\216\245\346\224\266\346\225\260\346\215\256", nullptr));
        chkHexDisp->setText(QCoreApplication::translate("MainWindow", "HEX\346\230\276\347\244\272", nullptr));
        BtnClearRecv->setText(QCoreApplication::translate("MainWindow", "\346\270\205\347\251\272", nullptr));
        groupBox_3->setTitle(QCoreApplication::translate("MainWindow", "\345\217\221\351\200\201\346\225\260\346\215\256", nullptr));
        chkHexSend->setText(QCoreApplication::translate("MainWindow", "HEX\345\217\221\351\200\201", nullptr));
        BtnSend->setText(QCoreApplication::translate("MainWindow", "\345\217\221\351\200\201", nullptr));
        cmd2->setText(QCoreApplication::translate("MainWindow", "\345\217\221\351\200\201\346\214\207\344\273\244", nullptr));
        cmd1->setText(QCoreApplication::translate("MainWindow", "\345\217\221\351\200\201\346\214\207\344\273\244", nullptr));
        label_3->setText(QCoreApplication::translate("MainWindow", "\347\253\257\345\217\243\345\217\267\357\274\232", nullptr));
        comboBox->setItemText(0, QCoreApplication::translate("MainWindow", "COM1", nullptr));
        comboBox->setItemText(1, QCoreApplication::translate("MainWindow", "COM2", nullptr));
        comboBox->setItemText(2, QCoreApplication::translate("MainWindow", "COM3", nullptr));
        comboBox->setItemText(3, QCoreApplication::translate("MainWindow", "COM4", nullptr));

        label_4->setText(QCoreApplication::translate("MainWindow", "\346\263\242\347\211\271\347\216\207\357\274\232", nullptr));
        comboBox_2->setItemText(0, QCoreApplication::translate("MainWindow", "1200", nullptr));
        comboBox_2->setItemText(1, QCoreApplication::translate("MainWindow", "4800", nullptr));
        comboBox_2->setItemText(2, QCoreApplication::translate("MainWindow", "9600", nullptr));
        comboBox_2->setItemText(3, QCoreApplication::translate("MainWindow", "38400", nullptr));
        comboBox_2->setItemText(4, QCoreApplication::translate("MainWindow", "115200", nullptr));

        label_5->setText(QCoreApplication::translate("MainWindow", "bps", nullptr));
        btnOpen->setText(QCoreApplication::translate("MainWindow", "\346\211\223\345\274\200\344\270\262\345\217\243", nullptr));
        btnSend->setText(QCoreApplication::translate("MainWindow", "\345\217\221\351\200\201", nullptr));
        btnClear->setText(QCoreApplication::translate("MainWindow", "\346\270\205\347\251\272", nullptr));
        label_6->setText(QCoreApplication::translate("MainWindow", "\344\270\262\345\217\243\345\217\221\351\200\201\347\252\227\345\217\243", nullptr));
        label_7->setText(QCoreApplication::translate("MainWindow", "\345\221\274\345\220\270\347\201\257\351\200\237\345\272\246", nullptr));
        label_8->setText(QCoreApplication::translate("MainWindow", "\345\221\274\345\220\270\347\201\257\344\272\256\345\272\246", nullptr));
        label_9->setText(QCoreApplication::translate("MainWindow", "\344\270\262\345\217\243\346\216\245\346\224\266\347\252\227\345\217\243", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
