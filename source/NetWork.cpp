#include "gdefine.h"
#include "NetWork.h"

#include "Application.h"

void ShuffleCards()
{
	//�漴����
	srand(GetTickCount());

	//���ư�˳��ź�
	for(int iSet=0; iSet < GConfig::Obj()->GetCardsSet(); iSet++)
	{
		int nBase = iSet * SET_CARDS;
		for(int i=3,j=0;j<=12;i++,j++)
		{
			//����
			CPlayers::s_AllCards[ (j*4+0) + nBase].nColor = Card::HEITAO;
			CPlayers::s_AllCards[ (j*4+0) + nBase].nValue = i;
			//����
			CPlayers::s_AllCards[ (j*4+1) + nBase].nColor = Card::HONGTAO;
			CPlayers::s_AllCards[ (j*4+1) + nBase].nValue = i;
			//÷��
			CPlayers::s_AllCards[ (j*4+2) + nBase].nColor = Card::CAOHUA;
			CPlayers::s_AllCards[ (j*4+2) + nBase].nValue = i;
			//����
			CPlayers::s_AllCards[ (j*4+3) + nBase].nColor = Card::FANGKUAI;
			CPlayers::s_AllCards[ (j*4+3) + nBase].nValue = i;
		}
		CPlayers::s_AllCards[53 + nBase].nColor = Card::JOKER_S;	//С��
		CPlayers::s_AllCards[53 + nBase].nValue = Card::VALUE_JOKER_S;
		CPlayers::s_AllCards[52 + nBase].nColor = Card::JOKER_B;   //���
		CPlayers::s_AllCards[52 + nBase].nValue = Card::VALUE_JOKER_B;
	}

	//��Чϴ��
	int nRandPos;   
	Card cTempCard;

	int nAll = GConfig::Obj()->GetAllCards();
	for(int k=0; k<nAll; k++)
	{
		//����һ�����λ��;
		nRandPos = rand() % nAll;

		//�ѵ�ǰ����������λ�õ��ƽ���;
		cTempCard = CPlayers::s_AllCards[k];
		CPlayers::s_AllCards[k] = CPlayers::s_AllCards[nRandPos];
		CPlayers::s_AllCards[nRandPos] = cTempCard;	
	}
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//Single
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CNetWork_Single::GetCards()
{
	//ϴ��
	ShuffleCards();
}

void CNetWork_Single::GetRandomBanker()
{
	Sleep(2000);//�ô����һ����ʱ�俴�����ȡ����
	//�漴����
	srand(GetTickCount());
	CPlayers::s_nRandomBankerID = rand() % GConfig::Obj()->GetPlayers();
}

void CNetWork_Single::AgreeBanker(bool bAgree)
{
	//nothing
}

bool CNetWork_Single::GetAgreeBanker(int nTempBankerID)
{

	Card* pHandBegin = theApp.m_players.GetPlayerByID(nTempBankerID)->m_cardsInfo.arrHandCards;
	int nHandCount = theApp.m_players.GetPlayerByID(nTempBankerID)->m_cardsInfo.nHandCards;

	Cards arrHandCards;
	arrHandCards.resize(nHandCount);
	std::copy(pHandBegin, pHandBegin+nHandCount, arrHandCards.begin());

	int nValue = GConfig::Obj()->GetGameType()->AccountCardsValue(arrHandCards);

	for (int i=0; i<GConfig::Obj()->GetPlayers(); i++)
	{
		if (theApp.m_players[i]->m_baseInfo.nID == nTempBankerID) continue;

		pHandBegin = theApp.m_players[i]->m_cardsInfo.arrHandCards;
		nHandCount = theApp.m_players[i]->m_cardsInfo.nHandCards;
		arrHandCards.resize(nHandCount);
		std::copy(pHandBegin, pHandBegin+nHandCount, arrHandCards.begin());

		int nOtherValue = GConfig::Obj()->GetGameType()->AccountCardsValue(arrHandCards);

		if (nValue < nOtherValue) return false;
	}

	return true;
}

void CNetWork_Single::SendPrepareCards()
{
	//nothing
}

void CNetWork_Single::GetPrepareCards()
{
	if (CPlayers::s_nPlayingID == theApp.m_players[0]->m_baseInfo.nID)
	{
		return;
	}

	//ȡ��������ҳ�����Ŀǰ������鼸����
	//�ȼ��
	Cards arrPrevCards;

	if (CPlayers::s_nNewTurnID != CPlayers::s_nPlayingID)
	{
		int nID = CPlayers::s_nPlayingID;
		while (arrPrevCards.size() == 0)
		{
			nID = theApp.m_players.GetPrevPlayerID(nID);
			Card* pBegin = theApp.m_players.GetPlayerByID(nID)->m_cardsInfo.arrPlayingCards;
			int nCount = theApp.m_players.GetPlayerByID(nID)->m_cardsInfo.nPlayingCards;
			arrPrevCards.resize(nCount);
			std::copy(pBegin, pBegin+nCount, arrPrevCards.begin());
		}
		std::sort(arrPrevCards.begin(), arrPrevCards.end());
	}

	//����ѡ�����
	Card* pHandBegin = theApp.m_players.GetPlayerByID(CPlayers::s_nPlayingID)->m_cardsInfo.arrHandCards;
	int nHandCount = theApp.m_players.GetPlayerByID(CPlayers::s_nPlayingID)->m_cardsInfo.nHandCards;

	Cards arrHandCards;
	arrHandCards.resize(nHandCount);
	std::copy(pHandBegin, pHandBegin+nHandCount, arrHandCards.begin());

	//���
	Cards arrPrepareCards;
	GConfig::Obj()->GetGameType()->ComputerPrepareCards(arrPrepareCards, arrHandCards, arrPrevCards);

	if (arrPrevCards.size() == 0)//���Ա����ȳ�
	{
		assert(arrPrepareCards.size() > 0);//��֤�������
	}

	//����arrPrepareCards
	int* pPrepareBegin = theApp.m_players.GetPlayerByID(CPlayers::s_nPlayingID)->m_cardsInfo.arrPrepareCards;
	int& nPrepareCount = theApp.m_players.GetPlayerByID(CPlayers::s_nPlayingID)->m_cardsInfo.nPrepareCards;
	nPrepareCount = (int)arrPrepareCards.size();

	for (int i=0; i<nPrepareCount; i++)
	{
		Card* pCard = std::find(pHandBegin, pHandBegin+nHandCount, arrPrepareCards[i]);
		pPrepareBegin[i] = (int)(pCard - pHandBegin);
	}
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//Client
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

bool CNetWork_Client::GetPlayers(tstring& strDlgNetInfo, const tstring& strIP)
{
	//������������
	m_linkToServer.SocketInit();
	m_linkToServer.Create();
	if (!m_linkToServer.Connect(strIP))
	{
		m_linkToServer.Close();
		return false;
	}

	//���� baseinfo
	TCHAR sNetInfo[100];
	m_linkToServer.Send(&theApp.m_players[0]->m_baseInfo, sizeof(CPlayer::BaseInfo));

	//���� int
	m_linkToServer.Recv(&theApp.m_players[0]->m_baseInfo.nID, sizeof(int));

	//���� netinfo
	//4����.���idΪ1,��ʾ��3��netinfo��Ҫ����, ���idΪ3.��ʾֻ��һ��netinfo��Ҫ����
	for (int i=0; i < (GConfig::Obj()->GetPlayers() - theApp.m_players[0]->m_baseInfo.nID); ++i)
	{
		m_linkToServer.Recv(sNetInfo, sizeof(TCHAR)*100);
		strDlgNetInfo = sNetInfo;
	}

	//����������ҵ�baseinfo
	for (int i=0; i< GConfig::Obj()->GetPlayers(); ++i)
	{
		if (i == 0)
		{
			continue;
		}
		CPlayer::BaseInfo& info = theApp.m_players[i]->m_baseInfo;
		m_linkToServer.Recv(&info, sizeof(CPlayer::BaseInfo));
	}

	//�� ID �����������(������������CPlayer���������Լ�Ϊ0,������˳��)
	theApp.m_players.Sort();

	return true;
}


void CNetWork_Client::GetCards()
{
	//�ӷ���������
	m_linkToServer.Recv(CPlayers::s_AllCards, sizeof(Card) * GConfig::Obj()->GetAllCards());
}

void CNetWork_Client::GetRandomBanker()
{
	//�ӷ���������
	m_linkToServer.Recv(&CPlayers::s_nRandomBankerID, sizeof(int));
}

void CNetWork_Client::AgreeBanker(bool bAgree)
{
	//������������
	m_linkToServer.Send(&bAgree, sizeof(bool));
}

bool CNetWork_Client::GetAgreeBanker(int nTempBanker)
{
	//�ӷ���������
	bool bAgree = false;
	m_linkToServer.Recv(&bAgree, sizeof(bool));
	return bAgree;
}

void CNetWork_Client::SendPrepareCards()
{
	//����������ʹ�����
	int* pPrepare = theApp.m_players[0]->m_cardsInfo.arrPrepareCards;
	int nPrepare = theApp.m_players[0]->m_cardsInfo.nPrepareCards;
	m_linkToServer.Send(&nPrepare, sizeof(int));
	if (nPrepare == 0) nPrepare = 1;//����Ҫ���㶫��
	m_linkToServer.Send(pPrepare, sizeof(int) * nPrepare);
}

void CNetWork_Client::GetPrepareCards()
{
	if (CPlayers::s_nPlayingID == theApp.m_players[0]->m_baseInfo.nID)
	{
		return;
	}
	//�ӷ���������
	int* pPrepare = theApp.m_players.GetPlayerByID(CPlayers::s_nPlayingID)->m_cardsInfo.arrPrepareCards;
	int& nPrepare = theApp.m_players.GetPlayerByID(CPlayers::s_nPlayingID)->m_cardsInfo.nPrepareCards;
	m_linkToServer.Recv(&nPrepare, sizeof(int));
	int nCount = nPrepare;
	if (nPrepare == 0) nCount = 1;//����Ҫ�յ�ʲô��������ѽ
	m_linkToServer.Recv(pPrepare, sizeof(int) * nCount);
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//Server
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CNetWork_Server::Clear()
{
	vector<CLink*> ::const_iterator it = m_arrLinkToClients.begin();
	for(; it!= m_arrLinkToClients.end(); ++it)
	{
		delete (*it);
	}
	m_arrLinkToClients.clear();
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CNetWork_Server::CreateLinks()
{
	m_arrLinkToClients.resize(GConfig::Obj()->GetPlayers());
	for(unsigned int i=0; i<m_arrLinkToClients.size(); ++i)
	{
		m_arrLinkToClients[i] = new CLink();
	}
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

CLink* CNetWork_Server::GetLink(int nIndex)
{
	assert(nIndex >= 0 && nIndex <(int)m_arrLinkToClients.size());
	return m_arrLinkToClients[nIndex];
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

bool CNetWork_Server::GetPlayers(tstring& strDlgNetInfo, const tstring& strIP)
{
	//���տͻ��ļ�������
	CLink listen;
	listen.SocketInit();
	listen.Create();
	listen.Bind();
	listen.Listen();

	//��Ϣ
	strDlgNetInfo = tstring(theApp.m_players[0]->m_baseInfo.sName) + _T("�Ѿ�����������, �ȴ��������...");

	//��ǰ���IDΪ0 (���������)
	theApp.m_players[0]->m_baseInfo.nID = 0;
	int nRecvCount = 0;

	for (int i=0; i<GConfig::Obj()->GetPlayers(); i++)
	{
		if (i == 0)
		{
			continue;
		}

		//���¼��������н��������Ϣ(����,ͷ��)
		listen.Accept(GetLink(i));
		nRecvCount ++;

		CPlayer::BaseInfo& info = theApp.m_players[i]->m_baseInfo;
		GetLink(i)->Recv(&info, sizeof(CPlayer::BaseInfo));//���� baseinfo
		info.nScore = 0;
		info.nID = nRecvCount;
		GetLink(i)->Send(&info.nID, sizeof(int));//���� int

		//��ӡ��Ϣ
		strDlgNetInfo += info.sName;
		strDlgNetInfo += _T("������Ϸ,");

		if (nRecvCount == GConfig::Obj()->GetPlayers()-1)
		{
			strDlgNetInfo += _T("��ʼ��Ϸ!");
		}
		else
		{
			strDlgNetInfo += _T("�ȴ��������...");
		}

		//���ʹ�ӡ��Ϣ
		TCHAR sNetInfo[100];
		_stprintf(sNetInfo, strDlgNetInfo.c_str());
		for(int j=0; j<=i; j++)
		{
			if (theApp.m_players[0] == theApp.m_players[j])
			{
				continue;
			}
			GetLink(j)->Send(sNetInfo,sizeof(char)*100);//���� TCHAR[100]
		}
	}

	//���������Ϣ
	for (int i=0; i<GConfig::Obj()->GetPlayers(); i++)
	{
		CPlayer::BaseInfo& info = theApp.m_players[i]->m_baseInfo;
		for(int j=0; j<GConfig::Obj()->GetPlayers(); j++)
		{
			if (theApp.m_players[0] == theApp.m_players[j] || j == i)
			{
				continue;
			}
			//����ÿ����ҵ�����(������Ҫ����ID)
			GetLink(j)->Send(&info, sizeof(CPlayer::BaseInfo));//����
		}
	}

	//�� ID �����������(������������CPlayer���������Լ�Ϊ0,������˳��)
	theApp.m_players.Sort();

	return true;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CNetWork_Server::GetCards()
{
	//ϴ��
	ShuffleCards();

	//���Ʒ��������ͻ���
	for(int i=0; i<GConfig::Obj()->GetPlayers(); i++)
	{
		if (i == 0)
		{
			continue;
		}
		GetLink(i)->Send(CPlayers::s_AllCards, sizeof(Card) * GConfig::Obj()->GetAllCards());
	}
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CNetWork_Server::GetRandomBanker()
{
	Sleep(1000);//�ô����һ����ʱ�俴�����ȡ����

	//�漴����
	srand(GetTickCount());

	CPlayers::s_nRandomBankerID = rand() % GConfig::Obj()->GetPlayers();

	//���������ͻ���
	for(int i=0; i<GConfig::Obj()->GetPlayers(); i++)
	{
		if (i == 0)
		{
			continue;
		}
		GetLink(i)->Send(&CPlayers::s_nRandomBankerID, sizeof(int));
	}
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CNetWork_Server::AgreeBanker(bool bAgree)
{
	for(int i=0; i<GConfig::Obj()->GetPlayers(); i++)
	{
		if (i == 0)
		{
			continue;
		}
		GetLink(i)->Send(&bAgree, sizeof(bool));
	}
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

bool CNetWork_Server::GetAgreeBanker(int nTempBanker)
{
	bool bAgree;
	GetLink(nTempBanker)->Recv(&bAgree, sizeof(bool));

	for(int i=0; i<GConfig::Obj()->GetPlayers(); i++)
	{
		if (i == 0)
			continue;

		if (nTempBanker == theApp.m_players[i]->m_baseInfo.nID)
			continue;

		GetLink(i)->Send(&bAgree, sizeof(bool));
	}

	return bAgree;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CNetWork_Server::SendPrepareCards()
{
	//���ҷ����Լ��Ĵ�����
	for(int i=0; i<GConfig::Obj()->GetPlayers(); i++)
	{
		if (i == 0)
		{
			continue;
		}
		int* pPrepare = theApp.m_players[0]->m_cardsInfo.arrPrepareCards;
		int nPrepare = theApp.m_players[0]->m_cardsInfo.nPrepareCards;
		GetLink(i)->Send(&nPrepare, sizeof(int));
		if (nPrepare == 0) nPrepare = 1; //����Ҫ����ʲô
		GetLink(i)->Send(pPrepare, sizeof(int) * nPrepare);
	}
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CNetWork_Server::GetPrepareCards()
{
	if (CPlayers::s_nPlayingID == theApp.m_players[0]->m_baseInfo.nID)
	{
		return;
	}

	CPlayer* pPlayer = theApp.m_players.GetPlayerByID(CPlayers::s_nPlayingID);
	int nPlayerIndex = pPlayer->GetIndex();

	int* pPrepare = theApp.m_players[nPlayerIndex]->m_cardsInfo.arrPrepareCards;
	int& nPrepare = theApp.m_players[nPlayerIndex]->m_cardsInfo.nPrepareCards;
	GetLink(nPlayerIndex)->Recv(&nPrepare, sizeof(int));
	int nCount = nPrepare;
	if (nCount == 0) nCount = 1;	//����Ҫ�շ��㶫��
	GetLink(nPlayerIndex)->Recv(pPrepare, sizeof(int) * nCount);

	for(int i=0; i<GConfig::Obj()->GetPlayers(); i++)
	{
		if (i == 0)
			continue;

		if (i == nPlayerIndex)
			continue;

		GetLink(i)->Send(&nPrepare, sizeof(int));
		GetLink(i)->Send(pPrepare, sizeof(int) * nCount);
	}
}

