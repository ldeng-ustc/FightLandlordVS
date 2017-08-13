#include "stdafx.h"
#include <vector>
#include <cstring>

#include "action_lib.h"
#include "mcts_board.h"
#include "mcts_ucb.h"
#include "game.h"
using namespace std;

typedef LongLongGroupUtil Util;

unordered_map<long long, vector<long long> > MCTS_Board::choiceLib;


MCTS_Board::MCTS_Board(Game game)
{
    for (int i = 0; i < NumOfPlayer; i++)
    {
        hands[i] = LongLongGroupUtil::groupToLongLong(game.getHand(((game.getLandlord() + i) % NumOfPlayer)));
    }
    type = game.getCurrentType();
    len = game.getCurrentTypeLen();
    pow = game.getCurrentTypePow();
    cntPlayer = (game.getCurrentGamer() - game.getLandlord() + NumOfPlayer) % NumOfPlayer;

    for (int i = 0; i < NumOfPlayer; i++)
    {
        if (choiceLib.count(hands[i]) == 0)
        {
            choiceLib[hands[i]] = Util::getActions(hands[i]);
        }
    }

}

size_t MCTS_Board::getHashCode() const
{
    size_t p1 = 31;
    size_t p2 = 31 * 31;
    size_t p3 = 31 * 31 * 31;
    size_t p4 = p3 * 31;
    size_t x1 = hash<long long>()(hands[0]);
    size_t x2 = hash<long long>()(hands[1]);
    size_t x3 = hash<long long>()(hands[2]);
    size_t x4 = hash<bool>()(lastPass)*p4 + hash<int>()((int)type)* p3 + hash<int>()(len)* p2 + hash<int>()(pow)* p1 + hash<int>()(cntPlayer);
    return x4*p3 + x3*p2 + x2*p1 + x1;
}

bool MCTS_Board::operator== (const MCTS_Board& b) const
{
    return hands[0] == b.hands[0]
        && hands[1] == b.hands[1]
        && hands[2] == b.hands[2]
        && type == b.type
        && len == b.len
        && pow == b.pow
        && cntPlayer == b.cntPlayer
        && lastPass == b.lastPass;
}

MCTS_Board& MCTS_Board::operator= (const MCTS_Board& b)
{
    memcpy(hands, b.hands, sizeof(hands));
    type = b.type;
    len = b.len;
    pow = b.pow;
    cntPlayer = b.cntPlayer;
    lastPass = b.lastPass;
    return *this;
}

void MCTS_Board::play(long long x)
{
    long long oldHand = hands[cntPlayer];
    hands[cntPlayer] -= x;
    long long &newHand = hands[cntPlayer];
    if (choiceLib.count(newHand) == 0)
    {
        vector<long long> tmpChoice = *(new vector<long long>());
        vector<long long> &oldChoice = choiceLib[oldHand];
        for (auto x : oldChoice)
        {
            if (Util::isIn(x, newHand))
                tmpChoice.push_back(x);
        }
        choiceLib[newHand] = tmpChoice;
    }

    cntPlayer++;
    cntPlayer %= NumOfPlayer;
    if (x != 0)
    {
        if (type == NoneType)
        {
            pair<CardType, int> data = ActionLib::findTypeLib[x];
            type = data.first;
            len = data.second;
        }
        pow = ActionLib::findPowLib[type][len][x];
        lastPass = false;
    }
    else
    {
        if (lastPass)
        {
            type = NoneType;
            len = pow = 0;
            lastPass = false;
        }
        else
            lastPass = true;
    }
}

vector<long long>& MCTS_Board::getActions() const
{
    long long hand = hands[cntPlayer];

    if (type == NoneType)
    {
        return choiceLib[hand];
    }
    else
    {
        vector<long long>& ans = *(new vector<long long>());
        vector<long long>& bigChoice = choiceLib[hand];
        
        for (auto x : bigChoice)
        {
            const pair<CardType, int> &data = ActionLib::findTypeLib[x];
            CardType xtype = data.first;
            int xlen = data.second;
            if (xtype == type&&xlen == len)
            {
                int xpow = ActionLib::findPowLib[xtype][xlen][x];
                if (xpow > pow)
                {
                    ans.push_back(x);
                }
            }
            else if (xtype > type || (xtype == type&&xlen > len))
                break;
        }
        if (type != Zhadan&&type != Huojian)
        {
            for (int i = 0; i < NumOfPow; i++)
            {
                for (auto x : ActionLib::lib[Zhadan][0][i])
                    if (LongLongGroupUtil::isIn(x, hand))
                        ans.push_back(x);
            }
        }
        if (type != Huojian)
        {
            long long tmp = ActionLib::lib[Huojian][0][0][0];
            if (LongLongGroupUtil::isIn(tmp, hand))
                ans.push_back(tmp);
        }
        ans.push_back(0);
        return ans;
    }
}

int MCTS_Board::isWin()
{
    for (int i = 0; i < NumOfPlayer; i++)
        if (hands[i] == 0LL)
            return i;
    return -1;
}

void MCTS_Board::prt()
{
    cout << "Hand:" << endl;
    for (int i = 0; i < NumOfPlayer; i++)
    {
        Util::prtInString(hands[i]);
    }
    cout << "cntPlayer:" << cntPlayer << endl;
    if (type != NoneType)
        cout << "Type:" << TypeNameCnStr[type];
    else
        cout << "Type:自由出牌";
    cout << "  Len:" << len << "  Pow:" << pow << "(" << NumToDigit[PowToNum[pow]] << ")" << endl;
}

void MCTS_Board::prt(ostream& out)
{
    out << "Hand:" << endl;
    for (int i = 0; i < NumOfPlayer; i++)
    {
        out << Util::getString(hands[i]) << endl;
    }
    out << "cntPlayer:" << cntPlayer << endl;
    if (type != NoneType)
        out << "Type:" << TypeNameCnStr[type];
    else
        out << "Type:自由出牌";
    out << "  Len:" << len << "  Pow:" << pow << "(" << NumToDigit[PowToNum[pow]] << ")" << endl;
}
