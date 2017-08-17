#ifndef monte_carlo_intelligence_H
#define monte_carlo_intelligence_H

#include <unordered_map>
#include "type.h"
#include "game.h"

using namespace std;

class McIntelligence
{
private:
    static const int timeLimit;
    unordered_map<long long, pair<int, int> > results;
    long long selectBestMove(double& winRate);
    long long selectBestMove();
public:
    pair<string, CardType> makeDecision(const Game& game, double& winRate);
    pair<string, CardType> makeDecision(const Game& game);


};


#endif