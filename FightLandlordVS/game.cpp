/*************************
** 文件:game.cpp
** 描述:Game类的实现
** 备注:Game类的接口参见game.h
**************************/

#include "stdafx.h"
#include <iostream>
#include <vector>
#include <algorithm>
#include <ctime>
#include "random.h"
#include "card.h"
#include "type.h"
#include "game.h"
using namespace std;


void Game::showHand() const
{
	cout << "玩家手牌情况" << endl;
	for (int i = 0; i<NumOfPlayer; i++)
	{
		cout << "玩家【" << i << " 】:  ";
		for (auto p : hand[i])
		{
			p.prtDigit();
			//p.prt();
		}
		cout << endl;
	}
}

void Game::showLandlordCard() const
{
	cout << "地主牌：  ";
	for (auto p : landlordCard)
	{
		p.prtDigit();
	}
	cout << endl;
}

void Game::showLandlord() const
{
	cout << "地主： 玩家" << "【" << landlord << " 】" << endl;
}

void Game::showBaseInformation() const
{
	showMessage("游戏的基本信息：");
    cout << "游戏种子：" << seed << endl;
	cout << "游戏第【" << round << " 】轮正在进行，轮到玩家【"
		<< getCurrentGamer() << " 】出牌。" << endl;
	showLandlord();
	showLandlordCard();
}

void Game::showHistory(int r) const
{
	if (r<0 || r >= round)
	{
		showError("错误：试图显示不存在的回合历史。");
		return;
	}
    cout << "R-" << r << " 玩家【" << (r + landlord) % 3 << "】: ";
	if (history[r].empty())
	{
		cout << "PASS" << endl;
	}
	else
	{
		for (auto p : history[r])
		{
			cout << p.getDigit();
		}
		cout << endl;
	}
}

void Game::showCurrentRequired() const
{
	cout << "当前需要： ";
	if (currentType == NoneType)
	{
		cout << "任意牌型。" << endl;
	}
	else
	{
		cout << TypeNameCnStr[currentType];
		if (TypeHasLength[currentType])
			cout << " 长度为：" << currentTypeLen;
        cout << " 大小为:" << currentPower << "（牌面：" << NumToDigit[PowToNum[currentPower]] << ")" << endl;
	}
}

Game::Game()
{
	stage = 0;
	round = 0;
}

void Game::showTestStatus() const
{
	cout << "--------------------" << endl;
	cout << "阶段：" << stage << endl;
	switch (stage)
	{
	case 0:
		showMessage("游戏尚未开始。");
		break;
	case 1:
		showMessage("正在选择地主。");
		showLandlordCard();
		break;
	case 2:
		showMessage("游戏正在进行中。");
		showBaseInformation();
        showCurrentRequired();
        cout << "剩余手牌：" << "【0】：" << getHandSize(0) << "张    【1】：" << getHandSize(1) << "张    【2】：" << getHandSize(2) << "张" << endl;
        cout << "历史记录：" << endl;
        for (int i = max(0, round - 6); i < round; i++)
            showHistory(i);
		break;
	case 3:
		showMessage("游戏结束。");
		cout << "玩 家【" << winner << " 】率先将牌出完。" << endl;
		cout << "胜利者为: ";
		if (landlord == winner)
			cout << "地主" << endl;
		else
			cout << "农民" << endl;
	}

	cout << "--------------------" << endl;
    if (stage == 2)
    {
        if (round != 0)
        {
            cout << "上家出牌：";
            showHistory(round - 1);
        }
        if (currentType == NoneType)
            cout << "目前可随意出牌。" << endl;
        else if (history[round - 1].size() != 0)
        {
            cout << "目前要压的牌为：";
            for (auto x : history[round - 1])
                x.prtDigit();
            cout << endl;
        }
        else
        {
            cout << "目前要压的牌为：";
            for (auto x : history[round - 2])
                x.prtDigit();
            cout << endl;
        }
        cout << "请玩家【" << getCurrentGamer() << " 】出牌" << endl;
        cout << "您的手牌：";
        for (auto x : hand[getCurrentGamer()])
            x.prtDigit();
        cout << endl;
    }
}

