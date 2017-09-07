#pragma once

#define WHITE D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f)
#define GREY D3DXCOLOR(0.2f, 0.2f, 0.2f, 1.0f)
#define BLACK D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f)
#define RED D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f)
#define GREEN D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f)
#define BLUE D3DXCOLOR(0.0f, 0.0f, 1.0f, 1.0f)
#define YELLOW D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f)
#define PURPLE D3DXCOLOR(1.0f, 0.0f, 1.0f, 1.0f)
#define SKYBLUE D3DXCOLOR(0.0f, 1.0f, 1.0f, 1.0f)

namespace D3DUTILITY
{
	D3DMATERIAL9 InitMtrl(D3DXCOLOR a, D3DXCOLOR d, D3DXCOLOR s, D3DXCOLOR e, float p);

	const D3DMATERIAL9 WHITE_MTRL = InitMtrl(WHITE, WHITE, WHITE, BLACK, 8.0f);
	const D3DMATERIAL9 BLACK_MTRL = InitMtrl(BLACK, BLACK, BLACK, BLACK, 8.0f);
	const D3DMATERIAL9 RED_MTRL = InitMtrl(RED, RED, RED, BLACK, 8.0f);
	const D3DMATERIAL9 GREEN_MTRL = InitMtrl(GREEN, GREEN, GREEN, BLACK, 8.0f);
	const D3DMATERIAL9 BLUE_MTRL = InitMtrl(BLUE, BLUE, BLUE, BLACK, 8.0f);
	const D3DMATERIAL9 YELLOW_MTRL = InitMtrl(YELLOW, YELLOW, YELLOW, BLACK, 8.0f);

	void ComputeNormal(D3DXVECTOR3* out, D3DXVECTOR3* p0, D3DXVECTOR3* p1, D3DXVECTOR3* p2);

	D3DLIGHT9 InitDirectional(D3DXVECTOR3* dir, D3DXCOLOR* c);
	D3DLIGHT9 InitPoint(D3DXVECTOR3* pos, D3DXCOLOR* c);
	D3DLIGHT9 InitSpot(D3DXVECTOR3* dir, D3DXVECTOR3* pos, D3DXCOLOR* c);

	inline float GetDistance(const D3DXVECTOR3& _vStart, const D3DXVECTOR3& _vDest)
	{
		return D3DXVec3Length(&(_vDest - _vStart));
	}

	float GetAngle(const D3DXVECTOR3& _vStart, const D3DXVECTOR3& _vDest,bool _toDegree=false);
	// pt가 rc안에 포함되는지 검사한다.(PtInRect()라는 API함수는 음수(-)값 처리를 못했다.)
	inline BOOL IsInRect(RECT* rc, POINT pt)
	{
		if ((rc->left <= pt.x) && (pt.x <= rc->right) &&
			(rc->bottom <= pt.y) && (pt.y <= rc->top))
			return TRUE;

		return FALSE;
	}
	inline D3DXVECTOR3& GetSlidingVector(IN const D3DXVECTOR3& _vMove, IN const D3DXVECTOR3& _vNormal)
	{
		return _vMove - D3DXVec3Dot(&_vMove, &_vNormal) * _vNormal;
	}
}

/* 추가: 버텍스 FVF 정보 셋팅 옵션 */

// Flexible Vertex Format(유연한 버텍스 포맷) - 사용 할 정점의 다양한 정보를 사용자가 직접 필요한 부분만 골라서 정의하여 쓸 수 있는 방법
//	1. Position 정점의 좌표 x,y,z(float)				: D3DFVF_XYZ - 정점 포맷이 변환되지 않은 정점의 위치 좌표
// 	2. RHW (float)                                  : D3DFVF_XYZRHW - 정점 포맷이 변환 된 정점의 위치 좌표 (D3DFVF_XYZ 또는 D3DFVF_NORMAL과 같이 사용불가)
// 	3. Blending Weight Data 결합 가중치 (float)		: D3DFVF_XYZB1 ~ D3DFVF_XYZB5
// 	4. Vertex Normal 정점의 법선 벡터 x,y,z(float)	: D3DFVF_NORMAL
// 	5. Vertex Point Size 정점의 점 크기 (float)		: D3DFVF_PSIZE
// 	6. Diffuse Color (DWORD)						: D3DFVF_DIFFUSE			// 확산광		// Ambient 환경 반사광
// 	7. Specular Color (DWORD)						: D3DFVF_SPECULAR			// 정반사광
// 	8. Texture Coordinate Set 1 (float)             : D3DFVF_TEX0 - D3DFVF_TEX8

struct ST_PC_VERTEX
{
	D3DXVECTOR3		p;
	D3DCOLOR		c;

	static const int FVF = D3DFVF_XYZ | D3DFVF_DIFFUSE;

	ST_PC_VERTEX() {}
	ST_PC_VERTEX(D3DXVECTOR3 _p, D3DCOLOR _c) : p(_p), c(_c) {}
};

struct ST_PT_VERTEX
{
	D3DXVECTOR3		p;
	D3DXVECTOR2		t;

	static const int FVF = D3DFVF_XYZ | D3DFVF_TEX1;

	ST_PT_VERTEX() {}
	ST_PT_VERTEX(D3DXVECTOR3 _p, D3DXVECTOR2 _t) : p(_p), t(_t) {}
};

struct ST_PN_VERTEX
{
	D3DXVECTOR3		p;
	D3DXVECTOR3		n;

	static const int FVF = D3DFVF_XYZ | D3DFVF_NORMAL;

	ST_PN_VERTEX() {}
	ST_PN_VERTEX(D3DXVECTOR3 _p, D3DXVECTOR3 _n) : p(_p), n(_n) {}
};

struct ST_PNT_VERTEX
{
	D3DXVECTOR3		p;
	D3DXVECTOR3		n;
	D3DXVECTOR2		t;

	static const int FVF = D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_TEX1;

	ST_PNT_VERTEX() {}
	ST_PNT_VERTEX(D3DXVECTOR3 _p, D3DXVECTOR3 _n, D3DXVECTOR2 _t) : p(_p), n(_n), t(_t) {}
};

struct ST_RHWC_VERTEX
{
	D3DXVECTOR4 p;
	D3DCOLOR	c;

	static const int FVF = D3DFVF_XYZRHW | D3DFVF_DIFFUSE;

	ST_RHWC_VERTEX() {}
	ST_RHWC_VERTEX(D3DXVECTOR4 _p, D3DCOLOR _c) : p(_p), c(_c) {}
};

struct ST_SPHERE
{
	float		fRadius;
	D3DXMATRIXA16 matW;
	D3DXVECTOR3	vCenter;
};

struct ST_SIZE
{
	float fWidth;
	float fHeight;

	ST_SIZE() : fWidth(0.0f), fHeight(0.0f) {}
	ST_SIZE(float w, float h) : fWidth(w), fHeight(h) {}
};