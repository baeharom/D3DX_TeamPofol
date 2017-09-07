#include "stdafx.h"
#include "cFrame.h"


cFrame::cFrame()
	: m_pMtlTex(NULL)
{
	D3DXMatrixIdentity(&m_matWorldTM);
}


cFrame::~cFrame()
{
}

void cFrame::Render()
{
	if (m_pMtlTex && !m_vecPNTVertex.empty())
	{
		g_pD3DDevice->SetTransform(D3DTS_WORLD, &m_matWorldTM);
		g_pD3DDevice->SetTexture(0, m_pMtlTex->GetTexture());
		g_pD3DDevice->SetMaterial(&m_pMtlTex->GetMtl());
		g_pD3DDevice->SetFVF(ST_PNT_VERTEX::FVF);
		g_pD3DDevice->DrawPrimitiveUP(D3DPT_TRIANGLELIST, m_vecPNTVertex.size() / 3,
			&m_vecPNTVertex[0], sizeof(ST_PNT_VERTEX));
	}

	for each(auto p in m_vecChild)
		p->Render();
}

void cFrame::AddChild(cFrame* pChild)
{
	if (pChild)
	{
		SAFE_ADDREF(pChild);

		m_vecChild.push_back(pChild);
	}
}

void cFrame::CalcLocalTM(cFrame* pParent)
{
	// 각 부위별 로컬TM을 계산한다.
	// NODE_TM 정보는 부모의 TM정보까지 적용이 된 매트릭스 이다.
	// 그래서 NODE_TM에서 부모의 정보를 제외 시키면 각 부위별 로컬TM이 된다.
	// 재귀적으로 모든 자식들의 로컬TM도 계산해 준다.

	m_matLocalTM = m_matWorldTM;

	if (pParent)
	{
		D3DXMATRIXA16 matInvParent;
		D3DXMatrixInverse(&matInvParent, NULL, &pParent->GetWorldTM());
		m_matLocalTM *= matInvParent;
	}

	for each (auto p in m_vecChild)
		p->CalcLocalTM(this);
}

void cFrame::Update(int nKeyFrame, D3DXMATRIXA16* pParent)
{
	// 월드TM을 계산한다.
	// 로컬T : CalcLocalTranslationMatrix()
	// 로컬R : CalcLocalRotationMatrix()
	// 월드TM = R * T
	// 부모가 존재 할 경우: 월드TM = 월드TM * 부모TM
	// 모든 자식들을 재귀적으로 돌린다.

	D3DXMATRIXA16 matR, matT;
	CalcLocalRotationMatrix(nKeyFrame, matR);
	CalcLocalTranslationMatrix(nKeyFrame, matT);
	m_matWorldTM = matR * matT;

	if (pParent)
	{
		m_matWorldTM *= *pParent;
	}

	for each (auto p in m_vecChild)
		p->Update(nKeyFrame, &m_matWorldTM);
}

void cFrame::CalcLocalTranslationMatrix(IN int nKeyFrame, OUT D3DXMATRIXA16& mat)
{
	/*
	키프레임이 최소 키프레임 보다 낮은 경우 최소 키프레임의 위치로 고정
	키프레임이 최대 키프레임 보다 클 경우 최대 키프레임의 위치로 고정
	두 개의 키프레임 사이 값이 나온 경우 선형 보간을 통해서 위치를 계산한다.
	이동 매트릭스 : 아이덴티디 매트릭스의 _41, _42, _43 값 변경
	*/
	D3DXMatrixIdentity(&mat);
	
	if (m_vecPosTrack.empty())
	{
		mat._41 = m_matLocalTM._41;
		mat._42 = m_matLocalTM._42;
		mat._43 = m_matLocalTM._43;
	}
	else if (nKeyFrame <= m_vecPosTrack.front().f)
	{
		mat._41 = m_vecPosTrack.front().v.x;
		mat._42 = m_vecPosTrack.front().v.y;
		mat._43 = m_vecPosTrack.front().v.z;
	}
	else if (nKeyFrame >= m_vecPosTrack.back().f)
	{
		mat._41 = m_vecPosTrack.back().v.x;
		mat._42 = m_vecPosTrack.back().v.y;
		mat._43 = m_vecPosTrack.back().v.z;
	}
	else
	{
		int nNext;
		for (int i = 0; i < m_vecPosTrack.size(); ++i)
		{
			if (m_vecPosTrack[i].f > nKeyFrame)
			{
				nNext = i;
				break;
			}
		}
		int nPrev = nNext - 1;

		float t = (nKeyFrame - m_vecPosTrack[nPrev].f) / (float)(m_vecPosTrack[nNext].f - m_vecPosTrack[nPrev].f);

		D3DXVECTOR3 v;
		D3DXVec3Lerp(&v, &m_vecPosTrack[nPrev].v, &m_vecPosTrack[nNext].v, t);
		mat._41 = v.x;
		mat._42 = v.y;
		mat._43 = v.z;
	}
}

void cFrame::CalcLocalRotationMatrix(IN int nKeyFrame, OUT D3DXMATRIXA16& mat)
{
	/*
	회전 매트릭스 : D3DxMatrixRotationQuaternion()

	두 개의 키프레임 사이 값이 나온 경우 퀀터니온 보간(회전 보간)을 통해서 위치를 계산한다.
	퀀터니온 보간 함수 : D3DXQuaternionSlerp()
	*/
	D3DXMatrixIdentity(&mat);

	if (m_vecRotTrack.empty())
	{
		mat = m_matLocalTM;
		mat._41 = 0.0f;
		mat._42 = 0.0f;
		mat._43 = 0.0f;
	}
	else if (nKeyFrame <= m_vecRotTrack.front().f)
	{
		D3DXMatrixRotationQuaternion(&mat, &m_vecRotTrack.front().q);
	}
	else if (nKeyFrame >= m_vecRotTrack.back().f)
	{
		D3DXMatrixRotationQuaternion(&mat, &m_vecRotTrack.back().q);
	}
	else
	{
		int nNext;
		for (int i = 0; i < m_vecRotTrack.size(); ++i)
		{
			if (m_vecRotTrack[i].f > nKeyFrame)
			{
				nNext = i;
				break;
			}
		}
		int nPrev = nNext - 1;

		float t = (nKeyFrame - m_vecRotTrack[nPrev].f) / (float)(m_vecRotTrack[nNext].f - m_vecRotTrack[nPrev].f);

		D3DXQUATERNION q;
		D3DXQuaternionSlerp(&q, &m_vecRotTrack[nPrev].q, &m_vecRotTrack[nNext].q, t);
		D3DXMatrixRotationQuaternion(&mat, &q);
	}
}
