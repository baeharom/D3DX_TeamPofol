#pragma once
class cFrustum;
class cQuadTree
{
	/// 쿼드트리에 보관되는 4개의 코너값에 대한 상수값
	enum			CornerType { CORNER_TL, CORNER_TR, CORNER_BL, CORNER_BR };
	/// 이웃노드 처리용 상수값
	enum { EDGE_UP, EDGE_DN, EDGE_LT, EDGE_RT };
	/// 쿼드트리와 프러스텀간의 관계
	enum			QuadLocation {
		FRUSTUM_OUT = 0,				/// 프러스텀에서 완전벗어남
		FRUSTUM_PARTIALLY_IN = 1,	/// 프러스텀에 부분포함
		FRUSTUM_COMPLETELY_IN = 2,	/// 프러스텀에 완전포함
		FRUSTUM_UNKNOWN = -1
	};		/// 모르겠음(^^;)

private:
	cQuadTree*	m_pChild[4];		/// QuadTree의 4개의 자식노드

	cQuadTree*	m_pParent;			/// Triangle Crack(Popping)을 막기위해서 사용한다.
	cQuadTree*	m_pNeighbor[4];		/// Triangle Crack(Popping)을 막기위해서 사용한다.

	int			m_nCenter;			/// QuadTree에 보관할 첫번째 값
	int			m_nCorner[4];		/// QuadTree에 보관할 두번째 값
									///    TopLeft(TL)      TopRight(TR)
									///              0------1
									///              |      |
									///              |      |
									///              2------3
									/// BottomLeft(BL)      BottomRight(BR)
	BOOL		m_bCulled;			/// 프러스텀에서 컬링된 노드인가?
	float		m_fRadius;			/// 노드를 감싸는 경계구(bounding sphere)의 반지름
public:
	/// 최초 루트노드 생성자
	cQuadTree(int cx, int cy);
	/// 하위 자식노드 생성자
	cQuadTree(cQuadTree* pParent);
	~cQuadTree();
	void Release();
private:
	/// 자식 노드를 추가한다.
	cQuadTree*	AddChild(int nCornerTL, int nCornerTR, int nCornerBL, int nCornerBR);
	/// 4개의 코너값을 셋팅한다.
	BOOL		SetCorners(int nCornerTL, int nCornerTR, int nCornerBL, int nCornerBR);
	/// Quadtree를 4개의 하위 트리로 부분분할(subdivide)한다.
	BOOL		SubDivide();		// Quadtree를 subdivide한다.

	/// 출력할 폴리곤의 인덱스를 생성한다.
	int			GenTriIndex(int nTris, LPVOID pIndex, ST_PNT_VERTEX* pHeightMap, float fLODRatio);
	/// 카메라와 현재 노드와의 거리값을 기준으로 LOD값을 구한다.
	int			GetLODLevel(ST_PNT_VERTEX* pHeightMap, float fLODRatio)
	{
		float d = GetDistance((pHeightMap + m_nCenter)->p, g_pCamera->GetvPos());
		return  max((int)(d * fLODRatio), 1);
	}

	/// 현재 노드가 LOD등급으로 볼때  출력이 가능한 노드인가?
	BOOL		IsVisible(ST_PNT_VERTEX* pHeightMap, float fLODRatio)
	{
		return ((m_nCorner[CORNER_TR] - m_nCorner[CORNER_TL]) <= GetLODLevel(pHeightMap,fLODRatio));
	}
	/// 현재노드가 프러스텀에 포함되는가?
	int			IsInFrustum(ST_PNT_VERTEX* pHeightMap);
	/// _IsInFrustum()함수의 결과에 따라 프러스텀 컬링 수행
	void		FrustumCull(ST_PNT_VERTEX* pHeightMap);
	/// 이웃노드를 만든다.(삼각형 찢어짐 방지용)
	void		BuildNeighborNode(cQuadTree* pRoot, ST_PNT_VERTEX* pHeightMap, int cx);
	/// 쿼드트리를 검색해서 4개 코너값과 일치하는 노드를 찾는다.
	cQuadTree*	FindNode(ST_PNT_VERTEX* pHeightMap, int _0, int _1, int _2, int _3);
	/// 4개 방향(상단,하단,좌측,우측)의 이웃노드 인덱스를 구한다.
	int			GetNodeIndex(int ed, int cx, int& _0, int& _1, int& _2, int& _3);
	/// 모든 자식노드의 m_bCulled값을 FALSE로 한다
	void		AllInFrustum();
public:
	/// QuadTree를 구축한다.
	BOOL		Build(ST_PNT_VERTEX* pHeightMap);
	BOOL BuildQuadTree(ST_PNT_VERTEX* pHeightMap);
	///	삼각형의 인덱스를 만들고, 출력할 삼각형의 개수를 반환한다.
	int			GenerateIndex(LPVOID pIndex, ST_PNT_VERTEX* pHeightMap, float _fLODRatio);
};

