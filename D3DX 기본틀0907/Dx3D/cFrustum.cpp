#include "stdafx.h"
#include "cFrustum.h"


cFrustum::cFrustum()
{
}


cFrustum::~cFrustum()
{
}

bool cFrustum::Make()
{
	D3DXMATRIXA16 matView,matProj,matInv;
	g_pD3DDevice->GetTransform(D3DTS_VIEW, &matView);
	g_pD3DDevice->GetTransform(D3DTS_PROJECTION, &matProj);
	matView *= matProj;
	
	// ������ı��� ��ġ�� ��� 3���� ������ǥ�� ���� (-1,-1,0) ~ (1,1,1)������ ������ �ٲ��.
	// m_vtx�� �� ���������� ��谪�� �־�д�.
	m_vVertex[0].x = -1.0f;	m_vVertex[0].y = -1.0f;	m_vVertex[0].z = 0.0f;
	m_vVertex[1].x = 1.0f;	m_vVertex[1].y = -1.0f;	m_vVertex[1].z = 0.0f;
	m_vVertex[2].x = 1.0f;	m_vVertex[2].y = -1.0f;	m_vVertex[2].z = 1.0f;
	m_vVertex[3].x = -1.0f;	m_vVertex[3].y = -1.0f;	m_vVertex[3].z = 1.0f;
	m_vVertex[4].x = -1.0f;	m_vVertex[4].y = 1.0f;	m_vVertex[4].z = 0.0f;
	m_vVertex[5].x = 1.0f;	m_vVertex[5].y = 1.0f;	m_vVertex[5].z = 0.0f;
	m_vVertex[6].x = 1.0f;	m_vVertex[6].y = 1.0f;	m_vVertex[6].z = 1.0f;
	m_vVertex[7].x = -1.0f;	m_vVertex[7].y = 1.0f;	m_vVertex[7].z = 1.0f;

	// view * proj�� ������� ���Ѵ�.
	D3DXMatrixInverse(&matInv, NULL, &matView);

	// Vertex_���� = Vertex_local * Matrix_world * Matrix_view * Matrix_Proj �ε�,
	// Vertex_world = Vertex_local * Matrix_world�̹Ƿ�,
	// Vertex_���� = Vertex_world * Matrix_view * Matrix_Proj �̴�.
	// Vertex_���� = Vertex_world * ( Matrix_view * Matrix_Proj ) ����
	// �����( Matrix_view * Matrix_Proj )^-1�� �纯�� ���ϸ�
	// Vertex_���� * �����( Matrix_view * Matrix_Proj )^-1 = Vertex_World �� �ȴ�.
	// �׷��Ƿ�, m_vtx * matInv = Vertex_world�� �Ǿ�, ������ǥ���� �������� ��ǥ�� ���� �� �ִ�.
	for (int i = 0; i < 8; i++)
		D3DXVec3TransformCoord(&m_vVertex[i], &m_vVertex[i], &matInv);

	// 0���� 5���� ���������� near����� ������ܰ� �����ϴ��̹Ƿ�, ���� ��ǥ�� ���ؼ� 2�� ������
	// ī�޶��� ��ǥ�� ���� �� �ִ�.(��Ȯ�� ��ġ�ϴ� ���� �ƴϴ�.)
	m_vPos = (m_vVertex[0] + m_vVertex[5]) / 2.0f;
	// ����� ������ǥ�� �������� ����� �����
	// ���Ͱ� �������� ���ʿ��� �ٱ������� ������ �����̴�.
		D3DXPlaneFromPoints(&m_plane[0], m_vVertex+4, m_vVertex+7, m_vVertex+6);	// �� ���(top)
		D3DXPlaneFromPoints(&m_plane[1], m_vVertex  , m_vVertex+1, m_vVertex+2);	// �� ���(bottom)
		D3DXPlaneFromPoints(&m_plane[2], m_vVertex  , m_vVertex+4, m_vVertex+5);	// �� ���(near)
	D3DXPlaneFromPoints(&m_plane[3], m_vVertex + 2, m_vVertex + 6, m_vVertex + 7);	// �� ���(far)
	D3DXPlaneFromPoints(&m_plane[4], m_vVertex, m_vVertex + 3, m_vVertex + 7);	// �� ���(left)
	D3DXPlaneFromPoints(&m_plane[5], m_vVertex + 1, m_vVertex + 5, m_vVertex + 6);	// �� ���(right)

	return TRUE;
}

bool cFrustum::IsInVertex(D3DXVECTOR3 * _pv)
{
	float		fDist;

	for(int i = 0 ; i < 6 ; i++ )
	{
		fDist = D3DXPlaneDotCoord(&m_plane[i], _pv);
		if (fDist > PLANE_EPSILON) return FALSE;	// plane�� normal���Ͱ� far�� ���ϰ� �����Ƿ� ����̸� ���������� �ٱ���
	}
	return TRUE;
}

bool cFrustum::IsInSphere(D3DXVECTOR3 * _pvC, float _radius)
{
	float		fDist;
	for (int i = 0; i < 6; ++i)
	{
		fDist = D3DXPlaneDotCoord(&m_plane[i], _pvC);
		if (fDist > (_radius + PLANE_EPSILON)) return FALSE;	// ���� �߽����� �Ÿ��� ���������� ũ�� �������ҿ� ����
	}
	return TRUE;
}
