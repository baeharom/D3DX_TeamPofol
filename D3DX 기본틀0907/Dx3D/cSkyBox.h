#pragma once
class cSkyBox
{
private:
#define D3DFVF_SKYVERTEX (D3DFVF_XYZ | D3DFVF_TEX1 )
public:
	struct ST_SKY_VERTEX
	{
		float x, y, z;
		float u, v;
	};
private:
	LPDIRECT3DTEXTURE9			m_pTex[6];					/// 텍스처
	LPDIRECT3DVERTEXBUFFER9		m_pVB;							/// 지형출력용 정점버퍼
	D3DXMATRIXA16				m_matWorld;
public:
	cSkyBox();
	~cSkyBox();

	HRESULT Init(LPCSTR _texName[6]);
	HRESULT	LoadTextures(LPCSTR _texName[6]);
	HRESULT	CreateVertex();

	void	Release();
	void	Render();
};

