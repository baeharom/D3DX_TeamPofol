#pragma once
class cFrustum;
class cQuadTree
{
	/// ����Ʈ���� �����Ǵ� 4���� �ڳʰ��� ���� �����
	enum			CornerType { CORNER_TL, CORNER_TR, CORNER_BL, CORNER_BR };
	/// �̿���� ó���� �����
	enum { EDGE_UP, EDGE_DN, EDGE_LT, EDGE_RT };
	/// ����Ʈ���� �������Ұ��� ����
	enum			QuadLocation {
		FRUSTUM_OUT = 0,				/// �������ҿ��� �������
		FRUSTUM_PARTIALLY_IN = 1,	/// �������ҿ� �κ�����
		FRUSTUM_COMPLETELY_IN = 2,	/// �������ҿ� ��������
		FRUSTUM_UNKNOWN = -1
	};		/// �𸣰���(^^;)

private:
	cQuadTree*	m_pChild[4];		/// QuadTree�� 4���� �ڽĳ��

	cQuadTree*	m_pParent;			/// Triangle Crack(Popping)�� �������ؼ� ����Ѵ�.
	cQuadTree*	m_pNeighbor[4];		/// Triangle Crack(Popping)�� �������ؼ� ����Ѵ�.

	int			m_nCenter;			/// QuadTree�� ������ ù��° ��
	int			m_nCorner[4];		/// QuadTree�� ������ �ι�° ��
									///    TopLeft(TL)      TopRight(TR)
									///              0------1
									///              |      |
									///              |      |
									///              2------3
									/// BottomLeft(BL)      BottomRight(BR)
	BOOL		m_bCulled;			/// �������ҿ��� �ø��� ����ΰ�?
	float		m_fRadius;			/// ��带 ���δ� ��豸(bounding sphere)�� ������
public:
	/// ���� ��Ʈ��� ������
	cQuadTree(int cx, int cy);
	/// ���� �ڽĳ�� ������
	cQuadTree(cQuadTree* pParent);
	~cQuadTree();
	void Release();
private:
	/// �ڽ� ��带 �߰��Ѵ�.
	cQuadTree*	AddChild(int nCornerTL, int nCornerTR, int nCornerBL, int nCornerBR);
	/// 4���� �ڳʰ��� �����Ѵ�.
	BOOL		SetCorners(int nCornerTL, int nCornerTR, int nCornerBL, int nCornerBR);
	/// Quadtree�� 4���� ���� Ʈ���� �κк���(subdivide)�Ѵ�.
	BOOL		SubDivide();		// Quadtree�� subdivide�Ѵ�.

	/// ����� �������� �ε����� �����Ѵ�.
	int			GenTriIndex(int nTris, LPVOID pIndex, ST_PNT_VERTEX* pHeightMap, float fLODRatio);
	/// ī�޶�� ���� ������ �Ÿ����� �������� LOD���� ���Ѵ�.
	int			GetLODLevel(ST_PNT_VERTEX* pHeightMap, float fLODRatio)
	{
		float d = GetDistance((pHeightMap + m_nCenter)->p, g_pCamera->GetvPos());
		return  max((int)(d * fLODRatio), 1);
	}

	/// ���� ��尡 LOD������� ����  ����� ������ ����ΰ�?
	BOOL		IsVisible(ST_PNT_VERTEX* pHeightMap, float fLODRatio)
	{
		return ((m_nCorner[CORNER_TR] - m_nCorner[CORNER_TL]) <= GetLODLevel(pHeightMap,fLODRatio));
	}
	/// �����尡 �������ҿ� ���ԵǴ°�?
	int			IsInFrustum(ST_PNT_VERTEX* pHeightMap);
	/// _IsInFrustum()�Լ��� ����� ���� �������� �ø� ����
	void		FrustumCull(ST_PNT_VERTEX* pHeightMap);
	/// �̿���带 �����.(�ﰢ�� ������ ������)
	void		BuildNeighborNode(cQuadTree* pRoot, ST_PNT_VERTEX* pHeightMap, int cx);
	/// ����Ʈ���� �˻��ؼ� 4�� �ڳʰ��� ��ġ�ϴ� ��带 ã�´�.
	cQuadTree*	FindNode(ST_PNT_VERTEX* pHeightMap, int _0, int _1, int _2, int _3);
	/// 4�� ����(���,�ϴ�,����,����)�� �̿���� �ε����� ���Ѵ�.
	int			GetNodeIndex(int ed, int cx, int& _0, int& _1, int& _2, int& _3);
	/// ��� �ڽĳ���� m_bCulled���� FALSE�� �Ѵ�
	void		AllInFrustum();
public:
	/// QuadTree�� �����Ѵ�.
	BOOL		Build(ST_PNT_VERTEX* pHeightMap);
	BOOL BuildQuadTree(ST_PNT_VERTEX* pHeightMap);
	///	�ﰢ���� �ε����� �����, ����� �ﰢ���� ������ ��ȯ�Ѵ�.
	int			GenerateIndex(LPVOID pIndex, ST_PNT_VERTEX* pHeightMap, float _fLODRatio);
};

