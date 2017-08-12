#ifndef card_H
#define card_H

#include <iostream>
#include <vector>
using namespace std;

const static int NumOfPoker = 54; //一幅扑克牌的张数
const static int NumOfColor = 5;
const static int NumOfHand = 17;  //每个玩家初始手牌的张数
const static int NumOfPlayer = 3; //玩家个数
const static int MaxRound = 200;  //最高可能进行的回合数(注：斗地主中，至少每三回合出一张牌，故最多54*3<200)
const static int NumOfNumber = 16;  //牌的number的最大值为15（大王）
const static int NumOfPow = 15;		//Pow的最大值为14

const static int BlackJoker = 0x4E;   //小王的id
const static int RedJoker = 0x4F;     //大王的id

const static string ColorIcon[NumOfColor] = { "黑桃", "红桃", "梅花", "方片", "王" };
const static char NumToDigit[NumOfNumber] = { '*', 'A', '2', '3', '4', '5', '6', '7', '8', '9', 'X', 'J', 'Q', 'K', 'w', 'W' };
const static string NumToOutString[NumOfNumber] = { "*", "A", "2", "3", "4", "5", "6", "7", "8", "9", "10", "J", "Q", "K", "w", "W" };

inline int highTwo(int x)   //获取x作为id时代表花色的两位
{
	return x >> 4;
}

inline int lowTwo(int x)    //获取x作为id时代表数值的两位
{
	return x & 0xF;
}

class Card
{
	int id;     //0xXY ,X is color : ♠0 ♥1 ♣2 ♦3 Joker4 , Y is number 1:A ... A:10 .. D:K E:Black F:Red
	int color;  //0:♠ 1:♥ 2:♣ 3:♦ 4:joker
	int number; //1~13;
	//0x040E Black Joker, 0x040F Red Joker

public:
	Card();
	Card(int id);
	Card(int color, int number);
	Card(char digit);

	bool operator<(const Card& b) const;
	bool operator>(const Card& b) const;
	bool operator==(const Card& b) const;
	bool operator>=(const Card& b) const;
	bool operator<=(const Card& b) const;

	Card& operator=(const int id);
	Card& operator=(const Card& b);

	bool isNum(int num) const;
	bool isKing() const;
	int getNum() const;
	int getColor() const;
	char getDigit() const;
	int getId() const;
	int getPower() const;

	void prtDigit() const;
	void prt(int length = -1, char filled = ' ') const;  //默认输出长度4位，用空格填补

};

#endif // card
