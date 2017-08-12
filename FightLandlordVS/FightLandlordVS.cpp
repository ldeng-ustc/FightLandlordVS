// FightLandlordVS.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include <iostream>
#include <time.h>
#include "random.h"
#include "card.h"
#include "type.h"
#include "game.h"
#include "intelligence.h"
#include "mcts_board.h"
#include "mcts_ucb.h"
#include "action_lib.h"
#include <unordered_set>
#include <algorithm>

using namespace std;

typedef LongLongGroupUtil Util;

struct test 
{
    static void TestCanOutSize(int times)
    {
        srand((unsigned)time(NULL));
        long long sum = 0;
        int maxtime = 0;
        int sumtime = 0;
        for (int i = 0; i < times; i++)
        {
            Game g;
            g.start(rand());
            g.setLandlord();
            g.playCard(Intelligence::makeDecision(g));
            int st = clock();
            MCTS_Board board(g);
            vector<long long> &co = board.getActions();
            for (int j = 0; j < 10000;j++)
                board.getActions();
            int ed = clock();
            sum += co.size();
            cout << co.size() << ":" << (double)sum / (i + 1) << endl;
            maxtime = max(maxtime, ed - st);
            sumtime += ed - st;
            cout << "maxTime:" << maxtime << ":" << (double)sumtime / (i + 1) << ":" << ed - st << endl;
        }

    }

    static void TestReverseLab()
    {
        string str;
        cin >> str;
        long long group = 0;
        for (auto x : str)
        {
            group = Util::addCard(group, Card(x).getPower());
        }

        CardType type = ActionLib::findTypeLib[group].first;
        int len = ActionLib::findTypeLib[group].second;
        int pow = ActionLib::findPowLib[type][len][group];
        cout << TypeNameCnStr[type] << " Len:" << len << " Pow:" << pow << endl;
    }

};

struct release
{
    static void runGame()
    {
        while (true)
        {
            Game g;
            g = Game();
            cout << "请输入种子，输入0随机开始：";
            int seed;
            cin >> seed;

            int testType;
            cout << "请输入测试模式，0为人类操控所有玩家，1为人类当地主，2为人类当农民:" << endl;
            cin >> testType;

            g.start(seed);
            g.setLandlord();
            g.showTestStatus();

            int landlord = g.getLandlord();
            string str;
            cout << "FL >> ";
            while (!g.isEnd())
            {
                if ((testType == 1 && g.getCurrentGamer() != landlord) || (testType == 2 && g.getCurrentGamer() == landlord))
                {
                    string str = Intelligence::makeDecision(g);
                    cout << "【" << g.getCurrentGamer() << "】" << "使用智能决策出牌：" << str;
                    if (str == "")
                        cout << "PASS";
                    cout << endl;
                    g.playCard(str);
                    g.showTestStatus();
                    continue;
                }

                cin >> str;
                if (str == "*" || str == "p" || str == "pass")
                {
                    g.playCard("");
                    g.showTestStatus();
                }
                else if (str == "~" || str == "quit" || str == "exit")
                {
                    break;
                }
                else if (str=="sc"||str == "showCanOut")
                {
                    MCTS_Board board(g);
                    vector<long long> &co = board.getActions();
                    for (auto x : co)
                    {
                        LongLongGroupUtil::prtInString(x);
                    }
                    cout << "size:" << co.size() << endl;
                }
                else if (str == "scs" || str == "showCanOutSize")
                {
                    MCTS_Board board(g);
                    vector<long long> &co = board.getActions();
                    cout << "size:" << co.size() << endl;
                }
                else if (str == "sh" || str == "showHistory")
                {
                    g.showHistory();
                }
                else if (str == "st" || str == "showStatus")
                {
                    g.showStatus();
                }
                else if (str == "sr" || str == "showRestCard")
                {
                    g.showRestCard();
                }
                else if (str == "test" || str == "t")
                {
                    TypeTest::test();
                    continue;
                }
                else if (str == "md" || str == "makeDecision")
                {
                    cout << Intelligence::makeDecision(g) << endl;
                }
                else if (str == "u" || str == "ud" || str == "useDecision")
                {
                    string str = Intelligence::makeDecision(g);
                    cout << "【" << g.getCurrentGamer() << "】" << "使用智能决策出牌：" << str;
                    if (str == "")
                        cout << "PASS";
                    cout << endl;
                    g.playCard(str);
                    g.showTestStatus();
                }
                else if (str == "ucball")
                {
                    while (!g.isEnd())
                    {
                        MCTS_UCB mcts(g);
                        long long move = mcts.getBestAction();
                        Util::prtInString(move);
                        g.playCard(Util::getString(move));
                        g.showStatus();
                    }
                    
                }
                else if (str == "ucb")
                {
                    MCTS_UCB mcts(g);
                    long long move = mcts.getBestAction();
                    Util::prtInString(move);
                    g.playCard(Util::getString(move));
                    g.showStatus();
                }
                else
                {
                    g.playCard(str);
                    g.showTestStatus();
                }
                if (!g.isEnd())
                    cout << "FL >> ";
            }
        }

    }
};

int _tmain(int argc, _TCHAR* argv[])
{
    TypeTest::init();
    ActionLib::init();
    //test::TestCanOutSize(1000);
    /*while (true)
    {
        test::TestReverseLab();
    }*/
    release::runGame();
	system("pause");
	return 0;
}

