#pragma once

#define g_pTextureManager cTextureManager::GetInstance()

class cTextureManager
{
	SINGLETON(cTextureManager);
	
private:
	map<string, LPDIRECT3DTEXTURE9>		m_mapTexture;


public:
	LPDIRECT3DTEXTURE9& AddTexture(string key, string fileName, OUT D3DXIMAGE_INFO* pImageInfo = NULL);

	void Destroy();

	LPDIRECT3DTEXTURE9& GetTexture(string key);

};

