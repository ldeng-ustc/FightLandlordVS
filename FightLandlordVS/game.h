#ifndef game_H
#define game_H

#include <iostream>
#include <string>
#include <vector>
#include "random.h"
#include "type.h"
using namespace std;

inline void showError(string msg)
{
	cout << "********************" << endl;
	cout << msg << endl;
	cout << "********************" << endl;
}

inline void showMessage(string msg)
{
	cout << msg << endl;
}

class Game
{
private:
    random r;
    int seed;
	int stage;          //0:start 1:choose landlord  2:gaming  3:end
	int round;          //每个玩家出牌算一轮，从0开始计数，第0轮地主出牌
	int landlord;
	int winner;
	CardType currentType;
	int currentTypeLen;
	int currentPower;
	vector<Card> hand[NumOfPlayer];    //hand ,hand[landlord] is the hand of landlord
	vector<Card> landlordCard;       //the open card of landlord, usually 3 cards
	vector<Card> history[MaxRound];   //history of every round
	vector<Card> restCard;
public:
    Game();                             //构造一局尚未开始的新游戏
	void showHand() const;            //打印玩家的手牌情况到屏幕
	void showLandlordCard() const;    //打印本局游戏的地主牌到屏幕
	void showLandlord() const;        //打印本局游戏的地主到屏幕
	void showBaseInformation() const; //打印本局游戏的基本信息到屏幕（进行到第几回合，轮到谁出牌，地主和地主牌情况）
	void showHistory(int r) const;
	void showCurrentRequired() const;
	void showStatus() const;            //打印当前游戏的状态到屏幕（阶段，所有玩家的手牌）
	void showHistory() const;
	void showRestCard() const;
    void showTestStatus() const;        //打印游戏时状态（只可见对手手牌张数）
	bool start(int seed = 0);             //开始游戏（只对尚未开始的游戏有效，即stage==0）
	bool setLandlord(int lord);         //设置地主为编号lord的玩家（只对正在进行地主选择的游戏有效，即stage==1）
	bool setLandlord();                 //随机设置地主（同上）
	bool playCard(string strGroup, CardType type = NoneType); //出牌，可不指定类型，出现歧义按可打出的第一种类型识别（顺序按类型编号）
	bool playCard(Group& group, CardType type = NoneType); //不保证group不被改变
    bool isLastPlayerPass() const;            //上一个玩家是否pass
	int getStage() const;                               //返回当前游戏阶段
	bool isEnd() const;
	int getCurrentGamer() const;                     //返回当前回合需要出牌的玩家
    int getLandlord() const;
	CardType getCurrentType() const;
	int getCurrentTypeLen() const;
    int getCurrentTypePow() const;
	const vector<Card>& getHand(int i) const;       //返回玩家i的手牌（不能被修改）
	const vector<Card>& getLandlordCard() const;    //返回本局游戏的地主牌（不能被修改）
    int getHandSize(int i) const;
};

#endif // game

