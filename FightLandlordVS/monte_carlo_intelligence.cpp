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

    vector<int> restCard;
    for (int i = 0; i < NumOfPow; i++)
    {
        for (int j = 0; j < rest[i]; j++)
        {
            restCard.push_back(i);
        }
    }

    random r((int)clock());
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
            if (i == cntPlayer)
            {
                hands[i] = cntHand;
            }
            else
            {
                int siz = game.getHandSize(i);
                for (int j = 0; j < siz; j++)
                {
                    hands[i] = Util::addCard(hands[i], restCard[count++]);
                }
            }
        }

        MCTS_Board board(hands, type, len, pow, cntPlayer, lastPass);
        MCTS_UCB mcts(board);
        //board.prt();
        mcts.getBestAction();
        auto &data = mcts.getResultList();
        for (auto x : data)
        {
            auto &winAndPlay = results[x.first];
            winAndPlay.first += x.second.first;
            winAndPlay.second += x.second.second;
        }
    }

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
        int win = data.second.first;
        int play = data.second.second;
        if (play != 0)
        {
            double cntRate = win / (double)play;
            if (cntRate > winRate)
            {
                move = data.first;
                winRate = cntRate;
            }
            //cout << Util::getString(data.first) << endl << win << "/" << play << " : " << cntRate << endl << endl;
        }
    }
    return move;
}

long long McIntelligence::selectBestMove()
{
    double x;
    selectBestMove(x);
}

using namespace std;

