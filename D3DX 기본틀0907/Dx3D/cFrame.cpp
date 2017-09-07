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
	// �� ������ ����TM�� ����Ѵ�.
	// NODE_TM ������ �θ��� TM�������� ������ �� ��Ʈ���� �̴�.
	// �׷��� NODE_TM���� �θ��� ������ ���� ��Ű�� �� ������ ����TM�� �ȴ�.
	// ��������� ��� �ڽĵ��� ����TM�� ����� �ش�.

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
	// ����TM�� ����Ѵ�.
	// ����T : CalcLocalTranslationMatrix()
	// ����R : CalcLocalRotationMatrix()
	// ����TM = R * T
	// �θ� ���� �� ���: ����TM = ����TM * �θ�TM
	// ��� �ڽĵ��� ��������� ������.

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
	Ű�������� �ּ� Ű������ ���� ���� ��� �ּ� Ű�������� ��ġ�� ����
	Ű�������� �ִ� Ű������ ���� Ŭ ��� �ִ� Ű�������� ��ġ�� ����
	�� ���� Ű������ ���� ���� ���� ��� ���� ������ ���ؼ� ��ġ�� ����Ѵ�.
	�̵� ��Ʈ���� : ���̵�Ƽ�� ��Ʈ������ _41, _42, _43 �� ����
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
	ȸ�� ��Ʈ���� : D3DxMatrixRotationQuaternion()

	�� ���� Ű������ ���� ���� ���� ��� ���ʹϿ� ����(ȸ�� ����)�� ���ؼ� ��ġ�� ����Ѵ�.
	���ʹϿ� ���� �Լ� : D3DXQuaternionSlerp()
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