void Game::showStatus() const
{
    cout << "--------------------" << endl;
    cout << "阶段：" << stage << endl;
    switch (stage)
    {
    case 0:
        showMessage("游戏尚未开始。");
        break;
    case 1:
        showMessage("正在选择地主。");
        showHand();
        showLandlordCard();
        break;
    case 2:
        showMessage("游戏正在进行中。");
        showBaseInformation();
        showHand();
        if (round != 0)
        {
            cout << "上一人出牌 ";
            showHistory(round - 1);
        }
        cout << "请玩家【" << getCurrentGamer() << " 】出牌" << endl;
        showCurrentRequired();
        break;
    case 3:
        showMessage("游戏结束。");
        cout << "玩 家【" << winner << " 】率先将牌出完。" << endl;
        cout << "胜利者为: ";
        if (landlord == winner)
            cout << "地主" << endl;
        else
            cout << "农民" << endl;
    }

    cout << "--------------------" << endl;
}




void Game::showHistory() const
{
	showMessage("历史记录：");
	for (int i = 0; i<round; i++)
	{
		showHistory(i);
	}
}

void Game::showRestCard() const
{
	showMessage("剩余的牌：");
	for (auto p : restCard)
	{
		p.prtDigit();
	}
	cout << endl;
}

bool Game::start(int seed)
{
    if (stage != 0)
	{
		//showError("Error: Try to start a started game.");
		showError("错误：试图开始已经开始的游戏。");
		return false;
	}
	else
	{
		if (seed == 0)
			seed = (int)time(NULL);
        this->seed = seed;
		stage = 1;
		int tmp[NumOfPoker];
		r = random(seed);
		for (int i = 0; i<NumOfPoker; i++)
		{
			tmp[i] = ((i / 13) << 4) + (i % 13 + 1);
			if (i / 13 == 4)
				tmp[i] += 13;
			restCard.push_back(tmp[i]);
		}
		sort(restCard.begin(), restCard.end());

		for (int i = NumOfPoker - 1; i >= 0; i--) //随机将一些数交换到最后
		{
			int nsw = r.next(0, i);
			swap(tmp[nsw], tmp[i]);
		}

		for (int i = 0; i<NumOfPlayer; i++)       //平均分配牌
		{
			for (int j = 0; j<NumOfHand; j++)
			{
				hand[i].push_back(tmp[i*NumOfHand + j]);
			}
			sort(hand[i].begin(), hand[i].end());
		}

		for (int i = NumOfPlayer*NumOfHand; i<NumOfPoker; i++)     //分配地主牌
		{
			landlordCard.push_back(tmp[i]);
		}
		sort(landlordCard.begin(), landlordCard.end());


		showMessage("游戏开始。");
		return true;
	}
}

bool Game::setLandlord(int lord)
{
	if (stage != 1)
	{
		showError("错误：试图在非选择地主阶段设置地主。");
		return false;
	}
	stage = 2;
	if (lord<0 || lord>NumOfPlayer)
	{
		showError("错误：试图将不存在的玩家设置为地主");
		return false;
	}
	landlord = lord;
	for (auto p : landlordCard)
	{
		hand[landlord].push_back(p);
	}
	sort(hand[landlord].begin(), hand[landlord].end());
	currentType = NoneType;
	currentTypeLen = 0;
    currentPower = 0;
	return true;
}

bool Game::setLandlord()
{
	if (stage != 1)
	{
		showError("错误：试图在非选择地主阶段设置地主。");
		return false;
	}
	return setLandlord(r.next(0, NumOfPlayer - 1));
}

bool Game::playCard(string strGroup, CardType type)
{
	if (stage != 2)
	{
		showError("错误：试图在非游戏阶段出牌。");
		return false;
	}
	Group group;
	TypeTest::makeGroup(strGroup, group);
	return playCard(group, type);
}

