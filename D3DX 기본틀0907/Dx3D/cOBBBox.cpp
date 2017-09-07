#include "stdafx.h"
#include "cOBBBox.h"


cOBBBox::cOBBBox(D3DXMATRIXA16* _matRT)
	:m_pmatRT(_matRT), m_vExtent(5,5,5)
{
	UpdateBox();
}


cOBBBox::~cOBBBox()
{
}

void cOBBBox::UpdateBox()
{
	m_CenterPos.x = m_pmatRT->_41;
	m_CenterPos.y = m_pmatRT->_42;
	m_CenterPos.z = m_pmatRT->_43;

	//X 축 저장
	m_Axis[0].x = m_pmatRT->_11;
	m_Axis[0].y = m_pmatRT->_12;
	m_Axis[0].z = m_pmatRT->_13;
	//Y 축 저장
	m_Axis[1].x = m_pmatRT->_21;
	m_Axis[1].y = m_pmatRT->_22;
	m_Axis[1].z = m_pmatRT->_23;
	//Z 축 저장
	m_Axis[2].x = m_pmatRT->_31;
	m_Axis[2].y = m_pmatRT->_32;
	m_Axis[2].z = m_pmatRT->_33;
}

bool cOBBBox::Collision(const cOBBBox* _other)
{
	UpdateBox(); //동적 오브젝트이므로 항상 포지션 업데이트를 한다.
	D3DXVECTOR3 vCDis =_other->m_CenterPos-m_CenterPos;
	D3DXVECTOR3 C[3];    //matrix C=A^T B,c_{ij}=Dot(A_i,B_j)
	D3DXVECTOR3 absC[3]; //|c_{ij}|
	D3DXVECTOR3 AD;      //Dot(A_i,D)
	float R0, R1, R;    //interval radii and distance between centers
	float R01;        //=R0+R1
	const double cutoff = 0.999999;
	bool existsParallelPair = false;
	for (int i = 0; i < 3; ++i)
	{
		AD[i] = D3DXVec3Dot(&m_Axis[i], &vCDis);
	}
	//A1,A2,A3
	for (int q = 0; q < 3; ++q)
	{
		for (int i = 0; i < 3; ++i)
		{
			C[q][i] = D3DXVec3Dot(&m_Axis[q], &_other->m_Axis[i]);
			absC[q][i] = fabsf(C[q][i]);
			if (absC[q][i] > cutoff)
				existsParallelPair = true;
		}
		R = fabsf(AD[q]);
		R1 = D3DXVec3Dot(&_other->m_vExtent, &absC[q]);
		R01 = m_vExtent[q] + R1;
		if (R > R01)
			return false;
	}
	//B1,B2,B3
	for (int i = 0; i < 3; ++i)
	{
		R = fabsf(D3DXVec3Dot(&_other->m_Axis[i], &vCDis));
		R0 = m_vExtent[0] * absC[i][0] + m_vExtent[1] * absC[i][1] +
			m_vExtent[2] * absC[i][2];
		R01 = R0 + _other->m_vExtent[i];
		if (R > R01)
			return false;
	}
	if (existsParallelPair == true)
		return TRUE;
	//A1xB1
	R = fabsf(AD[2] * C[1][0] - AD[1] * C[2][0]);
	R0 = m_vExtent[1] * absC[2][0] + m_vExtent[2] * absC[1][0];
	R1 = _other->m_vExtent[1] * absC[0][2] + _other->m_vExtent[2] * absC[0][1];
	R01 = R0 + R1;
	if (R > R01)
		return false;
	//A1xB2
	R = fabsf(AD[2] * C[1][1] - AD[1] * C[2][1]);
	R0 = m_vExtent[1] * absC[2][1] + m_vExtent[2] * absC[1][1];
	R1 = _other->m_vExtent[0] * absC[0][2] + _other->m_vExtent[2] * absC[0][0];
	R01 = R0 + R1;
	if (R > R01)
		return false;
	//A1xB3
	R = fabsf(AD[2] * C[1][2] - AD[1] * C[2][2]);
	R0 = m_vExtent[1] * absC[2][2] + m_vExtent[2] * absC[1][2];
	R1 = _other->m_vExtent[0] * absC[0][1] + _other->m_vExtent[1] * absC[0][0];
	R01 = R0 + R1;
	if (R > R01)
		return false;
	//A2xB1
	R = fabsf(AD[0] * C[2][0] - AD[2] * C[0][0]);
	R0 = m_vExtent[0] * absC[2][0] + m_vExtent[2] * absC[0][0];
	R1 = _other->m_vExtent[1] * absC[1][2] + _other->m_vExtent[2] * absC[1][1];
	R01 = R0 + R1;
	if (R > R01)
		return false;
	//A2xB2
	R = fabsf(AD[0] * C[2][1] - AD[2] * C[0][1]);
	R0 = m_vExtent[0] * absC[2][1] + m_vExtent[2] * absC[0][1];
	R1 = _other->m_vExtent[0] * absC[1][2] + _other->m_vExtent[2] * absC[1][0];
	R01 = R0 + R1;
	if (R > R01)
		return false;
	//A2xB3
	R = fabsf(AD[0] * C[2][2] - AD[2] * C[0][2]);
	R0 = m_vExtent[0] * absC[2][2] + m_vExtent[2] * absC[0][2];
	R1 = _other->m_vExtent[0] * absC[1][1] + _other->m_vExtent[1] * absC[1][0];
	R01 = R0 + R1;
	if (R > R01)
		return false;
	//A3xB1
	R = fabsf(AD[1] * C[0][0] - AD[0] * C[1][0]);
	R0 = m_vExtent[0] * absC[1][0] + m_vExtent[1] * absC[0][0];
	R1 = _other->m_vExtent[1] * absC[2][2] + _other->m_vExtent[2] * absC[2][1];
	R01 = R0 + R1;
	if (R > R01)
		return false;
	//A3xB2
	R = fabsf(AD[1] * C[0][1] - AD[0] * C[1][1]);
	R0 = m_vExtent[0] * absC[1][1] + m_vExtent[1] * absC[0][1];
	R1 = _other->m_vExtent[0] * absC[2][2] + _other->m_vExtent[2] * absC[2][0];
	R01 = R0 + R1;
	if (R > R01)
		return false;
	//A3xB3
	R = fabsf(AD[1] * C[0][2] - AD[0] * C[1][2]);
	R0 = m_vExtent[0] * absC[1][2] + m_vExtent[1] * absC[0][2];
	R1 = _other->m_vExtent[0] * absC[2][1] + _other->m_vExtent[1] * absC[2][0];
	R01 = R0 + R1;
	if (R > R01)
		return false;
	//겹치지 않으면 충돌헀다
	return true;
}

