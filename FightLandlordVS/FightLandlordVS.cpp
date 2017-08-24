// FightLandlordVS.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include <iostream>
#include <time.h>
#include "random.h"
#include "card.h"
#include "type.h"
#include "game.h"
#include "traditional_intelligence.h"
#include "mcts_board.h"
#include "mcts_ucb.h"
#include "action_lib.h"
#include "type_util.h"
#include "monte_carlo_intelligence.h"
#include <unordered_set>
#include <algorithm>
#include <fstream>
#include <map>
#include <string>
#include <sstream>

using namespace std;

typedef LongLongGroupUtil Util;

struct test 
{
    static void testCanOutSize(int times)
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
            g.playCard(TrIntelligence::makeDecision(g));
            int st = clock();
            MCTS_Board board(g);
            vector<long long> co(10);
            board.getActions(co);
            for (int j = 0; j < 10000; j++)
                board.getActions(co);
            int ed = clock();
            sum += co.size();
            cout << co.size() << ":" << (double)sum / (i + 1) << endl;
            maxtime = max(maxtime, ed - st);
            sumtime += ed - st;
            cout << "maxTime:" << maxtime << ":" << (double)sumtime / (i + 1) << ":" << ed - st << endl;
        }

    }

    static void testReverseLab()
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

    static void testTypeUtil()
    {
        for (int i = 0; i < NumOfType; i++)
        {
            for (int j = TypeMinLength[i]; j <= TypeMaxLength[i]; j++)
            {
                cout<<TypeNameCnStr[i]<<"-len:"<<j<<"---" << TypeUtil::pairToId(pair<CardType, int>((CardType)i, j)) << endl;
            }
        }
    };

    static void testAI()
    {
        int result = 0;
        int play = 0;
        int doubleKill = 0;
        int draw = 0;
        for (int seed = 57; seed <= 300; seed++)
        {
            string str;
            stringstream ss;
            ss << seed;
            ss >> str;
            str = "seed-" + str + ".txt";
            ofstream fs(str, ios::out);

            bool winLandlord = false;
            bool winFarmer = false;
            Game game;
            game.start(seed);
            game.setLandlord();
            MCTS_Board(game).prt(fs);
            play++;
            while (!game.isEnd())
            {
                if (game.getCurrentGamer() == game.getLandlord())
                {
                    McIntelligence mc;
                    string move = mc.makeDecision(game).first;
                    fs << game.getCurrentGamer() << ":" << move << "\tMc" << endl;
                    game.playCard(move);
                }
                else
                {
                    string move = TrIntelligence::makeDecision(game);
                    fs << game.getCurrentGamer() << ":" << move << "\tTr" << endl;
                    game.playCard(move);
                }
            }
            if (game.getWinner() == 0)
            {
                cout << seed << ":\tMcAI胜！" << endl;
                fs << seed << ":\tMcAI胜！" << endl;
                result++;
                winLandlord = true;
            }
            else
            {
                cout << seed << ":\tTrAI胜！" << endl;
                fs << seed << ":\tTrAI胜！" << endl;
            }
            cout << "当前McAI胜利:" << result << "/" << play << endl;
            fs << "当前McAI胜利:" << result << "/" << play << endl;
            
            game = Game();
            game.start(seed);
            game.setLandlord();
            MCTS_Board(game).prt(fs);
            play++;
            while (!game.isEnd())
            {
                if (game.getCurrentGamer() != game.getLandlord())
                {
                    McIntelligence mc;
                    string move = mc.makeDecision(game).first;
                    fs << game.getCurrentGamer() << ":" << move << "\t:Mc" << endl;
                    game.playCard(move);
                }
                else
                {
                    string move = TrIntelligence::makeDecision(game);
                    fs << game.getCurrentGamer() << ":" << move << "\t:Tr" << endl;
                    game.playCard(move);
                }
            }
            if (game.getWinner() == 1)
            {
                cout << seed << ":\tMcAI胜！" << endl;
                fs << seed << ":\tMcAI胜！" << endl;
                result++;
                winFarmer = true;
            }
            else
            {
                cout << seed << ":\tTrAI胜！" << endl;
                fs << seed << ":\tTrAI胜！" << endl;
            }
            cout << "当前McAI胜利:" << result << "/" << play << endl;
            fs << "当前McAI胜利:" << result << "/" << play << endl;

            if (winFarmer&&winLandlord)
                doubleKill++;
            else if (winFarmer^winLandlord)
                draw++;
            cout << "双向胜利：" << doubleKill << "/" << play / 2 << endl;
            cout << "平局：" << draw << "/" << play / 2 << endl;
            fs << "双向胜利：" << doubleKill << "/" << play / 2 << endl;
            fs << "平局：" << draw << "/" << play / 2 << endl;
            MCTS_Board::init();
        }
    }

    static void testVector()
    {
        vector<string> a;
        a.push_back("100");
        a.push_back("200");
        vector<string> b;
        b = a;
        a[0][0] = '3';
        for (auto x : a)
            cout << x << ' ';
        cout << endl;
        for (auto x : b)
            cout << x << ' ';
        cout << endl;

        for (int i = 0; i < 10000000; i++)
        {
            a.push_back(string("1233"));
        }
        int st;
        st = clock();
        b = a;
        cout << "copyTime:" << clock() - st << endl;
    }
};

