#ifndef game_H
#define game_H

#include <iostream>
#include <string>
#include <vector>
#include "random.h"
#include "type.h"
using namespace std;

inline void showError(string msg)
{
	cout << "********************" << endl;
	cout << msg << endl;
	cout << "********************" << endl;
}

inline void showMessage(string msg)
{
	cout << msg << endl;
}

class Game
{
private:
    random r;
    int seed;
	int stage;          //0:start 1:choose landlord  2:gaming  3:end
	int round;          //ÿ����ҳ�����һ�֣���0��ʼ��������0�ֵ�������
	int landlord;
	int winner;
	CardType currentType;
	int currentTypeLen;
	int currentPower;
	vector<Card> hand[NumOfPlayer];    //hand ,hand[landlord] is the hand of landlord
	vector<Card> landlordCard;       //the open card of landlord, usually 3 cards
	vector<Card> history[MaxRound];   //history of every round
	vector<Card> restCard;
public:
    Game();                             //����һ����δ��ʼ������Ϸ
	void showHand() const;            //��ӡ��ҵ������������Ļ
	void showLandlordCard() const;    //��ӡ������Ϸ�ĵ����Ƶ���Ļ
	void showLandlord() const;        //��ӡ������Ϸ�ĵ�������Ļ
	void showBaseInformation() const; //��ӡ������Ϸ�Ļ�����Ϣ����Ļ�����е��ڼ��غϣ��ֵ�˭���ƣ������͵����������
	void showHistory(int r) const;
	void showCurrentRequired() const;
	void showStatus() const;            //��ӡ��ǰ��Ϸ��״̬����Ļ���׶Σ�������ҵ����ƣ�
	void showHistory() const;
	void showRestCard() const;
    void showTestStatus() const;        //��ӡ��Ϸʱ״̬��ֻ�ɼ���������������
	bool start(int seed = 0);             //��ʼ��Ϸ��ֻ����δ��ʼ����Ϸ��Ч����stage==0��
	bool setLandlord(int lord);         //���õ���Ϊ���lord����ң�ֻ�����ڽ��е���ѡ�����Ϸ��Ч����stage==1��
	bool setLandlord();                 //������õ�����ͬ�ϣ�
	bool playCard(string strGroup, CardType type = NoneType); //���ƣ��ɲ�ָ�����ͣ��������尴�ɴ���ĵ�һ������ʶ��˳�����ͱ�ţ�
	bool playCard(Group& group, CardType type = NoneType); //����֤group�����ı�
    bool isLastPlayerPass() const;            //��һ������Ƿ�pass
	int getStage() const;                               //���ص�ǰ��Ϸ�׶�
	bool isEnd() const;
	int getCurrentGamer() const;                     //���ص�ǰ�غ���Ҫ���Ƶ����
    int getLandlord() const;
	CardType getCurrentType() const;
	int getCurrentTypeLen() const;
    int getCurrentTypePow() const;
	const vector<Card>& getHand(int i) const;       //�������i�����ƣ����ܱ��޸ģ�
	const vector<Card>& getLandlordCard() const;    //���ر�����Ϸ�ĵ����ƣ����ܱ��޸ģ�
    int getHandSize(int i) const;
};

#endif // game

