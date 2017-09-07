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
	// 각 프레임의 메시 컨테이너에 있는 pSkinInfo를 이용하여 영향받는 모든 
	// 프레임의 매트릭스를 ppBoneMatrixPtrs에 연결한다.
	// pSkinInfo->GetNumBones() 으로 영향받는 본의 개수를 찾음.
	// pSkinInfo->GetBoneName(i) 로 i번 프레임의 이름을 찾음
	// D3DXFrameFind(루트 프레임, 프레임 이름) 로 프레임을 찾음.
	// 찾아서 월드매트릭스를 걸어줘라.
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

	//재귀적으로 모든 프레임에 대해서 실행.
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
	1.다음 애니메이션을 준비한다.
	2.0번 트랙에 있는 애니메이션 정보를 1번 트랙에 설정한다.
	3.0번 트랙의 정보를 1번 트랙에 설정한다.
	4.가중치를 설정한다.
	5.0번 트랙에 다음 애니메이션을 셋팅한다.
	6.블렌딩 시간에 따른 가중치 값을 계산한다.
	7.계산 된 가중치를 0번과 1번 트랙에 적용 시킨다
	8.블렌딩이 끝나면 1번 트랙을 비활성화 시킨다.
	*/
	/*
	GetTrackAnimationSet()			// 애니메이션 트랙 가져오기
	SetTrackAnimationSet()			// 애니메이션 트랙 설정하기

	GetTrackDesc()					// 애니메이션 트랙 정보 가져오기
	SetTrackDesc()					// 애니메이션 트랙 정보 셋팅하기

	SetTrackWeight()				// 애니메이션 가중치 설정하기
	SetTrackEnable()				// 애니메이션 활성화 설정
	*/
}

void cSkinnedMesh::Update(LPD3DXFRAME pFrame, LPD3DXFRAME pParent)
{
	// 2.
	// 스킨드 메쉬의 월드 매트릭스(컴바인드 트랜스폼 매트릭스) 업데이트
	ST_BONE* pBone = (ST_BONE*)pFrame;

	pBone->CombinedTransformationMatrix = pBone->TransformationMatrix;

	if (pParent)
		pBone->CombinedTransformationMatrix *= ((ST_BONE*)pParent)->CombinedTransformationMatrix;

	//재귀적으로 모든 프레임에 대해서 실행(부모 설정 제대로 해야함)
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
			// 영향을 주는 매트릭스 갯수 pSkinInfo->GetNumBones();
			// pCurrentBoneMatrices 를 계산하시오
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

			// 메쉬 데이터 업데이트
			pSkinInfo->UpdateSkinnedMesh(pBoneMesh->pCurrBoneMatrices, NULL, src, dest);

			pBoneMesh->pWorkMesh->UnlockVertexBuffer();
			pBoneMesh->pOrigMesh->UnlockVertexBuffer();
		}
	}

	//재귀적으로 모든 프레임에 대해서 실행.
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
			// 커스텀 컨테이너로 접근 해야 한다.(워크 메쉬를 사용 해야함)
			ST_BONE* pBone = (ST_BONE*)pFrame;
			ST_BONE_MESH* pBoneMesh = (ST_BONE_MESH*)pFrame->pMeshContainer;

			for (int i = 0; i < pBoneMesh->NumMaterials; ++i)
			{
				LPD3DXMATERIAL pMtlTex = &pBoneMesh->pMaterials[i];

				g_pD3DDevice->SetTransform(D3DTS_WORLD, &matW);
				g_pD3DDevice->SetMaterial(&pMtlTex->MatD3D);
				g_pD3DDevice->SetTexture(0, g_pTextureManager->GetTexture(pMtlTex->pTextureFilename));

				// 5.
				// pMesh->DrawSubset(i); => 애니메이션이 적용 된 워크 메쉬로 그려라
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
		// 총 애니메이션 개수를 사용하여 인덱스 번호를 설정한다.
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
