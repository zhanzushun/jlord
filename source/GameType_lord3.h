#pragma once

#include "Config.h"

class CGameType_Lord3 : public IGameType
{
public:

	//ȡ��һ��״̬
	virtual IState* GetNextState(IState*);

	//�����ƵĴ�С�Ƚ�(��������)
	virtual bool CardLess(const Card& card1, const Card& card2);

	//ȡ����(���ݲ�ͬ��Ϸ�����в�ͬ������)
	virtual tstring GetNameText(int nID, const tstring& sOriginal);

	virtual void GetText_DecideBanker(int i, tstring& sText);

	virtual bool CheckPrepareCards(const Cards& arrPrevCards, const Cards& arrCards);

	virtual void ComputerPrepareCards(Cards& arrCards, const Cards& arrHandCards, const Cards& arrPrevCards);

	virtual int AccountCardsValue(const Cards& arrHandCards);

};


