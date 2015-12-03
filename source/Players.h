#pragma once

#include "Config.h"

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//�����
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

template<typename T>
bool PtrLessThan(T* t1, T* t2) {return (*t1) < (*t2);}

class CPlayer
{
public:
	virtual ~CPlayer() {}

	struct BaseInfo
	{
		BaseInfo();
		TCHAR sName[16];				//����
		TCHAR sImage[16];				//ͷ��
		int nScore;						//�÷�
		int nID;						//�Է��������ڵ����Ϊ0���,�����ƴ�����(��ʱ��)
	};

	struct CardsInfo
	{
		Card arrHandCards[SET_CARDS];	//�����ϵ���
		Card arrOutCards[SET_CARDS];	//�������
		Card arrPlayingCards[SET_CARDS];//���ڴ����
		int nHandCards;					//���ϵ��Ƶĸ���
		int nOutCards;					//���ȥ���Ƶĸ���
		int nPlayingCards;				//���ڴ���Ƶĸ���
		int arrPrepareCards[SET_CARDS];	//������׼���������(�������Ƹ߳�һ���), ���������arrHandCards�е�����ֵ
		int nPrepareCards;				//������׼��������Ƶĸ���
	};

	BaseInfo m_baseInfo;				//������Ϣ
	CardsInfo m_cardsInfo;				//�Ƶ���Ϣ

public:

	void PlayCard();					//����(prepare to playing)
	void RemovePlayingCards();			//һ�ֽ���(playing to out)
	void AddBottomCards();				//�ѵ��Ƽ���
	void SortHandCards();				//�����ϵ�������
	int GetIndex();						//ȡ����players�е�����
	void GetPrepareCards(vector<Card>& cards);

	bool operator < (const CPlayer& player2); //��������(������������CPlayer���������Լ�Ϊ0,������˳��)
};

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//��Ҽ���
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

class CPlayers
{
public:
	~CPlayers() {Clear();}
	static Card s_AllCards[SET_CARDS*2];		//��ǰ�����е���
	static int s_nPlayingID;					//��ǰ�ֵ�ǰ������
	static int s_nPlayingID_MaxCard;			//��ǰ�ֱ����������
	static int s_nBankerID;						//��ǰ��ׯ��
	static int s_nRandomBankerID;				//��ǰ�����ׯ��
	static int s_nTurnCounts;					//��ǰ������
	static int s_nNewTurnID;					//�µ�һ�ֳ�����(-1��ʾ���µ�һ��)


	//��Ϸ��ʼ��
	void Init();

	//��ȡ����ֵ
	CPlayer* operator [] (int nIndex);

	//��ȡserver���ڵ�index
	int GetServerPlayerIndex();

	//����ID����
	CPlayer* GetPlayerByID(int nID);

	//������һ��ID
	int	GetNextPlayerID(int nID);

	//������һ��ID
	int	GetPrevPlayerID(int nID);

	//�� ID �����������(������������CPlayer���������Լ�Ϊ0,������˳��)
	void Sort();

	//��һ��
	void ResetData();

	//����
	size_t size() {return m_arrPlayers.size();}

	void Clear();

private:
	vector<CPlayer*> m_arrPlayers;				//�������
};