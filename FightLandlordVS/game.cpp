/*************************
** �ļ�:game.cpp
** ����:Game���ʵ��
** ��ע:Game��Ľӿڲμ�game.h
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
	cout << "����������" << endl;
	for (int i = 0; i<NumOfPlayer; i++)
	{
		cout << "��ҡ�" << i << " ��:  ";
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
	cout << "�����ƣ�  ";
	for (auto p : landlordCard)
	{
		p.prtDigit();
	}
	cout << endl;
}

void Game::showLandlord() const
{
	cout << "������ ���" << "��" << landlord << " ��" << endl;
}

void Game::showBaseInformation() const
{
	showMessage("��Ϸ�Ļ�����Ϣ��");
    cout << "��Ϸ���ӣ�" << seed << endl;
	cout << "��Ϸ�ڡ�" << round << " �������ڽ��У��ֵ���ҡ�"
		<< getCurrentGamer() << " �����ơ�" << endl;
	showLandlord();
	showLandlordCard();
}

void Game::showHistory(int r) const
{
	if (r<0 || r >= round)
	{
		showError("������ͼ��ʾ�����ڵĻغ���ʷ��");
		return;
	}
    cout << "R-" << r << " ��ҡ�" << (r + landlord) % 3 << "��: ";
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
	cout << "��ǰ��Ҫ�� ";
	if (currentType == NoneType)
	{
		cout << "�������͡�" << endl;
	}
	else
	{
		cout << TypeNameCnStr[currentType];
		if (TypeHasLength[currentType])
			cout << " ����Ϊ��" << currentTypeLen;
        cout << " ��СΪ:" << currentPower << "�����棺" << NumToDigit[PowToNum[currentPower]] << ")" << endl;
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
	cout << "�׶Σ�" << stage << endl;
	switch (stage)
	{
	case 0:
		showMessage("��Ϸ��δ��ʼ��");
		break;
	case 1:
		showMessage("����ѡ�������");
		showLandlordCard();
		break;
	case 2:
		showMessage("��Ϸ���ڽ����С�");
		showBaseInformation();
        showCurrentRequired();
        cout << "ʣ�����ƣ�" << "��0����" << getHandSize(0) << "��    ��1����" << getHandSize(1) << "��    ��2����" << getHandSize(2) << "��" << endl;
        cout << "��ʷ��¼��" << endl;
        for (int i = max(0, round - 6); i < round; i++)
            showHistory(i);
		break;
	case 3:
		showMessage("��Ϸ������");
		cout << "�� �ҡ�" << winner << " �����Ƚ��Ƴ��ꡣ" << endl;
		cout << "ʤ����Ϊ: ";
		if (landlord == winner)
			cout << "����" << endl;
		else
			cout << "ũ��" << endl;
	}

	cout << "--------------------" << endl;
    if (stage == 2)
    {
        if (round != 0)
        {
            cout << "�ϼҳ��ƣ�";
            showHistory(round - 1);
        }
        if (currentType == NoneType)
            cout << "Ŀǰ��������ơ�" << endl;
        else if (history[round - 1].size() != 0)
        {
            cout << "ĿǰҪѹ����Ϊ��";
            for (auto x : history[round - 1])
                x.prtDigit();
            cout << endl;
        }
        else
        {
            cout << "ĿǰҪѹ����Ϊ��";
            for (auto x : history[round - 2])
                x.prtDigit();
            cout << endl;
        }
        cout << "����ҡ�" << getCurrentGamer() << " ������" << endl;
        cout << "�������ƣ�";
        for (auto x : hand[getCurrentGamer()])
            x.prtDigit();
        cout << endl;
    }
}

void Game::showStatus() const
{
    cout << "--------------------" << endl;
    cout << "�׶Σ�" << stage << endl;
    switch (stage)
    {
    case 0:
        showMessage("��Ϸ��δ��ʼ��");
        break;
    case 1:
        showMessage("����ѡ�������");
        showHand();
        showLandlordCard();
        break;
    case 2:
        showMessage("��Ϸ���ڽ����С�");
        showBaseInformation();
        showHand();
        if (round != 0)
        {
            cout << "��һ�˳��� ";
            showHistory(round - 1);
        }
        cout << "����ҡ�" << getCurrentGamer() << " ������" << endl;
        showCurrentRequired();
        break;
    case 3:
        showMessage("��Ϸ������");
        cout << "�� �ҡ�" << winner << " �����Ƚ��Ƴ��ꡣ" << endl;
        cout << "ʤ����Ϊ: ";
        if (landlord == winner)
            cout << "����" << endl;
        else
            cout << "ũ��" << endl;
    }

    cout << "--------------------" << endl;
}




void Game::showHistory() const
{
	showMessage("��ʷ��¼��");
	for (int i = 0; i<round; i++)
	{
		showHistory(i);
	}
}

void Game::showRestCard() const
{
	showMessage("ʣ����ƣ�");
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
		showError("������ͼ��ʼ�Ѿ���ʼ����Ϸ��");
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

		for (int i = NumOfPoker - 1; i >= 0; i--) //�����һЩ�����������
		{
			int nsw = r.next(0, i);
			swap(tmp[nsw], tmp[i]);
		}

		for (int i = 0; i<NumOfPlayer; i++)       //ƽ��������
		{
			for (int j = 0; j<NumOfHand; j++)
			{
				hand[i].push_back(tmp[i*NumOfHand + j]);
			}
			sort(hand[i].begin(), hand[i].end());
		}

		for (int i = NumOfPlayer*NumOfHand; i<NumOfPoker; i++)     //���������
		{
			landlordCard.push_back(tmp[i]);
		}
		sort(landlordCard.begin(), landlordCard.end());


		showMessage("��Ϸ��ʼ��");
		return true;
	}
}

bool Game::setLandlord(int lord)
{
	if (stage != 1)
	{
		showError("������ͼ�ڷ�ѡ������׶����õ�����");
		return false;
	}
	stage = 2;
	if (lord<0 || lord>NumOfPlayer)
	{
		showError("������ͼ�������ڵ��������Ϊ����");
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
		showError("������ͼ�ڷ�ѡ������׶����õ�����");
		return false;
	}
	return setLandlord(r.next(0, NumOfPlayer - 1));
}

bool Game::playCard(string strGroup, CardType type)
{
	if (stage != 2)
	{
		showError("������ͼ�ڷ���Ϸ�׶γ��ơ�");
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

	if (group.size() == 0) //��ϣ��������
	{
		if (currentType == NoneType)
		{
			showError("���󣺵�ǰ������ƣ��޷�pass��");
			return false;
		}
		else
		{
			if (history[round - 1].size() == 0)  //����һ�غ�Ҳpass
			{
				currentType = NoneType;
				currentTypeLen = 0;
				currentPower = 0;
			}
			round++;
			return true;
		}
	}

	//�������
	if (!TypeTest::isInGroup(group, cntHand)) //�Ʊ���������
	{
		showError("�����޷����ƣ������Ʋ���������");
		return false;
	}

	if (currentType == NoneType)           //�����ǰ����Ȩ����Ҫָ�����ͻ򳤶�
	{
		if (type == NoneType)              //��û�и�������
		{
			TypeTest::getType(group, type, cntLen);   //���ȡһ������
			if (type == NoneType)
			{
				showError("�����޷����ƣ������������������Ͷ���ƥ�䡣");
				return false;
			}
		}
		else
			cntLen = TypeTest::getLength(group, type); //ֻҪҪָ������
	}
	else    //����ǰû����Ȩ
	{
		if (type == NoneType)  //��ȻĬ������Ϊ��ǰ���ͻ��߻����ը��
		{
            if (TypeTest::isType[Zhadan](group, 0))
				type = Zhadan;
			else if (TypeTest::isType[Huojian](group, 0))
				type = Huojian;
			else
				type = currentType;
		}
		else if (type != currentType)  //�����ж�һ�����������Ƿ���ȷ��������ȷ����Ϊ��ȷ
		{
			if (type != Huojian&&type != Zhadan)
			{
				showError("�� �棺���������뵱ǰҪ�����Ͳ����������Զ���Ϊ��ǰ���ͣ�");
				type = currentType;
			}
		}
		cntLen = TypeTest::getLength(group, type);
	}

    int pow;
    pow = TypeTest::canOutPower(group, currentType, currentTypeLen, currentPower);   //����һ���ܷ����
	if (pow == -1)
	{
		showError("�� �棺�޷����ơ����������˲��������س���ʧ�ܣ�");
		return false;
	}

	auto q = cntHand.begin();         //�˶ν��г���
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

	q = restCard.begin();     //�˶θ���ʣ�����
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

	if (cntHand.empty())    //���ƺ�����Ϊ�գ� ��Ϸ����
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
		showError("�� �棺�ڷ���Ϸ�׶�ѯ�ʵ�ǰ������ҡ���������-1��");
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
		showError("�� �棺�ڷ���Ϸ�׶�ѯ�ʵ�ǰ���͡���������NoneType,��-1��");
		return NoneType;
	}
	return currentType;
}

int Game::getCurrentTypeLen() const
{
	if (stage != 2)
	{
		showError("�� �棺�ڷ���Ϸ�׶�ѯ�ʵ�ǰ���ͳ��ȡ���������-1��");
		return -1;
	}
	return currentTypeLen;
}

int Game::getCurrentTypePow() const
{
    if (stage != 2)
    {
        showError("�� �棺�ڷ���Ϸ�׶�ѯ�ʵ�ǰ���ͳ��ȡ���������-1��");
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
		showError("������ͼ��ȡ��������ҵ����ơ������������[0]�����ƣ�");
		return hand[0];
	}
	return hand[i];
}

const vector<Card>& Game::getLandlordCard() const
{
	return landlordCard;
}
