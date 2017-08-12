#ifndef action_lib_H
#define action_lib_H

#include <vector>
#include <unordered_map>
#include "type.h"
#include "card.h"
#include "long_long_group_util.h"
using namespace std;

/**
 *用于储存所有可用的出牌方法，方便快速取用
 *每种出牌方法，用一个longlong表示的牌组代表
 *通过
*/

class ActionLib
{
public:
    static vector<long long> lib[NumOfType][MaxTypeLen][NumOfPow];
    static unordered_map<long long, pair<CardType,int> > findTypeLib;
    static unordered_map<long long, int> findPowLib[NumOfType][MaxTypeLen];
    static void initFeijidaiyi(vector<int>& carrys = vector<int>());
    static void initFeijidaier(vector<int>& carrys = vector<int>());
    static void init();
};

#endif