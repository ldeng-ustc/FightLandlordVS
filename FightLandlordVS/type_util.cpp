#include "stdafx.h"
#include "type_util.h"

using namespace std;

const int TypeUtil::typeOffside[NumOfType] = { 0, 1, 2, 3, 4, 5, 13, 21, 26, 30, 33, 34, 35, 36 };

int TypeUtil::pairToId(const pair<CardType, int> typeAndLen)
{
    CardType type = typeAndLen.first;
    int len = typeAndLen.second;
    if (type >= NumOfType || type < -1)
    {
        cout << "********错误：转换超出类型的范围*********" << endl;
        return -2;
    }
    if (len>TypeMaxLength[type] || len < TypeMinLength[type])
    {
        cout << "********错误：转换超出类型的范围*********" << endl;
        return -2;
    }
    if (type == -1)
        return -1;
    return typeOffside[type] + len - TypeMinLength[type];
}