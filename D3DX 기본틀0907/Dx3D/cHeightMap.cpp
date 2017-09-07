#include "stdafx.h"
#include "cHeightMap.h"
#include "cQuadTree.h"
#include "cMtlTex.h"

cHeightMap::cHeightMap()
	:m_cxDIB(0), m_cyDIB(0), m_czDIB(0),m_vScale(D3DXVECTOR3(1.f,1.f,1.f)),
	m_pMesh(nullptr), m_pMtlTex(nullptr), m_nTriangles(0), m_arrPNTVertex(nullptr)
	, m_pQuadTree(nullptr), m_fLODRatio(1.f), m_pIB(nullptr)
{
	D3DXMatrixScaling(&m_matWorld,3,3,3);
}


cHeightMap::~cHeightMap()
{
	SAFE_DELETE_ARRAY(m_arrPNTVertex);
	SAFE_RELEASE(m_pMesh);
	SAFE_RELEASE(m_pIB);
	SAFE_DELETE(m_pQuadTree);
}

HRESULT cHeightMap::Init(IN const char* _filePath, IN int _nX, IN int _nZ,IN float _fLODRatio, IN const char* _texPath, IN const D3DXVECTOR3* _pScale)
{
	m_vScale = *_pScale;
	m_cxDIB = _nX;
	m_czDIB = _nZ;
	//_nX 와 _nZ 는 (2^n+1)이어야 한다
	if (FAILED(BuildHeightMap(_filePath))) { Release(); return E_FAIL; }
	if (FAILED(LoadTextures(_texPath))) { Release(); return E_FAIL; }
	if (FAILED(CreateVIBuffer())) { Release(); return E_FAIL; }
	m_pQuadTree = new cQuadTree(m_cxDIB, m_czDIB);
	if (FAILED(BuildQuadTree())) { Release(); return E_FAIL; }
	m_cxDIB *= m_vScale.x;
	m_czDIB *= m_vScale.z;
	m_fLODRatio = _fLODRatio;
	return S_OK;
}

HRESULT cHeightMap::BuildHeightMap(IN const char * _filePath)
{
	m_arrPNTVertex = new ST_PNT_VERTEX[m_cxDIB*m_czDIB];
	FILE* fp = NULL;
	fopen_s(&fp, _filePath, "rb");
	for (int z = 0; z < m_czDIB; z++)
	{
		for (int x = 0; x < m_cxDIB; x++)
		{
			int nIndex = x + z * m_czDIB;
			m_arrPNTVertex[nIndex].p.x = (float)((x - m_cxDIB / 2)*m_vScale.x);
			m_arrPNTVertex[nIndex].p.y= ((unsigned char)fgetc(fp)) / 10.0f*m_vScale.y;
			m_arrPNTVertex[nIndex].p.z = -(float)((z - m_czDIB / 2)*m_vScale.z);
			D3DXVec3Normalize(&m_arrPNTVertex[nIndex].n, &m_arrPNTVertex[nIndex].p);
			//m_arrPNTVertex[nIndex].n = D3DXVECTOR3(0, 1, 0);
			m_arrPNTVertex[nIndex].t.x = (float)x / (float)(m_cxDIB - 1);
			m_arrPNTVertex[nIndex].t.y = (float)z / (float)(m_czDIB - 1);
		}
	}
	fclose(fp);
	return S_OK;
}

HRESULT cHeightMap::LoadTextures(IN const char * _texPath)
{
	D3DMATERIAL9 stMtl = WHITE_MTRL;

	m_pMtlTex = new cMtlTex;
	m_pMtlTex->AutoRelease();
	m_pMtlTex->SetMtl(stMtl);
	m_pMtlTex->SetTexture(g_pTextureManager->AddTexture(_texPath, _texPath));
	return S_OK;
}

