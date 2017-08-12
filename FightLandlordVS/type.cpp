/*************************
** 文件:type.cpp
** 描述:牌型测试函数的实现，以及TypeTest结构的实现
** 备注:TypeTest的接口参见type.h
**************************/

#include "stdafx.h"
#include <iostream>
#include <vector>
#include <set>
#include <map>
#include <unordered_map>
#include <queue>
#include <cstring>
#include <algorithm>
#include <time.h>
#include <string>
#include "random.h"
#include "card.h"
#include "type.h"
#include "game.h"
using namespace std;

//enum CardType{Danzhang=0,Duizi,Sanzhang,Sandaiyi,Sandaier,Shunzi,Liandui,Feiji,Feijidaiyi,Feijidaier,Zhadan,Sidaierdan,Sidaierdui,Huojian};

bool TypeTest::hasInit = false;
vector<bool(*)(const Group&)> TypeTest::hasType;
vector<bool(*)(const Group&, int)> TypeTest::isType;
vector<int(*)(const Group&, int)> TypeTest::getPower;


void changeGroupToArray(const Group& group, int arr[], int arrlen = NumOfNumber)   //按照牌的number值统计
{
	memset(arr, 0, sizeof(int)*arrlen);
	for (auto p : group)
	{
		arr[p.getNum()]++;
	}
}

void changeGroupToSortArray(const Group& group, int arr[], int arrlen = NumOfNumber)   //只统计能组成顺子的牌的数组 arr[0]:3的张数 arr[10]:K的张数 arr[11]:A的张数
{
	memset(arr, 0, sizeof(int)*arrlen);
	for (auto p : group)
	{
		int cntnum = p.getNum();
		if (cntnum == 1)
			arr[11]++;
		else if (cntnum >= 3 && cntnum <= 13)
			arr[cntnum - 3]++;
	}
}

void changeGroupToPowerArray(const Group& group, int arr[], int arrlen = NumOfPow)	//类似changeGroupToSortArray，但是统计所有的牌
{
	memset(arr, 0, sizeof(int)*arrlen);
	for (auto p : group)
	{
		arr[p.getPower()]++;
	}
}

bool hasDanZhang(const Group& group)
{
	if (!group.empty())
	{
		return true;
	}
	else
	{
		return false;
	}
}

bool hasDuizi(const Group& group)
{
	//sort(group.begin(),group.end());
	if (group.size() < 2)
		return false;
	else
	{
		for (auto p = group.begin(); (p + 1) != group.end(); p++)
		{
			if (p->getNum() == (p + 1)->getNum())
				return true;
		}
		return false;
	}
	return false;
}

bool hasSanzhang(const Group& group)
{
	if (group.size() < 3)
		return false;
	else
	{
		for (auto p = group.begin(); (p + 2) != group.end(); p++)
		{
			int nowNum = p->getNum();
			if (nowNum == (p + 1)->getNum() && nowNum == (p + 2)->getNum())
				return true;
		}
		return false;
	}
	return false;
}

bool hasSandaiyi(const Group& group)
{
	if (group.size() < 4)
		return false;
	else if (group.size() == 4)
	{
		if (group[0].getNum() == group[3].getNum())    //炸弹不是三带一
			return false;
		else
			return true;
	}
	else
	{
		return hasSanzhang(group);
	}
	return false;
}

bool hasSandaier(const Group& group)
{
	int arr[NumOfNumber];    //大小为i的牌有多少张
	changeGroupToArray(group, arr);
	int Three = 0;
	int Two = 0;
	for (int i = 1; i <= 13; i++)
	{
		if (arr[i] >= 2)
			Two++;
		if (arr[i] >= 3)
			Three++;
	}
	return (Three >= 1) && (Two >= 2);
}

int maxLengthOfShunzi(const Group& group)
{
	int sortArr[NumOfNumber];
	changeGroupToSortArray(group, sortArr);
	int maxlen = 0;
	for (int i = 0; i < NumCanInShun; i++)
	{
		if (sortArr[i] != 0)
		{
			int tmplen = 0;
			while (i < NumCanInShun&&sortArr[i++] != 0)tmplen++;
			maxlen = max(maxlen, tmplen);
		}
	}
	return maxlen;
}

bool hasShunzi(const Group& group, int len)    //是否有长度为len的顺子
{
	return maxLengthOfShunzi(group) >= len;
}

bool hasShunzi(const Group& group)    //是否有顺子
{
	return hasShunzi(group, 5);
}

int maxLengthOfLiandui(const Group& group)
{
	int sortArr[NumOfNumber];
	changeGroupToSortArray(group, sortArr);
	int maxlen = 0;
	for (int i = 0; i < NumCanInShun; i++)
	{
		if (sortArr[i] >= 2)
		{
			int tmplen = 0;
			while (i < NumCanInShun&&sortArr[i++] >= 2)tmplen++;
			maxlen = max(maxlen, tmplen);
		}
	}
	return maxlen;
}

bool hasLiandui(const Group& group, int len)
{
	return maxLengthOfLiandui(group) >= len;
}

bool hasLiandui(const Group& group)
{
	return hasLiandui(group, 3);
}

int maxLengthOfFeiji(const Group& group)
{
	int sortArr[NumOfNumber];
	changeGroupToSortArray(group, sortArr);
	int maxlen = 0;
	for (int i = 0; i < NumCanInShun; i++)
	{
		if (sortArr[i] >= 3)
		{
			int tmplen = 0;
			while (i < NumCanInShun&&sortArr[i++] >= 3)tmplen++;
			maxlen = max(maxlen, tmplen);
		}
	}
	return maxlen;
}

bool hasFeiji(const Group& group, int len)
{
	return maxLengthOfFeiji(group) >= len;
}

