#ifndef mcts_game_H
#define mcts_game_H

#include <vector>
#include <unordered_map>
#include "stdafx.h"

#include "type.h"
#include "game.h"
using namespace std;

typedef pair<vector<long long>, vector<pair<int, int> > > ChoiceList;

class MCTS_Board
{
private:
    static unordered_map<long long, ChoiceList> choiceLib;

    long long hands[NumOfPlayer];     //������ҵ����ƣ���3��ʼ��ʾ��ÿ3bit��ʾһ���Ƶ�����
    CardType type;      //������ǰ�ڵ���Ҫ��������
    int len;            //������ǰ���Ƴ��ȣ��޳�������Ϊ0
    int pow;            //������ǰ���ƴ�С
    int cntPlayer;      //������ǰӦ�������
    bool lastPass;      //��һ����Ƿ�PASS

public:
    static void init();
    
    MCTS_Board();
    MCTS_Board(Game game);
    MCTS_Board(long long hands[], CardType type, int len, int pow, int cntPlayer, bool lastPass);

    inline int getcntPlayer()
    {
        return cntPlayer;
    }

    size_t getHashCode() const;
    bool operator== (const MCTS_Board& b) const;
    MCTS_Board& operator= (const MCTS_Board& b);
    
    const vector<long long>& getActions(vector<long long> &target) const;
    void play(long long x);
    int isWin();    //�Ƿ�����һ��ʤ�������з�����ұ�ţ����򷵻�-1
    void prt() const;
    void prt(ostream& out)const;
    long long getCntHand() const;
};

namespace std
{
    template<>
    struct hash < MCTS_Board >
    {
        size_t operator()(const MCTS_Board& a) const
        {
            return a.getHashCode();
        }
    };
}

#endif