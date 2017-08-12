#include "stdafx.h"
#include "action_lib.h"

using namespace std;

typedef LongLongGroupUtil Util;
typedef pair<CardType, int> pci;
vector<long long> ActionLib::lib[NumOfType][MaxTypeLen][NumOfPow];
unordered_map<long long, pair<CardType, int> > ActionLib::findTypeLib;
unordered_map<long long, int> ActionLib::findPowLib[NumOfType][MaxTypeLen];

void ActionLib::initFeijidaiyi(vector<int>& carrys)
{
    long long group;
    int len;
    if (carrys.size() == 2u)
    {
        len = 2;
        for (int pow = 0; pow < MaxSortPow - len + 1; pow++)
        {
            group = 0;
            for (int i = 0; i < len; i++)
                group = Util::addCard(group, pow + i, 3);
            for (auto x : carrys)
                group = Util::addCard(group, x);
            if (Util::isLegal(group))
            {
                lib[Feijidaiyi][len][pow].push_back(group);
                if (findTypeLib.find(group) == findTypeLib.end())
                {
                    findTypeLib[group] = pci(Feijidaiyi, len);
                    findPowLib[Feijidaiyi][len][group] = pow;
                }
            }
        }

    }
    else if (carrys.size() == 3u)
    {
        len = 3;
        for (int pow = 0; pow < MaxSortPow - len + 1; pow++)
        {
            group = 0;
            for (int i = 0; i < len; i++)
                group = Util::addCard(group, pow + i, 3);
            for (auto x : carrys)
                group = Util::addCard(group, x);
            if (Util::isLegal(group))
            {
                lib[Feijidaiyi][len][pow].push_back(group);
                if (findTypeLib.find(group) == findTypeLib.end())
                {
                    findTypeLib[group] = pci(Feijidaiyi, len);
                    findPowLib[Feijidaiyi][len][group] = pow;
                }
            }
        }
    }
    else if (carrys.size() == 4u)
    {
        len = 4;
        for (int pow = 0; pow < MaxSortPow - len + 1; pow++)
        {
            group = 0;
            for (int i = 0; i < len; i++)
                group = Util::addCard(group, pow + i, 3);
            for (auto x : carrys)
                group = Util::addCard(group, x);
            if (Util::isLegal(group))
            {
                lib[Feijidaiyi][len][pow].push_back(group);
                if (findTypeLib.find(group) == findTypeLib.end())
                {
                    findTypeLib[group] = pci(Feijidaiyi, len);
                    findPowLib[Feijidaiyi][len][group] = pow;
                }
            }
        }
    }
    else if (carrys.size() == 5u)
    {
        len = 5;
        for (int pow = 0; pow < MaxSortPow - len + 1; pow++)
        {
            group = 0;
            for (int i = 0; i < len; i++)
                group = Util::addCard(group, pow + i, 3);
            for (auto x : carrys)
                group = Util::addCard(group, x);
            if (Util::isLegal(group))
            {
                lib[Feijidaiyi][len][pow].push_back(group);
                if (findTypeLib.find(group) == findTypeLib.end())
                {
                    findTypeLib[group] = pci(Feijidaiyi, len);
                    findPowLib[Feijidaiyi][len][group] = pow;
                }
            }
        }
        return;
    }

    int x = 0;
    if (!carrys.empty())
        x = carrys.back();
    if (x == 13)
        x = 14;
    else if (x == 14)
        return;
    for (; x < NumOfPow; x++)
    {
        carrys.push_back(x);
        ActionLib::initFeijidaiyi(carrys);
        carrys.pop_back();
    }
}

void ActionLib::initFeijidaier(vector<int>& carrys)
{
    long long group;
    int len;
    if (carrys.size() == 2u)
    {
        len = 2;
        for (int pow = 0; pow < MaxSortPow - len + 1; pow++)
        {
            group = 0;
            for (int i = 0; i < len; i++)
                group = Util::addCard(group, pow + i, 3);
            for (auto x : carrys)
                group = Util::addCard(group, x, 2);
            if (Util::isLegal(group))
            {
                lib[Feijidaier][len][pow].push_back(group);
                if (findTypeLib.find(group) == findTypeLib.end())
                {
                    findTypeLib[group] = pci(Feijidaier, len);
                    findPowLib[Feijidaier][len][group] = pow;
                }
            }
        }
    }
    else if (carrys.size() == 3u)
    {
        len = 3;
        for (int pow = 0; pow < MaxSortPow - len + 1; pow++)
        {
            group = 0;
            for (int i = 0; i < len; i++)
                group = Util::addCard(group, pow + i, 3);
            for (auto x : carrys)
                group = Util::addCard(group, x, 2);
            if (Util::isLegal(group))
            {
                lib[Feijidaier][len][pow].push_back(group);
                if (findTypeLib.find(group) == findTypeLib.end())
                {
                    findTypeLib[group] = pci(Feijidaier, len);
                    findPowLib[Feijidaier][len][group] = pow;
                }
            }
        }
    }
    else if (carrys.size() == 4u)
    {
        len = 4;
        for (int pow = 0; pow < MaxSortPow - len + 1; pow++)
        {
            group = 0;
            for (int i = 0; i < len; i++)
                group = Util::addCard(group, pow + i, 3);
            for (auto x : carrys)
                group = Util::addCard(group, x,2);
            if (Util::isLegal(group))
            {
                lib[Feijidaier][len][pow].push_back(group);
                if (findTypeLib.find(group) == findTypeLib.end())
                {
                    findTypeLib[group] = pci(Feijidaier, len);
                    findPowLib[Feijidaier][len][group] = pow;
                }
            }
        }
        return;
    }

    int x = 0;
    if (!carrys.empty())
        x = carrys.back();
    for (; x < MaxSortPow + 1; x++)
    {
        carrys.push_back(x);
        ActionLib::initFeijidaier(carrys);
        carrys.pop_back();
    }
}

