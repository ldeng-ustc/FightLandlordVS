#ifndef action_lib_H
#define action_lib_H

#include <vector>
#include <unordered_map>
#include "type.h"
#include "card.h"
#include "long_long_group_util.h"
using namespace std;

/**
 *���ڴ������п��õĳ��Ʒ������������ȡ��
 *ÿ�ֳ��Ʒ�������һ��longlong��ʾ���������
 *ͨ��
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