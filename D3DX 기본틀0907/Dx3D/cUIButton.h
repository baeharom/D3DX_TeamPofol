#pragma once
#include "cUIObject.h"

class cUIButton;

class iUIButtonDelegate
{
public:
	virtual void OnClick(cUIButton* pSender) PURE;
};

class cUIButton : public cUIObject
{
private:
	enum eButtonState
	{
		E_NORMAL,
		E_MOUSEOVER,
		E_SELETED,
		E_COUNT
	};
	eButtonState		m_eButtonState;
	LPDIRECT3DTEXTURE9	m_aTexture[E_COUNT];

	SYNTHESIZE(iUIButtonDelegate*, m_pDelegate, Delegate);

public:
	cUIButton();
	virtual ~cUIButton();

	virtual void SetTexture(string sNormal, string sMouseOver, string sSelected);

	virtual void Update() override;
	virtual void Render(LPD3DXSPRITE pSprite) override;
};

