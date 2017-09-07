#pragma once
class cImage;
class cKeyManager;
//2D 이미지용 
#define g_pImageManager cImageManager::GetInstance()
class cImageManager
{
private:
	SINGLETON(cImageManager);
private:
	LPD3DXSPRITE			m_pSprite;
	map<string, cImage*>		m_mapImage;
public:
	void Init();
	void Release();
public:

	cImage* AddImage(string _strKey,const char* _fileName, float _x, float _y);
	cImage* AddImage(string _strKey, const char* _fileName, float _x, float _y,float _w,float _h);
	

	void Render(string _strKey,int _a=255);
	void RenderAndPos(string _strKey, float _x, float _y, int _a = 255); //좌표 바꾸면서 그림
	void RenderAndPosCenter(string _strKey, float _x, float _y, int _a = 255); //센터 좌표로 그린다
	void RenderAndSize(string _strKey, float _w, float _h, int _a=255); //사이즈 바꾸면서 그림
	void Render(string _strKey, float _x, float _y,float _w, float _h, int _a = 255); //모두 바꾸면서 그림
	cImage* FindImage(string key);

	bool CollisionCheck(string key);
};
