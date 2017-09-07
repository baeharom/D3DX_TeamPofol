#include "stdafx.h"
#include "cSkinnedMesh.h"
#include "cAllocateHierarchy.h"


cSkinnedMesh::cSkinnedMesh()
	: m_pRoot(NULL)
	, m_pAnimController(NULL),m_bBlending(false),m_fActionTime(0.f),m_fBlendTime(1.f)
{
}


cSkinnedMesh::~cSkinnedMesh()
{
	cAllocateHierarchy alloc;
	D3DXFrameDestroy(m_pRoot, &alloc);

	SAFE_RELEASE(m_pAnimController);
}

void cSkinnedMesh::Load(char* szFolder, char* szFile)
{
	string sFolder(szFolder);
	string sFile(szFile);
	string sFullPath = sFolder + "\\" + sFile;

	cAllocateHierarchy alloc;
	alloc.SetFolder(sFolder);

	D3DXLoadMeshHierarchyFromX(sFullPath.c_str(), D3DXMESH_MANAGED, g_pD3DDevice,
		&alloc, NULL, &m_pRoot, &m_pAnimController);
	SetupBoneMatrixPtrs(m_pRoot);
}

void cSkinnedMesh::SetupBoneMatrixPtrs(LPD3DXFRAME pFrame)
{
	// 1.
	// �� �������� �޽� �����̳ʿ� �ִ� pSkinInfo�� �̿��Ͽ� ����޴� ��� 
	// �������� ��Ʈ������ ppBoneMatrixPtrs�� �����Ѵ�.
	// pSkinInfo->GetNumBones() ���� ����޴� ���� ������ ã��.
	// pSkinInfo->GetBoneName(i) �� i�� �������� �̸��� ã��
	// D3DXFrameFind(��Ʈ ������, ������ �̸�) �� �������� ã��.
	// ã�Ƽ� �����Ʈ������ �ɾ����.
	if (pFrame == NULL) return;

	ST_BONE* pBone = (ST_BONE*)pFrame;
	if (pBone->pMeshContainer)
	{
		ST_BONE_MESH* pBoneMesh = (ST_BONE_MESH*)pBone->pMeshContainer;
		if (pBoneMesh->pSkinInfo)
		{
			LPD3DXSKININFO pSkinInfo = pBoneMesh->pSkinInfo;
			DWORD dwNumBone = pBoneMesh->pSkinInfo->GetNumBones();

			for (DWORD i = 0; i < dwNumBone; ++i)
			{
				ST_BONE* p = (ST_BONE*)D3DXFrameFind(m_pRoot, pSkinInfo->GetBoneName(i));
				pBoneMesh->ppBoneMatrixPtrs[i] = &(p->CombinedTransformationMatrix);
			}
		}
	}

	//��������� ��� �����ӿ� ���ؼ� ����.
	if (pFrame->pFrameSibling)
		SetupBoneMatrixPtrs(pFrame->pFrameSibling);

	if (pFrame->pFrameFirstChild)
		SetupBoneMatrixPtrs(pFrame->pFrameFirstChild);
}

void cSkinnedMesh::Update()
{
	float fDeltaTime = g_pTimeManager->GetDeltaTime();
	m_pAnimController->AdvanceTime(fDeltaTime, NULL);

	Update(m_pRoot, NULL);
	UpdateSkinnedMesh(m_pRoot);

	//LPD3DXANIMATIONSET aniSet;
	//m_pAnimController->GetTrackAnimationSet(0, &aniSet);



	AnimationBlending();
	/*
	1.���� �ִϸ��̼��� �غ��Ѵ�.
	2.0�� Ʈ���� �ִ� �ִϸ��̼� ������ 1�� Ʈ���� �����Ѵ�.
	3.0�� Ʈ���� ������ 1�� Ʈ���� �����Ѵ�.
	4.����ġ�� �����Ѵ�.
	5.0�� Ʈ���� ���� �ִϸ��̼��� �����Ѵ�.
	6.���� �ð��� ���� ����ġ ���� ����Ѵ�.
	7.��� �� ����ġ�� 0���� 1�� Ʈ���� ���� ��Ų��
	8.������ ������ 1�� Ʈ���� ��Ȱ��ȭ ��Ų��.
	*/
	/*
	GetTrackAnimationSet()			// �ִϸ��̼� Ʈ�� ��������
	SetTrackAnimationSet()			// �ִϸ��̼� Ʈ�� �����ϱ�

	GetTrackDesc()					// �ִϸ��̼� Ʈ�� ���� ��������
	SetTrackDesc()					// �ִϸ��̼� Ʈ�� ���� �����ϱ�

	SetTrackWeight()				// �ִϸ��̼� ����ġ �����ϱ�
	SetTrackEnable()				// �ִϸ��̼� Ȱ��ȭ ����
	*/
}

void cSkinnedMesh::Update(LPD3DXFRAME pFrame, LPD3DXFRAME pParent)
{
	// 2.
	// ��Ų�� �޽��� ���� ��Ʈ����(�Ĺ��ε� Ʈ������ ��Ʈ����) ������Ʈ
	ST_BONE* pBone = (ST_BONE*)pFrame;

	pBone->CombinedTransformationMatrix = pBone->TransformationMatrix;

	if (pParent)
		pBone->CombinedTransformationMatrix *= ((ST_BONE*)pParent)->CombinedTransformationMatrix;

	//��������� ��� �����ӿ� ���ؼ� ����(�θ� ���� ����� �ؾ���)
	if (pBone->pFrameSibling)
		Update(pBone->pFrameSibling, pParent);

	if (pBone->pFrameFirstChild)
		Update(pBone->pFrameFirstChild, pBone);
}

