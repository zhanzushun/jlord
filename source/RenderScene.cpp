#include "gdefine.h"
#include "RenderScene.h"
#include "Application.h"

void GRenderScene::Render(bool bDrawCard, bool bDrawBottomCards, bool bDisplayBottomCards, bool bDrawCenterCard)
{
	//1. ������
	Display::Obj()->GetBackSurface()->Blt(0,0, GSurfaces::Obj()->GetSurface(_T("background"))->m_pSurface);

	//2. ��ͷ��
	Sprite* pSprite = NULL;
	for(unsigned int i=0; i<theApp.m_players.size(); ++i)
	{
		pSprite = GSprites::Obj()->GetSprite(theApp.m_players[i]->m_baseInfo.sImage);
		assert(pSprite);
		CPoint pos = GConfig::Obj()->GetPlayerPos(GConfig::HEAD,i,0);
		pSprite->SetPos(pos);
		pSprite->Draw();

		//������
		HDC hdc = Display::Obj()->GetBackSurface()->GetHDC();
		CDCHandle cdc(hdc);
		CFont font; font.CreatePointFont(GConfig::Obj()->GetNameFontSize(), _T("����"));
		HFONT hf = cdc.SelectFont(font);
		cdc.SetBkMode(TRANSPARENT);
		cdc.SetTextColor(GConfig::Obj()->GetNameColor());
		pos.y += pSprite->GetPos().Height() + 2;
		tstring sName = GConfig::Obj()->GetGameType()->GetNameText ( 
			theApp.m_players[i]->m_baseInfo.nID,
			theApp.m_players[i]->m_baseInfo.sName );

		TCHAR buf[50]; 
		_stprintf(buf, ",�÷�(%d)", theApp.m_players[i]->m_baseInfo.nScore); 

		sName += buf;
		cdc.TextOut(pos.x, pos.y, sName.c_str());
		cdc.SelectFont(hf);
		Display::Obj()->GetBackSurface()->ReleaseHDC(hdc);
	}

	//3 ����ť
	for(unsigned int i=0; i<theApp.m_arrSprites.size(); ++i)
	{
		Sprite* pTemp = theApp.m_arrSprites[i];
		pTemp->SetAction(pTemp == theApp.GetCurrentSprite() ? (theApp.IsMouseDown() ? "down" : "over") : "out");
		pTemp->Draw();
	}

	//4 ��������ֱ�ӷ���
	if (!bDrawCard)
	{
		Display::Obj()->Present();
		return;
	}

	//5 ���Ƹ���������ϵ���, �������, ���ڴ����, ׼���������
	Sprite* pCard = NULL;
	for(int i=0; i<GConfig::Obj()->GetPlayers(); ++i)
	{
		int nHandCards = theApp.m_players[i]->m_cardsInfo.nHandCards;
		for(int j=0; j<nHandCards; j++)
		{
			Card card = theApp.m_players[i]->m_cardsInfo.arrHandCards[j];
			pCard = GSprites::Obj()->GetSprite(card.GetSpriteName());
			assert(pCard);
			if (!pCard) throw BKException(true, _T("û�ҵ�������:%s\n"), card.GetSpriteName().c_str());

			//����λ��/����
			CPoint pos = GConfig::Obj()->GetPlayerPos(GConfig::HANDCARD, i, j);
			if (i==0)
			{
				for(int k=0; k<theApp.m_players[i]->m_cardsInfo.nPrepareCards; ++k)
				{
					if (theApp.m_players[i]->m_cardsInfo.arrPrepareCards[k] == j)
					{
						pos.y -= PREPARE_OFFSET_V;	//׼���������
						break;
					}
				}
			}
			pCard->SetAction ( (i==0) ? _T("defaction") : _T("back") );
			pCard->SetPos(pos);
			pCard->Draw();
		}

		int nOutCards = theApp.m_players[i]->m_cardsInfo.nOutCards;
		for(int j=0; j<nOutCards; j++)
		{
			Card card = theApp.m_players[i]->m_cardsInfo.arrOutCards[j];
			pCard = GSprites::Obj()->GetSprite(card.GetSpriteName());
			assert(pCard);
			CPoint pos = GConfig::Obj()->GetPlayerPos(GConfig::OUTCARD, i, 0);
			pCard->SetAction (_T("back"));
			pCard->SetPos(pos);
			pCard->Draw();
		}

		int nPlayingCards = theApp.m_players[i]->m_cardsInfo.nPlayingCards;
		for(int j=0; j<nPlayingCards; j++)
		{
			Card card = theApp.m_players[i]->m_cardsInfo.arrPlayingCards[j];
			pCard = GSprites::Obj()->GetSprite(card.GetSpriteName());
			assert(pCard);
			CPoint pos = GConfig::Obj()->GetPlayerPos(GConfig::PLAYINGCARD, i, j);
			pCard->SetAction (_T("defaction"));
			pCard->SetPos(pos);
			pCard->Draw();
		}
	}

	//6 ���Ƶ���
	if (bDrawBottomCards)
	{
		int nBottom = GConfig::Obj()->GetBottomCards();
		for(int i=0; i<nBottom; i++)
		{
			Card card = CPlayers::s_AllCards[GConfig::Obj()->GetAllCards()-1 - i];
			pCard = GSprites::Obj()->GetSprite(card.GetSpriteName());
			assert(pCard);
			if (!pCard) throw BKException(true, _T("û�ҵ�������:%s\n"), card.GetSpriteName().c_str());

			CPoint pos = GConfig::Obj()->GetPosition(GConfig::BOTTOM_CARD);
			pos.x += i * GConfig::Obj()->GetPlayerOffset(GConfig::HANDCARD, 0).x;

			pCard->SetAction (bDisplayBottomCards ? _T("defaction") : _T("back"));
			pCard->SetPos(pos);
			pCard->Draw();
		}
	}

	//7 ���ƴ��������ı�����
	if (bDrawCenterCard)
	{
		//�����ı�����
		Sprite* pCardBack = GSprites::Obj()->GetSprite(_T("card_back"));
		assert(pCardBack);
		pCardBack->SetPos(GConfig::Obj()->GetPosition(GConfig::SEND_CARD));
		pCardBack->Draw();
	}
}


