#include "FightLandlordGUI.h"
#include "card_images.h"
#include <QMessageBox>
#include "game.h"

FightLandlordGUI::FightLandlordGUI(QWidget *parent)
    : QMainWindow(parent)
{
    ui.setupUi(this);
    for (int i = 0; i < MaxHandNum; i++)
    {
        labMyHands[i] = new QLabel(this);
        labMyHands[i]->setGeometry(QRect(50, 50, 74, 98));
        showCard(labMyHands[i], Card(2, 7));
        labNextHands[i] = new QLabel(this);
        labLastHands[i] = new QLabel(this);
    }
}

FightLandlordGUI::~FightLandlordGUI()
{
    for (int i = 0; i < MaxHandNum; i++)
    {
        delete labMyHands[i];
        delete labNextHands[i];
        delete labLastHands[i];
    }
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

void FightLandlordGUI::showCard(QLabel *label, Card card)
{
    if (card.getId() == RedJoker)
        label->setPixmap(QPixmap::fromImage(*(CardImages::cardImages[53])));
    else if (card.getId() == BlackJoker)
        label->setPixmap(QPixmap::fromImage(*(CardImages::cardImages[52])));
    else
        label->setPixmap(QPixmap::fromImage(*(CardImages::cardImages[card.getColor() * 13 + card.getNum() - 1])));
}
