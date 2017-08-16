#include "stdafx.h"
#include "long_long_group_util.h"
#include "action_lib.h"
#include "type_util.h"
#include "type.h"

const int LongLongGroupUtil::bits = 4;
const int LongLongGroupUtil::mask = (1 << LongLongGroupUtil::bits) - 1;
const long long LongLongGroupUtil::zhadan[NumCanInShun + 1] = { 1LL << (2 + 0 * bits), 1LL << (2 + 1 * bits), 1LL << (2 + 2 * bits), 1LL << (2 + 3 * bits), 
                                                          1LL << (2 + 4 * bits), 1LL << (2 + 5 * bits), 1LL << (2 + 6 * bits), 1LL << (2 + 7 * bits), 
                                                          1LL << (2 + 8 * bits), 1LL << (2 + 9 * bits), 1LL << (2 + 10 * bits), 1LL << (2 + 11 * bits), 
                                                          1LL << (2 + 12 * bits) };
const long long LongLongGroupUtil::huojian = (1LL << (13 * bits)) + (1LL << (14 * bits));

long long LongLongGroupUtil::addCard(long long group, int cardpow)
{
    return group + ((long long)1 << (bits * cardpow));
}

long long LongLongGroupUtil::addCard(long long group, int cardpow, int n)
{
    return group + ((long long)n << (bits * cardpow));
}

void LongLongGroupUtil::prtInString(long long group)
{
    int cnt = 0;
    while (group != 0)
    {
        int tmp = (group & mask);
        for (int i = 0; i < tmp; i++)
            cout << NumToDigit[PowToNum[cnt]];
        cnt++;
        group /= ((long long)1 << bits);
    }
    cout << endl;
}

string LongLongGroupUtil::getString(long long group)
{
    string str = "";
    int cnt = 0;
    while (group != 0)
    {
        int tmp = (group & mask);
        for (int i = 0; i < tmp; i++)
            str+= NumToDigit[PowToNum[cnt]];
        cnt++;
        group /= ((long long)1 << bits);
    }
    return str;
}

int LongLongGroupUtil::getNumFromGroup(long long group, int cardPow)
{
    return (group >> (bits * cardPow)) & mask;
}

bool LongLongGroupUtil::isLegal(long long group)
{
    for (int i = 0; i < MaxSortPow + 1; i++)
    {
        int tmp = getNumFromGroup(group, i);
        if (tmp > 4 || tmp < 0)
            return false;
    }
    if (getNumFromGroup(group, 13) > 1 || getNumFromGroup(group, 14) > 1)
        return false;
    if ((group >> (bits * NumOfPow)) != 0)
        return false;
    return true;
}

bool LongLongGroupUtil::isIn(long long group, long long hands)
{
    while (group != 0)
    {
        if ((group & mask) > (hands & mask))
            return false;
        group >>= bits;
        hands >>= bits;
    }
    return true;
}

int LongLongGroupUtil::isZhadan(long long group)
{
    for (int i = 0; i <= NumCanInShun;i++)
        if (zhadan[i] == group)
            return i;
    return -1;
}

bool LongLongGroupUtil::isHuojian(long long group)
{
    return group == huojian;
}

long long LongLongGroupUtil::groupToLongLong(const Group& gr)
{
    long long ans = 0;
    for (auto x : gr)
    {
        ans = addCard(ans, x.getPower());
    }
    return ans;
}

const pair<vector<long long>,vector<pair<int,int> > >& LongLongGroupUtil::getActions(long long group)
{
    
    vector<long long>& ans = *(new vector<long long>());
    vector<pair<int, int> >& split = *(new vector<pair<int, int> >());

    long long& hand = group;

    bool has[NumOfType][NumOfPow] = { { false } };
    for (int i = 0; i <= Sandaier; i++)             //从单张到三带二，必须前一种存在，后一种才可能存在
    {
        split.push_back(pair<int, int>(ans.size(), ans.size()));
        for (int j = 0; j < NumOfPow; j++)
        {
            if (i == 0 || has[i - 1][j])
            {
                for (auto x : ActionLib::lib[i][0][j])
                {
                    if (LongLongGroupUtil::isIn(x, hand))
                    {
                        has[i][j] = true;
                        ans.push_back(x);
                        split[TypeUtil::pairToId(pair<CardType, int>((CardType)i, 0))].second++;
                    }
                }
            }
        }
    }

    for (int i = Shunzi; i <= Feijidaier; i++)
    {
        bool hasLen[20] = { false };
        for (int j = TypeMinLength[i]; j <= TypeMaxLength[i]; j++)
        {
            split.push_back(pair<int, int>(ans.size(), ans.size()));
            int maxPow = MaxSortPow - j + 1;
            for (int k = 0; k < maxPow; k++)
            {
                if (j == TypeMinLength[i] || hasLen[j - 1])
                {
                    for (auto x : ActionLib::lib[i][j][k])
                    {
                        if (LongLongGroupUtil::isIn(x, hand))
                        {
                            hasLen[j] = true;
                            ans.push_back(x);
                            split[TypeUtil::pairToId(pair<CardType, int>((CardType)i, j))].second++;
                        }
                    }
                }
            }
        }
    }

    split.push_back(pair<int, int>(ans.size(), ans.size()));
    for (int i = 0; i <= MaxSortPow; i++)
    {
        if (has[Sanzhang][i])
        {
            for (auto x : ActionLib::lib[Zhadan][0][i])
            {
                if (LongLongGroupUtil::isIn(x, hand))
                {
                    has[Zhadan][i] = true;
                    ans.push_back(x);
                    split[TypeUtil::pairToId(pair<CardType, int>(Zhadan, 0))].second++;
                }
            }
        }
    }

    for (int i = Sidaierdan; i <= Sidaierdui; i++)
    {
        split.push_back(pair<int, int>(ans.size(), ans.size()));
        for (int j = 0; j <= MaxSortPow; j++)
        {
            if (has[i - 1][j])
            {
                for (auto x : ActionLib::lib[i][0][j])
                {
                    if (LongLongGroupUtil::isIn(x, hand))
                    {
                        has[i][j] = true;
                        ans.push_back(x);
                        split[TypeUtil::pairToId(pair<CardType, int>((CardType)i, 0))].second++;
                    }
                }
            }
        }
    }

    split.push_back(pair<int, int>(ans.size(), ans.size()));
    long long tmp = ActionLib::lib[Huojian][0][0][0];
    if (LongLongGroupUtil::isIn(tmp, hand))
    {
        ans.push_back(tmp);
        split[TypeUtil::pairToId(pair<CardType, int>(Huojian, 0))].second++;
    }
    return *(new pair<vector<long long>, vector<pair<int, int> > >(ans, split));
}