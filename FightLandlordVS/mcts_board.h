#ifndef mcts_game_H
#define mcts_game_H

#include <vector>
#include <unordered_map>
#include "stdafx.h"

#include "type.h"
#include "game.h"
using namespace std;

class MCTS_Board
{
private:
    static unordered_map<long long, vector<long long> > choiceLib;

    long long hands[NumOfPlayer];     //������ҵ����ƣ���3��ʼ��ʾ��ÿ3bit��ʾһ���Ƶ�����
    CardType type;      //������ǰ�ڵ���Ҫ��������
    int len;            //������ǰ���Ƴ��ȣ��޳�������Ϊ0
    int pow;            //������ǰ���ƴ�С
    int cntPlayer;      //������ǰӦ�������
    bool lastPass;      //��һ����Ƿ�PASS

public:
    MCTS_Board(Game game);

    inline int getcntPlayer()
    {
        return cntPlayer;
    }

    size_t getHashCode() const;
    bool operator== (const MCTS_Board& b) const;
    MCTS_Board& operator= (const MCTS_Board& b);
    
    vector<long long>& getActions() const;
    void play(long long x);
    int isWin();    //�Ƿ�����һ��ʤ�������з�����ұ�ţ����򷵻�-1
    void prt();
    void prt(ostream& out);
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