bool hasFeiji(const Group& group)
{
	return hasFeiji(group, 2);
}

int maxLengthOfFeijidaiyi(const Group& group, int& pow)
{
	int sortArr[NumOfNumber];
	int arr[NumOfNumber];
	bool usedInFeiji[NumOfNumber];
	changeGroupToSortArray(group, sortArr);
	changeGroupToArray(group, arr);
	memset(usedInFeiji, 0, sizeof(bool)*NumOfNumber);
	int maxlen = 0;
	int maxPower = 0;
	for (int i = 0; i < NumCanInShun; i++)
	{
		if (sortArr[i] >= 3)
		{
			int tmplen = 0;
			int tmpst = i;
			int k = i;
			while (k < NumCanInShun&&sortArr[k++] >= 3)
			{
				tmplen++;

				if (tmplen >= maxlen)
				{
					memset(usedInFeiji, 0, sizeof(bool)*NumOfNumber);
					for (int j = 0; j < tmplen; j++)
					{
						usedInFeiji[SortNumToArrayNum[tmpst + j]] = true;
					}

					int maxYi = 0;
					for (int i = 1; i < NumOfNumber; i++)
					{
						if (!usedInFeiji[i])
							maxYi += arr[i];
						else
							maxYi += arr[i] - 3;
					}

					if (maxYi >= tmplen)
					{
						maxlen = tmplen;
						maxPower = tmpst;
					}
					else
						break;
				}
			}
		}
	}
	pow = maxPower;
	return maxlen;
}

int maxLengthOfFeijidaiyi(const Group& group)
{
	int pow;
	return maxLengthOfFeijidaiyi(group, pow);
}

bool hasFeijidaiyi(const Group& group, int len)
{
	int clen = maxLengthOfFeijidaiyi(group);
	return clen >= len;
}

bool hasFeijidaiyi(const Group& group)
{
	return hasFeijidaiyi(group, 2);
}

int maxLengthOfFeijidaier(const Group& group, int& pow)
{
	int sortArr[NumOfNumber];
	int arr[NumOfNumber];
	bool usedInFeiji[NumOfNumber];
	changeGroupToSortArray(group, sortArr);
	changeGroupToArray(group, arr);
	memset(usedInFeiji, 0, sizeof(bool)*NumOfNumber);
	int maxlen = 0;
	int maxPower = 0;
	for (int i = 0; i < NumCanInShun; i++)
	{
		if (sortArr[i] >= 3)
		{
			int tmplen = 0;
			int tmpst = i;
			int k = i;
			while (k < NumCanInShun&&sortArr[k++] >= 3)
			{
				tmplen++;

				if (tmplen > maxlen)
				{
					memset(usedInFeiji, 0, sizeof(bool)*NumOfNumber);
					for (int j = 0; j < tmplen; j++)
					{
						usedInFeiji[SortNumToArrayNum[tmpst + j]] = true;
					}

					int maxEr = 0;
					for (int i = 1; i < NumOfNumber; i++)
					{
						if (!usedInFeiji[i])
						{
							if (arr[i] >= 2)
								maxEr += arr[i] / 2;
						}
					}

					if (maxEr >= tmplen)
					{
						maxlen = tmplen;
						maxPower = tmpst;
					}
					else
						break;
				}
			}
		}
	}
	pow = maxPower;
	return maxlen;
}

int maxLengthOfFeijidaier(const Group& group)
{
	int pow;
	return maxLengthOfFeijidaier(group, pow);
}

bool hasFeijidaier(const Group& group, int len)
{
	int clen = maxLengthOfFeijidaier(group);
	return clen >= len;
}

bool hasFeijidaier(const Group& group)
{
	return hasFeijidaier(group, 2);
}

bool hasZhadan(const Group& group)
{
	int arr[NumOfNumber];
	changeGroupToArray(group, arr);
	for (int i = 1; i <= 13; i++)
	{
		if (arr[i] >= 4)
			return true;
	}
	return false;
}

bool hasSidaierdan(const Group& group)
{
	return hasZhadan(group) && (group.size() >= 6);
}

bool hasSidaierdui(const Group& group)
{
	if (!hasZhadan(group))
		return false;
	int arr[NumOfNumber];
	changeGroupToArray(group, arr);
	int Two = 0;
	for (int i = 1; i <= 13; i++)
	{
		Two += arr[i] / 2;
	}
	if (Two >= 4)
		return true;
	return false;
}

bool hasHuojian(const Group& group)
{
	int l = group.size();
	return (group[l - 1].getId() == RedJoker) && (group[l - 2].getId() == BlackJoker);
}

bool isDanZhang(const Group& group, int)
{
	return group.size() == 1;
}

bool isDuizi(const Group& group, int)
{
	if (group.size() != 2)
		return false;
	else
		return group[0].getNum() == group[1].getNum();
}

bool isSanzhang(const Group& group, int)
{
	if (group.size() != 3)
		return false;
	else
		return group[0].getNum() == group[2].getNum();    //因为要求已排序，故只需比较两张牌
}

bool isSandaiyi(const Group& group, int)
{
	if (group.size() != 4)
		return false;
	else if (group[0].getNum() == group[3].getNum())
		return false;
	else
		return group[0].getNum() == group[2].getNum() || group[1].getNum() == group[3].getNum();
}

bool isSandaier(const Group& group, int)
{
	if (group.size() != 5)
		return false;
	else if (group[0].getNum() == group[2].getNum())
	{
		if (group[3].getNum() == group[4].getNum())
			return true;
		else
			return false;
	}
	else if (group[2].getNum() == group[4].getNum())
	{
		if (group[0].getNum() == group[1].getNum())
			return true;
		else
			return false;
	}
	return false;
}

