#include "stdafx.h"
#include "cFontManager.h"


cFontManager::cFontManager()
	:m_dwDrawTextFormat(DT_LEFT | DT_TOP | DT_NOCLIP)
	, m_dwColor(D3DCOLOR_XRGB(0, 0, 0))
{
}


cFontManager::~cFontManager()
{
}

void cFontManager::Init()
{
	D3DXFONT_DESC fd;
	ZeroMemory(&fd, sizeof(D3DXFONT_DESC));

	fd.Height = 20;
	fd.Width = 10;
	fd.Weight = FW_NORMAL;
	fd.Italic = false;
	fd.CharSet = DEFAULT_CHARSET;
	fd.OutputPrecision = OUT_DEFAULT_PRECIS;
	fd.PitchAndFamily = FF_DONTCARE;

	AddFontResource("Font/NanumGothicBold.ttf");
	strcpy_s(fd.FaceName, "NanumGothicBold");

	D3DXCreateFontIndirect(g_pD3DDevice, &fd, &m_mapFont[E_NORMAL]);
}

void cFontManager::Destroy()
{
	RemoveFontResource("NanumGothicBold");

	for each (auto p in m_mapFont)
		SAFE_RELEASE(p.second);

	m_mapFont.clear();
}

void cFontManager::Render(RECT _rc, const char* _str, eFontType _eFontType)
{
	m_mapFont[E_NORMAL]->DrawTextA(NULL, _str, strlen(_str), &_rc, m_dwDrawTextFormat, m_dwColor);
}

void cFontManager::Render(RECT _rc, const char * _str, DWORD _dwColor, eFontType _eFontType)
{
	m_mapFont[E_NORMAL]->DrawTextA(NULL, _str, strlen(_str), &_rc, m_dwDrawTextFormat, _dwColor);
}

LPD3DXFONT cFontManager::GetFont(eFontType e)
{
	if (m_mapFont.find(e) == m_mapFont.end())
	{
		switch (e)
		{
		case E_NORMAL:
			return m_mapFont[E_NORMAL];
			break;
		case E_CHAT:
			break;
		case E_QUEST:
		
			break;
		}
	}

	return m_mapFont[e];
}
