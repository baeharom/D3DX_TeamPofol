#include "stdafx.h"
#include "cTextureManager.h"


cTextureManager::cTextureManager()
{
}


cTextureManager::~cTextureManager()
{
}

LPDIRECT3DTEXTURE9& cTextureManager::AddTexture(string key, string fileName, OUT D3DXIMAGE_INFO* pImageInfo/*= NULL*/)
{
	if (pImageInfo)
	{
		if (!m_mapTexture[key])
		{
			D3DXCreateTextureFromFileEx(g_pD3DDevice, fileName.c_str(),
				D3DX_DEFAULT_NONPOW2, D3DX_DEFAULT_NONPOW2, D3DX_DEFAULT, 0,
				D3DFMT_UNKNOWN, D3DPOOL_MANAGED, D3DX_FILTER_NONE, D3DX_DEFAULT,
				0, pImageInfo, NULL, &m_mapTexture[key]);
		}
	}
	else
	{
		if (!m_mapTexture[key])
		{
			D3DXCreateTextureFromFile(g_pD3DDevice, fileName.c_str(), &m_mapTexture[key]);
		}
	}
	return m_mapTexture[key];
}

void cTextureManager::Destroy()
{
	map<string, LPDIRECT3DTEXTURE9>::iterator iter;

	for (iter = m_mapTexture.begin(); iter != m_mapTexture.end(); ++iter)
		SAFE_RELEASE(iter->second);

}

LPDIRECT3DTEXTURE9& cTextureManager::GetTexture(string key)
{
	LPDIRECT3DTEXTURE9 ret = NULL;

	if (m_mapTexture.find(key) != m_mapTexture.end())
		ret = m_mapTexture[key];

	return ret;
}
