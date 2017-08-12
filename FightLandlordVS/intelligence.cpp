#include "stdafx.h"
#include "intelligence.h"

using namespace std;

//typedef vector<Card> Group;

bool groupCmp(const Group& a,const Group& b)
{
    CardType ta;
    CardType tb;
    int la;
    int lb;
    int pa;
    int pb;
    TypeTest::getType(a, ta, la);
    TypeTest::getType(b, tb, lb);
    pa=TypeTest::getPower[ta](a, la);
    pb = TypeTest::getPower[tb](b, lb);

    if (ta >= Shunzi&&ta <= Feijidaier)
    {
        if (tb >= Shunzi&&tb <= Feijidaier)
        {
            if (ta != tb)
                return ta < tb;
            else if (pa != pb)
                return pa < pb;
            else
                return la < lb;
        }
        else
            return true;
    }
    else if ((ta == Danzhang || ta == Duizi) && (pa < NumToPow[2]))
    {
        if ((tb >= Shunzi) && (tb <= Feijidaier))
            return false;
        if ((tb == Danzhang || tb == Duizi) && (pb < NumToPow[2]))
        {
            if (pa != pb)
                return pa < pb;
            else
                return ta < tb;
        }
        else
            return true;
    }
    else if (ta >= Sanzhang && ta <= Sandaier)
    {
        if (tb >= Shunzi&&tb <= Feijidaier)
            return false;
        else if ((tb == Danzhang || tb == Duizi) && (pb < NumToPow[2]))
            return false;
        else if (tb >= Sanzhang&&tb <= Sandaier)
        {
            if (ta != tb)
                return ta > tb;
            else if (pa != pb)
                return pa < pb;
            else
                return la < lb;
        }
        else
            return true;
    }
    else if ((ta == Danzhang || ta == Duizi))   //大于等于2的单张对子
    {
        if ((ta == Danzhang || ta == Duizi) && pb >= NumToPow[2])
        {
            if (pa != pb)
                return pa < pb;
            else
                return ta < tb;
        }
        else if (tb >= Zhadan)
            return true;
        else
            return false;
    }
    else if (ta == Sidaierdan || ta == Sidaierdui)
    {
        if (tb < Zhadan)
            return false;
        else if (tb == Zhadan)
            return true;
        else if (tb == Sidaierdan || tb == Sidaierdui)
        {
            if (pa != pb)
                return pa < pb;
            else
                return ta < tb;
        }
        else
            return true;    //只剩火箭了
    }
    else if (ta == Zhadan)
    {
        if (tb == Zhadan)
            return pa < pb;
        else if (tb == Huojian)
            return true;
        else
            return false;
    }
    else
        return false;   //a为火箭

}

string groupToString(const Group& group)
{
    string str = "";
    for (auto x : group)
    {
        str += x.getDigit();
    }
    return str;
}

void prtGroup(const Group& group)
{
    for (auto x : group)
        cout << x.getDigit();
}

inline int getEnemyHandSize(const Game game, int gamer)
{
    int nextGamer = (gamer + 1) % 3;
    int lastGamer = (gamer + 2) % 3;
    if (gamer == game.getLandlord())
    {
        return min(game.getHandSize(lastGamer), game.getHandSize(nextGamer));
    }
    else
        return game.getHandSize(game.getLandlord());

}

string Intelligence::makeDecision(const Game game)
{
    Group g1;
    Group g2;

    vector<Group> bestWay;
    vector<Group> bestWayWithoutCarry;
    const Group hand = game.getHand(game.getCurrentGamer());
    int gamer = game.getCurrentGamer();
    int nextGamer = (gamer + 1) % 3;
    int lastGamer = (gamer + 2) % 3;

    if (hand.size() == 0)
        return "";

    TypeTest::getHands(hand, bestWay);
    TypeTest::getHandsWithoutCarry(hand, bestWayWithoutCarry);

    /*
    for (auto x : bestWay)
    {
        for (auto y : bestWay)
        {
            prtGroup(x);
            cout << " < ";
            prtGroup(y);
            cout<<"\t\t" << (groupCmp(x, y) ? "true" : "false") << endl;
            prtGroup(x);
            cout << " > ";
            prtGroup(y);
            cout <<"\t\t"<< (groupCmp(y, x) ? "true" : "false") << endl;
            cout << endl;
        }
    }*/

    CardType cntType = game.getCurrentType();
    int cntlen = game.getCurrentTypeLen();
    int cntpow = game.getCurrentTypePow();

    if (cntType == Danzhang || cntType == Duizi)        //先不考虑带牌情况下出单张或者对子
    {
        for (auto x : bestWayWithoutCarry)
        {
            sort(x.begin(), x.end());
        }
        sort(bestWayWithoutCarry.begin(), bestWayWithoutCarry.end(), groupCmp);
        for (auto x : bestWayWithoutCarry)
            prtGroup(x), cout << endl;

        for (auto x : bestWayWithoutCarry)
        {
            CardType type;
            int len;
            TypeTest::getType(x, type, len);
            if (type == cntType)
            {
                if (TypeTest::canOut(x, cntType, cntlen, cntpow))
                {
                    return groupToString(x);
                }
            }
        }

    }

    for (auto x : bestWay)
        sort(x.begin(), x.end());
    sort(bestWay.begin(), bestWay.end(), groupCmp);
    for (auto x : bestWay)
        prtGroup(x), cout << endl;

    if (cntType == NoneType)
    {
        return groupToString(bestWay[0]);
    }
    else
    {
        for (auto x : bestWay)
        {
            CardType type;
            int len;
            TypeTest::getType(x, type, len);
            if (type == cntType&&len == cntlen)  //压牌
            {
                if (TypeTest::canOut(x, cntType, cntlen, cntpow))
                {
                    return groupToString(x);
                }
            }

            if ((cntType == Danzhang || cntType == Duizi))  //拆牌
            {
                int pow;
                pow = TypeTest::getPower[type](x, len);
                if ((pow == NumToPow[2]) && ((type >= Sanzhang) && (type <= Sandaier)) && (pow > cntpow) && (getEnemyHandSize(game, gamer) < 9))
                {
                    string str = "";
                    str += NumToDigit[PowToNum[pow]];
                    if (cntType == Duizi)
                        str += NumToDigit[PowToNum[pow]];
                    return str;
                }
            }

            if ((cntType == Danzhang))
            {
                int pow;
                pow = TypeTest::getPower[type](x, len);
                if (type == Duizi)
                {
                    int en = getEnemyHandSize(game, gamer);
                    if (((pow == NumToPow[2]) || en <= 6 || game.getHandSize(gamer) == 2) && (pow > cntpow) && (en < 10))
                    {
                        string str = "";
                        str += NumToDigit[PowToNum[pow]];
                        return str;
                    }
                }
            }

            if (type == Sidaierdan || type == Sidaierdui)
            {
                int pow;
                pow = TypeTest::getPower[type](x, len);
                if (getEnemyHandSize(game, gamer) <= 6)
                {
                    char c = NumToDigit[PowToNum[pow]];
                    string str = "";
                    str += c;
                    str += c;
                    str += c;
                    str += c;
                    Group g;
                    TypeTest::makeGroup(str, g);
                    if (TypeTest::canOut(g, cntType, cntlen, cntpow))
                    {
                        return str;
                    }
                }
            }

            if ((type == Zhadan || type == Huojian) && (getEnemyHandSize(game,gamer) <= 8))
            {
                if (TypeTest::canOut(x, cntType, cntlen, cntpow))
                {
                    return groupToString(x);
                }
            }
        }



        return "";
    }
}