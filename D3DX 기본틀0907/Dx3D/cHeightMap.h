#pragma once
#include "iMap.h"
class cMtlTex;
class cQuadTree;
class cHeightMap:public iMap,public cObject
{
private:
	D3DXMATRIXA16		m_matWorld;
	int						m_cxDIB;		/// DIB의 가로픽셀수
	int						m_czDIB;		/// DIB의 세로픽셀수
	int						m_cyDIB;		/// DIB의 최대높이값(즉 0 ~ 255사이의 값)
	D3DXVECTOR3				m_vScale;		/// x scale, y scale, z scale
	ST_PNT_VERTEX*		m_arrPNTVertex;	/// 높이맵의 정점배열
	LPD3DXMESH			m_pMesh;
	cMtlTex*			m_pMtlTex;
	
	int						m_nTriangles;	/// 출력할 삼각형의 개수
	cQuadTree*				m_pQuadTree;	/// 쿼드트리 객체의 포인터
	LPDIRECT3DINDEXBUFFER9 m_pIB;
	float					m_fLODRatio;	/// LOD처리시에 사용될 값

public:
	cHeightMap();
	virtual ~cHeightMap();

	HRESULT Init(IN const char* _filePath,IN int _nX,IN int _nZ,IN float _fLODRatio,IN const char* _texPath,IN const D3DXVECTOR3* _pScale);
	HRESULT BuildHeightMap(IN const char* _filePath);
	HRESULT	LoadTextures(IN const char* _texPath);
	HRESULT CreateVIBuffer();
	HRESULT BuildQuadTree();

	virtual void Load(IN char* szFilePath, IN D3DXMATRIXA16* pMat = NULL) override;
	virtual bool GetHeight(IN const float& x, OUT float& y, IN const float& z) override;
	virtual vector<D3DXVECTOR3>& GetVertex() override;
	virtual vector<DWORD>& GetIndex() override;

	HRESULT Render();
};

