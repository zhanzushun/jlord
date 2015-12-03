#include "GDefine.h"

#include "GameType_lord3.h"

#include "Application.h"
#include "States.h"

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

IGameType* IGameType::CreateGameType(IGameType::EGameType type)
{
	if (type == T_LORD3)
	{
		return new CGameType_Lord3;
	}
	return NULL;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

IState* CGameType_Lord3::GetNextState(IState* pState)
{
	if (pState == NULL)
	{
		return &GStatus::s_stateInit;
	}
	else if (pState == &GStatus::s_stateInit)
	{
		return &GStatus::s_stateSendCards;
	}
	else if (pState == &GStatus::s_stateSendCards)
	{
		return &GStatus::s_stateDecideBanker;
	}
	else if (pState == &GStatus::s_stateDecideBanker)
	{
		return &GStatus::s_statePlayCards;
	}
	else if (pState == &GStatus::s_statePlayCards)
	{
		return &GStatus::s_stateInit;
	}
	assert(0);
	return NULL;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

bool CGameType_Lord3::CardLess(const Card& card1, const Card& card2)
{
	return card1.nValue < card2.nValue;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

tstring CGameType_Lord3::GetNameText(int nID, const tstring& sOriginal)
{
	if (CPlayers::s_nBankerID == nID)
	{
		return (sOriginal + _T("(����)"));
	}
	else
	{
		return sOriginal;
	}
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CGameType_Lord3::GetText_DecideBanker(int i, tstring& sText)
{
	if (i == IGameType::Decide_Banker_Random)
	{
		sText = _T("���ڲ����������");
	}
	else if (i == IGameType::Decide_Banker_PreConfirm)
	{
		sText = _T("�ȴ�\"") + sText + _T("\"ȷ���Ƿ�Ҫ������");
	}
	else if (i == IGameType::Decide_Banker_Random)
	{
		sText = _T("���ֵĵ�����\"") + sText + _T("\"���ֿ�ʼ");
	}
	else if (i == Decide_Banker_Msgbox)
	{
		sText = _T("���Ƿ�Ҫ������?");
	}
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/*						���ͼ���ֵ����

����(Cards_Type)		  ��ֵ(Cards_Value)           ����:
һ��:		 2				CCard.Value(��ֵ)			2
��С��:		 11

����:		 3				CCard.Value(��ֵ)			3

����һ:		 31				���ŵ�Card.Value(��ֵ)		4
����:		 4				CCard.Value(��ֵ)			4

��˳:		 5				��С�Ƶ�Card.Value(��ֵ)    5
����һ��:	 32				���ŵ�Card.Value(��ֵ)		5		

��˳:		 6				��С�Ƶ�Card.Value(��ֵ)    6
˫˳:		 222			��С�Ƶ�Card.Value(��ֵ)    6
��˳:	     33				��С�Ƶ�Card.Value(��ֵ)    6
�Ĵ�����:	 411			���ŵ�Card.Value(��ֵ)		6	 

��˳:		 7				��С�Ƶ�Card.Value(��ֵ)    7

��˳:		 8				��С�Ƶ�Card.Value(��ֵ)    8
˫˳		 2222			��С�Ƶ�Card.Value(��ֵ)    8
��˳������:  3311			��С���ŵ�Card.Value(��ֵ)  8
�Ĵ�����:	 422			���ŵ�Card.Value(��ֵ)		8

��˳:		 9				��С�Ƶ�Card.Value(��ֵ)    9
��˳:		 333			��С���ŵ�Card.Value(��ֵ)  9

��˳:		 A				��С�Ƶ�Card.Value(��ֵ)    10
˫˳:		 22222			��С�Ƶ�Card.Value(��ֵ)    10
��˳������:  3322			��С���ŵ�Card.Value(��ֵ)  10

��˳:		 B				��С�Ƶ�Card.Value(��ֵ)    11

��˳:		 C				��С�Ƶ�Card.Value(��ֵ)    12
˫˳:		 222222			��С���Ƶ�Card.Value(��ֵ)  12
��˳:		 3333			��С���ŵ�Card.Value(��ֵ)  12
��˳����:	 333111			��С���ŵ�Card.Value(��ֵ)  12

˫˳		 2222222		��С���Ƶ�Card.Value(��ֵ)  14

������:		 333222			��С���ŵ�Card.Value(��ֵ)  15
��˳:		 33333			��С���ŵ�Card.Value(��ֵ)  15

˫˳		 22222222		��С���Ƶ�Card.Value(��ֵ)  16
��˳���ĵ�:  33331111		��С���ŵ�Card.Value(��ֵ)  16

˫˳		 222222222		��С���Ƶ�Card.Value(��ֵ)  18
��˳		 333333			��С���ŵ�Card.Value(��ֵ)  18

˫˳		 2222222222		��С���Ƶ�Card.Value(��ֵ)  20
��˳���嵥:	 3333311111		��С���ŵ�Card.Value(��ֵ)  20
��˳���Ķ�:  33332222		��С���ŵ�Card.Value(��ֵ)  20
*/

struct CheckResult
{
	enum ECardType
	{
		T_NULL = 0,
		T_1_1 = 1,
		T_2_2, T_2_BOOM,
		T_3_3,
		T_4_31, T_4_BOOM,
		T_5_5, T_5_32, 
		T_6_6, T_6_222, T_6_33, T_6_411,
		T_7_7,
		T_8_8, T_8_2222, T_8_3311, T_8_422,
		T_9_9, T_9_333,
		T_10_A, T_10_22222, T_10_3322,
		T_11_B,
		T_12_C, T_12_222222, T_12_3333, T_12_333111,
		T_14_2222222,
		T_15_333222, T_15_33333,
		T_16_22222222, T_16_33331111,
		T_18_222222222, T_18_333333,
		T_20_2222222222, T_20_3333311111, T_20_33332222,
	};

	ECardType nCardType;
	int nValue;
	CheckResult() : nCardType(T_NULL), nValue(2) {}; 
};

//-------------------------------------------------------------------------------------------------------------
inline bool pSame2(const vector<Card>& arrCards, int nStart)
{
	return arrCards[nStart].nValue == arrCards[nStart+1].nValue;
}

//-------------------------------------------------------------------------------------------------------------
inline bool pSame3(const vector<Card>& arrCards, int nStart)
{
	return (arrCards[nStart].nValue == arrCards[nStart+1].nValue) &&
		(arrCards[nStart+1].nValue == arrCards[nStart+2].nValue);
}

//-------------------------------------------------------------------------------------------------------------
inline bool pSame4(const vector<Card>& arrCards, int nStart)
{
	return (arrCards[nStart].nValue == arrCards[nStart+1].nValue) &&
		(arrCards[nStart+1].nValue == arrCards[nStart+2].nValue) && 
		(arrCards[nStart+2].nValue == arrCards[nStart+3].nValue);
}

//-------------------------------------------------------------------------------------------------------------
bool pSerial(const vector<Card>& arrCards, int nStart, int nNumber, int nSameCount)
{
	int nValue = arrCards[nStart].nValue;

	for (int i=0; i<nNumber; i+=nSameCount)
	{
		if (nValue > Card::VALUE_A) return false;
		if (arrCards[nStart+i].nValue != nValue) return false;
		for (int j=1; j<nSameCount; j++)
		{
			if (arrCards[nStart+i+j-1].nValue != arrCards[nStart+i+j].nValue) return false;
		}
		nValue ++;
	}
	return true;
}

//-------------------------------------------------------------------------------------------------------------
//T_2_2 = 1, T_2_BOOM,
CheckResult pCheck2(const vector<Card>& arrCards)
{
	CheckResult r;
	if (arrCards[0].nValue == Card::VALUE_JOKER_S && arrCards[1].nValue == Card::VALUE_JOKER_B)
	{
		r.nValue = Card::VALUE_JOKER_S;
		r.nCardType = CheckResult::T_2_BOOM;
		return r;
	}
	if (pSame2(arrCards, 0))
	{
		r.nValue = arrCards[0].nValue;
		r.nCardType = CheckResult::T_2_2;
		return r;
	}
	return r;
}

//-------------------------------------------------------------------------------------------------------------
//T_3_3,
CheckResult pCheck3(const vector<Card>& arrCards)
{
	CheckResult r;
	if (pSame3(arrCards, 0))
	{
		r.nValue = arrCards[0].nValue;
		r.nCardType = CheckResult::T_3_3;
		return r;
	}
	return r;
}

//-------------------------------------------------------------------------------------------------------------
//T_4_31, T_4_BOOM,
CheckResult pCheck4(const vector<Card>& arrCards)
{
	CheckResult r;
	if (pSame3(arrCards, 0))
	{
		r.nValue = arrCards[0].nValue;
		r.nCardType = CheckResult::T_4_31;
		return r;
	}
	if (pSame3(arrCards, 1))
	{
		r.nValue = arrCards[1].nValue;
		r.nCardType = CheckResult::T_4_31;
		return r;
	}
	if (pSame4(arrCards, 0))
	{
		r.nValue = arrCards[0].nValue;
		r.nCardType = CheckResult::T_4_BOOM;
		return r;
	}
	return r;
}

//-------------------------------------------------------------------------------------------------------------
//T_5_5, T_5_32, 
CheckResult pCheck5(const vector<Card>& arrCards)
{
	CheckResult r;
	if (pSerial(arrCards, 0, 5, 1))
	{
		r.nCardType = CheckResult::T_5_5;
		r.nValue = arrCards[0].nValue;
		return r;
	}
	if (pSame3(arrCards, 0) && pSame2(arrCards, 3))
	{
		r.nCardType = CheckResult::T_5_32;
		r.nValue = arrCards[0].nValue;
		return r;
	}
	if (pSame3(arrCards, 2) && pSame2(arrCards, 0))
	{
		r.nCardType = CheckResult::T_5_32;
		r.nValue = arrCards[2].nValue;
		return r;
	}
	return r;
}

//-------------------------------------------------------------------------------------------------------------
//T_6_6, T_6_222, T_6_33, T_6_411,
CheckResult pCheck6(const vector<Card>& arrCards)
{
	CheckResult r;
	if (pSerial(arrCards, 0, 6, 1))
	{
		r.nCardType = CheckResult::T_6_6;
		r.nValue = arrCards[0].nValue;
		return r;
	}
	if (pSerial(arrCards, 0, 3, 2))
	{
		r.nCardType = CheckResult::T_6_222;
		r.nValue = arrCards[0].nValue;
		return r;
	}
	if (pSerial(arrCards, 0, 2, 3))
	{
		r.nCardType = CheckResult::T_6_33;
		r.nValue = arrCards[0].nValue;
		return r;
	}
	for (int i=0; i < 3; i++)
	{
		if (pSame4(arrCards, i))
		{
			r.nCardType = CheckResult::T_6_411;
			r.nValue = arrCards[i].nValue;
			return r;
		}
	}
	return r;
}

//-------------------------------------------------------------------------------------------------------------
//T_7_7,
CheckResult pCheck7(const vector<Card>& arrCards)
{
	CheckResult r;
	if (pSerial(arrCards, 0, 7, 1))
	{
		r.nCardType = CheckResult::T_7_7;
		r.nValue = arrCards[0].nValue;
	}
	return r;
}

//-------------------------------------------------------------------------------------------------------------
//T_8_8, T_8_2222, T_8_3311, T_8_422,
CheckResult pCheck8(const vector<Card>& arrCards)
{
	CheckResult r;
	if (pSerial(arrCards, 0, 8, 1))
	{
		r.nCardType = CheckResult::T_8_8;
		r.nValue = arrCards[0].nValue;
		return r;
	}
	//2222
	if (pSerial(arrCards, 0, 4, 2))
	{
		r.nCardType = CheckResult::T_8_2222;
		r.nValue = arrCards[0].nValue;
		return r;
	}
	//422
	if (pSame2(arrCards, 0) && pSame2(arrCards, 2) && pSame4(arrCards, 4))
	{
		r.nCardType = CheckResult::T_8_422;
		r.nValue = arrCards[4].nValue;
		return r;
	}
	if (pSame2(arrCards, 0) && pSame4(arrCards, 2) && pSame2(arrCards, 6))
	{
		r.nCardType = CheckResult::T_8_422;
		r.nValue = arrCards[2].nValue;
		return r;
	}
	if (pSame4(arrCards, 0) && pSame2(arrCards, 4) && pSame2(arrCards, 6))
	{
		r.nCardType = CheckResult::T_8_422;
		r.nValue = arrCards[0].nValue;
		return r;
	}
	//3311
	for (int i=0; i < 3; i++)
	{
		if (pSerial(arrCards, i, 2, 3))
		{
			r.nCardType = CheckResult::T_8_3311;
			r.nValue = arrCards[i].nValue;
			return r;
		}
	}
	return r;
}

//-------------------------------------------------------------------------------------------------------------
//T_9_9, T_9_333,
CheckResult pCheck9(const vector<Card>& arrCards)
{
	CheckResult r;
	int nCount = 9;
	if (pSerial(arrCards, 0, 9, 1))
	{
		r.nCardType = CheckResult::T_9_9;
		r.nValue = arrCards[0].nValue;
		return r;
	}
	if (pSerial(arrCards, 0, 3, 3))
	{
		r.nCardType = CheckResult::T_9_333;
		r.nValue = arrCards[0].nValue;
		return r;
	}
	return r;
}

//-------------------------------------------------------------------------------------------------------------
//T_10_A, T_10_22222, T_10_3322,
CheckResult pCheck10(const vector<Card>& arrCards)
{
	CheckResult r;
	if (pSerial(arrCards, 0, 10, 1))
	{
		r.nCardType = CheckResult::T_10_A;
		r.nValue = arrCards[0].nValue;
		return r;
	}
	if (pSerial(arrCards, 0, 5, 2))
	{
		r.nCardType = CheckResult::T_10_22222;
		r.nValue = arrCards[0].nValue;
		return r;
	}
	for (int i=0; i < 5; i+=2)
	{
		if (pSerial(arrCards, i, 2, 3))
		{
			r.nCardType = CheckResult::T_10_3322;
			r.nValue = arrCards[i].nValue;
			return r;
		}
	}
	return r;
}

//-------------------------------------------------------------------------------------------------------------
//T_11_B,
CheckResult pCheck11(const vector<Card>& arrCards)
{
	CheckResult r;
	if (pSerial(arrCards, 0, 11, 1))
	{
		r.nCardType = CheckResult::T_11_B;
		r.nValue = arrCards[0].nValue;
		return r;
	}
	return r;
}

//-------------------------------------------------------------------------------------------------------------
//T_12_C, T_12_222222, T_12_3333, T_12_333111,
CheckResult pCheck12(const vector<Card>& arrCards)
{
	CheckResult r;
	if (pSerial(arrCards, 0, 12, 1))
	{
		r.nCardType = CheckResult::T_12_C;
		r.nValue = arrCards[0].nValue;
		return r;
	}
	if (pSerial(arrCards, 0, 6, 2))
	{
		r.nCardType = CheckResult::T_12_222222;
		r.nValue = arrCards[0].nValue;
		return r;
	}
	if (pSerial(arrCards, 0, 4, 3))
	{
		r.nCardType = CheckResult::T_12_3333;
		r.nValue = arrCards[0].nValue;
		return r;
	}
	for (int i=0; i < 4; i++)
	{
		if (pSerial(arrCards, i, 3, 3))
		{
			r.nCardType = CheckResult::T_12_333111;
			r.nValue = arrCards[i].nValue;
			return r;
		}
	}
	return r;
}

//-------------------------------------------------------------------------------------------------------------
//T_14_2222222,
CheckResult pCheck14(const vector<Card>& arrCards)
{
	CheckResult r;
	if (pSerial(arrCards, 0, 7, 2))
	{
		r.nCardType = CheckResult::T_14_2222222;
		r.nValue = arrCards[0].nValue;
		return r;
	}
	return r;
}

//-------------------------------------------------------------------------------------------------------------
//T_15_333222, T_15_33333,
CheckResult pCheck15(const vector<Card>& arrCards)
{
	CheckResult r;
	if (pSerial(arrCards, 0, 5, 3))
	{
		r.nCardType = CheckResult::T_15_33333;
		r.nValue = arrCards[0].nValue;
		return r;
	}
	for (int i=0; i < 15-8; i+=2)
	{
		if (pSerial(arrCards, i, 3, 3))
		{
			r.nCardType = CheckResult::T_15_333222;
			r.nValue = arrCards[i].nValue;
			return r;
		}
	}
	return r;
}

//-------------------------------------------------------------------------------------------------------------
//T_16_22222222, T_16_33331111
CheckResult pCheck16(const vector<Card>& arrCards)
{
	CheckResult r;
	if (pSerial(arrCards, 0, 8, 2))
	{
		r.nCardType = CheckResult::T_16_22222222;
		r.nValue = arrCards[0].nValue;
		return r;
	}
	for (int i=0; i < 16-11; i++)
	{
		if (pSerial(arrCards, i, 4, 3))
		{
			r.nCardType = CheckResult::T_16_33331111;
			r.nValue = arrCards[i].nValue;
			return r;
		}
	}
	return r;
}

//-------------------------------------------------------------------------------------------------------------
//T_18_222222222, T_18_333333,

CheckResult pCheck18(const vector<Card>& arrCards)
{
	CheckResult r;
	if (pSerial(arrCards, 0, 9, 2))
	{
		r.nCardType = CheckResult::T_18_222222222;
		r.nValue = arrCards[0].nValue;
		return r;
	}
	if (pSerial(arrCards, 0, 6, 3))
	{
		r.nCardType = CheckResult::T_18_333333;
		r.nValue = arrCards[0].nValue;
		return r;
	}
	return r;
}

//-------------------------------------------------------------------------------------------------------------
//T_20_2222222222, T_20_3333311111, T_20_33332222,
CheckResult pCheck20(const vector<Card>& arrCards)
{
	CheckResult r;
	if (pSerial(arrCards, 0, 10, 2))
	{
		r.nCardType = CheckResult::T_20_2222222222;
		r.nValue = arrCards[0].nValue;
		return r;
	}
	for (int i=0; i < 20-14; i++)
	{
		if (pSerial(arrCards, i, 5, 3))
		{
			r.nCardType = CheckResult::T_20_3333311111;
			r.nValue = arrCards[i].nValue;
			return r;
		}
	}
	for (int i=0; i < 20-11; i+=2)
	{
		if (pSerial(arrCards, i, 4, 3))
		{
			r.nCardType = CheckResult::T_20_33332222;
			r.nValue = arrCards[i].nValue;
			return r;
		}
	}
	return r;
}

//-------------------------------------------------------------------------------------------------------------
bool CGameType_Lord3::CheckPrepareCards(const vector<Card>& arrPrevCards, const vector<Card>& arrCards)
{
	CheckResult r;
	CheckResult r2;
	switch(arrCards.size())
	{
	case 1:
		if (arrPrevCards.size() == 0) return true;
		if (arrPrevCards.size() != 1) return false;
		return (arrPrevCards[0] < arrCards[0]);
	case 2:
		r = pCheck2(arrCards);
		if (r.nCardType == CheckResult::T_NULL) return false;
		if (arrPrevCards.size() == 0) return true;
		r2 = pCheck2(arrPrevCards);
		if (r2.nCardType == CheckResult::T_2_BOOM) return false;	//����ը��(��С��)
		if (r.nCardType == CheckResult::T_2_BOOM) return true;      //����ը��(��С��)
		return (r2.nCardType == r.nCardType) && (r2.nValue < r.nValue);
	case 3:
		r = pCheck3(arrCards);
		if (r.nCardType == CheckResult::T_NULL) return false;
		if (arrPrevCards.size() == 0) return true;
		r2 = pCheck3(arrPrevCards);
		return (r2.nCardType == r.nCardType) && (r2.nValue < r.nValue);
	case 4:
		r = pCheck4(arrCards);
		if (r.nCardType == CheckResult::T_NULL) return false;
		if (arrPrevCards.size() == 0) return true;
		if (r.nCardType == CheckResult::T_4_BOOM && r2.nCardType != CheckResult::T_4_BOOM) return true; //ը��
		r2 = pCheck4(arrPrevCards);
		return (r2.nCardType == r.nCardType) && (r2.nValue < r.nValue);
	case 5:
		r = pCheck5(arrCards);
		if (r.nCardType == CheckResult::T_NULL) return false;
		if (arrPrevCards.size() == 0) return true;
		r2 = pCheck5(arrPrevCards);
		return (r2.nCardType == r.nCardType) && (r2.nValue < r.nValue);
	case 6:
		r = pCheck6(arrCards);
		if (r.nCardType == CheckResult::T_NULL) return false;
		if (arrPrevCards.size() == 0) return true;
		r2 = pCheck6(arrPrevCards);
		return (r2.nCardType == r.nCardType) && (r2.nValue < r.nValue);
	case 7:
		r = pCheck7(arrCards);
		if (r.nCardType == CheckResult::T_NULL) return false;
		if (arrPrevCards.size() == 0) return true;
		r2 = pCheck7(arrPrevCards);
		return (r2.nCardType == r.nCardType) && (r2.nValue < r.nValue);
	case 8:
		r = pCheck8(arrCards);
		if (r.nCardType == CheckResult::T_NULL) return false;
		if (arrPrevCards.size() == 0) return true;
		r2 = pCheck8(arrPrevCards);
		return (r2.nCardType == r.nCardType) && (r2.nValue < r.nValue);
	case 9:
		r = pCheck9(arrCards);
		if (r.nCardType == CheckResult::T_NULL) return false;
		if (arrPrevCards.size() == 0) return true;
		r2 = pCheck9(arrPrevCards);
		return (r2.nCardType == r.nCardType) && (r2.nValue < r.nValue);
	case 10:
		r = pCheck10(arrCards);
		if (r.nCardType == CheckResult::T_NULL) return false;
		if (arrPrevCards.size() == 0) return true;
		r2 = pCheck10(arrPrevCards);
		return (r2.nCardType == r.nCardType) && (r2.nValue < r.nValue);
	case 11:
		r = pCheck11(arrCards);
		if (r.nCardType == CheckResult::T_NULL) return false;
		if (arrPrevCards.size() == 0) return true;
		r2 = pCheck11(arrPrevCards);
		return (r2.nCardType == r.nCardType) && (r2.nValue < r.nValue);
	case 12:
		r = pCheck12(arrCards);
		if (r.nCardType == CheckResult::T_NULL) return false;
		if (arrPrevCards.size() == 0) return true;
		r2 = pCheck12(arrPrevCards);
		return (r2.nCardType == r.nCardType) && (r2.nValue < r.nValue);
	case 13:
		return false;
	case 14:
		r = pCheck14(arrCards);
		if (r.nCardType == CheckResult::T_NULL) return false;
		if (arrPrevCards.size() == 0) return true;
		r2 = pCheck14(arrPrevCards);
		return (r2.nCardType == r.nCardType) && (r2.nValue < r.nValue);
	case 15:
		r = pCheck15(arrCards);
		if (r.nCardType == CheckResult::T_NULL) return false;
		if (arrPrevCards.size() == 0) return true;
		r2 = pCheck15(arrPrevCards);
		return (r2.nCardType == r.nCardType) && (r2.nValue < r.nValue);
	case 16:
		r = pCheck16(arrCards);
		if (r.nCardType == CheckResult::T_NULL) return false;
		if (arrPrevCards.size() == 0) return true;
		r2 = pCheck14(arrPrevCards);
		return (r2.nCardType == r.nCardType) && (r2.nValue < r.nValue);
	case 17:
		return false;
	case 18:
		r = pCheck18(arrCards);
		if (r.nCardType == CheckResult::T_NULL) return false;
		if (arrPrevCards.size() == 0) return true;
		r2 = pCheck18(arrPrevCards);
		return (r2.nCardType == r.nCardType) && (r2.nValue < r.nValue);
	case 19:
		return false;
	case 20:
		r = pCheck20(arrCards);
		if (r.nCardType == CheckResult::T_NULL) return false;
		if (arrPrevCards.size() == 0) return true;
		r2 = pCheck20(arrPrevCards);
		return (r2.nCardType == r.nCardType) && (r2.nValue < r.nValue);
	}
	return false;
}

//=============================================================================================================================
//=============================================================================================================================
//=============================================================================================================================
//=============================================================================================================================
//=============================================================================================================================
// ���Գ���
//=============================================================================================================================
//=============================================================================================================================
//=============================================================================================================================
//=============================================================================================================================
//=============================================================================================================================

//-------------------------------------------------------------------------------------------------------------

void pGetCardsTableInfo(vector<int>& arrCounter, vector<int>& arrMinValue, const vector<int>& arrCardsTable)
{
	arrCounter.clear();
	arrMinValue.clear();
	arrCounter.resize(5);
	arrMinValue.resize(5);
	for(int i=Card::VALUE_2; i>=3; i--)
	{
		arrCounter[arrCardsTable[i]] ++;
		arrMinValue[arrCardsTable[i]] = i;
	}
}


//-------------------------------------------------------------------------------------------------------------
CheckResult pCheckCards(const vector<Card>& arrCards)
{
	switch(arrCards.size())
	{
	case 1:
		{
			CheckResult r;
			r.nCardType = CheckResult::T_1_1;
			r.nValue = arrCards[0].nValue;
			return r;
		}
	case 2:
		return pCheck2(arrCards);
	case 3:
		return pCheck3(arrCards);
	case 4:
		return pCheck4(arrCards);
	case 5:
		return pCheck5(arrCards);
	case 6:
		return pCheck6(arrCards);
	case 7:
		return pCheck7(arrCards);
	case 8:
		return pCheck8(arrCards);
	case 9:
		return pCheck9(arrCards);
	case 10:
		return pCheck10(arrCards);
	case 11:
		return pCheck11(arrCards);
	case 12:
		return pCheck12(arrCards);
	case 13:
		return CheckResult();
	case 14:
		return pCheck14(arrCards);
	case 15:
		return pCheck15(arrCards);
	case 16:
		return pCheck16(arrCards);
	case 17:
		return CheckResult();
	case 18:
		return pCheck18(arrCards);
	case 19:
		return CheckResult();
	case 20:
		return pCheck20(arrCards);
	}
	return CheckResult();
}


//-------------------------------------------------------------------------------------------------------------
//˫��
void pSearch_Jokers(Cards& arrCards, const Cards& arrHandCards, const vector<int>& arrCardsTable, const CheckResult& chkPrev)
{
	arrCards.clear();
	int nHandCards = (int)arrHandCards.size();

	//n���Ƶĸ���(n:0~4)
	vector<int> arrCounter(5);

	//n���Ƶ���С��ֵ(n:0~4)
	vector<int> arrMinValue(5);

	pGetCardsTableInfo(arrCounter, arrMinValue, arrCardsTable);

	//˫��
	if ((arrCardsTable[Card::VALUE_JOKER_B] == 1 && arrCardsTable[Card::VALUE_JOKER_S] == 1) && 
		((nHandCards == 2) || 
		(nHandCards == 3) ||		//˫���ӵ���
		((nHandCards == 4) && (arrCounter[2] == 1)) ||	//˫����һ��;
		((nHandCards == 5) && (arrCounter[3] == 1)) ||	//˫��������;
		((nHandCards == 5) && (arrCounter[4] == 1)) ||	//˫��������
		((nHandCards == 6) && (arrCounter[3] == 1)) ||	//˫����31
		((nHandCards == 7) && (arrCounter[3] == 1) && (arrCounter[2] == 1)) ||	//˫����32
		((nHandCards == 8) && (arrCounter[4] == 1) && (arrCounter[1] == 2))	//˫����42
		))
	{
		arrCards.push_back(arrHandCards[nHandCards-1]);
		arrCards.push_back(arrHandCards[nHandCards-2]);
		return;
	}
}

//-------------------------------------------------------------------------------------------------------------
//������ֵѰ��ĳ���Ƶ�λ��
int pFindByValue(const Cards& arrHandCards, int nValue)
{
	for (int i=0; i<(int)arrHandCards.size(); i++)
	{
		if (arrHandCards[i].nValue == nValue)
			return i;
	}
	return -1;
}

//-------------------------------------------------------------------------------------------------------------
//Ѱ�ҵ��Ƴ�
void pSearch_1(Cards& arrCards, const Cards& arrHandCards, const vector<int>& arrCardsTable, const CheckResult& chkPrev)
{
	arrCards.clear();
	pSearch_Jokers(arrCards, arrHandCards, arrCardsTable, chkPrev);
	if (arrCards.size() > 0) return;

	int nHandCards = (int)arrHandCards.size();

	////���ϵ���̫��.������
	//if (chkPrev.nValue >= Card::VALUE_JOKER_S && nHandCards > 3)
	//{
	//	return;
	//}

	//�����д���. ����
	if (arrCardsTable[Card::VALUE_JOKER_B] == 1 && chkPrev.nValue == Card::VALUE_JOKER_S)
	{
		arrCards.push_back(arrHandCards[nHandCards-1]);
		return;
	}

	//������
	int i = 0;
	int j = 0;
	for(i=chkPrev.nValue+1; i<=Card::VALUE_JOKER_B; i++)
	{
		if (arrCardsTable[i] == 1)
		{
			int nIndex = pFindByValue(arrHandCards, i);
			if (nIndex >= 0) arrCards.push_back(arrHandCards[nIndex]);
			return;
		}
	}

	//�����ŵ�(�Ӵ�С)
	for (i=Card::VALUE_JOKER_B; i>chkPrev.nValue; i--)
	{
		if (arrCardsTable[i] == 2)
		{
			int nIndex = pFindByValue(arrHandCards, i);
			if (nIndex >= 0) arrCards.push_back(arrHandCards[nIndex]);
			return;
		}
	}

	//�����ŵ�
	for (i=Card::VALUE_2; i>chkPrev.nValue; i--)
	{
		int nIndex = pFindByValue(arrHandCards, i);
		if (nIndex >= 0) arrCards.push_back(arrHandCards[nIndex]);
		return;
	}
}

//-------------------------------------------------------------------------------------------------------------
//Ѱ�Ҷ��Ƴ�
void pSearch_2(Cards& arrCards, const Cards& arrHandCards, const vector<int>& arrCardsTable, const CheckResult& chkPrev)
{
	arrCards.clear();
	pSearch_Jokers(arrCards, arrHandCards, arrCardsTable, chkPrev);
	if (arrCards.size() > 0) return;

	//����
	int i=0,j=0;
	for (i=chkPrev.nValue+1; i<Card::VALUE_JOKER_S; i++)
	{
		if (arrCardsTable[i] == 2)
		{
			int nIndex = pFindByValue(arrHandCards, i);
			if (nIndex >= 0)
			{
				arrCards.push_back(arrHandCards[nIndex]);
				arrCards.push_back(arrHandCards[nIndex+1]);
				return;
			}
		}
	}

	//�����ŵ�
	if (chkPrev.nValue >= 12)   //����ǱȽϴ����,�ſ��ǲ���
	{ 
		for(i=Card::VALUE_2; i>=chkPrev.nValue+1; i--)
		{
			if (arrCardsTable[i] == 3)
			{
				int nIndex = pFindByValue(arrHandCards, i);
				if (nIndex >= 0)
				{
					arrCards.push_back(arrHandCards[nIndex]);
					arrCards.push_back(arrHandCards[nIndex+1]);
					return;
				}
			}
		}
	}
}


//-------------------------------------------------------------------------------------------------------------
//Ѱ�����Ƴ�
void pSearch_3(Cards& arrCards, const Cards& arrHandCards, const vector<int>& arrCardsTable, const CheckResult& chkPrev)
{
	arrCards.clear();
	pSearch_Jokers(arrCards, arrHandCards, arrCardsTable, chkPrev);
	if (arrCards.size() > 0) return;

	int i=0,j=0;
	for(i=chkPrev.nValue+1; i<Card::VALUE_JOKER_S; i++)
	{
		if( arrCardsTable[i] == 3)
		{
			int nIndex = pFindByValue(arrHandCards, i);
			if (nIndex >= 0)
			{
				arrCards.push_back(arrHandCards[nIndex]);
				arrCards.push_back(arrHandCards[nIndex+1]);
				arrCards.push_back(arrHandCards[nIndex+2]);
				return;
			}
		}
	}
}

//-------------------------------------------------------------------------------------------------------------
//Ѱ��ը����
void pSearch_4(Cards& arrCards, const Cards& arrHandCards, const vector<int>& arrCardsTable, const CheckResult& chkPrev)
{
	arrCards.clear();
	pSearch_Jokers(arrCards, arrHandCards, arrCardsTable, chkPrev);
	if (arrCards.size() > 0) return;

	//����;
	int i;
	for(i=chkPrev.nValue+1; i<Card::VALUE_JOKER_S; i++)
	{
		if( arrCardsTable[i] == 4)
		{
			int nIndex = pFindByValue(arrHandCards, i);
			if (nIndex >= 0)
			{
				arrCards.push_back(arrHandCards[nIndex]);
				arrCards.push_back(arrHandCards[nIndex+1]);
				arrCards.push_back(arrHandCards[nIndex+2]);
				arrCards.push_back(arrHandCards[nIndex+3]);
				return;
			}
		}
	}
}

//-------------------------------------------------------------------------------------------------------------
//Ѱ������1��
void pSearch_31(Cards& arrCards, const Cards& arrHandCards, const vector<int>& arrCardsTable, const CheckResult& chkPrev)
{
	arrCards.clear();
	pSearch_Jokers(arrCards, arrHandCards, arrCardsTable, chkPrev);
	if (arrCards.size() > 0) return;

	int i,j;
	int nHandCards = (int)arrHandCards.size();

	int nMax = Card::VALUE_A;
	if (nHandCards <= 5)
	{
		nMax = Card::VALUE_2;//����2һ�㲻��
	}

	for(i=chkPrev.nValue+1; i<=nMax; i++)
	{
		if (arrCardsTable[i] == 3)
		{
			for(j=3; j<=Card::VALUE_2; j++)
			{
				if (arrCardsTable[j] == 1)
				{
					int nIndex = pFindByValue(arrHandCards, i); assert(nIndex >= 0);
					arrCards.push_back(arrHandCards[nIndex]);
					arrCards.push_back(arrHandCards[nIndex+1]);
					arrCards.push_back(arrHandCards[nIndex+2]);

					nIndex = pFindByValue(arrHandCards, j); assert(nIndex >= 0);
					arrCards.push_back(arrHandCards[nIndex]);
					return;
				}
			}
		}
	}
}

//-------------------------------------------------------------------------------------------------------------
//Ѱ������2��
void pSearch_32(Cards& arrCards, const Cards& arrHandCards, const vector<int>& arrCardsTable, const CheckResult& chkPrev)
{
	arrCards.clear();
	pSearch_Jokers(arrCards, arrHandCards, arrCardsTable, chkPrev);
	if (arrCards.size() > 0) return;

	int i,j;
	int nHandCards = (int)arrHandCards.size();

	int nMax = Card::VALUE_A;
	if (nHandCards <= 5)
	{
		nMax = Card::VALUE_2;//����2һ�㲻��
	}

	for(i=chkPrev.nValue+1; i<=nMax; i++)
	{
		if (arrCardsTable[i] == 3)
		{
			for(j=3; j<Card::VALUE_2; j++)
			{
				if (arrCardsTable[j] == 2)
				{
					int nIndex = pFindByValue(arrHandCards, i); assert(nIndex >= 0);
					arrCards.push_back(arrHandCards[nIndex]);
					arrCards.push_back(arrHandCards[nIndex+1]);
					arrCards.push_back(arrHandCards[nIndex+2]);

					nIndex = pFindByValue(arrHandCards, j); assert(nIndex >= 0);
					arrCards.push_back(arrHandCards[nIndex]);
					arrCards.push_back(arrHandCards[nIndex+1]);
					return;
				}
			}
		}
	}
}

//-------------------------------------------------------------------------------------------------------------
//Ѱ���Ĵ�2��
void pSearch_411(Cards& arrCards, const Cards& arrHandCards, const vector<int>& arrCardsTable, const CheckResult& chkPrev)
{
	arrCards.clear();
	pSearch_Jokers(arrCards, arrHandCards, arrCardsTable, chkPrev);
	if (arrCards.size() > 0) return;

	int i,j,k;
	int nHandCards = (int)arrHandCards.size();

	for(i=chkPrev.nValue+1; i<=Card::VALUE_2; i++)
	{
		if (arrCardsTable[i] == 4)
		{
			for(j=3; j<Card::VALUE_2; j++)
			{
				if (arrCardsTable[j] == 1)
				{
					for(k=j+1; k<Card::VALUE_2; k++)
					{
						if (arrCardsTable[k] == 1)
						{
							int nIndex = pFindByValue(arrHandCards, i); assert(nIndex >= 0);
							arrCards.push_back(arrHandCards[nIndex]);
							arrCards.push_back(arrHandCards[nIndex+1]);
							arrCards.push_back(arrHandCards[nIndex+2]);
							arrCards.push_back(arrHandCards[nIndex+3]);

							nIndex = pFindByValue(arrHandCards, j); assert(nIndex >= 0);
							arrCards.push_back(arrHandCards[nIndex]);

							nIndex = pFindByValue(arrHandCards, k); assert(nIndex >= 0);
							arrCards.push_back(arrHandCards[nIndex]);
							return;
						}
					}
				}
			}
		}
	}
}

//-------------------------------------------------------------------------------------------------------------
//Ѱ���Ĵ�2˫
void pSearch_422(Cards& arrCards, const Cards& arrHandCards, const vector<int>& arrCardsTable, const CheckResult& chkPrev)
{
	arrCards.clear();
	pSearch_Jokers(arrCards, arrHandCards, arrCardsTable, chkPrev);
	if (arrCards.size() > 0) return;

	int i,j,k;
	int nHandCards = (int)arrHandCards.size();

	for(i=chkPrev.nValue+1; i<=Card::VALUE_2; i++)
	{
		if (arrCardsTable[i] == 4)
		{
			for(j=3; j<Card::VALUE_2; j++)
			{
				if (arrCardsTable[j] == 2)
				{
					for(k=j+1; k<Card::VALUE_2; k++)
					{
						if (arrCardsTable[k] == 2)
						{
							int nIndex = pFindByValue(arrHandCards, i); assert(nIndex >= 0);
							arrCards.push_back(arrHandCards[nIndex]);
							arrCards.push_back(arrHandCards[nIndex+1]);
							arrCards.push_back(arrHandCards[nIndex+2]);
							arrCards.push_back(arrHandCards[nIndex+3]);

							nIndex = pFindByValue(arrHandCards, j); assert(nIndex >= 0);
							arrCards.push_back(arrHandCards[nIndex]);
							arrCards.push_back(arrHandCards[nIndex+1]);

							nIndex = pFindByValue(arrHandCards, k); assert(nIndex >= 0);
							arrCards.push_back(arrHandCards[nIndex]);
							arrCards.push_back(arrHandCards[nIndex+1]);
							return;
						}
					}
				}
			}
		}
	}
}


//-------------------------------------------------------------------------------------------------------------
//Ѱ����˳������
void pSearch_3311(Cards& arrCards, const Cards& arrHandCards, const vector<int>& arrCardsTable, const CheckResult& chkPrev)
{
	arrCards.clear();
	pSearch_Jokers(arrCards, arrHandCards, arrCardsTable, chkPrev);
	if (arrCards.size() > 0) return;

	//����;
	int i,j,k;

	for(i=chkPrev.nValue+1; i<Card::VALUE_2; i++)   //2һ�㲻�ܳ���˳����;
	{
		if (arrCardsTable[i] == 3 && arrCardsTable[i+1] == 3)
		{
			for(j=3; j<Card::VALUE_2; j++)
			{
				if (arrCardsTable[j] == 1)
				{
					//ɨ��ڶ��ŵ���;
					for(k=j+1; k<Card::VALUE_2; k++)
					{
						if (arrCardsTable[k] == 1)
						{
							int nIndex = pFindByValue(arrHandCards, i); assert(nIndex >= 0);
							arrCards.push_back(arrHandCards[nIndex]);
							arrCards.push_back(arrHandCards[nIndex+1]);
							arrCards.push_back(arrHandCards[nIndex+2]);
							arrCards.push_back(arrHandCards[nIndex+3]);
							arrCards.push_back(arrHandCards[nIndex+4]);
							arrCards.push_back(arrHandCards[nIndex+5]);

							nIndex = pFindByValue(arrHandCards, j); assert(nIndex >= 0);
							arrCards.push_back(arrHandCards[nIndex]);

							nIndex = pFindByValue(arrHandCards, k); assert(nIndex >= 0);
							arrCards.push_back(arrHandCards[nIndex]);
							return;
						}
					}
				}
			}
		}
	}
}

//-------------------------------------------------------------------------------------------------------------
//Ѱ����˳������
void pSearch_3322(Cards& arrCards, const Cards& arrHandCards, const vector<int>& arrCardsTable, const CheckResult& chkPrev)
{
	arrCards.clear();
	pSearch_Jokers(arrCards, arrHandCards, arrCardsTable, chkPrev);
	if (arrCards.size() > 0) return;

	//����;
	int i,j,k;

	for(i=chkPrev.nValue+1; i<Card::VALUE_2; i++)   //2һ�㲻�ܳ���˳����;
	{
		if (arrCardsTable[i] == 3 && arrCardsTable[i+1] == 3)
		{
			for(j=3; j<Card::VALUE_2; j++)
			{
				if (arrCardsTable[j] == 2)
				{
					//ɨ��ڶ��ŵ���;
					for(k=j+1; k<Card::VALUE_2; k++)
					{
						if (arrCardsTable[k] == 2)
						{
							int nIndex = pFindByValue(arrHandCards, i); assert(nIndex >= 0);
							arrCards.push_back(arrHandCards[nIndex]);
							arrCards.push_back(arrHandCards[nIndex+1]);
							arrCards.push_back(arrHandCards[nIndex+2]);
							arrCards.push_back(arrHandCards[nIndex+3]);
							arrCards.push_back(arrHandCards[nIndex+4]);
							arrCards.push_back(arrHandCards[nIndex+5]);

							nIndex = pFindByValue(arrHandCards, j); assert(nIndex >= 0);
							arrCards.push_back(arrHandCards[nIndex]);
							arrCards.push_back(arrHandCards[nIndex+1]);

							nIndex = pFindByValue(arrHandCards, k); assert(nIndex >= 0);
							arrCards.push_back(arrHandCards[nIndex]);
							arrCards.push_back(arrHandCards[nIndex+1]);
							return;
						}
					}
				}
			}
		}
	}
}

//-------------------------------------------------------------------------------------------------------------
//Ѱ��222
void pSearch_222(Cards& arrCards, const Cards& arrHandCards, const vector<int>& arrCardsTable, const CheckResult& chkPrev)
{
	arrCards.clear();
	pSearch_Jokers(arrCards, arrHandCards, arrCardsTable, chkPrev);
	if (arrCards.size() > 0) return;

	int i;

	if (chkPrev.nValue >= Card::VALUE_Q )
	{
		return;
	}

	for(i=chkPrev.nValue+1; i<=Card::VALUE_Q; i++)
	{
		if (arrCardsTable[i] == 2 && arrCardsTable[i+1] == 2 && arrCardsTable[i+2] == 2)
		{
			int nIndex = pFindByValue(arrHandCards, i); assert(nIndex >= 0);
			arrCards.push_back(arrHandCards[nIndex]);
			arrCards.push_back(arrHandCards[nIndex+1]);
			arrCards.push_back(arrHandCards[nIndex+2]);
			arrCards.push_back(arrHandCards[nIndex+3]);
			arrCards.push_back(arrHandCards[nIndex+4]);
			arrCards.push_back(arrHandCards[nIndex+5]);
			return;
		}
	}
}

//-------------------------------------------------------------------------------------------------------------
void pSearch_2222(Cards& arrCards, const Cards& arrHandCards, const vector<int>& arrCardsTable, const CheckResult& chkPrev)
{
	arrCards.clear();
	pSearch_Jokers(arrCards, arrHandCards, arrCardsTable, chkPrev);
	if (arrCards.size() > 0) return;

	int i;

	if (chkPrev.nValue >= Card::VALUE_J )
	{
		return;
	}

	for(i=chkPrev.nValue+1; i<=Card::VALUE_J; i++)
	{
		if (arrCardsTable[i] == 2 && arrCardsTable[i+1] == 2 && arrCardsTable[i+2] == 2 && arrCardsTable[i+3] == 2)
		{
			int nIndex = pFindByValue(arrHandCards, i); assert(nIndex >= 0);
			arrCards.push_back(arrHandCards[nIndex]);
			arrCards.push_back(arrHandCards[nIndex+1]);
			arrCards.push_back(arrHandCards[nIndex+2]);
			arrCards.push_back(arrHandCards[nIndex+3]);
			arrCards.push_back(arrHandCards[nIndex+4]);
			arrCards.push_back(arrHandCards[nIndex+5]);
			arrCards.push_back(arrHandCards[nIndex+6]);
			arrCards.push_back(arrHandCards[nIndex+7]);
			return;
		}
	}
}

//-------------------------------------------------------------------------------------------------------------
//Ѱ��5��˳��;
void pSearch_5_5(Cards& arrCards, const Cards& arrHandCards, const vector<int>& arrCardsTable, const CheckResult& chkPrev)
{
	arrCards.clear();
	pSearch_Jokers(arrCards, arrHandCards, arrCardsTable, chkPrev);
	if (arrCards.size() > 0) return;

	int i,j;

	//�������Ƶĸ���;
	int counter_1;
	int counter_2;
	int counter_3;

	//�����10,j,q,k,A,���Ƕ������Ƶĸ���,���̫�پͿ���Ѱ��ը��;

	if (chkPrev.nValue >= 10 ) 
	{
		return;
	}
	
	for (i=chkPrev.nValue+1; i<=10; i++)
	{
		//��һ��ɨ��,���Ƿ���ȫ���ĵ�˳;

		counter_1 = 0;
		for(j=i; j<i+5; j++)
		{
			if (arrCardsTable[j] != 1)
			{
				break;
			}
			counter_1++;
		}

		//�����������Ƿ����5;
		if (counter_1 == 5)
		{
			int nIndex = pFindByValue(arrHandCards, i); assert(nIndex >= 0);
			arrCards.push_back(arrHandCards[nIndex]);
			arrCards.push_back(arrHandCards[nIndex+1]);
			arrCards.push_back(arrHandCards[nIndex+2]);
			arrCards.push_back(arrHandCards[nIndex+3]);
			arrCards.push_back(arrHandCards[nIndex+4]);
			return;
		}

		//�ڶ���ɨ��;
		counter_1 = 0;
		counter_2 = 0;
		counter_3 = 0;

		for(j=i; j<i+5; j++)
		{
			if (arrCardsTable[j] == 1)
			{
				counter_1++;
			}
			else if (arrCardsTable[j] == 2)
			{
				counter_2++;
			}
			else if (arrCardsTable[j] == 3)
			{
				counter_3++;
			}
			else
			{
				break;//���û���ƻ���������(ը��),�򲻿���;
			}
		}

		//�����Ƿ����5;
		if (counter_1 == 4 && counter_2 + counter_3 == 1)
		{
			//���ƴ���;
			int nIndex = pFindByValue(arrHandCards, i); assert(nIndex >= 0);
			arrCards.push_back(arrHandCards[nIndex]);
			while(arrCards.size() < 5)
			{
				nIndex ++;
				while (arrHandCards[nIndex].nValue == arrHandCards[nIndex-1].nValue) 
					nIndex ++;
				arrCards.push_back(arrHandCards[nIndex]);
			}
			return;
		}
	}
}


//-------------------------------------------------------------------------------------------------------------
//Ѱ��6��˳��;
void pSearch_6_6(Cards& arrCards, const Cards& arrHandCards, const vector<int>& arrCardsTable, const CheckResult& chkPrev)
{
	arrCards.clear();
	pSearch_Jokers(arrCards, arrHandCards, arrCardsTable, chkPrev);
	if (arrCards.size() > 0) return;

	int i,j;

	//�������Ƶĸ���;
	int counter_1;
	int counter_2;
	int counter_3;

	//�����9,10,j,q,k,A,���Ƕ������Ƶĸ���,���̫�پͿ���Ѱ��ը��;

	if (chkPrev.nValue >= 9 ) 
	{
		return;
	}

	for (i=chkPrev.nValue+1; i<=9; i++)
	{
		//��һ��ɨ��,���Ƿ���ȫ���ĵ�˳;

		counter_1 = 0;
		for(j=i; j<i+6; j++)
		{
			if (arrCardsTable[j] != 1)
			{
				break;
			}
			counter_1++;
		}

		//�����������Ƿ����6
		if (counter_1 == 6)
		{
			int nIndex = pFindByValue(arrHandCards, i); assert(nIndex >= 0);
			arrCards.push_back(arrHandCards[nIndex]);
			arrCards.push_back(arrHandCards[nIndex+1]);
			arrCards.push_back(arrHandCards[nIndex+2]);
			arrCards.push_back(arrHandCards[nIndex+3]);
			arrCards.push_back(arrHandCards[nIndex+4]);
			arrCards.push_back(arrHandCards[nIndex+5]);
			return;
		}

		//�ڶ���ɨ��;
		counter_1 = 0;
		counter_2 = 0;
		counter_3 = 0;

		for(j=i; j<i+6; j++)
		{
			if (arrCardsTable[j] == 1)
			{
				counter_1++;
			}
			else if (arrCardsTable[j] == 2)
			{
				counter_2++;
			}
			else if (arrCardsTable[j] == 3)
			{
				counter_3++;
			}
			else
			{
				break;//���û���ƻ���������(ը��),�򲻿���;
			}
		}

		//�����Ƿ����6;
		if ((counter_1 == 5 && counter_2 + counter_3 == 1) ||
			(counter_1 == 4 && counter_2 + counter_3 == 2) )
		{
			//���ƴ���;
			int nIndex = pFindByValue(arrHandCards, i); assert(nIndex >= 0);
			arrCards.push_back(arrHandCards[nIndex]);
			while(arrCards.size() < 6)
			{
				nIndex ++;
				while (arrHandCards[nIndex].nValue == arrHandCards[nIndex-1].nValue) 
					nIndex ++;
				arrCards.push_back(arrHandCards[nIndex]);
			}
			return;
		}
	}
}


//-------------------------------------------------------------------------------------------------------------
//Ѱ��7~9��˳��;
void pSearch_7_9(Cards& arrCards, const Cards& arrHandCards, const vector<int>& arrCardsTable, const CheckResult& chkPrev, int nLong)
{
	arrCards.clear();
	pSearch_Jokers(arrCards, arrHandCards, arrCardsTable, chkPrev);
	if (arrCards.size() > 0) return;

	int i,j;

	//�������Ƶĸ���;
	int counter_1;
	int counter_2;
	int counter_3;

	if (chkPrev.nValue >= Card::VALUE_2 - nLong) 
	{
		return;
	}

	for (i=chkPrev.nValue+1; i<=Card::VALUE_2-nLong; i++)
	{
		//��һ��ɨ��,���Ƿ���ȫ���ĵ�˳;

		counter_1 = 0;
		for(j=i; j<i+nLong; j++)
		{
			if (arrCardsTable[j] != 1)
			{
				break;
			}
			counter_1++;
		}

		//�����������Ƿ����nLong
		if (counter_1 == nLong)
		{
			int nIndex = pFindByValue(arrHandCards, i); assert(nIndex >= 0);
			while ((int)arrCards.size() < nLong)
			{
				arrCards.push_back(arrHandCards[nIndex]);
				nIndex ++;
			}
			return;
		}

		//�ڶ���ɨ��;
		counter_1 = 0;
		counter_2 = 0;
		counter_3 = 0;

		for(j=i; j<i+nLong; j++)
		{
			if (arrCardsTable[j] == 1)
			{
				counter_1++;
			}
			else if (arrCardsTable[j] == 2)
			{
				counter_2++;
			}
			else if (arrCardsTable[j] == 3)
			{
				counter_3++;
			}
			else
			{
				break;//���û���ƻ���������(ը��),�򲻿���;
			}
		}

		//�����Ƿ����nLong;
		if ((counter_1 == nLong-1 && counter_2 + counter_3 == 1) ||
			(counter_1 == nLong-2 && counter_2 + counter_3 == 2) ||
			(counter_1 == nLong-3 && counter_2 + counter_3 == 3) )
		{
			//���ƴ���;
			int nIndex = pFindByValue(arrHandCards, i); assert(nIndex >= 0);
			arrCards.push_back(arrHandCards[nIndex]);
			while((int)arrCards.size() < nLong)
			{
				nIndex ++;
				while (arrHandCards[nIndex].nValue == arrHandCards[nIndex-1].nValue) 
					nIndex ++;
				arrCards.push_back(arrHandCards[nIndex]);
			}
			return;
		}
	}
}

//-------------------------------------------------------------------------------------------------------------
//Ѱ��10~12��˳��
void pSearch_10_12(Cards& arrCards, const Cards& arrHandCards, const vector<int>& arrCardsTable, const CheckResult& chkPrev, int nLong)
{
	arrCards.clear();
	pSearch_Jokers(arrCards, arrHandCards, arrCardsTable, chkPrev);
	if (arrCards.size() > 0) return;

	int i,j;

	//�������Ƶĸ���;
	int counter_1;
	int counter_2;
	int counter_3;

	if (chkPrev.nValue >= Card::VALUE_2 - nLong) 
	{
		return;
	}

	for (i=chkPrev.nValue+1; i<=Card::VALUE_2-nLong; i++)
	{
		//��һ��ɨ��,���Ƿ���ȫ���ĵ�˳;

		counter_1 = 0;
		for(j=i; j<i+nLong; j++)
		{
			if (arrCardsTable[j] != 1)
			{
				break;
			}
			counter_1++;
		}

		//�����������Ƿ����nLong
		if (counter_1 == nLong)
		{
			int nIndex = pFindByValue(arrHandCards, i); assert(nIndex >= 0);
			while ((int)arrCards.size() < nLong)
			{
				arrCards.push_back(arrHandCards[nIndex]);
				nIndex ++;
			}
			return;
		}

		//�ڶ���ɨ��;
		counter_1 = 0;
		counter_2 = 0;
		counter_3 = 0;

		for(j=i; j<i+nLong; j++)
		{
			if (arrCardsTable[j] == 1)
			{
				counter_1++;
			}
			else if (arrCardsTable[j] == 2)
			{
				counter_2++;
			}
			else if (arrCardsTable[j] == 3)
			{
				counter_3++;
			}
			else
			{
				break;//���û���ƻ���������(ը��),�򲻿���;
			}
		}

		//�����Ƿ����nLong;
		if ((counter_1 == nLong-1 && counter_2 + counter_3 == 1) ||
			(counter_1 == nLong-2 && counter_2 + counter_3 == 2) ||
			(counter_1 == nLong-3 && counter_2 + counter_3 == 3) ||
			(counter_1 == nLong-4 && counter_2 + counter_3 == 4))
		{
			//���ƴ���;
			int nIndex = pFindByValue(arrHandCards, i); assert(nIndex >= 0);
			arrCards.push_back(arrHandCards[nIndex]);
			while((int)arrCards.size() < nLong)
			{
				nIndex ++;
				while (arrHandCards[nIndex].nValue == arrHandCards[nIndex-1].nValue) 
					nIndex ++;
				arrCards.push_back(arrHandCards[nIndex]);
			}
			return;
		}
	}
}

//-------------------------------------------------------------------------------------------------------------

void CGameType_Lord3::ComputerPrepareCards(Cards& arrCards, const Cards& arrHandCards, const Cards& arrPrevCards)
{
	//������
	CheckResult chkPrev = pCheckCards(arrPrevCards);

	//arrCardsTable ��¼ÿ��ֵ�����м���
	vector<int> arrCardsTable(Card::VALUE_JOKER_B + 1);
	unsigned int i=0;
	for(i=0; i< arrCardsTable.size(); i++)
	{
		arrCardsTable[i] = 0;
	}
	for(i=0; i< arrHandCards.size(); i++)
	{
		arrCardsTable[arrHandCards[i].nValue]++;
	}

	bool bMust = (arrPrevCards.size() == 0);
	if (bMust)
	{
		//����

		//n���Ƶĸ���(n:0~4)
		vector<int> arrCounter(5);
		//n���Ƶ���С��ֵ(n:0~4)
		vector<int> arrMinValue(5);

		pGetCardsTableInfo(arrCounter, arrMinValue, arrCardsTable);

		if (arrCounter[3] >= 2 && arrCounter[1] >= arrCounter[2]) //3���Ƹ�������2, ��2���Ƹ���С�ڵ��Ƹ���
		{
			//Ѱ��3311;�ҵ�����;
			pSearch_3311(arrCards, arrHandCards, arrCardsTable, chkPrev);
			if (arrCards.size() > 0) return;
		}

		if (arrCounter[3] >= 2 && arrCounter[2] >= arrCounter[1])
		{
			//Ѱ��3322;�ҵ�����;
			pSearch_3322(arrCards, arrHandCards, arrCardsTable, chkPrev);
			if (arrCards.size() > 0) return;
		}

		if (arrCounter[1] >= 3)
		{
			pSearch_10_12(arrCards, arrHandCards, arrCardsTable, chkPrev, 12);
			if (arrCards.size() > 0) return;

			pSearch_10_12(arrCards, arrHandCards, arrCardsTable, chkPrev, 11);
			if (arrCards.size() > 0) return;

			pSearch_10_12(arrCards, arrHandCards, arrCardsTable, chkPrev, 10);
			if (arrCards.size() > 0) return;

			pSearch_7_9(arrCards, arrHandCards, arrCardsTable, chkPrev, 9);
			if (arrCards.size() > 0) return;

			pSearch_7_9(arrCards, arrHandCards, arrCardsTable, chkPrev, 8);
			if (arrCards.size() > 0) return;

			pSearch_7_9(arrCards, arrHandCards, arrCardsTable, chkPrev, 7);
			if (arrCards.size() > 0) return;

			pSearch_6_6(arrCards, arrHandCards, arrCardsTable, chkPrev);
			if (arrCards.size() > 0) return;

			pSearch_5_5(arrCards, arrHandCards, arrCardsTable, chkPrev);
			if (arrCards.size() > 0) return;
		}

		if (arrCounter[2] >= 3)
		{
			pSearch_2222(arrCards, arrHandCards, arrCardsTable, chkPrev);
			if (arrCards.size() > 0) return;

			pSearch_222(arrCards, arrHandCards, arrCardsTable, chkPrev);
			if (arrCards.size() > 0) return;
		}

		if (arrCounter[4] > 0 && arrCounter[1] >= 2 && arrCounter[2] >= 2)
		{
			if (arrMinValue[1] < arrMinValue[2])
			{
				pSearch_411(arrCards, arrHandCards, arrCardsTable, chkPrev);
				if (arrCards.size() > 0) return;
			}
			else
			{
				pSearch_422(arrCards, arrHandCards, arrCardsTable, chkPrev);
				if (arrCards.size() > 0) return;
			}
		}
		else if(arrCounter[4] > 0 && arrCounter[1] >= 2)
		{
			pSearch_411(arrCards, arrHandCards, arrCardsTable, chkPrev);
			if (arrCards.size() > 0) return;
		}
		else if (arrCounter[4] > 0 && arrCounter[2] >= 2)
		{
			pSearch_422(arrCards, arrHandCards, arrCardsTable, chkPrev);
			if (arrCards.size() > 0) return;
		}

		if (arrCounter[3] > 0 && (arrMinValue[1] < arrMinValue[2]) && arrCounter[1] > 0)
		{
			//Ѱ��31;
			pSearch_31(arrCards, arrHandCards, arrCardsTable, chkPrev);
			if (arrCards.size() > 0) return;
		}
		else if (arrCounter[3] > 0 && arrMinValue[2] < arrMinValue[1] && arrCounter[2] > 0 )
		{
			//Ѱ��32;
			pSearch_32(arrCards, arrHandCards, arrCardsTable, chkPrev);
			if (arrCards.size() > 0) return;
		}

		if (arrMinValue[1] < arrMinValue[2])
		{
			//Ѱ��1;
			pSearch_1(arrCards, arrHandCards, arrCardsTable, chkPrev);
			if (arrCards.size() > 0) return;
		}
		else
		{
			//Ѱ��2;
			pSearch_2(arrCards, arrHandCards, arrCardsTable, chkPrev);
			if (arrCards.size() > 0) return;
		}

		//Ѱ��1;
		pSearch_1(arrCards, arrHandCards, arrCardsTable, chkPrev);
		if (arrCards.size() > 0) return;
	}
	else
	{
		//����

		switch (chkPrev.nCardType)
		{
		case CheckResult::T_1_1:
			pSearch_1(arrCards, arrHandCards, arrCardsTable, chkPrev); return;

		case CheckResult::T_2_2:
			pSearch_2(arrCards, arrHandCards, arrCardsTable, chkPrev); return;

		case CheckResult::T_3_3:
			pSearch_3(arrCards, arrHandCards, arrCardsTable, chkPrev); return;

		case CheckResult::T_4_BOOM:
			pSearch_4(arrCards, arrHandCards, arrCardsTable, chkPrev); return;

		case CheckResult::T_4_31:
			pSearch_31(arrCards, arrHandCards, arrCardsTable, chkPrev); return;

		case CheckResult::T_5_32:
			pSearch_32(arrCards, arrHandCards, arrCardsTable, chkPrev); return;

		case CheckResult::T_6_411:
			pSearch_411(arrCards, arrHandCards, arrCardsTable, chkPrev); return;

		case CheckResult::T_8_422:
			pSearch_422(arrCards, arrHandCards, arrCardsTable, chkPrev); return;

		case CheckResult::T_8_3311:
			pSearch_3311(arrCards, arrHandCards, arrCardsTable, chkPrev); return;

		case CheckResult::T_10_3322:
			pSearch_3322(arrCards, arrHandCards, arrCardsTable, chkPrev); return;

		case CheckResult::T_5_5:
			pSearch_5_5(arrCards, arrHandCards, arrCardsTable, chkPrev); return;

		case CheckResult::T_6_6:
			pSearch_6_6(arrCards, arrHandCards, arrCardsTable, chkPrev); return;

		case CheckResult::T_7_7:
			pSearch_7_9(arrCards, arrHandCards, arrCardsTable, chkPrev, 7); return;

		case CheckResult::T_8_8:
			pSearch_7_9(arrCards, arrHandCards, arrCardsTable, chkPrev, 8); return;

		case CheckResult::T_9_9:
			pSearch_7_9(arrCards, arrHandCards, arrCardsTable, chkPrev, 9); return;

		case CheckResult::T_10_A:
			pSearch_10_12(arrCards, arrHandCards, arrCardsTable, chkPrev, 10); return;

		case CheckResult::T_11_B:
			pSearch_10_12(arrCards, arrHandCards, arrCardsTable, chkPrev, 11); return;

		case CheckResult::T_12_C:
			pSearch_10_12(arrCards, arrHandCards, arrCardsTable, chkPrev, 12); return;

		case CheckResult::T_6_222:
			pSearch_222(arrCards, arrHandCards, arrCardsTable, chkPrev); return;

		case CheckResult::T_8_2222:
			pSearch_2222(arrCards, arrHandCards, arrCardsTable, chkPrev); return;
		}
	}
}

//-------------------------------------------------------------------------------------------------------------

//������ֵ;
int CGameType_Lord3::AccountCardsValue(const Cards& arrHandCards)
{
	//arrCardsTable ��¼ÿ��ֵ�����м���
	vector<int> arrCardsTable(Card::VALUE_JOKER_B + 1);
	for(unsigned int i=0; i< arrCardsTable.size(); i++)
	{
		arrCardsTable[i] = 0;
	}
	for(i=0; i< arrHandCards.size(); i++)
	{
		arrCardsTable[arrHandCards[i].nValue]++;
	}

	//����ֵ
	int nCardsValue = 0;

	if (arrCardsTable[Card::VALUE_JOKER_B] == 1 && arrCardsTable[Card::VALUE_JOKER_S] == 1) nCardsValue += 200;
	else if (arrCardsTable[Card::VALUE_JOKER_B] == 1) nCardsValue += 80;
	else if (arrCardsTable[Card::VALUE_JOKER_S] == 1) nCardsValue += 40;

	switch (arrCardsTable[Card::VALUE_2])
	{
	case 1:
		nCardsValue += 20;
		break;
	case 2:
		nCardsValue += 40;
		break;
	case 3:
		nCardsValue += 60;
		break;
	case 4:
		nCardsValue += 200;
		break;
	}

	switch (arrCardsTable[Card::VALUE_A])
	{
	case 1:
		nCardsValue += 5;
		break;
	case 2:
		nCardsValue += 10;
		break;
	case 3:
		nCardsValue += 30;
		break;
	case 4:
		nCardsValue += 150;
		break;
	}

	for( int i=3;i<14;i++)
	{
		if( arrCardsTable[i] == 4 )
		{
			nCardsValue += 100 + i*5;
		}
	}
	return nCardsValue;
}