bool isShunzi(const Group& group, int len)
{
	if (len > TypeMaxLength[Shunzi] || len<TypeMinLength[Shunzi])
		return false;
	if ((int)group.size() != len)
		return false;
	else
	{
		for (int i = 0; i < len - 1; i++)
		{
			if (group[i].getPower() != (group[i + 1].getPower() - 1))
				return false;
			if (group[i + 1].getPower() > ArrayNumToSortNum[1])
				return false;
		}
		return true;
	}
}

bool isLiandui(const Group& group, int len)
{
	if (len>TypeMaxLength[Liandui] || len < TypeMinLength[Liandui])
		return false;
	if ((int)group.size() != len * 2)
		return false;
	else
	{
		int cnt = group[0].getPower();
		for (int i = 0; i<len; i++)
		{
			if (group[i * 2].getPower() != group[i * 2 + 1].getPower())
				return false;
			if (group[i * 2].getPower() != cnt)
				return false;
			if (cnt>ArrayNumToSortNum[1])
				return false;
			cnt++;
		}
		return true;
	}
}

bool isFeiji(const Group& group, int len)
{
	if (len > TypeMaxLength[Feiji] || len < TypeMinLength[Feiji])
		return false;
	if ((int)group.size() != len * 3)
		return false;
	else
	{
		int cnt = group[0].getPower();
		for (int i = 0; i<len; i++)
		{
			if (group[i * 3].getPower() != group[i * 3 + 1].getPower() || group[i * 3].getPower() != group[i * 3 + 2].getPower())
				return false;
			if (group[i * 3].getPower() != cnt)
				return false;
			if (cnt>ArrayNumToSortNum[1])
				return false;
			cnt++;
		}
		return true;
	}
}

bool isFeijidaiyi(const Group& group, int len)
{
	if (len > TypeMaxLength[Feijidaiyi] || len<TypeMinLength[Feijidaiyi])
		return false;
	if ((int)group.size() != len * 4)
		return false;
	return maxLengthOfFeijidaiyi(group) == len;
}

bool isFeijidaier(const Group& group, int len)
{
	if (len>TypeMaxLength[Feijidaier] || len < TypeMinLength[Feijidaier])
		return false;
	if ((int)group.size() != len * 5)
		return false;
	return maxLengthOfFeijidaier(group) == len;
}

bool isZhadan(const Group& group, int)
{
	if (group.size() != 4)
		return false;
	else
		return group[0].getNum() == group[3].getNum();
}

bool isSidaierdan(const Group& group, int)
{
	if (group.size() != 6)
		return false;
	else if (group[0].getNum() == group[3].getNum())
		return true;
	else if (group[1].getNum() == group[4].getNum())
		return true;
	else if (group[2].getNum() == group[5].getNum())
		return true;
	else
		return false;
}

bool isSidaierdui(const Group& group, int)
{
	if (group.size() != 8)
		return false;
	return hasSidaierdui(group);
}

bool isHuojian(const Group& group, int)
{
	if (group.size() != 2)
		return false;
	else
		return hasHuojian(group);
}

int getDanzhang(const Group& group, int)
{
	return group[0].getPower();
}

int getDuizi(const Group& group, int)
{
	return group[0].getPower();
}

int getSanzhang(const Group& group, int)
{
	return group[0].getPower();
}

int getSandaiyi(const Group& group, int)
{
	if (group[0].getNum() == group[2].getNum())
		return group[0].getPower();
	else
		return group[1].getPower();
}

int getSandaier(const Group& group, int)
{
	if (group[0].getNum() == group[2].getNum())
		return group[0].getPower();
	else
		return group[2].getPower();
}

int getShunzi(const Group& group, int)
{
	return group[0].getPower();
}

int getLiandui(const Group& group, int)
{
	return group[0].getPower();
}

int getFeiji(const Group& group, int)
{
	return group[0].getPower();
}

int getFeijidaiyi(const Group& group, int)
{
	int pow;
	maxLengthOfFeijidaiyi(group, pow);
	return pow;
}

int getFeijidaier(const Group& group, int)
{
	int pow;
	maxLengthOfFeijidaier(group, pow);
	return pow;
}

int getZhadan(const Group& group, int)
{
	return group[0].getPower();
}

int getSidaierdan(const Group& group, int)
{
	if (group[0].getNum() == group[3].getNum())
		return group[0].getPower();
	else if (group[1].getNum() == group[4].getNum())
		return group[1].getPower();
	else
		return group[2].getPower();
}

int getSidaierdui(const Group& group, int)
{
	if (group[0].getNum() == group[3].getNum())
		return group[0].getPower();
	else if (group[2].getNum() == group[5].getNum())
		return group[2].getPower();
	else
		return group[4].getPower();
}

int getHuojian(const Group& group, int)  //火箭的大小无关紧要，大概吧
{
	return 0;
}

TypeTest::TypeTest()
{
	if (hasInit)
		return;
	TypeTest::init();
}

