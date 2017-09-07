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
	Ŀ���� ������(��)���� ��ü - �̸� ����, ��Ʈ���� �ʱ�ȭ
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
	�޽� �����̳� ä���
	1. Ŀ���� �����̳�(���޽�)�� ��ü
	2. �������� �޽�, ��ũ �޽� ���� �� ����
	*/

	ST_BONE_MESH* pBoneMesh = new ST_BONE_MESH;

	pBoneMesh->Name = new char[strlen(Name) + 1];
	strcpy(pBoneMesh->Name, Name);

	// step 1. �޽� ����
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

	// step 2. pSkinInfo ����
	SAFE_ADDREF(pSkinInfo);
	pBoneMesh->pSkinInfo = pSkinInfo;

	// step 3. pSkinInfo->GetNumBones()�� ����
	// ������� ��ġ�� ��� ���� ���� ��Ʈ���� ���� ����
	// ppBoneMatrixPtrs, pBoneOffsetMatrices, pCurrentBoneMatrices�� �����Ҵ�
	DWORD dwNumBones = pSkinInfo->GetNumBones();
	pBoneMesh->ppBoneMatrixPtrs = new D3DXMATRIX*[dwNumBones];
	pBoneMesh->pBoneOffsetMatrices = new D3DXMATRIX[dwNumBones];
	pBoneMesh->pCurrBoneMatrices = new D3DXMATRIX[dwNumBones];

	// step 4. ���� �Ҵ�� pBoneOffsetMatrices ��Ʈ������ �� ����.
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
	// ������ ����
	SAFE_DELETE_ARRAY(pFrameToFree->Name);
	SAFE_DELETE(pFrameToFree);

	return S_OK;
}

STDMETHODIMP cAllocateHierarchy::DestroyMeshContainer(THIS_ LPD3DXMESHCONTAINER pMeshContainerToFree)
{
	// �����̳� ����
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
