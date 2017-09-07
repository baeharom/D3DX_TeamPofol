#pragma once
#include "iMap.h"
class cMtlTex;
class cQuadTree;
class cHeightMap:public iMap,public cObject
{
private:
	D3DXMATRIXA16		m_matWorld;
	int						m_cxDIB;		/// DIB�� �����ȼ���
	int						m_czDIB;		/// DIB�� �����ȼ���
	int						m_cyDIB;		/// DIB�� �ִ���̰�(�� 0 ~ 255������ ��)
	D3DXVECTOR3				m_vScale;		/// x scale, y scale, z scale
	ST_PNT_VERTEX*		m_arrPNTVertex;	/// ���̸��� �����迭
	LPD3DXMESH			m_pMesh;
	cMtlTex*			m_pMtlTex;
	
	int						m_nTriangles;	/// ����� �ﰢ���� ����
	cQuadTree*				m_pQuadTree;	/// ����Ʈ�� ��ü�� ������
	LPDIRECT3DINDEXBUFFER9 m_pIB;
	float					m_fLODRatio;	/// LODó���ÿ� ���� ��

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