void TypeTest::init()
{
	if (hasInit)
		return;

	hasInit = true;

	hasType.push_back(hasDanZhang);
	hasType.push_back(hasDuizi);
	hasType.push_back(hasSanzhang);
	hasType.push_back(hasSandaiyi);
	hasType.push_back(hasSandaier);
	hasType.push_back(hasShunzi);
	hasType.push_back(hasLiandui);
	hasType.push_back(hasFeiji);
	hasType.push_back(hasFeijidaiyi);
	hasType.push_back(hasFeijidaier);
	hasType.push_back(hasZhadan);
	hasType.push_back(hasSidaierdan);
	hasType.push_back(hasSidaierdui);
	hasType.push_back(hasHuojian);

	isType.push_back(isDanZhang);
	isType.push_back(isDuizi);
	isType.push_back(isSanzhang);
	isType.push_back(isSandaiyi);
	isType.push_back(isSandaier);
	isType.push_back(isShunzi);
	isType.push_back(isLiandui);
	isType.push_back(isFeiji);
	isType.push_back(isFeijidaiyi);
	isType.push_back(isFeijidaier);
	isType.push_back(isZhadan);
	isType.push_back(isSidaierdan);
	isType.push_back(isSidaierdui);
	isType.push_back(isHuojian);

	getPower.push_back(getDanzhang);
	getPower.push_back(getDuizi);
	getPower.push_back(getSanzhang);
	getPower.push_back(getSandaiyi);
	getPower.push_back(getSandaier);
	getPower.push_back(getShunzi);
	getPower.push_back(getLiandui);
	getPower.push_back(getFeiji);
	getPower.push_back(getFeijidaiyi);
	getPower.push_back(getFeijidaier);
	getPower.push_back(getZhadan);
	getPower.push_back(getSidaierdan);
	getPower.push_back(getSidaierdui);
	getPower.push_back(getHuojian);
}

Group& TypeTest::makeGroup(string str, Group& group)
{
	int len = str.length();
	for (int i = 0; i < len; i++)
	{
		group.push_back(Card(str[i]));
	}
	sort(group.begin(), group.end());
	return group;
}

CardType TypeTest::getType(const Group& group, CardType& ct, int& len)
{
	for (int i = 0; i < NumOfType; i++)
	{
		for (int j = TypeMinLength[i]; j <= TypeMaxLength[i]; j++)
		{
			if (TypeTest::isType[i](group, j))
			{
				ct = (CardType)i;
				len = j;
				return (CardType)i;
			}
		}
	}
	ct = NoneType;
	len = 0;
	return NoneType;
}

bool TypeTest::isInGroup(const Group& subGroup, const Group& mainGroup)
{
	auto p = subGroup.begin();
	auto q = mainGroup.begin();
	while (p != subGroup.end() && q != mainGroup.end())
	{
		if (p->getNum() == q->getNum())
		{
			p++;
			q++;
		}
		else
		{
			q++;
		}
	}
	if (p == subGroup.end())
		return true;
	else
		return false;
}

int TypeTest::getLength(const Group& group, CardType type)
{
	return group.size() / TypeLengthDiv[type];
}

int TypeTest::canOutPower(const Group& group, CardType type, int len, int power)    //返回-1或power
{
    if (type == NoneType)
    {
        TypeTest::getType(group, type, len);
        return getPower[type](group, len);
    }
	else if (isType[Huojian](group, len))
	{
		return 0;
	}
	else if (type != Zhadan&&isType[Zhadan](group, len))
	{
		return getPower[Zhadan](group, len);
	}
	else if (!isType[type](group, len))
	{
		//showMessage("所给牌型或长度与牌组不相符。");
		return -1;
	}
	else
	{
		int pow = getPower[type](group, len);
		if (pow <= power)
		{
			//showMessage("所给牌型比应出牌型小。");
			return -1;
		}
		return pow;
	}
}

bool TypeTest::canOut(const Group& group, CardType type, int len, int power)
{
	if (canOutPower(group, type, len, power) == -1)
		return false;
	else
		return true;
}

long long powerArrayToLongLong(const int powerArray[])		//其实算一个hash
{
	long long ans = 0;
	for (int i = 0; i < NumOfPow; i++)
	{
		long long tmp = (long long)powerArray[i] << (3 * i);
		ans += tmp;
	}
	return ans;
}

void longLongToPowerArray(long long x, int powerArray[]) {
    for (int i = 0; i < NumOfPow; i++) {
        powerArray[i] = (int)(x & 0x7L);
        x >>= 3;
    }
}

int getRestDistance(long long x) {
    int ans = 0;
    while (x != 0)
    {
        ans += (x & 0x7L) ? 1 : 0;
        x >>= 3;
    }
    return ans;
}

int getRestDistanceInCarry(long long x, int fn)     //fn为飞机中的个数
{
    int num[5] = { 0 };
    int ans = 0;
    if (x >> 36 == 9)   //有王炸，那么王炸不拆。
    {
        ans++;
        x -= (long long)9 << 36;
    }
    while (x != 0)
    {
        num[x & 0x7L]++;
        x >>= 3;
    }

    if (num[1] >= fn)       //先满足飞机带牌
        num[1] -= fn;
    else if (num[2] >= fn)
        num[2] -= fn;
    else if (num[1] + 2 * num[2] >= fn)
    {
        fn -= num[1];
        num[1] = 0;
        num[2] -= ((fn + 1) >> 1);
        if (fn % 2 != 0)
            num[1]++;
    }

    while (num[3] != 0)
    {
        if (num[1] != 0)
            num[1]--;
        else if (num[2] != 0)
            num[2]--;
        num[3]--;
        ans++;
    }

    while (num[4] != 0)
    {
        if (num[1] >= 2)
            num[1] -= 2;
        else if (num[2] >= 2)
            num[2] -= 2;
        num[4]--;
        ans++;
    }

    ans += num[1];
    ans += num[2];
    return ans;
}