bool Game::playCard(Group& group, CardType type)
{
	int cntGamer = getCurrentGamer();
	Group& cntHand = hand[cntGamer];
	int cntLen = currentTypeLen;

	if (group.size() == 0) //若希望不出牌
	{
		if (currentType == NoneType)
		{
			showError("错误：当前必须出牌，无法pass。");
			return false;
		}
		else
		{
			if (history[round - 1].size() == 0)  //若上一回合也pass
			{
				currentType = NoneType;
				currentTypeLen = 0;
				currentPower = 0;
			}
			round++;
			return true;
		}
	}

	//若想出牌
	if (!TypeTest::isInGroup(group, cntHand)) //牌必须在手中
	{
		showError("错误：无法出牌，所出牌不在手牌中");
		return false;
	}

	if (currentType == NoneType)           //如果当前有牌权，这要指定类型或长度
	{
		if (type == NoneType)              //若没有给定类型
		{
			TypeTest::getType(group, type, cntLen);   //则获取一波类型
			if (type == NoneType)
			{
				showError("错误：无法出牌，所给牌组与任意牌型都不匹配。");
				return false;
			}
		}
		else
			cntLen = TypeTest::getLength(group, type); //只要要指定长度
	}
	else    //若当前没有牌权
	{
		if (type == NoneType)  //显然默认类型为当前类型或者火箭或炸弹
		{
            if (TypeTest::isType[Zhadan](group, 0))
				type = Zhadan;
			else if (TypeTest::isType[Huojian](group, 0))
				type = Huojian;
			else
				type = currentType;
		}
		else if (type != currentType)  //否则，判断一下所给类型是否正确，若不正确，改为正确
		{
			if (type != Huojian&&type != Zhadan)
			{
				showError("警 告：所给牌型与当前要求牌型不符。（将自动改为当前牌型）");
				type = currentType;
			}
		}
		cntLen = TypeTest::getLength(group, type);
	}

    int pow;
    pow = TypeTest::canOutPower(group, currentType, currentTypeLen, currentPower);   //测试一下能否进行
	if (pow == -1)
	{
		showError("警 告：无法出牌。（已跳过此操作，返回出牌失败）");
		return false;
	}

	auto q = cntHand.begin();         //此段进行出牌
	auto p = group.begin();
	for (; p != group.end();)
	{
		if (q->getNum() == p->getNum())
		{
			history[round].push_back(*q);
			q = cntHand.erase(q);
			p++;
		}
		else
			q++;
	}

	q = restCard.begin();     //此段更新剩余的牌
	p = history[round].begin();
	for (; p != history[round].end();)
	{
		if (q->getId() == p->getId())
		{
			q = restCard.erase(q);
			p++;
		}
		else
			q++;
	}

	round++;
	currentType = type;
	currentTypeLen = cntLen;
    if (!TypeHasLength[type])
        currentTypeLen = 0;
	currentPower = pow;

	if (cntHand.empty())    //出牌后手牌为空， 游戏结束
	{
		stage = 3;
		winner = cntGamer;
	}
	return true;
}

bool Game::isLastPlayerPass() const
{
    if (round == 0)
        return false;
    return (history[round - 1].size() == 0 && currentType != NoneType);
}

int Game::getStage() const
{
	return stage;
}

bool Game::isEnd() const
{
	return stage == 3;
}

int Game::getHandSize(int i) const
{
    return hand[i].size();
}

int Game::getCurrentGamer() const
{
	if (stage != 2)
	{
		showError("警 告：在非游戏阶段询问当前出牌玩家。（将返回-1）");
		return -1;
	}
	return (landlord + round) % NumOfPlayer;
}

int Game::getLandlord() const
{
    return landlord;
}

CardType Game::getCurrentType() const
{
	if (stage != 2)
	{
		showError("警 告：在非游戏阶段询问当前牌型。（将返回NoneType,即-1）");
		return NoneType;
	}
	return currentType;
}

int Game::getCurrentTypeLen() const
{
	if (stage != 2)
	{
		showError("警 告：在非游戏阶段询问当前牌型长度。（将返回-1）");
		return -1;
	}
	return currentTypeLen;
}

int Game::getCurrentTypePow() const
{
    if (stage != 2)
    {
        showError("警 告：在非游戏阶段询问当前牌型长度。（将返回-1）");
        return -1;
    }
    return currentPower;
}

int Game::getRound() const
{
    return round;
}

const vector<Card>& Game::getHistory(int round) const
{
    return history[round];
}

const vector<Card>& Game::getHand(int i) const
{
	if (i>NumOfPlayer || i<0)
	{
		showError("错误：试图读取不存在玩家的手牌。（将返回玩家[0]的手牌）");
		return hand[0];
	}
	return hand[i];
}

const vector<Card>& Game::getLandlordCard() const
{
	return landlordCard;
}
