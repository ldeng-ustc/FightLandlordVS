/*************************
** 文件:card.cpp
** 描述:Card类的实现
** 备注:Card类的接口参见crad.h
**************************/

#include "stdafx.h"
#include <iostream>
#include <vector>
#include "random.h"
#include "card.h"
#include "type.h"
#include "game.h"
using namespace std;

/*inline int highTwo(int x)   //获取x作为id时代表花色的两位
{
return x>>4;
}

inline int lowTwo(int x)    //获取x作为id时代表数值的两位
{
return x&0xF;
}*/

Card::Card() {}

Card::Card(int id)
{
	this->id = id;
	color = highTwo(id);
	number = lowTwo(id);
	//printf("in:0x%X,color:%d,number:%d\n",id,color,number);
}

Card::Card(int color, int number)
{
	id = (color << 4) + number;
    this->color = color;
    this->number = number;
}

Card::Card(char digit)
{
	switch (digit)
	{
	case 'W':
		id = RedJoker;
		break;
	case 'w':
		id = BlackJoker;
		break;
	case 'X':
		id = 0xA;
		break;
	case 'J':
		id = 0xB;
		break;
	case 'Q':
		id = 0xC;
		break;
	case 'K':
		id = 0xD;
		break;
	case 'A':
		id = 0x1;
		break;
	default:
		if (digit >= '2'&&digit <= '9')
			id = digit - '0';
		else
		{
			showError(string("错误：试图用非法字符初始化卡片,使用的字符为：'") + digit + "'。（将初始化为0x0）");
			id = 0;
		}
	}
	number = lowTwo(id);
	color = highTwo(id);
}

bool Card::operator<(const Card& b) const
{
	/*if(this->color==4)
	{
	if(b.color==4)
	return this->number<b.number;
	else
	return false;
	}
	else
	{
	if(b.color==4)
	return true;
	else if(this->number<3)
	{
	if(b.number<3)
	return this->number<b.number;
	else
	return false;
	}
	else
	{
	if(b.number<3)
	return true;
	else
	return this->number<b.number;
	}
	}*/
	if (this->getPower() == b.getPower())
		return (this->color)>b.color;
	else
		return this->getPower()<b.getPower();
}

bool Card::operator>(const Card& b) const
{
	return b<(*this);
}

bool Card::operator==(const Card& b) const
{
	return !((*this)<b || (*this)>b);
}

bool Card::operator<=(const Card& b) const
{
	return (*this)<b || (*this) == b;
}

bool Card::operator>=(const Card& b) const
{
	return (*this)>b || (*this) == b;
}

Card& Card::operator=(const int id)
{
	this->id = id;
	color = highTwo(id);
	number = lowTwo(id);
	return (*this);
}

Card& Card::operator=(const Card& b)
{
	this->id = b.id;
	this->number = b.number;
	this->color = b.color;
	return (*this);
}

bool Card::isNum(int num) const
{
	return (number == num) && (color != 4);
}

bool Card::isKing() const
{
	return color == 4;
}

void Card::prt(int length, char filled) const
{
	if (length<0)
		length = ColorIcon[color].length() + 3;
	cout << ColorIcon[color];
	cout << NumToOutString[number];
	length -= ColorIcon[color].length() + NumToOutString[number].length();
	for (int i = 0; i<length; i++)
		cout << filled;
}

int Card::getNum() const
{
	return number;
}

int Card::getColor() const
{
	return color;
}

char Card::getDigit() const
{
	return NumToDigit[number];
}

int Card::getId() const
{
	return id;
}

int Card::getPower() const
{
	if (number >= 3 && number <= 13)   //3~K的大小为number0-10
		return number - 3;
	else if (number == 1 || number == 2)   //A为11 2为12
		return number + 10;
	else
		return number - 1;    //王为13和14
}

void Card::prtDigit() const
{
	cout << getDigit();
}
