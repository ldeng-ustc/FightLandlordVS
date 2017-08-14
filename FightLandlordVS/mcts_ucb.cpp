#include "stdafx.h"
#include "mcts_ucb.h"
#include <tuple>
#include <fstream>
#include <algorithm>

int MCTS_UCB::timeLimit = (int)(3.5 * CLOCKS_PER_SEC);
double MCTS_UCB::confident = 1.96;
int tttt = 0;

MCTS_UCB::MCTS_UCB(Game game) :board(game), r(1725)
{
    sumPlay = 0;
    winAndPlay.clear();
    winAndPlay.rehash(2000000);
}

long long MCTS_UCB::getBestAction()
{
    tttt = 0;
    const vector<long long>& choices = board.getActions();
    if (choices.size() == 1u)
        return choices[0];
    int st = clock();
    int i = 0;
    int tmpst = clock();
    while ((clock() - st) < timeLimit)
    {
        MCTS_Board tmpBoard = board;
        runSimulations(tmpBoard);
        i++;
        if ((i & 0x3ff) == 0)
        {
            cout << "time:" << clock() - tmpst << endl;
            tmpst = clock();
        }
    }

    long long move = selectBestMove();
    return move;
}

void MCTS_UCB::runSimulations(MCTS_Board& board)
{
    vector<tuple<MCTS_Board, long long, int> > path;  //记录哪位玩家在哪个局面下了哪种下法
    bool expand = false;    //本次模拟是否已拓展

    while (board.isWin() == -1) //模拟至有人获胜
    {
        const vector<long long> &choices = board.getActions();
        unordered_map<long long, pair<int, int> > &tmpResult = winAndPlay[board];
        bool flag = false;  //是否有着法没有模拟
        double maxUCB = 0;
        long long move = 0;

        for (auto x : choices)
        {
            if (tmpResult.count(x) == 0)
            {
                flag = true;
                break;
            }
            double cntUCB = getUCB(board, x);
            if (cntUCB > maxUCB)
            {
                maxUCB = cntUCB;
                move = x;
            }
        }
        if (flag)   //随机着法
        {
            move = choices[r.next(0, choices.size() - 1)];
        }

        if (tmpResult.count(move) == 0)    //没有统计信息，没拓展过，则拓展
        {
            if (!expand)
            {
                expand = true;
                winAndPlay[board][move] = pair<int, int>(0, 0);
                path.push_back(make_tuple(board, move, board.getcntPlayer()));
            }
        }
        else
        {
            path.push_back(make_tuple(board, move, board.getcntPlayer()));
        }
        board.play(move);
    }

    bool winner[NumOfPlayer] = { false };
    if (board.isWin() == 0)
        winner[0] = true;
    else
        winner[1] = winner[2] = true;

    for (auto x : path)     //反向传播
    {
        pair<int, int> &data = winAndPlay[get<0>(x)][get<1>(x)];
        if (winner[get<2>(x)])
        {
            data.first++;
        }
        data.second++;
    }
    sumPlay++;
}

long long MCTS_UCB::selectBestMove()
{
    const vector<long long>& choice = board.getActions();
    double winRate = -1.0;
    long long move = 0;
    for (auto x : choice)
    {
        pair<int, int> data = winAndPlay[board][x];
        int win = data.first;
        int play = data.second;
        if (play != 0)
        {
            Util::prtInString(x);
            cout << win << ',' << play << endl;
            double cntRate = win / (double)play;
            if (cntRate > winRate)
            {
                move = x;
                winRate = cntRate;
            }
        }
    }
    return move;
}
