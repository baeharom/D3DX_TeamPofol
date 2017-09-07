#pragma once

#define g_pFontManager cFontManager::GetInstance()

class cFontManager
{
	SINGLETON(cFontManager);

public:
	enum eFontType
	{
		E_NORMAL,
		E_CHAT,
		E_QUEST
	};

private:
	SYNTHESIZE(DWORD, m_dwDrawTextFormat, DrawTextFormat);
	SYNTHESIZE(DWORD, m_dwColor, Color);
private:
	map<eFontType, LPD3DXFONT> m_mapFont;
	RECT	m_rcDraw;
public:
	void Init();
	void Destroy();
	void Render(RECT _rc, const char* _str,eFontType _eFontType = E_NORMAL);
	void Render(RECT _rc, const char* _str, DWORD _dwColor, eFontType _eFontType = E_NORMAL);
public:
	LPD3DXFONT GetFont(eFontType e);
};

