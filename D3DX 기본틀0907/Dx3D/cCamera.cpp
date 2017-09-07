#include "stdafx.h"
#include "cCamera.h"


cCamera::cCamera()
	: MINDISTANCE(20.f)
	, MAXDISTANCE(100.f)
	, m_fDistance(20.f)
	, m_vEye(0, 0, -m_fDistance)
	, m_vLookAt(0, 0, 0)
	, m_vUp(0, 1, 0)
	, m_fRotX(0)
	, m_fRotY(0)
	, m_isRButtonDown(false)
{
}


cCamera::~cCamera()
{
}

void cCamera::Init()
{
	RECT rc;
	GetClientRect(g_hWnd, &rc);

	D3DXMATRIXA16 matProj;
	D3DXMatrixPerspectiveFovLH(&matProj, D3DX_PI / 4.0f, rc.right / (float)rc.bottom, 1, 1000);
	g_pD3DDevice->SetTransform(D3DTS_PROJECTION, &matProj);
}

void cCamera::Update(D3DXVECTOR3* pTarget/*= NULL*/)
{
	m_vEye = D3DXVECTOR3(0, 0, -m_fDistance);

	D3DXMATRIXA16 matRotX, matRotY;
	D3DXMatrixRotationX(&matRotX, m_fRotX);
	D3DXMatrixRotationY(&matRotY, m_fRotY);

	D3DXVec3TransformCoord(&m_vEye, &m_vEye, &(matRotX * matRotY));

	if (pTarget)
	{
		m_vLookAt = *pTarget;
		m_vEye = *pTarget + m_vEye;
		m_vLookAt.y += 6;
	}

	D3DXMATRIXA16 matView;
	D3DXMatrixLookAtLH(&matView, &m_vEye, &m_vLookAt, &m_vUp);
	g_pD3DDevice->SetTransform(D3DTS_VIEW, &matView);
}

void cCamera::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_RBUTTONDOWN:
	{
		m_ptPrevMouse.x = LOWORD(lParam);
		m_ptPrevMouse.y = HIWORD(lParam);
		m_isRButtonDown = true;
	}
	break;
	case WM_RBUTTONUP:
	{
		m_isRButtonDown = false;
	}
	break;
	case WM_MOUSEMOVE:
	{
		if (m_isRButtonDown)
		{
			POINT ptCurrMouse;
			ptCurrMouse.x = LOWORD(lParam);
			ptCurrMouse.y = HIWORD(lParam);

			m_fRotX += (ptCurrMouse.y - m_ptPrevMouse.y) / 100.0f;
			m_fRotY += (ptCurrMouse.x - m_ptPrevMouse.x) / 100.0f;

			// X 축 회전은 -90 ~ 90 도 사이로 고정
			if (m_fRotX <= -D3DX_PI * 0.5f + D3DX_16F_EPSILON)
				m_fRotX = -D3DX_PI * 0.5f + D3DX_16F_EPSILON;
			else if (m_fRotX >= D3DX_PI * 0.5f - D3DX_16F_EPSILON)
				m_fRotX = D3DX_PI * 0.5f - D3DX_16F_EPSILON;

			m_ptPrevMouse = ptCurrMouse;
		}
	}
	break;
	case WM_MOUSEWHEEL:
	{
		m_fDistance -= GET_WHEEL_DELTA_WPARAM(wParam) / 20.0f;
		//if (m_fDistance > MAXDISTANCE)m_fDistance = MAXDISTANCE;
		//else if (m_fDistance < MINDISTANCE)m_fDistance = MINDISTANCE;
	}
	break;
	}
}