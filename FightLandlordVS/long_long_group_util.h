#ifndef long_long_group_util_H
#define long_long_group_util_H

#include "type.h"

class LongLongGroupUtil
{
public:
    static int bits;
    static int mask;

    static long long addCard(long long group, int cardpow);
    static long long addCard(long long group, int cardpow, int n);
    static void prtInString(long long group);
    static string getString(long long group);
    static int getNumFromGroup(long long group, int cardPow);
    static bool isLegal(long long group);
    static bool isIn(long long group,long long hands); //whether group is in hands
    static long long groupToLongLong(const Group& gr);
    static vector<long long>& getActions(long long group);
};

#endif
