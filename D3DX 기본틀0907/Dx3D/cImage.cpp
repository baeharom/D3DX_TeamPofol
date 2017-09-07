#include "stdafx.h"
#include "cImage.h"


cImage::cImage()
	: m_fOrgW(0),m_fOrgH(0),m_fCurW(0),m_fCurH(0)
	, m_pTexture(nullptr), 
	m_dwColor(D3DCOLOR_ARGB(255, 255, 255, 255))
{
	D3DXMatrixIdentity(&m_matWorld);
}


cImage::~cImage()
{
	SAFE_RELEASE(m_pTexture);
}

void cImage::Init(string _strKey, float _x, float _y)
{
	D3DXIMAGE_INFO stImageInfo;
	// 텍스쳐 생성
	D3DXCreateTextureFromFileEx(g_pD3DDevice, _strKey.c_str(),
		D3DX_DEFAULT_NONPOW2, D3DX_DEFAULT_NONPOW2, D3DX_DEFAULT, 0,
		D3DFMT_UNKNOWN, D3DPOOL_MANAGED, D3DX_FILTER_NONE, D3DX_DEFAULT,
		0, &stImageInfo, NULL, &m_pTexture);

	m_fCurW=m_fOrgW = stImageInfo.Width;
	m_fCurH=m_fOrgH = stImageInfo.Height;

	m_matWorld._11 = m_fCurW / m_fOrgW;
	m_matWorld._22 = m_fCurH / m_fOrgH;
	m_matWorld._41 = _x;
	m_matWorld._42 = _y;
	SetRect(&m_rcDraw, 0, 0, m_fCurW, m_fCurH);
}

void cImage::Init(string _strKey,float _x,float _y, float _w, float _h)
{
	D3DXIMAGE_INFO stImageInfo;
	// 텍스쳐 생성
	D3DXCreateTextureFromFileEx(g_pD3DDevice, _strKey.c_str(),
		D3DX_DEFAULT_NONPOW2, D3DX_DEFAULT_NONPOW2, D3DX_DEFAULT, 0,
		D3DFMT_UNKNOWN, D3DPOOL_MANAGED, D3DX_FILTER_NONE, D3DX_DEFAULT,
		0, &stImageInfo, NULL, &m_pTexture);

	m_fCurW = _w;
	m_fCurH = _h;
	m_fOrgW = stImageInfo.Width;
	m_fOrgH = stImageInfo.Height;

	m_matWorld._11 = m_fCurW / m_fOrgW;
	m_matWorld._22 = m_fCurH / m_fOrgH;
	m_matWorld._41 = _x;
	m_matWorld._42 = _y;

	SetRect(&m_rcDraw, 0, 0, m_fCurW, m_fCurH);
}

void cImage::Render(LPD3DXSPRITE pSprite, int _a)
{
	m_dwColor = D3DCOLOR_ARGB(_a, 255, 255, 255);

	pSprite->Begin(D3DXSPRITE_ALPHABLEND | D3DXSPRITE_SORT_TEXTURE);

	pSprite->SetTransform(&m_matWorld);
	pSprite->Draw(m_pTexture, &m_rcDraw, &D3DXVECTOR3(0, 0, 0), &D3DXVECTOR3(0, 0, 0), m_dwColor);

	pSprite->End();
}

void cImage::RenderAndPos(LPD3DXSPRITE pSprite,float _x, float _y, int _a)
{
	m_matWorld._41 =_x;
	m_matWorld._42 =_y;
	m_dwColor = D3DCOLOR_ARGB(_a, 255, 255, 255);

	pSprite->Begin(D3DXSPRITE_ALPHABLEND | D3DXSPRITE_SORT_TEXTURE);
	
	pSprite->SetTransform(&m_matWorld);
	pSprite->Draw(m_pTexture, &m_rcDraw, &D3DXVECTOR3(0, 0, 0), &D3DXVECTOR3(0, 0, 0), m_dwColor);

	pSprite->End();
}

void cImage::RenderAndPosCenter(LPD3DXSPRITE pSprite, float _x, float _y, int _a)
{
	m_matWorld._41 = _x-(m_fCurW/2);
	m_matWorld._42 = _y- (m_fCurH / 2);
	m_dwColor = D3DCOLOR_ARGB(_a, 255, 255, 255);

	pSprite->Begin(D3DXSPRITE_ALPHABLEND | D3DXSPRITE_SORT_TEXTURE);

	pSprite->SetTransform(&m_matWorld);
	pSprite->Draw(m_pTexture, &m_rcDraw, &D3DXVECTOR3(0, 0, 0), &D3DXVECTOR3(0, 0, 0), m_dwColor);

	pSprite->End();
}

void cImage::RenderAndSize(LPD3DXSPRITE pSprite,float _w, float _h, int _a)
{
	m_fCurW = _w;
	m_fCurH = _h;
	m_matWorld._11 = m_fCurW / m_fOrgW;
	m_matWorld._22 = m_fCurH / m_fOrgH;
	m_dwColor = D3DCOLOR_ARGB(_a, 255, 255, 255);

	SetRect(&m_rcDraw, 0, 0, (int)(m_fOrgW*m_matWorld._11)+1, (int)(m_fOrgH*m_matWorld._22)+1);
	pSprite->Begin(D3DXSPRITE_ALPHABLEND | D3DXSPRITE_SORT_TEXTURE);

	pSprite->SetTransform(&m_matWorld);
	pSprite->Draw(m_pTexture, &m_rcDraw, &D3DXVECTOR3(0, 0, 0), &D3DXVECTOR3(0, 0, 0), m_dwColor);

	pSprite->End();
}

void cImage::Render(LPD3DXSPRITE pSprite,float _x, float _y, float _w, float _h, int _a)
{
	m_fCurW = _w;
	m_fCurH = _h;
	m_matWorld._11 = m_fCurW / m_fOrgW;
	m_matWorld._22 = m_fCurH / m_fOrgH;
	m_matWorld._41 = _x;
	m_matWorld._42 = _y;
	m_dwColor = D3DCOLOR_ARGB(_a, 255, 255, 255);

	SetRect(&m_rcDraw, 0, 0, (int)(m_fOrgW*m_matWorld._11) + 1, (int)(m_fOrgH*m_matWorld._22) + 1);
	pSprite->Begin(D3DXSPRITE_ALPHABLEND | D3DXSPRITE_SORT_TEXTURE);

	pSprite->SetTransform(&m_matWorld);
	pSprite->Draw(m_pTexture, &m_rcDraw, &D3DXVECTOR3(0, 0, 0), &D3DXVECTOR3(0, 0, 0), m_dwColor);

	pSprite->End();
}
