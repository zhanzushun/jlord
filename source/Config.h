#pragma once

#define HEAD_IMAGE_COUNT 4	//�ܹ�4��ͷ��

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//һ����
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

struct Card
{
	static const int HEITAO = 0;		//����
	static const int HONGTAO = 1;		//����
	static const int CAOHUA = 2;		//�ݻ�
	static const int FANGKUAI = 3;		//����
	static const int JOKER_S = 0;		//С��
	static const int JOKER_B = 1;		//���
	static const int VALUE_J = 11;
	static const int VALUE_Q = 12;
	static const int VALUE_K = 13;
	static const int VALUE_A = 14;
	static const int VALUE_2 = 15;
	static const int VALUE_JOKER_S = 16;
	static const int VALUE_JOKER_B = 17;

	int nColor;						//�ƵĻ�ɫ
	int nValue;						//�ƵĴ�С. 3~17

	tstring GetSpriteName();
	bool operator < (const Card& c2) const;
	Card():nColor(-1),nValue(-1){}
	bool operator == (const Card& card2) {return nColor == card2.nColor && nValue == card2.nValue;}
};

typedef vector<Card> Cards;

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//�������� (��������. ����ͻ���. ���������)
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

class INetWork
{
public:
	virtual ~INetWork() {}
	virtual bool GetPlayers(tstring& strDlgNetInfo, const tstring& strIP) = 0; //ȡ�������Ϣ
	virtual void GetCards() = 0;					//ȡ��������
	virtual void GetRandomBanker() = 0;				//�������ׯ��
	virtual void AgreeBanker(bool) = 0;				//������������:�Ƿ�ͬ�⵱ׯ��
	virtual bool GetAgreeBanker(int nTemp) = 0;		//�ӷ���������:�����Ƿ�ͬ�⵱ׯ��
	virtual void SendPrepareCards() = 0;			//������������:���˴�����
	virtual void GetPrepareCards() = 0;				//�ӷ��������մ�����
};


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//״̬�ӿ� (��Ϸ�д��ڲ�ͬ�Ķ���״̬)
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

class IState
{
public:
	virtual ~IState() {}
	virtual void RunOneFrame() = 0;
	virtual void SetActive(){};
	virtual void OnMouseClick(){};
};

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//��Ϸ���� (������,80�ֵ�)
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

class IGameType
{
public:
	enum EGameType
	{
		T_LORD3,		//������(3��)
		T_JOKER_LOAD,	//���·��
		T_80,			//��ʮ��
	};

	//����������
	virtual ~IGameType() {}

	//ȡ��һ����Ϸ״̬
	virtual IState* GetNextState(IState*) = 0;

	//�����ƵĴ�С�Ƚ�(��������)
	virtual bool CardLess(const Card& card1, const Card& card2) = 0;

	//��������
	static IGameType* CreateGameType(EGameType type);

	//ȷ���Ƿ���Գ���
	virtual bool CheckPrepareCards(const Cards& arrPrevCards, const Cards& arrCards) = 0;

	//���Գ���
	virtual void ComputerPrepareCards(Cards& arrCards, const Cards& arrHandCards, const Cards& arrPrevCards) = 0;

	//������ֵ
	virtual int AccountCardsValue(const Cards& arrHandCards) = 0;

	//���ֵ�����
	virtual tstring GetNameText(int nID, const tstring& sOriginal) {return sOriginal;}

	//ȡ�ù���ѡ��ׯ�ҵ��ı�
	static const int Decide_Banker_Random = 0;		//���ׯ��
	static const int Decide_Banker_PreConfirm = 1;	//��ȷ�ϵ�ׯ��
	static const int Decide_Banker_Confirm = 2;		//ȷ�ϵ�ׯ��
	static const int Decide_Banker_Msgbox = 3;		//msgbox
	virtual void GetText_DecideBanker(int i, tstring& sText){};
};

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//�����ļ�
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

