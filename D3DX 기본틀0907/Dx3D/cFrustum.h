#pragma once
#define g_pFrustum cFrustum::GetInstance()
/// 프러스텀에 정확하게 포함되지 않더라도, 약간의 여분을 주어서 프러스텀에 포함시키기 위한 값
#define PLANE_EPSILON	0.1f
/*
 프러스텀 컬링을 하기위한 클래스
*/
class cFrustum //절두체
{
private:
	SINGLETON(cFrustum);
private:
	D3DXVECTOR3 m_vVertex[8];
	D3DXVECTOR3	m_vPos;		// 현재 카메라의 월드좌표
	D3DXPLANE	m_plane[6];
public:
	bool Make();
	bool IsInVertex(D3DXVECTOR3* _pv);
	bool IsInSphere(D3DXVECTOR3* _pvC, float _radius);

};

