#include "FightLandlordGUI.h"
#include <QtWidgets/QApplication>
#include <QMessageBox>
#include "card_images.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    CardImages::init();
    FightLandlordGUI w;
    w.show();
    //QPushButton *p = (new QPushButton(QString("newButton")));
    //p->show();
    return a.exec();
}
