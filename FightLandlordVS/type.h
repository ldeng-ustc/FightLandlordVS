#ifndef type_H
#define type_H

#include <iostream>
#include <vector>
#include <string>
#include <list>
#include "card.h"
using namespace std;

typedef vector<Card> Group;

const int NumOfType = 14; //牌型种类数，13可用，14不可用
const int NumCanInShun = 12;  //能在顺子中的牌的种类数，11可用，12不可用
const int MaxSortPow = 12;	//同上，注意A=11能，Pow(2)=12不能
const int MaxTypeLen = 13;  //顺子长度最长，为5-12

enum CardType { NoneType = -1, Danzhang = 0, Duizi, Sanzhang, Sandaiyi, Sandaier, Shunzi, Liandui, Feiji, Feijidaiyi, Feijidaier, Zhadan, Sidaierdan, Sidaierdui, Huojian };
const string TypeNameEnStr[NumOfType] = { "Danzhang", "Duizi", "Sanzhang", "Sandaiyi", "Sandaier", "Shunzi", "Liandui", "Feiji", "Feijidaiyi", "Feijidaier", "Zhadan", "Sidaierdan", "Sidaierdui", "Huojian" };
const string TypeNameCnStr[NumOfType] = { "单张", "对子", "三张", "三带一", "三带二", "顺子", "连对", "飞机", "飞机带一", "飞机带二", "炸弹", "四带二单", "四带二对", "火箭" };
const bool TypeHasLength[NumOfType] = { false, false, false, false, false, true, true, true, true, true, false, false, false, false };
const int TypeMinLength[NumOfType] = { 0, 0, 0, 0, 0, 5, 3, 2, 2, 2, 0, 0, 0, 0 };
const int TypeMaxLength[NumOfType] = { 0, 0, 0, 0, 0, 12, 10, 6, 5, 4, 0, 0, 0, 0 };
const int TypeLengthDiv[NumOfType] = { 1, 1, 1, 1, 1, 1, 2, 3, 4, 5, 1, 1, 1, 1 };

const int SortNumToArrayNum[NumOfNumber] = { 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 1 };  //sortArray的下标表示的牌在array中的下标
const int ArrayNumToSortNum[NumOfNumber] = { -1, 11, 12, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 13, 14 };
const int PowToNum[NumOfNumber] = { 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 1, 2, 14, 15 };
const int NumToPow[NumOfNumber] = { -1, 11, 12, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 14, 15 };


struct TypeTest
{
	static bool hasInit;
	static void init();

	static vector<bool(*)(const Group&)> hasType;
	/*一个储存函数指针的可变长数组，
	*其中hasType[CardType](group)表示在指定（必须保证已排序的）牌组group中，判断是否存在CardType型的牌
	*/

	static vector<bool(*)(const Group&, int)> isType;
	/*一个储存函数指针的可变长数组，
	*其中isType[CardType](group,len)表示判断指定（必须保证已排序的）牌组group是否是CardType型且长度（如果存在）为len的牌
	*需要判断牌型长度的类型有：顺子，连对，飞机，飞机带一，飞机带二（长度皆为顺的长度，如33344479长度为2）
	*对于其它不需要判断长度的牌型，实现中将第二个int参数作为哑元
	*/

	static vector<int(*)(const Group&, int)> getPower;
	/*
	* 应该保证所调用准确，否则不保证行为及稳定性
	*/

	TypeTest();

	static Group& makeGroup(string str, Group& group);  //按str中的每一个char作为digit生成牌组并排序，保存在group中，返回group
	static CardType getType(const Group& group, CardType& ct, int& len);      //保存第一个匹配的牌型及长度，若无匹配，返回NoneType
	static bool isInGroup(const Group& subGroup, const Group& mainGroup);    //判断subGroup是否在mainGroup中，两牌组都必须排序
	static int getLength(const Group& group, CardType type);
	static int canOutPower(const Group& group, CardType type, int len, int power);
	//判断group是否能压过指定的type len power的牌，必须指定type，能则返回给定牌组的power，否则返回-1
	static bool canOut(const Group& group, CardType type, int len, int power);
	//static int bestSplit(const Group& group, vector<Group>& bestWay);
	//static int countHandsWithoutCarry(const Group& group,vector<Group>& bestWay, int cntType = 0, int lastPow = 0);
	//static int TypeTest::getDiv(const Group& group, int cardNumber);
    static int getHands(const Group& group, vector<Group>& bestWay);
    static int getHandsWithoutCarry(const Group& group,vector<Group>& bestWay);
    static void test();
};

#endif // type
