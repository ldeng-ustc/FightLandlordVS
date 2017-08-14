#ifndef type_util_H
#define type_util_H

#include "type.h"

using namespace std;

class TypeUtil
{
private:
    static const int typeOffside[NumOfType];
public:
    static int pairToId(const pair<CardType, int> typeAndLen);

};

#endif
