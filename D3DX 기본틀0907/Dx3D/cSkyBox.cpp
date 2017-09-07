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
 /* D3DCULL_NONE : �޸��� �ø�(Culling)�� ���� �ʴ´�.
 * D3DCULL_CW   : �޸��� ��ȸ������ �ø�(Culling) �Ѵ�.
 * D3DCULL_CCW  : �޸��� ���� ȸ������ �ø�(Culling) �Ѵ�.
 * D3DCULL_FORCE_DWORD : �� �������� ���������� 32 ��Ʈ ����� ������ �Ѵ�. �� ���� ������ �ʾҴ�.
 */
	//g_pD3DDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
	// D3DSAMP_ADDRESSU : u ��ǥ�� ����ϴ� �ؽ�ó��巹�̸��. ����Ʈ�� D3DTADDRESS_WRAP �̴�. �� �ڼ��� ������, ��D3DTEXTUREADDRESS ���� �����Ұ�. 
	// D3DSAMP_ADDRESSV : v ��ǥ�� ����ϴ� �ؽ�ó��巹�̸��. ����Ʈ�� D3DTADDRESS_WRAP �̴�. �� �ڼ��� ������, ��D3DTEXTUREADDRESS ���� �����Ұ�. 
	// D3DTADDRESS_CLAMP: ���� [0.0, 1.0] �� ������ �ؽ�ó ��ǥ��, ����, 0.0 ���� 1.0 �� �ؽ�ó �÷��� �����ȴ�. 
	g_pD3DDevice->SetSamplerState(0, D3DSAMP_ADDRESSU, D3DTADDRESS_CLAMP);
	g_pD3DDevice->SetSamplerState(0, D3DSAMP_ADDRESSV, D3DTADDRESS_CLAMP);

	g_pD3DDevice->SetFVF(D3DFVF_SKYVERTEX);
	g_pD3DDevice->SetStreamSource(0, m_pVB, 0, sizeof(ST_SKY_VERTEX));

	m_matWorld._41 = g_pCamera->GetvPos().x;
	m_matWorld._42 = g_pCamera->GetvPos().y;
	m_matWorld._43 = g_pCamera->GetvPos().z;
	g_pD3DDevice->SetTransform(D3DTS_WORLD, &m_matWorld);
	// z ���۸� ����. ( z ���� ������ �ʰ� ���� )
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
