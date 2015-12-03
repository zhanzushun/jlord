#pragma once

#include "Config.h"
#include "Wnd.h"
#include "Players.h"

class CApp;
extern CApp theApp;

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//Ӧ�ó�����
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#define FRAME_TIME 20	//ÿ֡ʱ�� : 20����

class CApp : public IApplication
{
	friend class CMainWnd;
public:
	CApp();

	//����
	virtual HINSTANCE GetHInstance() {return m_hInstance;}
	CPoint		GetMousePos();
	bool		IsMouseDown() {return m_bMouseDown;}
	Sprite*		GetCurrentSprite() {return m_pCurrentSprite;}
	Sprite*		GetSprite(const tstring& sSprite) {return GSprites::Obj()->GetSprite(sSprite);}
	bool		IsActive() {return m_bActive;}
	IState*		GetCurrentState() {return m_pCurrentState;}

	//����
	bool Init(HINSTANCE hInstance);
	void NewGame();
	void SetState(IState* pState);
	void Clear();
	bool Quit();
	void OnMouseClick();

	///////////////////
	virtual void Run()
	{
		MSG msg;
		for (;;)
		{
			while (!::PeekMessage(&msg, NULL, 0, 0, PM_NOREMOVE))
			{
				OnIdle();
			}
			BOOL bRet = ::GetMessage(&msg, NULL, 0, 0);
			if (bRet == -1) continue;
			if (!bRet) break;
			if (!PreTranslateMessage(msg))
			{
				::TranslateMessage(&msg);
				::DispatchMessage(&msg);
			}
		}
	}

	///////////////////
	virtual BOOL PreTranslateMessage(MSG& msg)
	{
		return FALSE;
	}

	virtual bool OnIdle();

	//����
public:
	CPlayers m_players;
	vector<Sprite*> m_arrSprites;	//���л�еľ���(��z���С��������)

protected:
	HINSTANCE m_hInstance;			//ʵ�����
	CMainWnd m_wnd;					//������
	bool m_bActive;					//��ǰ�����Ƿ񼤻�״̬
	IState* m_pCurrentState;		//��ǰ��Ϸ״̬
	Sprite* m_pCurrentSprite;		//��ǰ������ڵľ���
	bool m_bMouseDown;				//����Ƿ���
};

