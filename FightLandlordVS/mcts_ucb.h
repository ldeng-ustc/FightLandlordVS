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
    static const int timeLimit;
    static const double confident;
    random r;
    
    MCTS_Board board;
    unordered_map<MCTS_Board, unordered_map<long long, pair<int, int> > > winAndPlay;
    int sumPlay;
    int maxPlay;            //最多的策略模拟次数
    int maxPlayWin;
    long long maxMove;
    int secPlay;            //次多的策略模拟次数
    int secPlayWin;
    long long secMove;

    inline double getUCB(const MCTS_Board &board,long long x)       //计算UCB值
    {
        pair<int, int>& data = winAndPlay[board][x];
        int win = data.first;
        int play = data.second;
        return win / (double)play + sqrt(confident*log(sumPlay) / play);
    }

public:
    MCTS_UCB(Game game);
    MCTS_UCB(MCTS_Board board);
    //void reset(MCTS_Board board);
    long long getBestAction(int cntTimeLimit = MCTS_UCB::timeLimit);
    const unordered_map<long long, pair<int, int> >& getResultList();
    void runSimulations(MCTS_Board& board);
    long long selectBestMove();
    void play(long long move);

};

#endif
