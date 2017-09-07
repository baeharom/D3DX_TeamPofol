#include "stdafx.h"
#include "cStaticCollision.h"


cStaticCollision::cStaticCollision()
	:m_vScale(3,3,3), m_bStatic(true),m_vPos(0,0,0), m_fRadius(5.f)
{
	UpdatePos(m_vPos);
}

cStaticCollision::cStaticCollision(IN const D3DXVECTOR3& _vPos, bool _bStatic)
	:m_vScale(3,3,3), m_fRadius(5.f),m_bStatic(_bStatic)
{
	m_vPos = _vPos;
	UpdatePos(m_vPos);
}

cStaticCollision::~cStaticCollision()
{
}


void cStaticCollision::UpdatePos(IN const D3DXVECTOR3& _vPos)
{
	m_vPos = _vPos;
	m_vPos.y += 3; //Ä³¸¯ÅÍ Áß¾Ó
	D3DXMatrixScaling(&m_matScale, m_vScale.x, m_vScale.y, m_vScale.z);
	m_vMinVer = m_vPos - m_vScale; 
	m_vMaxVer = m_vPos + m_vScale;
}

bool cStaticCollision::IsSphereCollision(IN const cStaticCollision*& _other)
{
	float dis=GetDistance(m_vPos, _other->m_vPos);
	return dis<m_fRadius+_other->m_fRadius;
}

bool cStaticCollision::IsAABBCollision(IN const cStaticCollision*& _other)
{
	if (m_vMinVer.x >_other->m_vMaxVer.x || m_vMaxVer.x < _other->m_vMinVer.x)return false;
	if (m_vMinVer.y >_other->m_vMaxVer.y || m_vMaxVer.x < _other->m_vMinVer.y)return false;
	if (m_vMinVer.z >_other->m_vMaxVer.z || m_vMaxVer.x < _other->m_vMinVer.z)return false;

	return true;
}

bool cStaticCollision::IsCollision(IN const D3DXVECTOR3& _vPos,IN const cStaticCollision*& _other)
{
	UpdatePos(_vPos);
	if (!IsSphereCollision(_other))return false;

	return IsAABBCollision(_other);
}

bool cStaticCollision::IsCollision(IN const cStaticCollision *& _other)
{
	if (!IsSphereCollision(_other))return false;

	return IsAABBCollision(_other);
}

D3DXVECTOR3 cStaticCollision::MakeSlideVec(IN const D3DXVECTOR3 & _vPos)
{
	return D3DXVECTOR3();
}

void cStaticCollision::Render()
{
	D3DXVECTOR3 vVertex[8];
	vVertex[0] = D3DXVECTOR3(m_vMinVer.x, m_vMinVer.y, m_vMaxVer.z);
	vVertex[1] = D3DXVECTOR3(m_vMinVer.x, m_vMaxVer.y, m_vMaxVer.z);
	vVertex[2] = D3DXVECTOR3(m_vMaxVer.x, m_vMaxVer.y, m_vMaxVer.z);
	vVertex[3] = D3DXVECTOR3(m_vMaxVer.x, m_vMinVer.y, m_vMaxVer.z);
	vVertex[4] = D3DXVECTOR3(m_vMinVer.x, m_vMinVer.y, m_vMinVer.z);
	vVertex[5] = D3DXVECTOR3(m_vMinVer.x, m_vMaxVer.y, m_vMinVer.z);
	vVertex[6] = D3DXVECTOR3(m_vMaxVer.x, m_vMaxVer.y, m_vMinVer.z);
	vVertex[7] = D3DXVECTOR3(m_vMaxVer.x, m_vMinVer.y, m_vMinVer.z);
	vector<D3DXVECTOR3> vecVertex;

	vecVertex.push_back(vVertex[0]);
	vecVertex.push_back(vVertex[1]);
	vecVertex.push_back(vVertex[2]);
	vecVertex.push_back(vVertex[0]);
	vecVertex.push_back(vVertex[2]);
	vecVertex.push_back(vVertex[3]);
					
	vecVertex.push_back(vVertex[7]);
	vecVertex.push_back(vVertex[6]);
	vecVertex.push_back(vVertex[5]);
	vecVertex.push_back(vVertex[7]);
	vecVertex.push_back(vVertex[5]);
	vecVertex.push_back(vVertex[4]);
							
	vecVertex.push_back(vVertex[4]);
	vecVertex.push_back(vVertex[5]);
	vecVertex.push_back(vVertex[1]);
	vecVertex.push_back(vVertex[4]);
	vecVertex.push_back(vVertex[1]);
	vecVertex.push_back(vVertex[0]);
							 
	vecVertex.push_back(vVertex[3]);
	vecVertex.push_back(vVertex[2]);
	vecVertex.push_back(vVertex[6]);
	vecVertex.push_back(vVertex[3]);
	vecVertex.push_back(vVertex[6]);
	vecVertex.push_back(vVertex[7]);
							 	
	vecVertex.push_back(vVertex[1]);
	vecVertex.push_back(vVertex[5]);
	vecVertex.push_back(vVertex[6]);
	vecVertex.push_back(vVertex[1]);
	vecVertex.push_back(vVertex[6]);
	vecVertex.push_back(vVertex[2]);
							  	
	vecVertex.push_back(vVertex[4]);
	vecVertex.push_back(vVertex[0]);
	vecVertex.push_back(vVertex[3]);
	vecVertex.push_back(vVertex[4]);
	vecVertex.push_back(vVertex[3]);
	vecVertex.push_back(vVertex[7]);

	D3DXMATRIXA16 matW;
	D3DXMatrixIdentity(&matW);
	g_pD3DDevice->SetTransform(D3DTS_WORLD, &matW);
	g_pD3DDevice->SetRenderState(D3DRS_FILLMODE, g_bFillRender ? D3DFILL_WIREFRAME : D3DFILL_SOLID);
	g_pD3DDevice->SetFVF(D3DFVF_XYZ);
	g_pD3DDevice->DrawPrimitiveUP(D3DPT_TRIANGLELIST, vecVertex.size() / 3,
		&vecVertex[0], sizeof(D3DXVECTOR3));

	D3DXMATRIXA16 matS;
	D3DXMatrixScaling(&matS, m_vScale.x, m_vScale.y, m_vScale.z);
	D3DXMatrixTranslation(&matW, m_vPos.x, m_vPos.y+3, m_vPos.z);
	matS *= matW;
	g_pD3DDevice->SetTransform(D3DTS_WORLD, &matS);
	g_pMeshDebug->DrawSubset(0);
}

