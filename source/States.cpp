#include "GDefine.h"
#include "States.h"

#include "Application.h"
#include "RenderScene.h"

#define CLOSE_EVENT(e)   if (e) {CloseHandle(e); e = NULL;}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//״̬����
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

CStateInit			GStatus::s_stateInit;
CStateSendCards		GStatus::s_stateSendCards;
CStatePlayCards		GStatus::s_statePlayCards;
CStateDecideBanker	GStatus::s_stateDecideBanker;

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// CStateInit
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CStateInit::SetActive()
{
	theApp.GetSprite("button_play")->SetVisible(false);
	theApp.GetSprite("button_quit")->SetVisible(false);
	theApp.GetSprite("button_playcard")->SetVisible(false);
	theApp.GetSprite("button_skipcard")->SetVisible(false);

	//�����߳�
	m_hEventEndThread = CreateEvent(0, FALSE, FALSE, 0);
	_beginthread(ThreadFunc, 0, this);
}

void CStateInit::RunOneFrame()
{
	if (m_hEventEndThread)
	{
		DWORD state = WaitForSingleObject(m_hEventEndThread, 0);
		if (state == WAIT_OBJECT_0)
		{
			CLOSE_EVENT(m_hEventEndThread);
			//�߳��Ѿ�����
			pOnThreadEnd();
		}
	}

	GRenderScene::Render(false);
	Display::Obj()->Present();
}

void __cdecl CStateInit::ThreadFunc (void* pData)
{
	CStateInit* pThis = (CStateInit*) pData;
	//���ͬ������(�ȴ������˶�ͬ���¾�)
	SetEvent(pThis->m_hEventEndThread);
}

