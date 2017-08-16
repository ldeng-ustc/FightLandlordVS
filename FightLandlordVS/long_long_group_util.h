#ifndef long_long_group_util_H
#define long_long_group_util_H

#include "type.h"

class LongLongGroupUtil
{
public:
    static const int bits;
    static const int mask;
    static const long long zhadan[NumCanInShun + 1];
    static const long long huojian;

    static long long addCard(long long group, int cardpow);
    static long long addCard(long long group, int cardpow, int n);
    static void prtInString(long long group);
    static string getString(long long group);
    static int getNumFromGroup(long long group, int cardPow);
    static bool isLegal(long long group);
    static bool isIn(long long group,long long hands); //whether group is in hands
    static int isZhadan(long long group);   //若是炸弹，返回pow，否则返回-1
    static bool isHuojian(long long group); //是火箭
    static long long groupToLongLong(const Group& gr);
    static const pair<vector<long long>,vector<pair<int,int> > >& getActions(long long group);
};

#endif
