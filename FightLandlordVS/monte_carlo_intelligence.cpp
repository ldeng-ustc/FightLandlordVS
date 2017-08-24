#include "stdafx.h"
#include "monte_carlo_intelligence.h"
#include "mcts_ucb.h"
#include "action_lib.h"
#include <ctime>
#include <algorithm>

const int McIntelligence::timeLimit = (int)(17 * CLOCKS_PER_SEC);

pair<string, CardType> McIntelligence::makeDecision(const Game& game, double& winRate)
{
    int st = clock();
    times = 0;
    int round = game.getRound();
    int rest[NumOfPow] = { 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 1, 1 };
    for (int i = 0; i < round; i++)
    {
        const vector<Card>& his = game.getHistory(i);
        for (auto x : his)
        {
            rest[x.getPower()]--;
        }
    }
    const vector<Card>& hand = game.getHand(game.getCurrentGamer());
    long long cntHand = 0;
    for (auto x : hand)
    {
        rest[x.getPower()]--;
        cntHand = Util::addCard(cntHand, x.getPower());
    }

    long long hands[NumOfPlayer];
    CardType type = game.getCurrentType();
    int len = game.getCurrentTypeLen();
    int pow = game.getCurrentTypePow();
    int cntPlayer = game.getCurrentGamer();
    bool lastPass = game.isLastPlayerPass();
    int landlord = game.getLandlord();

    vector<int> restCard;
    for (int i = 0; i < NumOfPow; i++)
    {
        for (int j = 0; j < rest[i]; j++)
        {
            restCard.push_back(i);
        }
    }

    random r(1725);
    while (clock() - st < timeLimit)
    {
        for (int i = restCard.size() - 1; i >= 0; i--)
        {
            int rand = r.next(0, i);
            swap(restCard[rand], restCard[i]);
        }

        int count = 0;
        for (int i = 0; i < NumOfPlayer; i++)
        {
            hands[i] = 0;
            if ((landlord + i) % NumOfPlayer == cntPlayer)
            {
                //cout << "setHand:" << i << endl;
                hands[i] = cntHand;
            }
            else
            {
                int siz = game.getHandSize((landlord + i) % NumOfPlayer);
                for (int j = 0; j < siz; j++)
                {
                    hands[i] = Util::addCard(hands[i], restCard[count++]);
                }
            }
        }
        int boardCntPlayer = cntPlayer + NumOfPlayer - landlord;
        boardCntPlayer %= NumOfPlayer;
        MCTS_Board board(hands, type, len, pow, boardCntPlayer, lastPass);
        MCTS_UCB mcts(board);

        vector<long long> choices(10);
        board.getActions(choices);
        if (choices.size() == 1)
        {
            results.insert(pair<long long, double>(choices[0], 0.5));
            break;
        }
        else
        {
            bool flag = false;
            for (auto x : choices)
            {
                if (x == cntHand)
                {
                    results.insert(pair<long long, double>(x, 1));
                    flag = true;
                    break;
                }
            }
            if (flag)
                break;
        }

        //board.prt();
        mcts.getBestAction();
        auto &data = mcts.getResultList();
        vector<long long> moveList;
        vector<int> playList;
        vector<int> winList;
        int sumPlay=0;
        int sumWin=0;
        for (auto x : data)
        {
            moveList.push_back(x.first);
            playList.push_back(x.second.second);
            winList.push_back(x.second.first);
            sumPlay += x.second.second;
            sumWin += x.second.first;
        }
        double maxRate = 0;
        double secRate = 0;
        for (int i = 0; i < (int)moveList.size(); i++)
        {
            double &data = results[moveList[i]];
            data += (double)winList[i] / sumWin;
            if (data > maxRate)
            {
                secRate = maxRate;
                maxRate = data;
            }
            else if (data > secRate)
                secRate = data;
        }
        times++;
        if (maxRate - secRate > 3)  //结果足够好了，跳出
            break;
    }//调试中析构可能很慢，看起来像挂了

    long long move = selectBestMove(winRate);
    CardType actionType = ActionLib::findTypeLib[move].first;
    return pair<string, CardType>(Util::getString(move), actionType);
}

pair<string, CardType> McIntelligence::makeDecision(const Game& game)
{
    double x;
    return makeDecision(game, x);
}

long long McIntelligence::selectBestMove(double& winRate)
{
    winRate = -1.0;
    long long move = 0;

    for (auto data : results)
    {
        /*int win = data.second.first;
        int play = data.second.second;
        if (play != 0)
        {
            double cntRate = win / (double)play;
            if (cntRate > winRate)
            {
                move = data.first;
                winRate = cntRate;
            }
            cout << win << "/" << play << " : " << cntRate << ":\t" << Util::getString(data.first) << endl;
        }*/
        if (data.second > winRate)
        {
            winRate = data.second;
            move = data.first;
        }
        //cout << "rate:" << data.second << ":\t" << Util::getString(data.first) << endl;
    }
    //if (times != 0)
        //winRate /= times;
    //else
        //winRate = NAN;
    winRate = INFINITY;
    return move;
}

long long McIntelligence::selectBestMove()
{
    double x;
    return selectBestMove(x);
}

using namespace std;

