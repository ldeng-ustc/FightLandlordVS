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
    static int timeLimit;
    static double confident;
    static unordered_map<MCTS_Board, vector<long long> > choiceLib;
    random r;
    
    MCTS_Board board;
    unordered_map<MCTS_Board, unordered_map<long long, pair<int, int> > > winAndPlay;
    int sumPlay;

    static const vector<long long>& getChoice(MCTS_Board board);    //用于查询当前可选择的行动

    inline double getUCB(const MCTS_Board &board,long long x)       //计算UCB值
    {
        pair<int, int>& data = winAndPlay[board][x];
        int win = data.first;
        int play = data.second;
        return win / (double)play + sqrt(confident*log(sumPlay) / play);
    }

public:
    MCTS_UCB(Game game);

    long long getBestAction()
    {
        const vector<long long>& choices = getChoice(board);

        if (choices.size() == 1u)
            return choices[0];

        //winAndPlay.clear();
        int st = clock();
        
        int i = 0;
        int tmpst = clock();
        while ((clock() - st) < timeLimit)
        {
            //cout << "running..." << endl;
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

    void runSimulations(MCTS_Board& board)
    {
        vector<pair<MCTS_Board, pair<long long, int> > > path;  //记录哪位玩家在哪个局面下了哪种下法
        bool expand = false;    //本次模拟是否已拓展

        int st = clock();

        while (board.isWin() == -1) //模拟至有人获胜
        {
            const vector<long long> &choices = getChoice(board);
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

            if (clock() - st > 1000)
            {
                board.prt();
                cout << "Random:" << flag << endl << "move:";
                Util::prtInString(move);
                cout << endl;
            }
                

            if (tmpResult.count(move) == 0)    //没有统计信息，没拓展过，则拓展
            {
                if (!expand)
                {
                    expand = true;
                    winAndPlay[board][move] = pair<int, int>(0, 0);
                    path.push_back(pair<MCTS_Board, pair<long long, int> >(board, pair<long long, int>(move, board.getcntPlayer())));
                }
            }
            else
            {
                path.push_back(pair<MCTS_Board, pair<long long, int> >(board, pair<long long, int>(move, board.getcntPlayer())));
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
            pair<int, int> &data = winAndPlay[x.first][x.second.first];
            if (winner[x.second.second])
            {
                data.first++;
            }
            data.second++;
        }
        sumPlay++;
    }

    long long selectBestMove()
    {
        const vector<long long>& choice = getChoice(board);
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


};

#endif