static const int SET_CARDS = 54;	//һ������
static const int PREPARE_OFFSET_V = 20;

class GConfig : public SingletonT<GConfig>
{
	friend class SingletonT<GConfig>;
public:
	enum EPosFlag
	{
		HEAD, HANDCARD,	OUTCARD, PLAYINGCARD,			//��Ӧ�ڸ���ҵ�ͷ��/�Ƶ�λ��
		QUIT_BTN, PLAY_BTN,	PLAYCARD_BTN, SKIPCARD_BTN,	//��ťλ��
		SEND_CARD,										//������ʼλ��
		BOTTOM_CARD,									//���Ƶ�λ��
	};

	//��������
	int GetWidth()			{return m_nWidth;}
	int GetHeight()			{return m_nHeight;}
	int GetPlayers()		{return m_nPlayers;}		//��Ҹ���
	int GetCardsSet()		{return m_nCardsSet;}		//������
	int GetBottomCards()	{return m_nBottomCards;}	//������
	int GetMoveCardFrames() {return m_nMoveCardFrames;}	//�ƶ�һ���Ƶ�֡��
	CPoint GetPosition(EPosFlag ePos);					//��ͼλ��
	int GetAllCards()		{return GetCardsSet()*SET_CARDS;}
	int GetPlayerCards()	{return (GetAllCards() - GetBottomCards()) / GetPlayers();}
	CPoint GetPlayerPos(EPosFlag flag, int nPlayerIndex, int nPlayerCardIndex);
	CPoint GConfig::GetPlayerOffset(GConfig::EPosFlag flag, int nPlayerIndex);
	int GetNameFontSize()	{return m_nNameFontSize;}
	DWORD GetNameColor()	{return m_clrName;}
	int GetInfoFontSize()	{return m_nInfoFontSize;}
	DWORD GetInfoColor()	{return m_clrInfo;}
	CPoint GetInfoPos()		{return m_pos_Info;}

	INetWork* GetNetWork()  {return m_pNetWork;}
	IGameType* GetGameType(){return m_pGameType;}

	//����
	void SetNetWork(INetWork* pNetWork) {m_pNetWork = pNetWork; }

public:
	~GConfig() 
	{
		delete m_pNetWork;
		delete m_pGameType;
	}
	void Load(const tstring& sFile);

private:
	GConfig() 
	{
		m_bLoaded = false;
		m_pGameType = NULL;
		m_pNetWork = NULL;
	}

	bool m_bLoaded;

	struct CardPosInfo
	{
		CPoint m_pos;
		CPoint m_offset;
	};

	int m_nWidth;										//���
	int m_nHeight;										//�߶�
	int m_nPlayers;										//��Ҹ���
	int m_nCardsSet;									//������
	int m_nBottomCards;									//��������
	int m_nMoveCardFrames;								//���ƵĶ������ƶ���֡?
	vector<CPoint> m_pos_Head;							//ͷ��λ��
	vector<CardPosInfo> m_pos_HandCard;					//�����Ƶ�λ��
	vector<CardPosInfo> m_pos_OutCard;					//����Ƶ�λ��
	vector<CardPosInfo> m_pos_PlayingCard;				//���ڴ���Ƶ�λ��
	CPoint m_pos_Button_Quit;							//��ťλ��
	CPoint m_pos_Button_Play;							//��ťλ��
	CPoint m_pos_Button_PlayCard;						//��ťλ��
	CPoint m_pos_Button_SkipCard;						//��ťλ��
	CPoint m_pos_SendCard;								//�����Ƶ�λ��
	CPoint m_pos_BottomCard;							//����λ��
	CPoint m_pos_Info;									//��Ϣ��ʾ��λ��
	COLORREF m_clrInfo;									//��Ϣ��ɫ
	int m_nInfoFontSize;								//�����С
	COLORREF m_clrName;									//������ɫ
	int m_nNameFontSize;								//���������С

	IGameType* m_pGameType;
	INetWork* m_pNetWork;
};