void cSkinnedMesh::UpdateSkinnedMesh(LPD3DXFRAME pFrame)
{
	if (pFrame == NULL) return;

	ST_BONE* pBone = (ST_BONE*)pFrame;
	if (pBone->pMeshContainer)
	{
		ST_BONE_MESH* pBoneMesh = (ST_BONE_MESH*)pBone->pMeshContainer;
		if (pBoneMesh->pSkinInfo)
		{
			LPD3DXSKININFO pSkinInfo = pBoneMesh->pSkinInfo;
			DWORD dwNumBones = pSkinInfo->GetNumBones();
			// 3.
			// ������ �ִ� ��Ʈ���� ���� pSkinInfo->GetNumBones();
			// pCurrentBoneMatrices �� ����Ͻÿ�
			// pCurrentBoneMatrices = pBoneOffsetMatrices * ppBoneMatrixPtrs
			for (DWORD i = 0; i < dwNumBones; ++i)
			{
				pBoneMesh->pCurrBoneMatrices[i] =
					pBoneMesh->pBoneOffsetMatrices[i] * *(pBoneMesh->ppBoneMatrixPtrs[i]);
			}

			BYTE* src = NULL;
			BYTE* dest = NULL;

			pBoneMesh->pOrigMesh->LockVertexBuffer(D3DLOCK_READONLY, (void**)&src);
			pBoneMesh->pWorkMesh->LockVertexBuffer(0, (void**)&dest);

			// �޽� ������ ������Ʈ
			pSkinInfo->UpdateSkinnedMesh(pBoneMesh->pCurrBoneMatrices, NULL, src, dest);

			pBoneMesh->pWorkMesh->UnlockVertexBuffer();
			pBoneMesh->pOrigMesh->UnlockVertexBuffer();
		}
	}

	//��������� ��� �����ӿ� ���ؼ� ����.
	if (pFrame->pFrameSibling)
		UpdateSkinnedMesh(pFrame->pFrameSibling);

	if (pFrame->pFrameFirstChild)
		UpdateSkinnedMesh(pFrame->pFrameFirstChild);
}

void cSkinnedMesh::Render(LPD3DXFRAME pFrame /*= NULL*/)
{
	D3DXMATRIXA16 matW;
	D3DXMatrixIdentity(&matW);

	if (pFrame == NULL)
	{
		pFrame = m_pRoot;
	}

	if (pFrame)
	{
		if (pFrame->pMeshContainer)
		{
			// 4.
			// Ŀ���� �����̳ʷ� ���� �ؾ� �Ѵ�.(��ũ �޽��� ��� �ؾ���)
			ST_BONE* pBone = (ST_BONE*)pFrame;
			ST_BONE_MESH* pBoneMesh = (ST_BONE_MESH*)pFrame->pMeshContainer;

			for (int i = 0; i < pBoneMesh->NumMaterials; ++i)
			{
				LPD3DXMATERIAL pMtlTex = &pBoneMesh->pMaterials[i];

				g_pD3DDevice->SetTransform(D3DTS_WORLD, &matW);
				g_pD3DDevice->SetMaterial(&pMtlTex->MatD3D);
				g_pD3DDevice->SetTexture(0, g_pTextureManager->GetTexture(pMtlTex->pTextureFilename));

				// 5.
				// pMesh->DrawSubset(i); => �ִϸ��̼��� ���� �� ��ũ �޽��� �׷���
				pBoneMesh->pWorkMesh->DrawSubset(i);
			}
		}

		if (pFrame->pFrameSibling)
		{
			Render(pFrame->pFrameSibling);
		}

		if (pFrame->pFrameFirstChild)
		{
			Render(pFrame->pFrameFirstChild);
		}
	}
}

void cSkinnedMesh::AnimationBlending()
{
	static int nNextIndex = 0;

	if (GetAsyncKeyState(VK_SPACE) & 0x0001)
	{
		LPD3DXANIMATIONSET pNextAnimSet, pZeroAni;
		// �� �ִϸ��̼� ������ ����Ͽ� �ε��� ��ȣ�� �����Ѵ�.
		++nNextIndex %= m_pAnimController->GetMaxNumAnimationSets();
		m_pAnimController->GetAnimationSet(nNextIndex, &pNextAnimSet);
		m_bBlending = true;
		m_pAnimController->GetTrackAnimationSet(0, &pZeroAni);
		m_pAnimController->SetTrackAnimationSet(1, pZeroAni);
		m_pAnimController->SetTrackEnable(1, true);

		D3DXTRACK_DESC stTrackDesc;
		m_pAnimController->GetTrackDesc(0, &stTrackDesc);
		m_pAnimController->SetTrackDesc(1, &stTrackDesc);

		m_pAnimController->SetTrackAnimationSet(0, pNextAnimSet);

		SAFE_RELEASE(pNextAnimSet);
	}
	if (!m_bBlending)return;
	m_fActionTime += g_pTimeManager->GetDeltaTime();
	if (m_fActionTime >= m_fBlendTime)
	{
		m_fActionTime = 0.f;
		m_pAnimController->SetTrackWeight(0, 1);
		m_pAnimController->SetTrackWeight(1, 0);
		m_pAnimController->SetTrackEnable(1, false);
		return;
	}
	float t = m_fActionTime / m_fBlendTime;
	
	m_pAnimController->SetTrackWeight(0, t);
	m_pAnimController->SetTrackWeight(1, 1-t);

}
