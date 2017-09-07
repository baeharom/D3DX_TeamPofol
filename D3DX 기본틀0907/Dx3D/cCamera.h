#pragma once
#define g_pCamera cCamera::GetInstance()
class cCamera
{
private:
	SINGLETON(cCamera);
private:
	const float			MINDISTANCE, MAXDISTANCE;
	float				m_fDistance;
	D3DXVECTOR3			m_vEye;
	D3DXVECTOR3			m_vLookAt;
	D3DXVECTOR3			m_vUp;

	float		m_fRotY;
	float		m_fRotX;

	bool		m_isRButtonDown;
	POINT		m_ptPrevMouse;
public:

	void Init();
	void Update(D3DXVECTOR3* pTarget = NULL);

	void WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
	D3DXVECTOR3& GetvPos() { return m_vEye; }
};

