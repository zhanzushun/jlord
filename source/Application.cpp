#include "GDefine.h"
#include "gdefine.h"
#include "Application.h"

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// WinMain ����
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

CApp theApp;


int WINAPI _tWinMain(HINSTANCE hInstance, HINSTANCE /*hPrevInstance*/, LPTSTR lpstrCmdLine, int nCmdShow)
{
	try
	{
		//�漴����
		srand(GetTickCount());

		//��ʼ�� WSA
		WSADATA wsaData;
		WORD wVersionRequested = MAKEWORD(1, 1);
		int nResult = WSAStartup(wVersionRequested, &wsaData);
		if (nResult != 0)return 0;

		//��ʼ��Ӧ�ó���
		if (!theApp.Init(hInstance))
		{
			return 1;
		}

		//������Ϣѭ��
		theApp.Run();

		//�˳�
		theApp.Clear();

		//�˳�wsa
		WSACleanup();
	}

	catch(BKException e)
	{
		AtlMessageBox(NULL, e.message.c_str());
		AtlMessageBox(NULL, _T("Ӧ�ó����˳�"));
	}

	catch(...)
	{
		AtlMessageBox(NULL, _T("δ֪�Ĵ���.Ӧ�ó����˳�"));
	}

	return 0;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// APP ��
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

CApp::CApp() 
{
	m_hInstance = NULL;
	m_pCurrentState = NULL;
	m_pCurrentSprite = NULL;
	m_bActive = true;
	m_bMouseDown = false;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//��ʼ��

bool CApp::Init(HINSTANCE hInstance)
{
	m_hInstance = hInstance;

	//Load��Ϸ����
	GConfig::Obj()->Load("game_config.xml");

	//��ʼ�����
	m_players.Init();

	//ȡ�ò�ͬ�������������
	CBeginDialog dlg;
	if (dlg.DoModal() == IDCANCEL)
		return false;

	//����������
	m_wnd.Create(0);
	m_wnd.ShowWindow(SW_SHOW);

	//��ʼ�� Display
	Display::Obj()->CreateWindowedDisplay(GConfig::Obj()->GetWidth()-1, GConfig::Obj()->GetHeight()-1, m_wnd);

	//��ʼ����Դ
	DWORD t = timeGetTime();
	GSurfaces::Obj()->Load("surfaces.xml");
	GSprites::Obj()->Load("sprite_cards.xml");
	GSprites::Obj()->Load("sprite_others.xml");

	m_arrSprites.push_back(GetSprite("button_play"));
	m_arrSprites.push_back(GetSprite("button_quit"));
	m_arrSprites.push_back(GetSprite("button_playcard"));
	m_arrSprites.push_back(GetSprite("button_skipcard"));
	GetSprite("button_play")->SetPos (GConfig::Obj()->GetPosition(GConfig::PLAY_BTN));
	GetSprite("button_quit")->SetPos (GConfig::Obj()->GetPosition(GConfig::QUIT_BTN));
	GetSprite("button_playcard")->SetPos (GConfig::Obj()->GetPosition(GConfig::PLAYCARD_BTN));
	GetSprite("button_skipcard")->SetPos (GConfig::Obj()->GetPosition(GConfig::SKIPCARD_BTN));


	//����Ϸ
	NewGame();

	return true;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//���д���: ֱ������֡ѭ��

bool CApp::OnIdle()
{
	DWORD t = timeGetTime();
	if (m_pCurrentState)
	{
		//���µ�ǰ����
		CPoint pt = GetMousePos();
		m_pCurrentSprite = NULL;
		int n = (int)m_arrSprites.size();
		for(int i=n-1; i>=0; i--)
		{
			if (m_arrSprites[i]->HitTest(pt) && m_arrSprites[i]->IsEnabled())
			{
				m_pCurrentSprite = m_arrSprites[i];
			}
		}
		//���õ�ǰ״̬�Ĵ���
		m_pCurrentState->RunOneFrame();	
	}
	//TCHAR buf[50]; _stprintf(buf,"%d\n", timeGetTime()-t); OutputDebugString(buf);
	DWORD t2 = timeGetTime();

	//����֡����
	if (t2 - t < FRAME_TIME)
	{
		Sleep(FRAME_TIME - (t2 - t));
	}
	return true;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//�˳�

bool CApp::Quit()
{
	if (AtlMessageBox(m_wnd, _T("����˳���?"), _T(""), MB_OKCANCEL) == IDOK)
	{
		PostQuitMessage(0);
		return true;
	}
	return false;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//���/�ͷ���Դ

void CApp::Clear()
{
	GSprites::Obj()->Clear();
	GSurfaces::Obj()->Clear();
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//���λ��

CPoint CApp::GetMousePos()
{
	CPoint pt;
	GetCursorPos(&pt);
	m_wnd.ScreenToClient(&pt);
	return pt;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//����״̬

void CApp::SetState(IState* pState)
{
	assert(pState);
	pState->SetActive();
	m_pCurrentState = pState;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CApp::OnMouseClick()
{
	if (m_pCurrentState)
	{
		m_pCurrentState->OnMouseClick();
	}
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CApp::NewGame()
{
	//��Ϸ����
	m_players.ResetData();

	//��Ϸ״̬
	theApp.SetState(GConfig::Obj()->GetGameType()->GetNextState(m_pCurrentState));

}

