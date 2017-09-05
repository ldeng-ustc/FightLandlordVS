/********************************************************************************
** Form generated from reading UI file 'FightLandlordGUI.ui'
**
** Created by: Qt User Interface Compiler version 5.8.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_FIGHTLANDLORDGUI_H
#define UI_FIGHTLANDLORDGUI_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QTextEdit>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_FightLandlordGUIClass
{
public:
    QWidget *centralWidget;
    QPushButton *btnHelloWorld;
    QTextEdit *textEdit;
    QLabel *label;
    QMenuBar *menuBar;
    QToolBar *mainToolBar;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *FightLandlordGUIClass)
    {
        if (FightLandlordGUIClass->objectName().isEmpty())
            FightLandlordGUIClass->setObjectName(QStringLiteral("FightLandlordGUIClass"));
        FightLandlordGUIClass->resize(1089, 725);
        centralWidget = new QWidget(FightLandlordGUIClass);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        btnHelloWorld = new QPushButton(centralWidget);
        btnHelloWorld->setObjectName(QStringLiteral("btnHelloWorld"));
        btnHelloWorld->setGeometry(QRect(510, 440, 112, 34));
        textEdit = new QTextEdit(centralWidget);
        textEdit->setObjectName(QStringLiteral("textEdit"));
        textEdit->setGeometry(QRect(360, 550, 191, 31));
        label = new QLabel(centralWidget);
        label->setObjectName(QStringLiteral("label"));
        label->setGeometry(QRect(160, 410, 74, 98));
        FightLandlordGUIClass->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(FightLandlordGUIClass);
        menuBar->setObjectName(QStringLiteral("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 1089, 30));
        FightLandlordGUIClass->setMenuBar(menuBar);
        mainToolBar = new QToolBar(FightLandlordGUIClass);
        mainToolBar->setObjectName(QStringLiteral("mainToolBar"));
        FightLandlordGUIClass->addToolBar(Qt::TopToolBarArea, mainToolBar);
        statusBar = new QStatusBar(FightLandlordGUIClass);
        statusBar->setObjectName(QStringLiteral("statusBar"));
        FightLandlordGUIClass->setStatusBar(statusBar);

        retranslateUi(FightLandlordGUIClass);
        QObject::connect(btnHelloWorld, SIGNAL(clicked()), FightLandlordGUIClass, SLOT(showHelloWorld()));
        QObject::connect(textEdit, SIGNAL(textChanged()), FightLandlordGUIClass, SLOT(showChangedText()));

        QMetaObject::connectSlotsByName(FightLandlordGUIClass);
    } // setupUi

    void retranslateUi(QMainWindow *FightLandlordGUIClass)
    {
        FightLandlordGUIClass->setWindowTitle(QApplication::translate("FightLandlordGUIClass", "FightLandlordGUI", Q_NULLPTR));
        btnHelloWorld->setText(QApplication::translate("FightLandlordGUIClass", "PushButton", Q_NULLPTR));
        label->setText(QApplication::translate("FightLandlordGUIClass", "TextLabel", Q_NULLPTR));
    } // retranslateUi

};

namespace Ui {
    class FightLandlordGUIClass: public Ui_FightLandlordGUIClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_FIGHTLANDLORDGUI_H
