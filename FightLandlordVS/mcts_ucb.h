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

    static const vector<long long>& getChoice(MCTS_Board board);    //���ڲ�ѯ��ǰ��ѡ����ж�

    inline double getUCB(const MCTS_Board &board,long long x)       //����UCBֵ
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
        vector<pair<MCTS_Board, pair<long long, int> > > path;  //��¼��λ������ĸ��������������·�
        bool expand = false;    //����ģ���Ƿ�����չ

        int st = clock();

        while (board.isWin() == -1) //ģ�������˻�ʤ
        {
            const vector<long long> &choices = getChoice(board);
            unordered_map<long long, pair<int, int> > &tmpResult = winAndPlay[board];
            bool flag = false;  //�Ƿ����ŷ�û��ģ��
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
            if (flag)   //����ŷ�
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
                

            if (tmpResult.count(move) == 0)    //û��ͳ����Ϣ��û��չ��������չ
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

        for (auto x : path)     //���򴫲�
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
