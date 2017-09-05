#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_FightLandlordGUI.h"

class FightLandlordGUI : public QMainWindow
{
    Q_OBJECT

public:
    FightLandlordGUI(QWidget *parent = Q_NULLPTR);

public slots:
    void showHelloWorld();
    void showChangedText();

private:
    Ui::FightLandlordGUIClass ui;
    QLabel *labMyHands[20];         //labÎªlabelµÄËõÐ´
    QLabel *labNextHands[20];
    QLabel *labLastHands[20];
};
