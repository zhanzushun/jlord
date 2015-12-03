#pragma once

#define FRAME_TIME 20	//ÿ֡ʱ�� : 20����

namespace Sprites
{

////////////////////////////////////////////////////////////////////////////////////////////////////////
//ͼ��
struct SurfaceData
{
	tstring m_sName;			//ͼ������
	tstring m_sFile;			//ͼ���ļ�
	vector<CRect> m_arrImages;	//����ͼ���е�ͼ��(λ��)
	Surface* m_pSurface;		//ͼ������
	bool m_bKeyColor;			//�Ƿ���͸��ɫ
	Color m_keyColor;			//͸��ɫ�� RGB ֵ.
	void Load(const CXmlNode& ndSurface);
	SurfaceData():m_pSurface(NULL){}
	virtual ~SurfaceData() {delete m_pSurface;}
	void LoadSurface();
};

////////////////////////////////////////////////////////////////////////////////////////////////////////
//����

struct Action
{
	tstring		m_sName;			//����������
	tstring		m_sSurfName;		//ͼ������� (һ������ֻ����һ��ͼ����)
	CPoint		m_basePoint;		//��׼��
	int			m_nIndexInSurf;		//��һ֡��surface�е�����ֵ
	int			m_nFrameCount;		//֡��
	int			m_nFrameTime;		//��һ֡����ʱ�� (ƽ��)
	Action() : m_basePoint(0,0), m_nFrameCount(1), m_nIndexInSurf(0), m_nFrameTime(1) {}
};

////////////////////////////////////////////////////////////////////////////////////////////////////////
//��������

struct SpriteData
{
	tstring					m_sName;		//��������
	map<tstring, Action>	m_mapActions;	//��������
	tstring					m_sDefAction;	//ȱʡ����
	CPoint					m_ptDefPos;		//ȱʡλ��
	void Load(const CXmlNode& ndSprite);
};

////////////////////////////////////////////////////////////////////////////////////////////////////////
//����

class Sprite
{
public:
	virtual ~Sprite(){}

	virtual void Draw(CRect* prc = 0, unsigned char* pnAlpha = 0);
	virtual void Load(const CXmlNode& ndSprite);
	static Sprite* New(tstring sType);
	SpriteData	m_data;
	void SetPos(CPoint pos) {m_pos = pos;}
	void SetAction(const tstring& sName) 
	{
		if (m_data.m_mapActions.find(sName) != m_data.m_mapActions.end())
		{
			m_sCurrentAction = sName;
		}
	}
	CRect GetPos();

protected:
	CPoint m_pos;				//��ǰλ��
	tstring m_sCurrentAction;	//��ǰ����
	int m_nCurrentFrame;		//��ǰ֡
};

////////////////////////////////////////////////////////////////////////////////////////////////////////
//���� : ��ť(����������ͼ��:out,down,over)
class ButtonNotify
{
public:
	virtual ~ButtonNotify() {}
	virtual void OnButton(const tstring& sName) = 0;
};

class Button : public Sprite
{
public:
	Button() : m_tDownTime(0), m_state(E_STATE_OUT), m_pNotify(0), m_bVisible(true) {}
	virtual ~Button(){}

	enum EState
	{
		E_STATE_OVER,
		E_STATE_OUT,
		E_STATE_DOWN,
		E_STATE_DISABLED,
	};
	virtual void SetState(EState st);
	virtual EState GetState() {return m_state;}
	void SetNotify(ButtonNotify* pNotify) {m_pNotify = pNotify;}
	void SetVisible(bool bNewVal) {m_bVisible = bNewVal;};
	bool GetVisible() {return m_bVisible;}
	virtual void Draw(CRect* prc = 0, unsigned char* pnAlpha = 0)
	{
		if (m_bVisible) Sprite::Draw(prc, pnAlpha);
	}

private:
	ButtonNotify* m_pNotify;
	EState m_state;
	int m_tDownTime;
	static const int DOWN_FRAMES = 6;	//���ְ��µ�ʱ�� (6 * FRAME_TIME ����)
	bool m_bVisible;
};

////////////////////////////////////////////////////////////////////////////////////////////////////////
//���鼯��

class GSprites : public SingletonT<GSprites>
{
	friend class SingletonT<GSprites>;
private:
	GSprites() {m_bLoaded = false;}

public:
	virtual ~GSprites() {Clear();}
	virtual void Clear();
	virtual void Load(const tstring& sFile, bool bClear = false);
	bool IsValid() {return m_bLoaded;}

	Sprite* GetSprite(const tstring& sKey) 
	{
		map<tstring, Sprite*>::const_iterator it = m_mapSprites.find(sKey);
		if (it == m_mapSprites.end()) {return NULL;}
		return (it->second);
	}

	Button* GetButton(const tstring& sKey) 
	{
		map<tstring, Sprite*>::const_iterator it = m_mapSprites.find(sKey);
		if (it == m_mapSprites.end()) {return NULL;}
		return dynamic_cast<Button*>(it->second);
	}

protected:
	map<tstring, Sprite*> m_mapSprites;

private:
	bool m_bLoaded;
};

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//ͼƬ����

class GSurfaces : public SingletonT<GSurfaces>
{
	friend class SingletonT<GSurfaces>;
private:
	GSurfaces() {m_bLoaded = false;}

public:
	virtual ~GSurfaces() {Clear();}
	virtual void Clear();
	virtual void Load(const tstring& sFile, bool bClear = false);

	bool IsValid() {return m_bLoaded;}
	SurfaceData* GetSurface(const tstring& sKey);
protected:
	map<tstring, SurfaceData*> m_mapSurfs;

private:
	bool m_bLoaded;
};

}//end namespace Display