HRESULT cHeightMap::CreateVIBuffer()
{
	D3DXCreateMeshFVF((m_cxDIB - 1)*(m_czDIB - 1)*2, m_cxDIB*m_czDIB, D3DXMESH_MANAGED | D3DXMESH_32BIT,
		ST_PNT_VERTEX::FVF, g_pD3DDevice, &m_pMesh);

	m_pMesh->GetIndexBuffer(&m_pIB);

	ST_PNT_VERTEX* pV = NULL;
	m_pMesh->LockVertexBuffer(NULL, (void**)&pV);
	memcpy(pV, m_arrPNTVertex, m_cxDIB*m_czDIB * sizeof(ST_PNT_VERTEX));
	m_pMesh->UnlockVertexBuffer();

	// 3. 속성 버퍼 채우기: 텍스쳐가 1개이기 때문에 면의 갯수 만큼 0으로 셋팅 해준다.
	DWORD* pA = NULL;
	m_pMesh->LockAttributeBuffer(NULL, &pA);
	ZeroMemory(pA, (m_cxDIB - 1)*(m_czDIB - 1) * 2*sizeof(DWORD));
	m_pMesh->UnlockAttributeBuffer();

	// 4. 최적화
	vector<DWORD> vecAdjBuf(m_pMesh->GetNumFaces() * 3);
	m_pMesh->GenerateAdjacency(D3DX_16F_EPSILON, &vecAdjBuf[0]);
	m_pMesh->OptimizeInplace(D3DXMESHOPT_COMPACT | D3DXMESHOPT_ATTRSORT | D3DXMESHOPT_VERTEXCACHE,
		&vecAdjBuf[0], NULL, NULL, NULL);

	return S_OK;
}

HRESULT cHeightMap::BuildQuadTree()
{
	m_pQuadTree->Build(m_arrPNTVertex);
	return S_OK;
}

void cHeightMap::Load(IN char * szFilePath, IN D3DXMATRIXA16 * pMat)
{
}

bool cHeightMap::GetHeight(IN const float & x, OUT float & y, IN const float & z)
{

	float X = x + m_cxDIB/2;
	float Z = z + m_czDIB/2;

	if (X< 0 || X >= m_cxDIB || Z < 0 || Z >= m_czDIB)
		return false;
	//1--3
	//|\ |
	//| \|
	//0--2
	// 캐릭터가 서있는 타일 에서의 0번의 x, z 인덱스 번호
	Z = m_czDIB -1- Z;
	int nX=(int)X / m_vScale.x;
	int nZ= (int)Z / m_vScale.z;

	// 0번 위치 좌표로 부터의 캐릭터 위치
	float fDeltaX = X - nX;
	float fDeltaZ = Z - nZ;

	int xSize = m_cxDIB / m_vScale.x;

	int _0 = (nZ + 0) * xSize + nX + 0;
	int _1 = (nZ + 1) * xSize + nX + 0;
	int _2 = (nZ + 0) * xSize + nX + 1;
	int _3 = (nZ + 1) * xSize + nX + 1;

	if (fDeltaX + fDeltaZ < m_vScale.x)	// 아래쪽 삼각형에 있을 경우
	{
		float zY = m_arrPNTVertex[_1].p.y - m_arrPNTVertex[_0].p.y;
		float xY = m_arrPNTVertex[_2].p.y - m_arrPNTVertex[_0].p.y;
		y = m_arrPNTVertex[_0].p.y + zY * fDeltaZ + xY * fDeltaX;
	}
	else // 윗쪽 삼각형에 있을 경우
	{
		fDeltaX = m_vScale.x - fDeltaX;
		fDeltaZ = m_vScale.x - fDeltaZ;

		float xY = m_arrPNTVertex[_1].p.y - m_arrPNTVertex[_3].p.y;
		float zY = m_arrPNTVertex[_2].p.y - m_arrPNTVertex[_3].p.y;
		y = m_arrPNTVertex[_3].p.y + zY * fDeltaZ + xY * fDeltaX;
	}

	return true;
}

vector<D3DXVECTOR3>& cHeightMap::GetVertex()
{
	// TODO: 여기에 반환 구문을 삽입합니다.
	vector<D3DXVECTOR3> v;
	return v;
}

vector<DWORD>& cHeightMap::GetIndex()
{
	// TODO: 여기에 반환 구문을 삽입합니다.
	vector<DWORD> v;
	return v;
}


HRESULT cHeightMap::Render()
{
	LPDWORD		pI;
	if (FAILED(m_pIB->Lock(0, (m_cxDIB - 1)*(m_czDIB - 1) * 6 * sizeof(DWORD), (void**)&pI, 0)))
		return E_FAIL;
	m_nTriangles = m_pQuadTree->GenerateIndex(pI, m_arrPNTVertex,m_fLODRatio);

	m_pIB->Unlock();

	g_pD3DDevice->SetTransform(D3DTS_WORLD, &m_matWorld);

	g_pD3DDevice->SetMaterial(&m_pMtlTex->GetMtl());
	g_pD3DDevice->SetTexture(0, m_pMtlTex->GetTexture());
	m_pMesh->DrawSubset(0);

	return S_OK;
}
