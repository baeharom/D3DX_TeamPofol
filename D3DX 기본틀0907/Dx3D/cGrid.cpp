#include "stdafx.h"
#include "cGrid.h"
#include "cPyramid.h"


cGrid::cGrid()
{
	D3DXMatrixIdentity(&m_matWorld);
}


cGrid::~cGrid()
{
	for each (auto p in m_vecPyramid)
	{
		SAFE_RELEASE(p);
	}
}

void cGrid::Setup()
{
	int nNumLine = 50;
	float fInterval = 1.0f;
	float fMax = fInterval * nNumLine;

	D3DXCOLOR c = D3DCOLOR_XRGB(255, 255, 255);

	for (int i = 1; i <= nNumLine; ++i)
	{
		m_vecPCVertex.push_back(ST_PC_VERTEX(D3DXVECTOR3(-fMax, 0, i), c));
		m_vecPCVertex.push_back(ST_PC_VERTEX(D3DXVECTOR3( fMax, 0, i), c));

		m_vecPCVertex.push_back(ST_PC_VERTEX(D3DXVECTOR3(-fMax, 0,-i), c));
		m_vecPCVertex.push_back(ST_PC_VERTEX(D3DXVECTOR3( fMax, 0,-i), c));

		m_vecPCVertex.push_back(ST_PC_VERTEX(D3DXVECTOR3( i, 0,-fMax), c));
		m_vecPCVertex.push_back(ST_PC_VERTEX(D3DXVECTOR3( i, 0, fMax), c));

		m_vecPCVertex.push_back(ST_PC_VERTEX(D3DXVECTOR3(-i, 0,-fMax), c));
		m_vecPCVertex.push_back(ST_PC_VERTEX(D3DXVECTOR3(-i, 0, fMax), c));
	}

	c = D3DCOLOR_XRGB(255, 0, 0);
	m_vecPCVertex.push_back(ST_PC_VERTEX(D3DXVECTOR3(-fMax, 0, 0), c));
	m_vecPCVertex.push_back(ST_PC_VERTEX(D3DXVECTOR3( fMax, 0, 0), c));

	D3DXMATRIXA16 matS, matR, matW;
	D3DXMatrixScaling(&matS, 8.0f, 0.4f, 0.4f);
	D3DXMatrixIdentity(&matR);
	matW = matS * matR;
	cPyramid* pPyramid = new cPyramid;
	pPyramid->Setup(c, matW);
	m_vecPyramid.push_back(pPyramid);

	c = D3DCOLOR_XRGB(0, 255, 0);
	m_vecPCVertex.push_back(ST_PC_VERTEX(D3DXVECTOR3( 0,-fMax, 0), c));
	m_vecPCVertex.push_back(ST_PC_VERTEX(D3DXVECTOR3( 0, fMax, 0), c));

	D3DXMatrixScaling(&matS, 8.0f, 0.4f, 0.4f);
	D3DXMatrixRotationZ(&matR, D3DX_PI / 2);
	matW = matS * matR;
	pPyramid = new cPyramid;
	pPyramid->Setup(c, matW);
	m_vecPyramid.push_back(pPyramid);

	c = D3DCOLOR_XRGB(0, 0, 255);
	m_vecPCVertex.push_back(ST_PC_VERTEX(D3DXVECTOR3( 0, 0,-fMax), c));
	m_vecPCVertex.push_back(ST_PC_VERTEX(D3DXVECTOR3( 0, 0, fMax), c));

	D3DXMatrixScaling(&matS, 8.0f, 0.4f, 0.4f);
	D3DXMatrixRotationY(&matR, -D3DX_PI / 2);
	matW = matS * matR;
	pPyramid = new cPyramid;
	pPyramid->Setup(c, matW);
	m_vecPyramid.push_back(pPyramid);
}

void cGrid::Render()
{
	g_pD3DDevice->SetRenderState(D3DRS_LIGHTING, false);

	g_pD3DDevice->SetTransform(D3DTS_WORLD, &m_matWorld);
	g_pD3DDevice->SetTexture(0, NULL);
	g_pD3DDevice->SetFVF(ST_PC_VERTEX::FVF);
	g_pD3DDevice->DrawPrimitiveUP(D3DPT_LINELIST, m_vecPCVertex.size() / 2,
		&m_vecPCVertex[0], sizeof(ST_PC_VERTEX));

	for each (auto p in m_vecPyramid)
	{
		p->Render();
	}

	g_pD3DDevice->SetRenderState(D3DRS_LIGHTING, true);
}