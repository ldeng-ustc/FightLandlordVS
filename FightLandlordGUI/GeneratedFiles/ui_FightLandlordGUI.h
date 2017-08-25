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
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_FightLandlordGUIClass
{
public:
    QMenuBar *menuBar;
    QToolBar *mainToolBar;
    QWidget *centralWidget;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *FightLandlordGUIClass)
    {
        if (FightLandlordGUIClass->objectName().isEmpty())
            FightLandlordGUIClass->setObjectName(QStringLiteral("FightLandlordGUIClass"));
        FightLandlordGUIClass->resize(600, 400);
        menuBar = new QMenuBar(FightLandlordGUIClass);
        menuBar->setObjectName(QStringLiteral("menuBar"));
        FightLandlordGUIClass->setMenuBar(menuBar);
        mainToolBar = new QToolBar(FightLandlordGUIClass);
        mainToolBar->setObjectName(QStringLiteral("mainToolBar"));
        FightLandlordGUIClass->addToolBar(mainToolBar);
        centralWidget = new QWidget(FightLandlordGUIClass);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        FightLandlordGUIClass->setCentralWidget(centralWidget);
        statusBar = new QStatusBar(FightLandlordGUIClass);
        statusBar->setObjectName(QStringLiteral("statusBar"));
        FightLandlordGUIClass->setStatusBar(statusBar);

        retranslateUi(FightLandlordGUIClass);

        QMetaObject::connectSlotsByName(FightLandlordGUIClass);
    } // setupUi

    void retranslateUi(QMainWindow *FightLandlordGUIClass)
    {
        FightLandlordGUIClass->setWindowTitle(QApplication::translate("FightLandlordGUIClass", "FightLandlordGUI", Q_NULLPTR));
    } // retranslateUi

};

namespace Ui {
    class FightLandlordGUIClass: public Ui_FightLandlordGUIClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_FIGHTLANDLORDGUI_H