void CStateInit::pOnThreadEnd()
{
	m_hEventEndThread = NULL;
	theApp.SetState(GConfig::Obj()->GetGameType()->GetNextState(this));
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// CStateSendCards
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CStateSendCards::SetActive()
{
	m_nCardIndex = 0;
	m_nPlayerIndex = 0;
	m_hEventEndThread = NULL;

	theApp.GetSprite("button_quit")->SetVisible(true);

	//�����߳�: ϴ��
	m_hEventEndThread = CreateEvent(0, FALSE, FALSE, 0);
	_beginthread(ThreadFunc, 0, this);

}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CStateSendCards::RunOneFrame()
{
	if (m_hEventEndThread)
	{
		DWORD state = WaitForSingleObject(m_hEventEndThread, 0);
		if (state == WAIT_OBJECT_0)
		{
			CLOSE_EVENT(m_hEventEndThread);
			//�߳��Ѿ�����
			pOnThreadEnd();
		}
	}

	if (m_hEventEndThread)
	{
		//����ϴ��, ������
		GRenderScene::Render(false);
		Display::Obj()->Present();
	}
	else
	{
		//����
		pDrawMoveCard();
	}
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void __cdecl CStateSendCards::ThreadFunc (void* pData)
{
	CStateSendCards* pThis = (CStateSendCards*) pData;
	try
	{
		GConfig::Obj()->GetNetWork()->GetCards();
	}
	catch(...)
	{
		::MessageBox(NULL, "�����������, ������������������.\n��Ϸ����!", 0, 0);
		::ExitProcess(NULL);
	}
	SetEvent(pThis->m_hEventEndThread);
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CStateSendCards::pOnThreadEnd()
{
	//����: ��IDΪ0����
	m_nCardIndex = 0;
	m_nPlayerIndex = theApp.m_players.GetServerPlayerIndex();
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CStateSendCards::pDrawMoveCard()
{
	//׼��������Ϣ
	int nPlayerCardIndex = m_nCardIndex / GConfig::Obj()->GetPlayers();
	CPoint p1 = GConfig::Obj()->GetPosition(GConfig::SEND_CARD);
	CPoint p2 = GConfig::Obj()->GetPlayerPos(GConfig::HANDCARD, m_nPlayerIndex, nPlayerCardIndex);

	Card card = CPlayers::s_AllCards[m_nCardIndex];
	theApp.m_players[m_nPlayerIndex]->m_cardsInfo.arrHandCards[nPlayerCardIndex] = card;

	//��λ�û���
	int nFrames = GConfig::Obj()->GetMoveCardFrames();
	GRenderScene::RenderMovingCard(false, false, true, &card, p1, p2, nFrames, true);
	m_ptCardPos = p2;

	//׼����һ����
	theApp.m_players[m_nPlayerIndex]->m_cardsInfo.nHandCards = nPlayerCardIndex + 1;
	m_nCardIndex++;
	if (m_nCardIndex >= GConfig::Obj()->GetAllCards() - GConfig::Obj()->GetBottomCards())
	{
		//��������
		//������������
		for(int i=0; i<GConfig::Obj()->GetPlayers(); i++)
		{
			theApp.m_players[i]->SortHandCards();
		}

		//������һ��״̬
		theApp.SetState(GConfig::Obj()->GetGameType()->GetNextState(this));
		return;
	}

	//��һ�����
	m_nPlayerIndex++;
	if (m_nPlayerIndex >= GConfig::Obj()->GetPlayers())
		m_nPlayerIndex = 0;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// CStateDecideBanker
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CStateDecideBanker::SetActive()
{
	m_nTempCount = 0;

	//�����߳�: ȷ��ׯ��
	m_hEventEndThread = CreateEvent(0, FALSE, FALSE, 0);
	_beginthread(ThreadFunc, 0, this);

}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CStateDecideBanker::RunOneFrame()
{
	//�ж��߳��Ƿ����.��������� pOnThreadEnd
	if (m_hEventEndThread)
	{
		DWORD state = WaitForSingleObject(m_hEventEndThread, 0);
		if (state == WAIT_OBJECT_0)
		{
			CLOSE_EVENT(m_hEventEndThread);
		}
	}

	//���ݵ�ǰ״̬����
	GRenderScene::Render(true, true, false);
	HDC hdc = Display::Obj()->GetBackSurface()->GetHDC();
	CDCHandle cdc(hdc);
	
	CFont font; font.CreatePointFont(GConfig::Obj()->GetInfoFontSize(), _T("����"));
	HFONT hf = cdc.SelectFont(font);
	cdc.SetTextColor(GConfig::Obj()->GetInfoColor());
	cdc.SetBkMode(TRANSPARENT);

	tstring sName;
	if (m_hEventEndThread)
	{
		if (m_nTempCount == 0)
		{
			//���ڲ������ׯ��
			GConfig::Obj()->GetGameType()->GetText_DecideBanker(IGameType::Decide_Banker_Random, sName);
		}
		else
		{
			//���ׯ���Ѳ���, ��ʽׯ��δ����
			sName = theApp.m_players.GetPlayerByID(CPlayers::s_nBankerID)->m_baseInfo.sName;
			GConfig::Obj()->GetGameType()->GetText_DecideBanker(IGameType::Decide_Banker_PreConfirm, sName);
		}
	}
	else
	{
		//��ʽׯ���Ѿ�����
		sName = theApp.m_players.GetPlayerByID(CPlayers::s_nBankerID)->m_baseInfo.sName;
		GConfig::Obj()->GetGameType()->GetText_DecideBanker(IGameType::Decide_Banker_Confirm, sName);
	}

	CPoint pos = GConfig::Obj()->GetInfoPos();
	cdc.TextOut(pos.x, pos.y, sName.c_str());
	cdc.SelectFont(hf);
	Display::Obj()->GetBackSurface()->ReleaseHDC(hdc);	
	Display::Obj()->Present();

	if (m_hEventEndThread == NULL)
	{
		//��ʽׯ���Ѿ�����
		Sleep(300);

		//�ѵ��Ƽ���ׯ������
		CPlayer* pBanker = theApp.m_players.GetPlayerByID(CPlayers::s_nBankerID);
		pBanker->AddBottomCards();

		//������һ��״̬:����
		theApp.SetState(GConfig::Obj()->GetGameType()->GetNextState(this));
	}
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void __cdecl CStateDecideBanker::ThreadFunc (void* pData)
{
	CStateDecideBanker* pThis = (CStateDecideBanker*) pData;
	try
	{
		//ȡ�����ׯ��
		GConfig::Obj()->GetNetWork()->GetRandomBanker();
		CPlayers::s_nBankerID = CPlayers::s_nRandomBankerID;
		pThis->m_nTempCount = 1;

		while(1)
		{
			if (theApp.m_players[0]->m_baseInfo.nID == CPlayers::s_nBankerID)
			{
				//�����Ƿ�ͬ�⵱ׯ��
				Sleep(100);
				tstring str;
				GConfig::Obj()->GetGameType()->GetText_DecideBanker(IGameType::Decide_Banker_Msgbox,str);

				bool bAgree = AtlMessageBox(NULL, str.c_str(),
					theApp.m_players[0]->m_baseInfo.sName, MB_OKCANCEL) == IDOK;
				
				GConfig::Obj()->GetNetWork()->AgreeBanker(bAgree);
				
				//ͬ�������ѭ�������߳�
				if (bAgree) break;
			}
			else
			{
				//�������Ƿ�ͬ�⵱ׯ��
				bool bAgree = GConfig::Obj()->GetNetWork()->GetAgreeBanker(CPlayers::s_nBankerID);

				//ͬ�������ѭ�������߳�
				if (bAgree) break;

				//��Ҫ����ͬ���.
				if (pThis->m_nTempCount >= GConfig::Obj()->GetPlayers()-1) break;
			}

			//��һ��
			CPlayers::s_nBankerID ++;
			if (CPlayers::s_nBankerID >= GConfig::Obj()->GetPlayers())
			{
				CPlayers::s_nBankerID = 0;
			}
			pThis->m_nTempCount ++;
		}
	}
	catch(...)
	{
		::MessageBox(NULL, _T("�����������, ������������������.\n��Ϸ����!"), 0, 0);
		::ExitProcess(NULL);
	}
	SetEvent(pThis->m_hEventEndThread);
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// CStatePlayCards
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CStatePlayCards::SetActive()
{
	m_bPlaying = false;
	CPlayers::s_nTurnCounts = -1;
	CPlayers::s_nNewTurnID = -1;

	theApp.GetSprite("button_play")->SetVisible(false);
	theApp.GetSprite("button_quit")->SetVisible(true);
	theApp.GetSprite("button_playcard")->SetVisible(false);
	theApp.GetSprite("button_skipcard")->SetVisible(true);

	//�����߳�
	m_hEventEndThread = CreateEvent(0, FALSE, FALSE, 0);
	m_hEventPlayingStart = CreateEvent(0, FALSE, FALSE, 0);
	m_hEventPlayingEnd = CreateEvent(0, FALSE, FALSE, 0);
	m_hEventPrepareStart = CreateEvent(0, FALSE, FALSE, 0);
	m_hEventPrepareEnd = CreateEvent(0, FALSE, FALSE, 0);

	_beginthread(ThreadFunc, 0, this);
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CStatePlayCards::RunOneFrame()
{
	if (!m_bPlaying)
	{
		theApp.GetSprite("button_playcard")->SetVisible(false);
		theApp.GetSprite("button_skipcard")->SetVisible(false);
	}
	else if (!theApp.GetSprite("button_playcard")->IsVisible() && !theApp.GetSprite("button_skipcard")->IsVisible())
	{
		theApp.GetSprite("button_skipcard")->SetVisible(true);
	}

	//�ж��߳��Ƿ����
	if (m_hEventEndThread)
	{
		if (WaitForSingleObject(m_hEventEndThread, 0) == WAIT_OBJECT_0)
			CLOSE_EVENT(m_hEventEndThread);
	}

	if (m_hEventPlayingStart)
	{
		if (WaitForSingleObject(m_hEventPlayingStart, 0) == WAIT_OBJECT_0)
		{
			//����������ҳ��ƹ��̶���
			theApp.m_players.GetPlayerByID(CPlayers::s_nPlayingID)->PlayCard();
			SetEvent(m_hEventPlayingEnd);
		}
	}

	//�ж��Ƿ�ʼ����
	if (!m_bPlaying)
	{
		DWORD state = WaitForSingleObject(m_hEventPrepareStart, 0);
		{
			if (state == WAIT_OBJECT_0)
			{
				m_bPlaying = true;
			}
		}
	}
	
	//���ݵ�ǰ״̬����
	GRenderScene::Render(true, true, true);
	Display::Obj()->Present();

	if (m_hEventEndThread == NULL)
	{
		CLOSE_EVENT(m_hEventPrepareStart);
		CLOSE_EVENT(m_hEventPrepareEnd);
		CLOSE_EVENT(m_hEventPlayingStart);
		CLOSE_EVENT(m_hEventPlayingEnd);

		//�߳̽���.������һ��״̬
		theApp.NewGame();
	}
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CStatePlayCards::OnMouseClick()
{
	if (!m_bPlaying)
	{
		return;
	}

	CPoint ptMouse = theApp.GetMousePos();
	CPoint ptStart = GConfig::Obj()->GetPlayerPos(GConfig::HANDCARD, 0, 0);
	CRect rcCard = GSprites::Obj()->GetSprite(_T("card_back"))->GetPos();
	int nHeight = rcCard.Height();
	int nHSpace = GConfig::Obj()->GetPlayerOffset(GConfig::HANDCARD, 0).x;

	CRect rcStart(ptStart, CSize(nHSpace, nHeight));

	//1 �ж��Ƿ���� prepare ������
	int nPrepare = theApp.m_players[0]->m_cardsInfo.nPrepareCards;
	int* pPrepare = theApp.m_players[0]->m_cardsInfo.arrPrepareCards;

	//2 �ж��Ƿ����������������
	int nHitIndex = -1;
	bool bPrepare = false;
	int nHandCards = theApp.m_players[0]->m_cardsInfo.nHandCards;

	for (int i=0; i<nHandCards; ++i)
	{
		//�Ƿ���prepare��
		bool bPrepareTemp = false;
		for(int j=0; j<nPrepare; ++j)
		{
			if (i == pPrepare[j])
			{
				bPrepareTemp = true;
				break;
			}
		}

		//����λ��
		CRect rcTemp = rcStart + CPoint(nHSpace * i, 0);
		if ((i == nHandCards-1) || bPrepareTemp)
		{
			rcTemp.right = rcTemp.left + rcCard.Width();
			rcTemp.bottom = rcTemp.top + rcCard.Height();
		}
		if (bPrepareTemp) rcTemp -= CPoint(0, PREPARE_OFFSET_V);

		//���Ի���
		if (rcTemp.PtInRect(ptMouse))
		{
			bPrepare = bPrepareTemp;
			nHitIndex = i;
		}
	}

	if (nHitIndex >= 0)
	{
		if (bPrepare)
		{
			//��prepare��ɾ��������
			std::remove(pPrepare, pPrepare + nPrepare, nHitIndex);
			theApp.m_players[0]->m_cardsInfo.nPrepareCards --;
		}
		else
		{
			//����������ӵ� prepare ��
			theApp.m_players[0]->m_cardsInfo.arrPrepareCards[nPrepare] = nHitIndex;
			theApp.m_players[0]->m_cardsInfo.nPrepareCards ++;
		}
	}

	bool bHasPrepare = (theApp.m_players[0]->m_cardsInfo.nPrepareCards > 0);
	theApp.GetSprite("button_playcard")->SetVisible(bHasPrepare);
	theApp.GetSprite("button_skipcard")->SetVisible(!bHasPrepare);

	//3 �ж��Ƿ���� playcard �� skipcard ��ť��
	if (theApp.GetSprite("button_playcard")->HitTest(ptMouse) ||
		theApp.GetSprite("button_skipcard")->HitTest(ptMouse))
	{
		if (bHasPrepare)
		{
			//�ȼ��
			vector<Card> arrCards, arrPrevCards;
			theApp.m_players[0]->GetPrepareCards(arrCards);
	
			//������Ǳ����ȳ���
			if (CPlayers::s_nNewTurnID != theApp.m_players[0]->m_baseInfo.nID)
			{
				int i = 0;
				while (arrPrevCards.size() == 0)
				{
					assert(i<GConfig::Obj()->GetPlayers());
                    Card* pBegin = theApp.m_players[GConfig::Obj()->GetPlayers()-1 - i]->m_cardsInfo.arrPlayingCards;
					int nCount = theApp.m_players[GConfig::Obj()->GetPlayers()-1 - i]->m_cardsInfo.nPlayingCards;
					arrPrevCards.resize(nCount);
					std::copy(pBegin, pBegin+nCount, arrPrevCards.begin());
					i++;
				}
			}
			
			std::sort(arrPrevCards.begin(), arrPrevCards.end());
			std::sort(arrCards.begin(), arrCards.end());

			if (! GConfig::Obj()->GetGameType()->CheckPrepareCards(arrPrevCards, arrCards))
			{
				//��鲻ͨ��.���������
				theApp.m_players[0]->m_cardsInfo.nPrepareCards = 0;
				theApp.GetSprite("button_playcard")->SetVisible(false);
				theApp.GetSprite("button_skipcard")->SetVisible(true);
				return;
			}
		}
		else
		{
			if (CPlayers::s_nNewTurnID == theApp.m_players[0]->m_baseInfo.nID)
			{
				//�������
				return;
			}
		}

		//���� bPlaying Ϊfalse
		m_bPlaying = false;
		//֪ͨ�߳�
		SetEvent(m_hEventPrepareEnd);
	}
}


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void __cdecl CStatePlayCards::ThreadFunc (void* pData)
{
	CStatePlayCards* pThis = (CStatePlayCards*) pData;
	try
	{
		CPlayers::s_nPlayingID = CPlayers::s_nBankerID;	//ׯ���ȳ���
		CPlayers::s_nPlayingID_MaxCard = CPlayers::s_nPlayingID;
		bool bMyself = theApp.m_players[0]->m_baseInfo.nID == CPlayers::s_nPlayingID;

		CPlayers::s_nTurnCounts = 0;
		CPlayers::s_nNewTurnID =  CPlayers::s_nPlayingID;

		while(1)
		{
			bMyself = theApp.m_players[0]->m_baseInfo.nID == CPlayers::s_nPlayingID;
			if (bMyself)
			{
				//���˳���
				SetEvent(pThis->m_hEventPrepareStart);
				//�ȴ����ƽ���
				DWORD state = WaitForSingleObject(pThis->m_hEventPrepareEnd, INFINITE);
				if (state == WAIT_OBJECT_0)
				{
					//���ƽ���
					//���͵�ǰ�ִ������(������ֲ�Ҫ.�� arrPlayingCards ����Ϊ��
					GConfig::Obj()->GetNetWork()->SendPrepareCards();
				}
			}

			//ȡ�ô�����
			GConfig::Obj()->GetNetWork()->GetPrepareCards();

			//
			CPlayers::s_nNewTurnID = -1;

			//����.���Ƴ��ƹ���(ˢ��arrPlayingCards)
			SetEvent(pThis->m_hEventPlayingStart);
			DWORD state = WaitForSingleObject(pThis->m_hEventPlayingEnd, INFINITE);
			assert(state == WAIT_OBJECT_0);

			//�ж��Ƿ��µ�һ��
			int nNextPlayerID = theApp.m_players.GetNextPlayerID(CPlayers::s_nPlayingID);
			if (theApp.m_players.GetPlayerByID(CPlayers::s_nPlayingID)->m_cardsInfo.nPlayingCards == 0)
			{
				//���˭����, ����һ����Ҿ��Ǳ������������
				if (nNextPlayerID == CPlayers::s_nPlayingID_MaxCard)
				{
					//��һ�ֿ�ʼ
					CPlayers::s_nTurnCounts ++;
					CPlayers::s_nNewTurnID = nNextPlayerID;
				}
			}
			else
			{
				//������������
				CPlayers::s_nPlayingID_MaxCard = CPlayers::s_nPlayingID;
			}

			//�ж��Ƿ����ֽ���
			if (theApp.m_players.GetPlayerByID(CPlayers::s_nPlayingID)->m_cardsInfo.nHandCards == 0)
			{
				//���ֽ���
				CPlayer* pBanker = theApp.m_players.GetPlayerByID(CPlayers::s_nBankerID);
				int nFlag = 1;
				if (CPlayers::s_nPlayingID != CPlayers::s_nBankerID)
				{
					nFlag = -1;
				}

				pBanker->m_baseInfo.nScore += (2*nFlag);
				for (int i=0; i<GConfig::Obj()->GetPlayers(); ++i)
				{
					if (pBanker == theApp.m_players[i]) continue;
					theApp.m_players[i]->m_baseInfo.nScore -= (nFlag);
				}
				
				break;
			}
			else
			{
				//��һ����ҳ���
				CPlayers::s_nPlayingID = nNextPlayerID;
			}
		}
	}
	catch(...)
	{
		::MessageBox(NULL, _T("�����������, ������������������.\n��Ϸ����!"), 0, 0);
		::ExitProcess(NULL);
	}
	Sleep(300);
	SetEvent(pThis->m_hEventEndThread);
}