int TypeTest::getHands(const Group& g, vector<Group>& bestWay)
{
    unordered_map<long long, vector<long long> > last;		//最佳拆牌的上一步
    unordered_map<long long, int> dis;	//起点到该处的距离
    queue<long long> q;					//广度优先搜索
    int minDistance;        //包括将非顺出完的距离
    vector<long long> ans;

    int stArr[NumOfPow];		//开始牌组
    changeGroupToPowerArray(g, stArr);

    long long st = powerArrayToLongLong(stArr);
    dis[st] = 0;
    q.push(st);
    minDistance = getRestDistanceInCarry(st, 0);
    ans.push_back(st);


    //int tst = clock();
    while (!q.empty())        //广度优先搜索
    {
        long long x = q.front();
        q.pop();
        //cout << "in:" << oct << x << dec << endl;

        int arr[NumOfPow];
        int tmpArr[NumOfPow];
        longLongToPowerArray(x, arr);

        for (int i = 0; i < MaxSortPow; i++)
        {
            memcpy(tmpArr, arr, sizeof(arr));
            for (int j = i; j < MaxSortPow; j++)
            {
                if (tmpArr[j] < 1)   break;
                tmpArr[j] -= 1;
                if (j - i + 1 >= TypeMinLength[Shunzi])
                {
                    long long nt = powerArrayToLongLong(tmpArr);
                    if (dis.find(nt) != dis.end() && dis[nt] == dis[x] + 1)         //不是第一个且和上一个距离一致
                        last[nt].push_back(x);
                    else if (dis.find(nt) == dis.end())
                    {
                        q.push(nt);
                        dis[nt] = dis[x] + 1;
                        last[nt].push_back(x);      //更新q,dis,last

                        int tmpdis = dis[nt] + getRestDistanceInCarry(nt, 0);
                        if (minDistance > tmpdis)   //更新 minDistance 和 ans
                        {
                            ans.clear();
                            ans.push_back(nt);
                            minDistance = tmpdis;
                        }
                        else if (minDistance == tmpdis)
                            ans.push_back(nt);
                    }
                }
            }
        }   //End for i

        for (int i = 0; i < MaxSortPow; i++)        //连对
        {
            memcpy(tmpArr, arr, sizeof(arr));
            for (int j = i; j < MaxSortPow; j++)
            {
                if (tmpArr[j] < 2)   break;
                tmpArr[j] -= 2;
                if (j - i + 1 >= TypeMinLength[Liandui])
                {
                    long long nt = powerArrayToLongLong(tmpArr);
                    if (dis.find(nt) != dis.end() && dis[nt] == dis[x] + 1)         //不是第一个且和上一个距离一致
                        last[nt].push_back(x);
                    else if (dis.find(nt) == dis.end())
                    {
                        q.push(nt);
                        dis[nt] = dis[x] + 1;
                        last[nt].push_back(x);      //更新q,dis,last

                        int tmpdis = dis[nt] + getRestDistanceInCarry(nt,0);
                        if (minDistance > tmpdis)   //更新 minDistance 和 ans
                        {
                            ans.clear();
                            ans.push_back(nt);
                            minDistance = tmpdis;
                        }
                        else if (minDistance == tmpdis)
                            ans.push_back(nt);
                    }
                }
            }
        }//End for i

        for (int i = 0; i < MaxSortPow; i++)
        {
            memcpy(tmpArr, arr, sizeof(arr));
            for (int j = i; j < MaxSortPow; j++)
            {
                if (tmpArr[j] < 3)   break;
                tmpArr[j] -= 3;
                if (j - i + 1 >= TypeMinLength[Feiji])
                {
                    long long nt = powerArrayToLongLong(tmpArr);
                    if (dis.find(nt) != dis.end() && dis[nt] == dis[x] + 1)         //不是第一个且和上一个距离一致
                        last[nt].push_back(x);
                    else if (dis.find(nt) == dis.end())
                    {
                        q.push(nt);
                        dis[nt] = dis[x] + 1;
                        last[nt].push_back(x);      //更新q,dis,last

                        int tmpdis = dis[nt] + getRestDistanceInCarry(nt, j - i + 1);
                        if (minDistance > tmpdis)   //更新 minDistance 和 ans
                        {
                            ans.clear();
                            ans.push_back(nt);
                            minDistance = tmpdis;
                        }
                        else if (minDistance == tmpdis)
                            ans.push_back(nt);
                    }
                }
            }
        }//End for i

    }   //End While

    //int ted = clock();
    //cout << "BFS Time: " << (ted - tst) << "ms" << endl;
    //cout << "BFS Finished!" << endl;

    bestWay.clear();
    long long x = ans[0];
    long long y;
    int ansArr1[NumOfPow];
    int ansArr2[NumOfPow];
    longLongToPowerArray(x, ansArr1);

    vector<int> num[5];
    int countSt = 14;
    if (ansArr1[13] == 1 && ansArr1[14] == 1)
    {
        countSt = 12;
        Group tmpGroup;
        tmpGroup.push_back(Card(BlackJoker));
        tmpGroup.push_back(Card(RedJoker));
        bestWay.push_back(tmpGroup);
    }
    for (int i = countSt; i >=0; i--)    //小牌先带走，有王炸，则不带
    {
        num[ansArr1[i]].push_back(i);
    }

    bool flag = false;
    while (x != st)
    {
        y = last[x][0];
        longLongToPowerArray(y, ansArr2);
        int cntArr[NumOfPow];
        Group tmpGroup;
        tmpGroup.clear();
        int fn = 0;
        for (int i = 0; i < NumOfPow; i++)
        {
            cntArr[i] = ansArr2[i] - ansArr1[i];
            switch (cntArr[i])
            {
            case 3:
                tmpGroup.push_back(Card(1, PowToNum[i]));
                fn++;
            case 2:
                tmpGroup.push_back(Card(2, PowToNum[i]));
            case 1:
                tmpGroup.push_back(Card(3, PowToNum[i]));
            default:
                break;
            }

        }

        if (fn>0 && !flag)
        {
            flag = true;
            if ((int)num[1].size() >= fn)       //先满足飞机带牌
            {
                for (int i = 0; i < fn; i++)
                {
                    tmpGroup.push_back(Card(0, PowToNum[num[1].back()]));
                    num[1].pop_back();
                }
            }
            else if ((int)num[2].size() >= fn)
            {
                for (int i = 0; i < fn; i++)
                {
                    tmpGroup.push_back(Card(0, PowToNum[num[2].back()]));
                    tmpGroup.push_back(Card(1, PowToNum[num[2].back()]));
                    num[2].pop_back();
                }
            }
            else if ((int)(num[1].size() + 2 * num[2].size()) >= fn)
            {
                while (!num[1].empty())
                {
                    tmpGroup.push_back(Card(0, PowToNum[num[1].back()]));
                    num[1].pop_back();
                    fn--;
                }
                while (fn > 0)
                {
                    tmpGroup.push_back(Card(0, PowToNum[num[2].back()]));
                    tmpGroup.push_back(Card(1, PowToNum[num[2].back()]));
                    num[2].pop_back();
                    fn -= 2;
                }
                if (fn == -1)
                {
                    num[1].push_back(tmpGroup.back().getPower());
                    tmpGroup.pop_back();
                }
            }
        }

        if (!tmpGroup.empty())
            bestWay.push_back(tmpGroup);
        x = y;
        memcpy(ansArr1, ansArr2, sizeof(ansArr1));
    }

    while (num[3].size() != 0)
    {
        Group tmpGroup;
        for (int i = 0; i < 3; i++)
            tmpGroup.push_back(Card(i, PowToNum[num[3].back()]));
        num[3].pop_back();
        if (num[1].size() >= 1)
        {
            tmpGroup.push_back(Card(0, PowToNum[num[1].back()]));
            num[1].pop_back();
        }
        else if (num[2].size() >= 1)
        {
            tmpGroup.push_back(Card(0, PowToNum[num[2].back()]));
            num[2].pop_back();
        }
        bestWay.push_back(tmpGroup);
    }

    while (num[4].size() != 0)
    {
        Group tmpGroup;
        for (int i = 0; i < 4; i++)
            tmpGroup.push_back(Card(i, PowToNum[num[4].back()]));
        num[4].pop_back();
        if (num[1].size() >= 2)
        {
            tmpGroup.push_back(Card(0, PowToNum[num[1].back()]));
            num[1].pop_back();
            tmpGroup.push_back(Card(0, PowToNum[num[1].back()]));
            num[1].pop_back();
        }
        else if (num[2].size() >= 2)
        {
            tmpGroup.push_back(Card(0, PowToNum[num[2].back()]));
            num[2].pop_back();
            tmpGroup.push_back(Card(0, PowToNum[num[2].back()]));
            num[2].pop_back();
        }
        bestWay.push_back(tmpGroup);
    }

    while (num[1].size() != 0)
    {
        Group tmpGroup;
        tmpGroup.push_back(Card(0, PowToNum[num[1].back()]));
        num[1].pop_back();
        bestWay.push_back(tmpGroup);
    }
    while (num[2].size() != 0)
    {
        Group tmpGroup;
        tmpGroup.push_back(Card(0, PowToNum[num[2].back()]));
        tmpGroup.push_back(Card(1, PowToNum[num[2].back()]));
        num[2].pop_back();
        bestWay.push_back(tmpGroup);
    }

    return bestWay.size();
}

