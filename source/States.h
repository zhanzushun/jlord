#pragma once

#include "Config.h"

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//����״̬

class CStateInit;				//��ʼ״̬
class CStateSendCards;			//����״̬
class CStateDecideBanker;		//ȷ��ׯ��
class CStatePlayCards;			//����״̬

class GStatus
{
public:
	static CStateInit s_stateInit;
	static CStateSendCards s_stateSendCards;
	static CStatePlayCards s_statePlayCards;
	static CStateDecideBanker s_stateDecideBanker;
};

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//��ʼ״̬ (�ȴ�������Ҷ�ͬ����һ��)

class CStateInit : public IState
{
	//�ӿ�
public:
	virtual void SetActive();
	virtual void RunOneFrame();

	//�߳�
public:
	static void __cdecl ThreadFunc (void* pData);
protected:
	HANDLE m_hEventEndThread;
	void pOnThreadEnd();
};

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//����״̬

class CStateSendCards : public IState
{
//�ӿ�
public:
	virtual void SetActive();
	virtual void RunOneFrame();

//�߳�
public:
	static void __cdecl ThreadFunc (void* pData);
protected:
	HANDLE m_hEventEndThread;
	void pOnThreadEnd();

//˽�к���
	void pDrawMoveCard();

//����
	int m_nCardIndex;
	int m_nPlayerIndex;
	CPoint m_ptCardPos;
};

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//ѡ��ׯ��״̬

class CStateDecideBanker : public IState
{
	//�ӿ�
public:
	virtual void SetActive();
	virtual void RunOneFrame();

	//�߳�
public:
	static void __cdecl ThreadFunc (void* pData);
protected:
	HANDLE m_hEventEndThread;
	
	//����
	int m_nTempCount;//��Ը��ׯ�ҵĸ���
};

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//����״̬

class CStatePlayCards : public IState
{
	//�ӿ�
public:
	virtual void SetActive();
	virtual void RunOneFrame();
	virtual void OnMouseClick();

	//�߳�
public:
	static void __cdecl ThreadFunc (void* pData);
protected:
	HANDLE m_hEventEndThread;

	//����
	bool m_bPlaying;				//���˿ɳ���
	HANDLE m_hEventPrepareStart;
	HANDLE m_hEventPrepareEnd;
	HANDLE m_hEventPlayingStart;
	HANDLE m_hEventPlayingEnd;
};

