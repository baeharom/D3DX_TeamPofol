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
������: P(t) = Q + Vt
	==> P(x) = Qx + Vxt
	==> P(y) = Qy + Vyt
	==> P(z) = Qz + Vzt

	Q = �������� �����
	V = �������� ����

��: x^2 + y^2 + z^2 = r^2

���� �������� ���� ������: (Qx + Vxt)^2 + (Qy + Vyt)^2 + (Qz + Vzt)^2 = r^2

���� �������� t�� ���� 2�� ���������� ����
(Vx + Vy + Vz)^2 * t^2 + 2(QxVx + QyVy + QzVz)t+ (Qx^2 + Qy^2 + Qz^2) - r^2 = 0
(VV)t^2 + 2(QV)t + QQ - rr = 0

�Ǻ��� = b^2 - 4ac ==> 4(QV)^2 - 4(VV)(QQ - rr) = QV^2 - (VV)(QQ - rr)
�Ǻ��� >= 0 ������ �����Ѵ�(�浹)
*/

/*
�� �޽� ����
D3DXCreateSphere(g_pD3DDevice, 1.5f, 10, 10, &m_pBigSphere, NULL);
*/