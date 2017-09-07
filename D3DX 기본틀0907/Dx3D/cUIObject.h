#pragma once
class cUIObject : public cObject
{
protected:
	vector<cUIObject*>		m_vecChild;
	D3DXMATRIXA16			m_matWorld;

	SYNTHESIZE(D3DXVECTOR3, m_vLocalPos, LocalPos);
	SYNTHESIZE(cUIObject*, m_pParent, Parent);
	SYNTHESIZE(ST_SIZE, m_stSize, Size);
	SYNTHESIZE(bool, m_isDebugRender, DebugRender);
	SYNTHESIZE(int, m_nTag, Tag);

public:
	cUIObject();
	virtual ~cUIObject();

	virtual void Update();
	virtual void UpdateWorldTM();
	virtual void UpdateChildren();
	virtual void Render(LPD3DXSPRITE pSprite);
	virtual void AddChild(cUIObject* pChild);
	virtual cUIObject* GetchildByTag(int nTag);
};

