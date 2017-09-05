#include "FightLandlordGUI.h"
#include "card_images.h"
#include <QMessageBox>

FightLandlordGUI::FightLandlordGUI(QWidget *parent)
    : QMainWindow(parent)
{
    ui.setupUi(this);
    for (int i = 0; i < ;i++)



}

void FightLandlordGUI::showHelloWorld()
{
    QMessageBox::information(this, "hello", "hello <font size=20>world!</font>");
}

int cnt = 0;
void FightLandlordGUI::showChangedText()
{
    //this->ui.label->setText(this->ui.textEdit->toPlainText());
    this->ui.label->setPixmap(QPixmap::fromImage(*(CardImages::cardImages[cnt])));
    cnt++;
}
