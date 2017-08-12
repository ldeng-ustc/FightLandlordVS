#include "stdafx.h"
#include "mcts_ucb.h"

int MCTS_UCB::timeLimit = (int)(4 * CLOCKS_PER_SEC);
double MCTS_UCB::confident = 1.96;
unordered_map<MCTS_Board, vector<long long> > MCTS_UCB::choiceLib;

const vector<long long>& MCTS_UCB::getChoice(MCTS_Board board)
{
    bool hasGetAction = (choiceLib.count(board) != 0);
    vector<long long>& choices = hasGetAction ? choiceLib[board] : board.getActions();
    if (!hasGetAction)
        choiceLib[board] = choices;
    return choices;
}


MCTS_UCB::MCTS_UCB(Game game) :board(game), r(1725)
{
    sumPlay = 0;
    winAndPlay.clear();
}

long long MCTS_UCB::getBestAction()
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

void MCTS_UCB::runSimulations(MCTS_Board& board)
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

long long MCTS_UCB::selectBestMove()
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