void GRenderScene::RenderMovingCard(bool bDrawBottomCards, bool bDisplayBottomCards, bool bDrawCenterCard, 
							   Card* pMovingCard, CPoint p1, CPoint p2, int nFrames, bool bMovingBack)
{
	Sprite* pCard = NULL;
	CPoint pos = p1;
	for(int i=0; i<nFrames-1; i++)
	{
		pos.x += (p2.x-p1.x) / nFrames;
		pos.y += (p2.y-p1.y) / nFrames;
		GRenderScene::Render(true, bDrawBottomCards, bDisplayBottomCards, bDrawCenterCard);

		tstring sCard = pMovingCard->GetSpriteName();
		pCard = GSprites::Obj()->GetSprite(sCard);
		assert(pCard);
		if (!pCard) throw BKException(true, _T("û�ҵ�������:%s\n"), sCard.c_str());
		pCard->SetAction ( (bMovingBack) ? _T("back") : _T("defaction") );
		pCard->SetPos(pos);
		pCard->Draw();

		Display::Obj()->Present();
	}

	pos = p2;
	GRenderScene::Render(true, bDrawBottomCards, bDisplayBottomCards, bDrawCenterCard);

	tstring sCard = pMovingCard->GetSpriteName();
	pCard = GSprites::Obj()->GetSprite(sCard);
	assert(pCard);
	if (!pCard) throw BKException(true, _T("û�ҵ�������:%s\n"), sCard.c_str());
	pCard->SetAction ( (bMovingBack) ? _T("back") : _T("defaction") );
	pCard->SetPos(pos);
	pCard->Draw();

	Display::Obj()->Present();
}
