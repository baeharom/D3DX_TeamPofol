#pragma once
#define g_pFrustum cFrustum::GetInstance()
/// �������ҿ� ��Ȯ�ϰ� ���Ե��� �ʴ���, �ణ�� ������ �־ �������ҿ� ���Խ�Ű�� ���� ��
#define PLANE_EPSILON	0.1f
/*
 �������� �ø��� �ϱ����� Ŭ����
*/
class cFrustum //����ü
{
private:
	SINGLETON(cFrustum);
private:
	D3DXVECTOR3 m_vVertex[8];
	D3DXVECTOR3	m_vPos;		// ���� ī�޶��� ������ǥ
	D3DXPLANE	m_plane[6];
public:
	bool Make();
	bool IsInVertex(D3DXVECTOR3* _pv);
	bool IsInSphere(D3DXVECTOR3* _pvC, float _radius);

};

