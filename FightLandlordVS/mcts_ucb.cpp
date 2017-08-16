#include "stdafx.h"
#include "mcts_ucb.h"
#include <tuple>
#include <fstream>
#include <algorithm>

const int MCTS_UCB::timeLimit = (int)(4 * CLOCKS_PER_SEC);
const double MCTS_UCB::confident = 1.96;
int tttt = 0;

MCTS_UCB::MCTS_UCB(Game game) :board(game), r(1725)
{
    sumPlay = 0;
    maxPlay = 0;
    maxPlayWin = 0;
    maxMove = -1;
    secPlay = 0;
    secPlayWin = 0;
    secMove = -1;
    winAndPlay.clear();
    winAndPlay.rehash(5000000);
}

MCTS_UCB::MCTS_UCB(MCTS_Board board) :board(board), r(1725)
{
    sumPlay = 0;
    maxPlay = 0;
    maxPlayWin = 0;
    maxMove = -1;
    secPlay = 0;
    secPlayWin = 0;
    secMove = -1;
    winAndPlay.clear();
    winAndPlay.rehash(5000000);
}

long long MCTS_UCB::getBestAction(int cntTimeLimit)
{
    tttt = 0;
    const vector<long long>& choices = board.getActions();
    if (choices.size() == 1u)
        return choices[0];
    long long hand = board.getCntHand();
    for (auto x : choices)
    {
        if (x == hand)
            return x;
    }

    int st = clock();
    while ((clock() - st) < cntTimeLimit)
    {
        MCTS_Board tmpBoard = board;
        runSimulations(tmpBoard);
        if ((((maxPlayWin / (double)maxPlay) > 0.99) && maxPlay > 2000) || (maxPlay - secPlay > 800 && secMove != -1) || sumPlay > 10000)   //若某个决策模拟次数已远大于其它决策，也可提前退出
        {
            break;
        }
    }
    long long move = selectBestMove();
    return move;
}

const unordered_map<long long, pair<int, int> >& MCTS_UCB::getResultList()
{
    return winAndPlay[board];
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

    int siz = path.size();
    for (int i = 0; i < siz;i++)     //反向传播
    {
        auto &x = path[i];
        pair<int, int> &data = winAndPlay[get<0>(x)][get<1>(x)];
        if (winner[get<2>(x)])
        {
            data.first++;
        }
        data.second++;
        if (i == 0)
        {
            if (data.second > maxPlay)
            {
                if (get<1>(x) == maxMove)
                {
                    maxPlay = data.second;
                    maxPlayWin = data.first;
                }
                else
                {
                    secPlay = maxPlay;
                    secPlayWin = maxPlayWin;
                    secMove = maxMove;
                    maxPlay = data.second;
                    maxPlayWin = data.first;
                    maxMove = get<1>(x);
                }
            }
            else if (data.second > secPlay)
            {
                if (get<1>(x) == maxMove)
                    ;
                else
                {
                    secPlay = data.second;
                    secPlayWin = data.first;
                    secMove = get<1>(x);
                }
            }
        }
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

void MCTS_UCB::play(long long move)
{
    auto &data = winAndPlay[board];
    if (data.count(move) != 0)
        sumPlay = data[move].second;
    else
        sumPlay = 0;
    maxPlay = 0;
    maxPlayWin = 0;
    maxMove = -1;
    secPlay = 0;
    secPlayWin = 0;
    secMove = -1;
    board.play(move);
}
