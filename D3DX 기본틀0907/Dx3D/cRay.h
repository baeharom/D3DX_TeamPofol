#pragma once
class cRay
{
private:
	enum eRaySpace
	{
		E_NONE,
		E_VIEW,
		E_WORLD
	};

	eRaySpace m_eRaySpace;

public:
	D3DXVECTOR3 m_vOrg;
	D3DXVECTOR3 m_vDir;

public:
	cRay();
	~cRay();

	static cRay RayAtViewSpace(int x, int y);
	static cRay RayAtWorldSpace(int x, int y);
	bool IsPicked(ST_SPHERE* pSphere);
};

/*
반직선: P(t) = Q + Vt
	==> P(x) = Qx + Vxt
	==> P(y) = Qy + Vyt
	==> P(z) = Qz + Vzt

	Q = 반직선의 출발점
	V = 반직선의 방향

구: x^2 + y^2 + z^2 = r^2

구와 반직선의 연립 방정식: (Qx + Vxt)^2 + (Qy + Vyt)^2 + (Qz + Vzt)^2 = r^2

연립 방정식을 t에 대한 2차 방적식으로 정리
(Vx + Vy + Vz)^2 * t^2 + 2(QxVx + QyVy + QzVz)t+ (Qx^2 + Qy^2 + Qz^2) - r^2 = 0
(VV)t^2 + 2(QV)t + QQ - rr = 0

판별식 = b^2 - 4ac ==> 4(QV)^2 - 4(VV)(QQ - rr) = QV^2 - (VV)(QQ - rr)
판별식 >= 0 교점이 존재한다(충돌)
*/

/*
구 메쉬 생성
D3DXCreateSphere(g_pD3DDevice, 1.5f, 10, 10, &m_pBigSphere, NULL);
*/