int TypeTest::getHandsWithoutCarry(const Group& g, vector<Group>& bestWay)
{
    unordered_map<long long, vector<long long> > last;		//最佳拆牌的上一步
    unordered_map<long long, int> dis;	//起点到该处的距离
    queue<long long> q;					//广度优先搜索
    int minDistance;        //包括将非顺出完的距离
    vector<long long> ans;

    int stArr[NumOfPow];		//开始牌组
    changeGroupToPowerArray(g, stArr);

    long long st = powerArrayToLongLong(stArr);
    dis[st] = 0;
    q.push(st);
    minDistance = getRestDistance(st);
    ans.push_back(st);


    int tst = clock();
    while (!q.empty())        //广度优先搜索
    {
        long long x = q.front();
        q.pop();
        //cout << "in:" << oct << x << dec << endl;

        int arr[NumOfPow];
        int tmpArr[NumOfPow];
        longLongToPowerArray(x, arr);

        for (int i = 0; i < MaxSortPow; i++)
        {
            memcpy(tmpArr, arr, sizeof(arr));
            for (int j = i; j < MaxSortPow; j++)
            {
                if (tmpArr[j] < 1)   break;
                tmpArr[j] -= 1;
                if (j - i + 1 >= TypeMinLength[Shunzi])
                {
                    long long nt = powerArrayToLongLong(tmpArr);
                    if (dis.find(nt) != dis.end() && dis[nt] == dis[x] + 1)         //不是第一个且和上一个距离一致
                        last[nt].push_back(x);
                    else if (dis.find(nt) == dis.end())
                    {
                        q.push(nt);
                        dis[nt] = dis[x] + 1;
                        last[nt].push_back(x);      //更新q,dis,last

                        int tmpdis = dis[nt] + getRestDistance(nt);
                        if (minDistance > tmpdis)   //更新 minDistance 和 ans
                        {
                            ans.clear();
                            ans.push_back(nt);
                            minDistance = tmpdis;
                        }
                        else if (minDistance == tmpdis)
                            ans.push_back(nt);
                    }
                }
            }
        }   //End for i

        for (int i = 0; i < MaxSortPow; i++)        //连对
        {
            memcpy(tmpArr, arr, sizeof(arr));
            for (int j = i; j < MaxSortPow; j++)
            {
                if (tmpArr[j] < 2)   break;
                tmpArr[j] -= 2;
                if (j - i + 1 >= TypeMinLength[Liandui])
                {
                    long long nt = powerArrayToLongLong(tmpArr);
                    if (dis.find(nt) != dis.end() && dis[nt] == dis[x] + 1)         //不是第一个且和上一个距离一致
                        last[nt].push_back(x);
                    else if (dis.find(nt) == dis.end())
                    {
                        q.push(nt);
                        dis[nt] = dis[x] + 1;
                        last[nt].push_back(x);      //更新q,dis,last

                        int tmpdis = dis[nt] + getRestDistance(nt);
                        if (minDistance > tmpdis)   //更新 minDistance 和 ans
                        {
                            ans.clear();
                            ans.push_back(nt);
                            minDistance = tmpdis;
                        }
                        else if (minDistance == tmpdis)
                            ans.push_back(nt);
                    }
                }
            }
        }//End for i

        for (int i = 0; i < MaxSortPow; i++)
        {
            memcpy(tmpArr, arr, sizeof(arr));
            for (int j = i; j < MaxSortPow; j++)
            {
                if (tmpArr[j] < 3)   break;
                tmpArr[j] -= 3;
                if (j - i + 1 >= TypeMinLength[Feiji])
                {
                    long long nt = powerArrayToLongLong(tmpArr);
                    if (dis.find(nt) != dis.end() && dis[nt] == dis[x] + 1)         //不是第一个且和上一个距离一致
                        last[nt].push_back(x);
                    else if (dis.find(nt) == dis.end())
                    {
                        q.push(nt);
                        dis[nt] = dis[x] + 1;
                        last[nt].push_back(x);      //更新q,dis,last

                        int tmpdis = dis[nt] + getRestDistance(nt);
                        if (minDistance > tmpdis)   //更新 minDistance 和 ans
                        {
                            ans.clear();
                            ans.push_back(nt);
                            minDistance = tmpdis;
                        }
                        else if (minDistance == tmpdis)
                            ans.push_back(nt);
                    }
                }
            }
        }//End for i

    }   //End While

    int ted = clock();
    cout << "BFS Time: " << (ted - tst) << "ms" << endl;
    //cout << "BFS Finished!" << endl;

    bestWay.clear();
    long long x = ans[0];
    long long y;
    int ansArr1[NumOfPow];
    int ansArr2[NumOfPow];
    longLongToPowerArray(x, ansArr1);
    for (int i = 0; i < 12; i++)
    {
        Group tmpGroup;
        switch (ansArr1[i])
        {
        case 4:
            tmpGroup.push_back(Card(0, PowToNum[i]));
        case 3:
            tmpGroup.push_back(Card(1, PowToNum[i]));
        case 2:
            tmpGroup.push_back(Card(2, PowToNum[i]));
        case 1:
            tmpGroup.push_back(Card(3, PowToNum[i]));
        default:
            break;
        }
        if (!tmpGroup.empty())
            bestWay.push_back(tmpGroup);
    }

    if (ansArr1[13] != 0 && ansArr1[14] != 0)
    {
        Group tmpGroup;
        tmpGroup.push_back(Card(BlackJoker));
        tmpGroup.push_back(Card(RedJoker));
        bestWay.push_back(tmpGroup);
    }
    else if (ansArr1[13] != 0)
    {
        Group tmpGroup;
        tmpGroup.push_back(Card(BlackJoker));
        bestWay.push_back(tmpGroup);
    }
    else if (ansArr1[14] != 0)
    {
        Group tmpGroup;
        tmpGroup.push_back(Card(RedJoker));
        bestWay.push_back(tmpGroup);
    }

    while (x != st)
    {
        y = last[x][0];
        longLongToPowerArray(y, ansArr2);
        int cntArr[NumOfPow];
        Group tmpGroup;
        tmpGroup.clear();
        for (int i = 0; i < NumOfPow; i++)
        {
            cntArr[i] = ansArr2[i] - ansArr1[i];
            switch (cntArr[i])
            {
            case 4:
                tmpGroup.push_back(Card(0, PowToNum[i]));
            case 3:
                tmpGroup.push_back(Card(1, PowToNum[i]));
            case 2:
                tmpGroup.push_back(Card(2, PowToNum[i]));
            case 1:
                tmpGroup.push_back(Card(3, PowToNum[i]));
            default:
                break;
            }
        }
        if (!tmpGroup.empty())
            bestWay.push_back(tmpGroup);
        x = y;
        memcpy(ansArr1, ansArr2, sizeof(ansArr1));
    }
    return bestWay.size();
}



