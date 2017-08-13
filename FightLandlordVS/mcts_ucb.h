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

class MCTS_UCB
{
public:
    static int timeLimit;
    static double confident;
    static unordered_map<MCTS_Board, vector<long long> > choiceLib;
    random r;
    
    MCTS_Board board;
    unordered_map<MCTS_Board, unordered_map<long long, pair<int, int> > > winAndPlay;
    int sumPlay;

    static const vector<long long>& getChoice(const MCTS_Board& board);    //用于查询当前可选择的行动

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
