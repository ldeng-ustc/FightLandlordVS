#ifndef mcts_game_H
#define mcts_game_H

#include <vector>
#include <unordered_map>
#include "stdafx.h"

#include "type.h"
#include "game.h"
using namespace std;

typedef pair<vector<long long>, vector<pair<int, int> > > ChoiceList;

class MCTS_Board
{
private:
    static unordered_map<long long, ChoiceList> choiceLib;

    long long hands[NumOfPlayer];     //三个玩家的手牌，从3开始表示，每3bit表示一种牌的数量
    CardType type;      //描述当前节点需要出牌类型
    int len;            //描述当前出牌长度，无长度类型为0
    int pow;            //描述当前出牌大小
    int cntPlayer;      //描述当前应出牌玩家
    bool lastPass;      //上一玩家是否PASS

public:
    static void init();
    
    MCTS_Board();
    MCTS_Board(Game game);
    MCTS_Board(long long hands[], CardType type, int len, int pow, int cntPlayer, bool lastPass);

    inline int getcntPlayer()
    {
        return cntPlayer;
    }

    size_t getHashCode() const;
    bool operator== (const MCTS_Board& b) const;
    MCTS_Board& operator= (const MCTS_Board& b);
    
    const vector<long long>& getActions(vector<long long> &target) const;
    void play(long long x);
    int isWin();    //是否有玩家获得胜利，若有返回玩家编号，否则返回-1
    void prt() const;
    void prt(ostream& out)const;
    long long getCntHand() const;
};

namespace std
{
    template<>
    struct hash < MCTS_Board >
    {
        size_t operator()(const MCTS_Board& a) const
        {
            return a.getHashCode();
        }
    };
}

#endif