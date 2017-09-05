#ifndef card_images_H
#define card_images_H

#include <string>
#include <sstream>
#include <QMessageBox>
#include "FightLandlordGUI.h"
#include "card.h"
using namespace std;

class CardImages
{
public:
    static QImage *cardImages[54];
    static QImage *backFamer;
    static QImage *backLandlord;
    static string colorName[4];
public:
    static void init()
    {
        colorName[0] = "SPADE";
        colorName[1] = "HEART";
        colorName[2] = "CLUB";
        colorName[3] = "DIAMOND";
        for (int color = 0; color < 4; color++)
        {
            for (int number = 1; number <= 13; number++)
            {
                int cnt = color * 13 + number - 1;
                cardImages[cnt]=new QImage();
                stringstream ss;
                ss << "Cards/CARDTYPE_" << colorName[color] << "_" << number << ".gif";
                string filename;
                ss >> filename;
                ////QMessageBox::information(nullptr, "warning", QString(filename.c_str()));
                //bool flag = cardImages[cnt]->load(QString(filename.c_str()));
                //if (!flag)
                //{
                //    QMessageBox::information(nullptr, "warning", "fail");
                //    break;
                //}
            }
        }
        cardImages[52] = new QImage();
        cardImages[52]->load("Cards/CARDTYPE_XIAOWANG.gif");
        cardImages[53] = new QImage();
        cardImages[53]->load("Cards/CARDTYPE_DAWANG.gif");
        backFamer = new QImage();
        backFamer->load("Cards/CARDBADK_Farmer.gif");
        backLandlord = new QImage();
        backLandlord->load("Cards/CARDBADK_Lord.gif");
    }
    static const QImage* getCardImage(Card card)
    {
        if (card.getId() == BlackJoker)
            return cardImages[52];
        else if (card.getId() == RedJoker)
            return cardImages[53];
        else
            return cardImages[card.getColor() * 13 + card.getNum() - 1];
    }
};

#endif