void cOBBBox::Render()
{
	D3DXVECTOR3 m_vMinVer;
	m_vMinVer.x=- m_vExtent.x;
	m_vMinVer.y=- m_vExtent.y;
	m_vMinVer.z=- m_vExtent.z;
	D3DXVECTOR3 m_vMaxVer;
	m_vMaxVer.x =m_vExtent.x;
	m_vMaxVer.y =m_vExtent.y;
	m_vMaxVer.z =m_vExtent.z;
	D3DXVECTOR3 vVertex[8];
	vVertex[0] = D3DXVECTOR3(m_vMinVer.x, m_vMinVer.y, m_vMaxVer.z);
	vVertex[1] = D3DXVECTOR3(m_vMinVer.x, m_vMaxVer.y, m_vMaxVer.z);
	vVertex[2] = D3DXVECTOR3(m_vMaxVer.x, m_vMaxVer.y, m_vMaxVer.z);
	vVertex[3] = D3DXVECTOR3(m_vMaxVer.x, m_vMinVer.y, m_vMaxVer.z);
	vVertex[4] = D3DXVECTOR3(m_vMinVer.x, m_vMinVer.y, m_vMinVer.z);
	vVertex[5] = D3DXVECTOR3(m_vMinVer.x, m_vMaxVer.y, m_vMinVer.z);
	vVertex[6] = D3DXVECTOR3(m_vMaxVer.x, m_vMaxVer.y, m_vMinVer.z);
	vVertex[7] = D3DXVECTOR3(m_vMaxVer.x, m_vMinVer.y, m_vMinVer.z);
	for (int i = 0; i < 8; ++i)
		D3DXVec3TransformCoord(&vVertex[i], &vVertex[i], m_pmatRT);
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
	//g_pD3DDevice->SetRenderState(D3DRS_FILLMODE, _fillRender ? D3DFILL_WIREFRAME : D3DFILL_SOLID);
	g_pD3DDevice->SetFVF(D3DFVF_XYZ);
	g_pD3DDevice->DrawPrimitiveUP(D3DPT_TRIANGLELIST, vecVertex.size() / 3,
		&vecVertex[0], sizeof(D3DXVECTOR3));
}
