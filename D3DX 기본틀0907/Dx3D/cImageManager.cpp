#include "stdafx.h"
#include "cImageManager.h"
#include "cImage.h"

cImageManager::cImageManager()
{
}


cImageManager::~cImageManager()
{
}

void cImageManager::Init()
{
	D3DXCreateSprite(g_pD3DDevice, &m_pSprite);
}

void cImageManager::Release()
{
	map<string, cImage*>::iterator iter;

	for (iter = m_mapImage.begin(); iter != m_mapImage.end(); ++iter)
		SAFE_RELEASE(iter->second);

	SAFE_RELEASE(m_pSprite);
}

cImage* cImageManager::AddImage(string _strKey, const char * _fileName, float _x, float _y)
{
	cImage* Image = FindImage(_strKey);
	if (Image)return Image;

	Image = new cImage;
	Image->Init(_fileName, _x, _y);
	m_mapImage.insert(make_pair(_strKey, Image));
	return Image;
}

cImage* cImageManager::AddImage(string _strKey, const char * _fileName, float _x, float _y, float _w, float _h)
{
	cImage* Image = FindImage(_strKey);
	if (Image)return Image;

	Image = new cImage;
	Image->Init(_fileName, _x, _y,_w,_h);
	m_mapImage.insert(make_pair(_strKey, Image));
	return Image;
}

void cImageManager::Render(string _strKey, int _a)
{
	FindImage(_strKey)->Render(m_pSprite,_a);
}

void cImageManager::RenderAndPos(string _strKey, float _x, float _y, int _a)
{
	FindImage(_strKey)->RenderAndPos(m_pSprite,_x,_y,_a);
}

void cImageManager::RenderAndPosCenter(string _strKey, float _x, float _y, int _a)
{
	FindImage(_strKey)->RenderAndPosCenter(m_pSprite, _x, _y, _a);
}

void cImageManager::RenderAndSize(string _strKey,float _w, float _h, int _a)
{
	FindImage(_strKey)->RenderAndSize(m_pSprite,_w,_h,_a);
}

void cImageManager::Render(string _strKey, float _x, float _y, float _w, float _h, int _a)
{
	FindImage(_strKey)->Render(m_pSprite,_x, _y,_w,_h,_a);
}

cImage* cImageManager::FindImage(string key)
{
	cImage* ret = NULL;

	if (m_mapImage.find(key) != m_mapImage.end())
		ret = m_mapImage[key];

	return ret;
}

bool cImageManager::CollisionCheck(string key)
{
	cImage* Image = FindImage(key);
	assert(Image&&"CollisionCheck 이미지 찾을 수 없음");

	RECT rc;
	int x = Image->GetmatW()._41;
	int y = Image->GetmatW()._42;
	SetRect(&rc, x, y, x + (int)Image->GetCurW(), y + (int)Image->GetCurH());
	POINT ptCurrMouse;
	GetCursorPos(&ptCurrMouse);
	ScreenToClient(g_hWnd, &ptCurrMouse);

	if (PtInRect(&rc, ptCurrMouse))
	{
		if (g_pKeyManager->IsOnceKeyDown(VK_LBUTTON))
			return true;
	}
	return false;
}
