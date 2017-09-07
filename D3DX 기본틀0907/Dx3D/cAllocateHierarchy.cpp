#include "stdafx.h"
#include "cAllocateHierarchy.h"


cAllocateHierarchy::cAllocateHierarchy()
{
}


cAllocateHierarchy::~cAllocateHierarchy()
{
}

STDMETHODIMP cAllocateHierarchy::CreateFrame(THIS_ LPCSTR Name, LPD3DXFRAME *ppNewFrame)
{
	/*
	커스텀 프레임(본)으로 교체 - 이름 설정, 매트릭스 초기화
	*/

	ST_BONE* pFrame = new ST_BONE;

	if (Name)
	{
		pFrame->Name = new char[strlen(Name) + 1];
		strcpy(pFrame->Name, Name);
	}
	else
	{
		pFrame->Name = NULL;
	}

	D3DXMatrixIdentity(&pFrame->TransformationMatrix);
	D3DXMatrixIdentity(&pFrame->CombinedTransformationMatrix);

	*ppNewFrame = pFrame;

	return S_OK;
}

STDMETHODIMP cAllocateHierarchy::CreateMeshContainer(THIS_ LPCSTR Name, CONST D3DXMESHDATA *pMeshData, CONST D3DXMATERIAL *pMaterials, CONST D3DXEFFECTINSTANCE *pEffectInstances, DWORD NumMaterials, CONST DWORD *pAdjacency, LPD3DXSKININFO pSkinInfo, LPD3DXMESHCONTAINER *ppNewMeshContainer)
{
	/*
	메쉬 컨테이너 채우기
	1. 커스텀 컨테이너(본메쉬)로 교체
	2. 오리지널 메쉬, 워크 메쉬 저장 및 복사
	*/

	ST_BONE_MESH* pBoneMesh = new ST_BONE_MESH;

	pBoneMesh->Name = new char[strlen(Name) + 1];
	strcpy(pBoneMesh->Name, Name);

	// step 1. 메쉬 복사
	SAFE_ADDREF(pMeshData->pMesh);
	pBoneMesh->pOrigMesh = pMeshData->pMesh;
	pBoneMesh->pOrigMesh->CloneMeshFVF(pBoneMesh->pOrigMesh->GetOptions(), pBoneMesh->pOrigMesh->GetFVF(), g_pD3DDevice, &pBoneMesh->pWorkMesh);

	pBoneMesh->pMaterials = new D3DXMATERIAL[NumMaterials];
	for (int i = 0; i < NumMaterials; ++i)
	{
		string filePath(pMaterials[i].pTextureFilename);
		string fullPath = m_sFolder + "\\" + filePath;

		g_pTextureManager->AddTexture(pMaterials[i].pTextureFilename, fullPath);
		pBoneMesh->pMaterials[i].MatD3D = pMaterials[i].MatD3D;
		pBoneMesh->pMaterials[i].pTextureFilename = new char[strlen(pMaterials[i].pTextureFilename) + 1];
		strcpy(pBoneMesh->pMaterials[i].pTextureFilename, pMaterials[i].pTextureFilename);
	}

	pBoneMesh->pEffects = NULL;

	pBoneMesh->NumMaterials = NumMaterials;

	pBoneMesh->pAdjacency = NULL;

	// step 2. pSkinInfo 저장
	SAFE_ADDREF(pSkinInfo);
	pBoneMesh->pSkinInfo = pSkinInfo;

	// step 3. pSkinInfo->GetNumBones()를 통해
	// 영향력을 미치는 모든 본에 대한 매트릭스 들을 세팅
	// ppBoneMatrixPtrs, pBoneOffsetMatrices, pCurrentBoneMatrices를 동적할당
	DWORD dwNumBones = pSkinInfo->GetNumBones();
	pBoneMesh->ppBoneMatrixPtrs = new D3DXMATRIX*[dwNumBones];
	pBoneMesh->pBoneOffsetMatrices = new D3DXMATRIX[dwNumBones];
	pBoneMesh->pCurrBoneMatrices = new D3DXMATRIX[dwNumBones];

	// step 4. 동적 할당된 pBoneOffsetMatrices 매트릭스에 값 저장.
	// pSkinInfo->GetBoneOffsetMatrix(i)
	for (DWORD i = 0; i < dwNumBones; ++i)
	{
		pBoneMesh->pBoneOffsetMatrices[i] = *(pSkinInfo->GetBoneOffsetMatrix(i));
	}

	*ppNewMeshContainer = pBoneMesh;

	return S_OK;
}

STDMETHODIMP cAllocateHierarchy::DestroyFrame(THIS_ LPD3DXFRAME pFrameToFree)
{
	// 프레임 해제
	SAFE_DELETE_ARRAY(pFrameToFree->Name);
	SAFE_DELETE(pFrameToFree);

	return S_OK;
}

STDMETHODIMP cAllocateHierarchy::DestroyMeshContainer(THIS_ LPD3DXMESHCONTAINER pMeshContainerToFree)
{
	// 컨테이너 해제
	ST_BONE_MESH* pBoneMesh = (ST_BONE_MESH*)pMeshContainerToFree;
	if (pBoneMesh == NULL)
		return E_FAIL;

	SAFE_RELEASE(pBoneMesh->pOrigMesh);
	SAFE_RELEASE(pBoneMesh->pWorkMesh);

	for (int i = 0; i < pBoneMesh->NumMaterials; ++i)
	{
		SAFE_DELETE_ARRAY(pBoneMesh->pMaterials[i].pTextureFilename);
	}
	SAFE_DELETE_ARRAY(pBoneMesh->pMaterials);

	SAFE_DELETE_ARRAY(pBoneMesh->ppBoneMatrixPtrs);
	SAFE_DELETE_ARRAY(pBoneMesh->pBoneOffsetMatrices);
	SAFE_DELETE_ARRAY(pBoneMesh->pCurrBoneMatrices);

	SAFE_RELEASE(pBoneMesh->pSkinInfo);

	SAFE_DELETE(pBoneMesh);

	return S_OK;
}
