#ifndef type_H
#define type_H

#include <iostream>
#include <vector>
#include <string>
#include <list>
#include "card.h"
using namespace std;

typedef vector<Card> Group;

const int NumOfType = 14; //������������13���ã�14������
const int NumCanInShun = 12;  //����˳���е��Ƶ���������11���ã�12������
const int MaxSortPow = 12;	//ͬ�ϣ�ע��A=11�ܣ�Pow(2)=12����
const int MaxTypeLen = 13;  //˳�ӳ������Ϊ5-12

enum CardType { NoneType = -1, Danzhang = 0, Duizi, Sanzhang, Sandaiyi, Sandaier, Shunzi, Liandui, Feiji, Feijidaiyi, Feijidaier, Zhadan, Sidaierdan, Sidaierdui, Huojian };
const string TypeNameEnStr[NumOfType] = { "Danzhang", "Duizi", "Sanzhang", "Sandaiyi", "Sandaier", "Shunzi", "Liandui", "Feiji", "Feijidaiyi", "Feijidaier", "Zhadan", "Sidaierdan", "Sidaierdui", "Huojian" };
const string TypeNameCnStr[NumOfType] = { "����", "����", "����", "����һ", "������", "˳��", "����", "�ɻ�", "�ɻ���һ", "�ɻ�����", "ը��", "�Ĵ�����", "�Ĵ�����", "���" };
const bool TypeHasLength[NumOfType] = { false, false, false, false, false, true, true, true, true, true, false, false, false, false };
const int TypeMinLength[NumOfType] = { 0, 0, 0, 0, 0, 5, 3, 2, 2, 2, 0, 0, 0, 0 };
const int TypeMaxLength[NumOfType] = { 0, 0, 0, 0, 0, 12, 10, 6, 5, 4, 0, 0, 0, 0 };
const int TypeLengthDiv[NumOfType] = { 1, 1, 1, 1, 1, 1, 2, 3, 4, 5, 1, 1, 1, 1 };

const int SortNumToArrayNum[NumOfNumber] = { 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 1 };  //sortArray���±��ʾ������array�е��±�
const int ArrayNumToSortNum[NumOfNumber] = { -1, 11, 12, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 13, 14 };
const int PowToNum[NumOfNumber] = { 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 1, 2, 14, 15 };
const int NumToPow[NumOfNumber] = { -1, 11, 12, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 14, 15 };


struct TypeTest
{
	static bool hasInit;
	static void init();

	static vector<bool(*)(const Group&)> hasType;
	/*һ�����溯��ָ��Ŀɱ䳤���飬
	*����hasType[CardType](group)��ʾ��ָ�������뱣֤������ģ�����group�У��ж��Ƿ����CardType�͵���
	*/

	static vector<bool(*)(const Group&, int)> isType;
	/*һ�����溯��ָ��Ŀɱ䳤���飬
	*����isType[CardType](group,len)��ʾ�ж�ָ�������뱣֤������ģ�����group�Ƿ���CardType���ҳ��ȣ�������ڣ�Ϊlen����
	*��Ҫ�ж����ͳ��ȵ������У�˳�ӣ����ԣ��ɻ����ɻ���һ���ɻ����������Ƚ�Ϊ˳�ĳ��ȣ���33344479����Ϊ2��
	*������������Ҫ�жϳ��ȵ����ͣ�ʵ���н��ڶ���int������Ϊ��Ԫ
	*/

	static vector<int(*)(const Group&, int)> getPower;
	/*
	* Ӧ�ñ�֤������׼ȷ�����򲻱�֤��Ϊ���ȶ���
	*/

	TypeTest();

	static Group& makeGroup(string str, Group& group);  //��str�е�ÿһ��char��Ϊdigit�������鲢���򣬱�����group�У�����group
	static CardType getType(const Group& group, CardType& ct, int& len);      //�����һ��ƥ������ͼ����ȣ�����ƥ�䣬����NoneType
	static bool isInGroup(const Group& subGroup, const Group& mainGroup);    //�ж�subGroup�Ƿ���mainGroup�У������鶼��������
	static int getLength(const Group& group, CardType type);
	static int canOutPower(const Group& group, CardType type, int len, int power);
	//�ж�group�Ƿ���ѹ��ָ����type len power���ƣ�����ָ��type�����򷵻ظ��������power�����򷵻�-1
	static bool canOut(const Group& group, CardType type, int len, int power);
	//static int bestSplit(const Group& group, vector<Group>& bestWay);
	//static int countHandsWithoutCarry(const Group& group,vector<Group>& bestWay, int cntType = 0, int lastPow = 0);
	//static int TypeTest::getDiv(const Group& group, int cardNumber);
    static int getHands(const Group& group, vector<Group>& bestWay);
    static int getHandsWithoutCarry(const Group& group,vector<Group>& bestWay);
    static void test();
};

#endif // type
