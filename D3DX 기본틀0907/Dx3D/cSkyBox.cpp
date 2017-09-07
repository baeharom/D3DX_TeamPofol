#include "stdafx.h"
#include "cSkyBox.h"


cSkyBox::cSkyBox()
{
	for (int i = 0; i < 6; i++)
		m_pTex[i] = NULL;
	m_pVB = NULL;
	D3DXMatrixIdentity(&m_matWorld);
}


cSkyBox::~cSkyBox()
{
	Release();
}

HRESULT cSkyBox::Init(LPCSTR _texName[6])
{
	if (FAILED(LoadTextures(_texName))) { Release(); return E_FAIL; }
	if (FAILED(CreateVertex())) { Release(); return E_FAIL; }

	return S_OK;
}

HRESULT cSkyBox::LoadTextures(LPCSTR _texName[6])
{
	for (int i = 0; i < 6; i++)
	{
		D3DXCreateTextureFromFile(g_pD3DDevice, _texName[i], &m_pTex[i]);
	}
	return S_OK;
}

HRESULT cSkyBox::CreateVertex()
{
	if (FAILED(g_pD3DDevice->CreateVertexBuffer(24 * sizeof(ST_SKY_VERTEX), 0, D3DFVF_SKYVERTEX,
		D3DPOOL_DEFAULT, &m_pVB, NULL)))
	{
		Release();
		return E_FAIL;
	}

	ST_SKY_VERTEX vertices[24] =
	{
		{ -250.0f, 250.0f, -250.0f, 0.0f, 0.0f },		// Top vertices
		{ 250.0f, 250.0f, -250.0f, 1.0f, 0.0f },
		{ -250.0f, 250.0f,  250.0f, 0.0f, 1.0f },
		{ 250.0f, 250.0f,  250.0f, 1.0f, 1.0f },

		{ -250.0f,  250.0f, 250.0f, 0.0f, 0.0f },		// Front vertices
		{ 250.0f,  250.0f, 250.0f, 1.0f, 0.0f },
		{ -250.0f, -250.0f, 250.0f, 0.0f, 1.0f },
		{ 250.0f, -250.0f, 250.0f, 1.0f, 1.0f },

		{ 250.0f,  250.0f, -250.0f, 0.0f, 0.0f },		// Back vertices
		{ -250.0f,  250.0f, -250.0f, 1.0f, 0.0f },
		{ 250.0f, -250.0f, -250.0f, 0.0f, 1.0f },
		{ -250.0f, -250.0f, -250.0f, 1.0f, 1.0f },

		{ -250.0f,  250.0f, -250.0f, 0.0f, 0.0f },		// Left vertices
		{ -250.0f,  250.0f,  250.0f, 1.0f, 0.0f },
		{ -250.0f, -250.0f, -250.0f, 0.0f, 1.0f },
		{ -250.0f, -250.0f,  250.0f, 1.0f, 1.0f },

		{ 250.0f,  250.0f,  250.0f, 0.0f, 0.0f },		// Right vertices
		{ 250.0f,  250.0f, -250.0f, 1.0f, 0.0f },
		{ 250.0f, -250.0f,  250.0f, 0.0f, 1.0f },
		{ 250.0f, -250.0f, -250.0f, 1.0f, 1.0f },

		{ -250.0f, -250.0f,  250.0f, 0.0f, 0.0f },		// Bottom vertices
		{ 250.0f, -250.0f,  250.0f, 1.0f, 0.0f },
		{ -250.0f, -250.0f, -250.0f, 0.0f, 1.0f },
		{ 250.0f, -250.0f, -250.0f, 1.0f, 1.0f }
	};

	VOID* pVertices;
	if (FAILED(m_pVB->Lock(0, 24 * sizeof(ST_SKY_VERTEX), (void**)&pVertices, 0)))
	{
		Release();
		return E_FAIL;
	}
	memcpy(pVertices, vertices, 24 * sizeof(ST_SKY_VERTEX));
	m_pVB->Unlock();

	return S_OK;
}

void cSkyBox::Release()
{
	SAFE_RELEASE(m_pVB);

	for (int i = 0; i < 6; i++)
	{
		SAFE_RELEASE(m_pTex[i]);
	}
}

void cSkyBox::Render()
{// Setup texture
 /* D3DCULL_NONE : 뒷면의 컬링(Culling)는 하지 않는다.
 * D3DCULL_CW   : 뒷면을 우회전으로 컬링(Culling) 한다.
 * D3DCULL_CCW  : 뒷면을 왼쪽 회전으로 컬링(Culling) 한다.
 * D3DCULL_FORCE_DWORD : 이 열거형을 강제적으로 32 비트 사이즈에 컴파일 한다. 이 값은 사용되지 않았다.
 */
	//g_pD3DDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
	// D3DSAMP_ADDRESSU : u 좌표로 사용하는 텍스처어드레싱모드. 디폴트는 D3DTADDRESS_WRAP 이다. 더 자세한 정보는, 「D3DTEXTUREADDRESS 」를 참조할것. 
	// D3DSAMP_ADDRESSV : v 좌표로 사용하는 텍스처어드레싱모드. 디폴트는 D3DTADDRESS_WRAP 이다. 더 자세한 정보는, 「D3DTEXTUREADDRESS 」를 참조할것. 
	// D3DTADDRESS_CLAMP: 범위 [0.0, 1.0] 의 외측의 텍스처 좌표가, 각각, 0.0 으로 1.0 의 텍스처 컬러로 설정된다. 
	g_pD3DDevice->SetSamplerState(0, D3DSAMP_ADDRESSU, D3DTADDRESS_CLAMP);
	g_pD3DDevice->SetSamplerState(0, D3DSAMP_ADDRESSV, D3DTADDRESS_CLAMP);

	g_pD3DDevice->SetFVF(D3DFVF_SKYVERTEX);
	g_pD3DDevice->SetStreamSource(0, m_pVB, 0, sizeof(ST_SKY_VERTEX));

	m_matWorld._41 = g_pCamera->GetvPos().x;
	m_matWorld._42 = g_pCamera->GetvPos().y;
	m_matWorld._43 = g_pCamera->GetvPos().z;
	g_pD3DDevice->SetTransform(D3DTS_WORLD, &m_matWorld);
	// z 버퍼를 끈다. ( z 값을 가지지 않게 설정 )
	g_pD3DDevice->SetRenderState(D3DRS_ZENABLE, FALSE);
	g_pD3DDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
	g_pD3DDevice->SetMaterial(&WHITE_MTRL);
	for (int i = 0; i < 6; i++)
	{
		g_pD3DDevice->SetTexture(0, m_pTex[i]);
		g_pD3DDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, i * 4, 6);
	}
	D3DXMatrixIdentity(&m_matWorld);
	g_pD3DDevice->SetRenderState(D3DRS_ZENABLE, TRUE);

}
