#pragma once
class cImage;
class cKeyManager;
//2D �̹����� 
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
	void RenderAndPos(string _strKey, float _x, float _y, int _a = 255); //��ǥ �ٲٸ鼭 �׸�
	void RenderAndPosCenter(string _strKey, float _x, float _y, int _a = 255); //���� ��ǥ�� �׸���
	void RenderAndSize(string _strKey, float _w, float _h, int _a=255); //������ �ٲٸ鼭 �׸�
	void Render(string _strKey, float _x, float _y,float _w, float _h, int _a = 255); //��� �ٲٸ鼭 �׸�
	cImage* FindImage(string key);

	bool CollisionCheck(string key);
};
