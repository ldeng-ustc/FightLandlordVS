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