void ActionLib::init()
{
    long long group;
    for (int pow = 0; pow < NumOfPow; pow++)
    {
        //单张
        group = Util::addCard(0, pow);
        lib[Danzhang][0][pow].push_back(group);
        findTypeLib[group] = pci(Danzhang, 0);
        findPowLib[Danzhang][0][group] = pow;
        //对子
        if (pow < 13)
        {
            group = Util::addCard(0, pow, 2);
            lib[Duizi][0][pow].push_back(group);
            findTypeLib[group] = pci(Duizi, 0);
            findPowLib[Duizi][0][group] = pow;

            //三张
            group = Util::addCard(0, pow, 3);
            lib[Sanzhang][0][pow].push_back(group);
            findTypeLib[group] = pci(Sanzhang, 0);
            findPowLib[Sanzhang][0][group] = pow;


            //三带一及三带二
            for (int carry = 0; carry < NumOfPow; carry++)
            {
                if (carry != pow)   //相等为炸弹
                {
                    group = 0;
                    group = Util::addCard(0, pow, 3);

                    lib[Sandaiyi][0][pow].push_back(Util::addCard(group, carry));   //三带一
                    findTypeLib[Util::addCard(group, carry)] = pci(Sandaiyi, 0);
                    findPowLib[Sandaiyi][0][Util::addCard(group, carry)] = pow;

                    if (carry != 13 && carry != 14)
                    {
                        lib[Sandaier][0][pow].push_back(Util::addCard(group, carry, 2));   //三带二
                        findTypeLib[Util::addCard(group, carry, 2)] = pci(Sandaier, 0);
                        findPowLib[Sandaier][0][Util::addCard(group, carry, 2)] = pow;
                    }
                }
            }
        }
        //顺子
        for (int len = TypeMinLength[Shunzi]; len <= TypeMaxLength[Shunzi] && pow + len - 1 < MaxSortPow; len++)
        {
            group = 0;
            for (int i = 0; i < len; i++)
                group = Util::addCard(group, pow + i);
            lib[Shunzi][len][pow].push_back(group);
            findTypeLib[group] = pci(Shunzi, len);
            findPowLib[Shunzi][len][group] = pow;
        }
        //连对
        for (int len = TypeMinLength[Liandui]; len <= TypeMaxLength[Liandui] && pow + len - 1 < MaxSortPow; len++)
        {
            group = 0;
            for (int i = 0; i < len; i++)
                group = Util::addCard(group, pow + i, 2);
            lib[Liandui][len][pow].push_back(group);
            findTypeLib[group] = pci(Liandui, len);
            findPowLib[Liandui][len][group] = pow;
        }
        //飞机
        for (int len = TypeMinLength[Feiji]; len <= TypeMaxLength[Feiji] && pow + len - 1 < MaxSortPow; len++)
        {
            group = 0;
            for (int i = 0; i < len; i++)
                group = Util::addCard(group, pow + i, 3);
            lib[Feiji][len][pow].push_back(group);
            findTypeLib[group] = pci(Feiji, len);
            findPowLib[Feiji][len][group] = pow;
        }

        if (pow < 13)
        {
            //炸弹
            group = Util::addCard(0, pow, 4);
            lib[Zhadan][0][pow].push_back(group);
            findTypeLib[group] = pci(Zhadan, 0);
            findPowLib[Zhadan][0][group] = pow;

            //四带二单
            group = Util::addCard(0, pow, 4);
            for (int i = 0; i < NumOfPow; i++)
            {
                for (int j = i; j < NumOfPow; j++)
                {
                    long long tmpGroup;
                    tmpGroup = Util::addCard(group, i);
                    tmpGroup = Util::addCard(tmpGroup, j);
                    if (Util::isLegal(tmpGroup))
                    {
                        lib[Sidaierdan][0][pow].push_back(tmpGroup);
                        findTypeLib[tmpGroup] = pci(Sidaierdan, 0);
                        findPowLib[Sidaierdan][0][tmpGroup] = pow;
                    }
                }
            }
            //四带二对
            group = Util::addCard(0, pow, 4);
            for (int i = 0; i < NumOfPow; i++)
            {
                for (int j = i; j < NumOfPow; j++)
                {
                    long long tmpGroup;
                    tmpGroup = Util::addCard(group, i, 2);
                    tmpGroup = Util::addCard(tmpGroup, j, 2);
                    if (Util::isLegal(tmpGroup))
                    {
                        lib[Sidaierdui][0][pow].push_back(tmpGroup);
                        findTypeLib[tmpGroup] = pci(Sidaierdui, 0);
                        findPowLib[Sidaierdui][0][tmpGroup] = pow;
                    }
                }
            }
        }
    }
    //飞机带一
    initFeijidaiyi();
    //飞机带二
    initFeijidaier();
    //火箭
    group = 0;
    group = Util::addCard(0, 13);
    group = Util::addCard(group, 14);
    lib[Huojian][0][0].push_back(group);
    findTypeLib[group] = pci(Huojian, 0);
    findPowLib[Huojian][0][group] = 0;
}