void TypeTest::test()
{
	string str;
	cin >> str;
	Group g;
	TypeTest::makeGroup(str, g);
	int arr[NumOfPow];
	changeGroupToPowerArray(g, arr);

	for (auto xi : arr)
		cout << xi << ' ';
	cout << endl;

	long long x = powerArrayToLongLong(arr);
    cout << oct << x << dec << endl;

	int arr2[NumOfPow];
	longLongToPowerArray(x, arr2);

	for (auto xi : arr2)
		cout << xi << ' ';
	cout << endl;

    vector<Group> bestWay;
    int tst = clock();
    getHandsWithoutCarry(g, bestWay);
    int ted = clock();
    for (auto xi : bestWay)
    {
        for (auto ti : xi)
            ti.prtDigit();
        cout << ' ';
    }
    cout << endl;
    cout << "time: " << (ted - tst) << "ms" << endl;

    tst = clock();
    TypeTest::getHands(g, bestWay);
    ted = clock();
    for (auto xi : bestWay)
    {
        for (auto ti : xi)
            ti.prtDigit();
        cout << ' ';
    }
    cout << endl;
    cout << "time: " << (ted - tst) << "ms" << endl;

}


/*
int TypeTest::countHandsWithoutCarry(const Group& group,vector<Group>& bestWay, int cnt, int lastPow)	//按顺序处理顺子、连对、飞机、散牌，cnt=0-3表示处理到的种类，lastPow表示处理到的顺子开头大小
{
bestWay.clear();

int arr[NumOfNumber];
changeGroupToPowerArray(group, arr); 

int ans = INT_MAX;

if (cnt == 0)
{
if (lastPow + 5 > MaxSortPow)	//若已经不再可能形成顺子
{
cnt = 1;
lastPow = 0;
}
else
{
for (int i = lastPow; i < MaxSortPow; i++)
{
for (int j = i; j < MaxSortPow; j++)
{
if (arr[j] == 0)
{
i = j;
break;
}
else if (j - i + 1 >= 5)	//长度已足够
{
Group g;
Group gs;
for (int k = 0; k < NumOfNumber; k++)	//生成一个去掉该顺子的牌组。
{
int t = 0;
if (k <= j&&k >= i)
{
t++;
gs.push_back(Card(NumToDigit[PowToNum[k]]));
}
for (; t < arr[k]; t++)
g.push_back(Card(NumToDigit[PowToNum[k]]));
}

int tmpans = 0;
vector<Group> tmpvg;
tmpans = TypeTest::countHandsWithoutCarry(g, tmpvg, 0, i) + 1;
if (tmpans < ans)
{
ans = tmpans;
bestWay.clear();
for (auto x : tmpvg)
{
bestWay.push_back(x);
}
bestWay.push_back(gs);
}
}
}
}
cnt++;
lastPow = 0;
}
}

if (cnt == 1)
{
if (lastPow + 3 > MaxSortPow)	//若已经不再可能形成连对
{
cnt = 2;
lastPow = 0;
}
else
{
for (int i = lastPow; i < MaxSortPow; i++)
{
for (int j = i; j < MaxSortPow; j++)
{
if (arr[j] < 2)
{
i = j;
break;
}
else if (j - i + 1 >= 3)	//长度已足够
{
Group g;
Group gs;
for (int k = 0; k < NumOfNumber; k++)	//生成一个去掉该顺子的牌组。
{
int t = 0;
if (k <= j&&k >= i)
{
t = 2;
gs.push_back(Card(NumToDigit[PowToNum[k]]));
gs.push_back(Card(NumToDigit[PowToNum[k]]));
}
for (; t < arr[k]; t++)
g.push_back(Card(NumToDigit[PowToNum[k]]));
}

int tmpans = 0;
vector<Group> tmpvg;
tmpans = TypeTest::countHandsWithoutCarry(g, tmpvg, 1, i) + 1;
if (tmpans < ans)
{
ans = tmpans;
bestWay.clear();
for (auto x : tmpvg)
{
bestWay.push_back(x);
}
bestWay.push_back(gs);
}
}
}
}
cnt++;
lastPow = 0;
}
}

if (cnt == 2)
{
if (lastPow + 2 > MaxSortPow)	//若已经不再可能形成飞机
{
cnt = 3;
lastPow = 0;
}
else
{
for (int i = lastPow; i < MaxSortPow; i++)
{
for (int j = i; j < MaxSortPow; j++)
{
if (arr[j] < 3)
{
i = j;
break;
}
else if (j - i + 1 >= 3)	//长度已足够
{
Group g;
Group gs;
for (int k = 0; k < NumOfNumber; k++)	//生成一个去掉该飞机的牌组。
{
int t = 0;
if (k <= j&&k >= i)
{
t = 3;
gs.push_back(Card(NumToDigit[PowToNum[k]]));
gs.push_back(Card(NumToDigit[PowToNum[k]]));
gs.push_back(Card(NumToDigit[PowToNum[k]]));
}
for (; t < arr[k]; t++)
g.push_back(Card(NumToDigit[PowToNum[k]]));
}

int tmpans = 0;
vector<Group> tmpvg;
tmpans = TypeTest::countHandsWithoutCarry(g, tmpvg, 2, i) + 1;
if (tmpans < ans)
{
ans = tmpans;
bestWay.clear();
for (auto x : tmpvg)
{
bestWay.push_back(x);
}
bestWay.push_back(gs);
}
}
}
}
cnt++;
lastPow = 0;
}
}


if (cnt == 3)
{
int tmpans = 0;
vector<Group> tmpvg;
for (int i = 0; i < NumOfNumber; i++)
{
if (arr[i]>0)
{
tmpans++;
Group tg;
for (int j = 0; j < arr[i]; j++)
{
tg.push_back(Card(NumToDigit[PowToNum[i]]));
}
tmpvg.push_back(tg);
}
}
if (tmpans < ans)
{
ans = tmpans;
for (auto x : tmpvg)
{
bestWay.push_back(x);
}
}
}
return ans;
}*/

/*
int TypeTest::getDiv(const Group& group, int cardNumber)	//判断一张牌在牌组中是否为散牌，1是散牌，0可作为散牌，负数不是散牌，负数时绝对值越大越重要
{
int arr[NumOfNumber];
changeGroupToArray(group,arr);
if (arr[cardNumber] == 0)
{
showError("错误：判断牌组中不存在牌的散度。（将返回-1，不是散牌）");
return -1;
}
else
{
int origin;
int remove;
vector<Group> vg;
origin = TypeTest::countHandsWithoutCarry(group, vg);
arr[cardNumber]--;
Group g;
for (int i = 0; i < NumOfNumber; i++)
{
for (int j = 0; j < arr[i]; j++)
g.push_back(Card(NumToDigit[i]));
}
remove = TypeTest::countHandsWithoutCarry(g, vg);
return origin - remove;
}
}

*/

/*
int TypeTest::bestSplit(const Group& group, vector<Group>& bestWay)     //已废弃，用getHands取代
{
	//未实现
	return 0;
}
*/

