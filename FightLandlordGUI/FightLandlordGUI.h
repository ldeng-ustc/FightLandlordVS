#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_FightLandlordGUI.h"

class FightLandlordGUI : public QMainWindow
{
    Q_OBJECT

public:
    FightLandlordGUI(QWidget *parent = Q_NULLPTR);

private:
    Ui::FightLandlordGUIClass ui;
};
