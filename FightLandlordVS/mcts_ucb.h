#ifndef mcts_ucb_H
#define mcts_ucb_H

#include <unordered_map>
#include <unordered_set>
#include <ctime>
#include <cmath>
#include "mcts_board.h"
#include "long_long_group_util.h"
#include "random.h"

using namespace std;
typedef LongLongGroupUtil Util;

class MCTS_UCB      //对同一局游戏，建议使用同一个MCTS_UCB对象
{
public:
    static int timeLimit;
    static double confident;
    random r;
    
    MCTS_Board board;
    unordered_map<MCTS_Board, unordered_map<long long, pair<int, int> > > winAndPlay;
    int sumPlay;

    inline double getUCB(const MCTS_Board &board,long long x)       //计算UCB值
    {
        pair<int, int>& data = winAndPlay[board][x];
        int win = data.first;
        int play = data.second;
        return win / (double)play + sqrt(confident*log(sumPlay) / play);
    }

public:
    MCTS_UCB(Game game);
    long long getBestAction();
    void runSimulations(MCTS_Board& board);
    long long selectBestMove();

};

#endif