struct release
{
    static void runGame();
};

int _tmain(int argc, _TCHAR* argv[])
{
    TypeTest::init();
    ActionLib::init();
    MCTS_Board::init();

    //test::testVector();
    test::testAI();

    //test::testCanOutSize(1000);
    /*while (true)
    {
        test::testReverseLab();
    }*/
    //test::testTypeUtil();
    //release::runGame();
	system("pause");
	return 0;
}

void release::runGame()
{
    while (true)
    {
        Game g;
        g = Game();
        cout << "请输入种子，输入0随机开始，输入-1退出：";
        int seed;
        cin >> seed;
        if (seed == -1)
            return;

        int testType;
        cout << "请输入测试模式，0为人类操控所有玩家，1为人类当地主，2为人类当农民:" << endl;
        cin >> testType;

        g.start(seed);
        g.setLandlord();
        g.showTestStatus();

        int landlord = g.getLandlord();


        int est = clock();
        MCTS_Board board(g);
        MCTS_UCB mcts(board);
        //cout << "buildTime:" << clock() - est << endl;

        string str;
        cout << "FL >> ";
        while (!g.isEnd())
        {
            if ((testType == 1 && g.getCurrentGamer() != landlord) || (testType == 2 && g.getCurrentGamer() == landlord))
            {
                string str = TrIntelligence::makeDecision(g);
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
            else if (str == "sc" || str == "showCanOut")
            {
                MCTS_Board board(g);
                vector<long long> co(10);
                board.getActions(co);
                for (auto x : co)
                {
                    LongLongGroupUtil::prtInString(x);
                }
                cout << "size:" << co.size() << endl;
            }
            else if (str == "scs" || str == "showCanOutSize")
            {
                MCTS_Board board(g);
                vector<long long> co(10);
                board.getActions(co);
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
                cout << TrIntelligence::makeDecision(g) << endl;
            }
            else if (str == "u" || str == "ud" || str == "useDecision")
            {
                string str = TrIntelligence::makeDecision(g);
                cout << "【" << g.getCurrentGamer() << "】" << "使用智能决策出牌：" << str;
                if (str == "")
                    cout << "PASS";
                cout << endl;
                g.playCard(str);
                g.showTestStatus();
            }
            else if (str == "ucball")
            {
                MCTS_Board board(g);
                if (board == mcts.board)
                    ;
                else
                    mcts = MCTS_UCB(board);

                while (!g.isEnd())
                {
                    long long move = mcts.getBestAction();
                    Util::prtInString(move);
                    g.playCard(Util::getString(move));
                    g.showStatus();
                    mcts.play(move);
                }

            }
            else if (str == "ucb")
            {
                MCTS_Board board(g);
                if (board == mcts.board)
                    ;
                else
                {
                    cout << "Something goes wrong!" << endl;
                    board.prt();
                    mcts.board.prt();
                    mcts = MCTS_UCB(board);
                }
                
                int dst = clock();
                long long move = mcts.getBestAction();
                cout << "wholeTime:" << clock() - dst << endl;
                Util::prtInString(move);
                g.playCard(Util::getString(move));
                g.showStatus();
                mcts.play(move);
            }
            else if (str == "mc")
            {
                double winRate = 0;
                McIntelligence mc;
                auto data = mc.makeDecision(g,winRate);
                cout << "Monte-Carlo Intelligence Report: "<<endl;
                cout << "Best option: ";
                if (data.first == "") cout << "PASS";
                else cout << data.first;
                cout << endl;
                cout << "Type is: " << TypeNameCnStr[data.second] << endl;
                cout << "Estimated win rate:" << winRate << endl;
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