#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_FightLandlordGUI.h"
#include "game.h"

class FightLandlordGUI : public QMainWindow
{
    Q_OBJECT

public:
    FightLandlordGUI(QWidget *parent = Q_NULLPTR);
    ~FightLandlordGUI();

public slots:
    void showHelloWorld();
    void showChangedText();

private:
    static const int MaxHandNum = 20;
    static void showCard(QLabel *label,Card card);
    Ui::FightLandlordGUIClass ui;
    QLabel *labMyHands[MaxHandNum];         //labÎªlabelµÄËõÐ´
    QLabel *labNextHands[MaxHandNum];
    QLabel *labLastHands[MaxHandNum];
};
