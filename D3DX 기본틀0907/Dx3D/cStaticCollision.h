#pragma once
//정적 충돌 구충돌 후 AABB 충돌체크를 한다.
//회전 값을 생각안함.  나무나  건물 같은 정적오브젝트용
//움직이는 물체도 이것으로 1차 충돌체크를 한다.
class cStaticCollision
{
private:
	SYNTHESIZE(D3DXVECTOR3, m_vScale, vScale);//육면체 크기용
private:
	D3DXMATRIXA16 m_matScale; 
	D3DXVECTOR3 m_vPos;
	D3DXVECTOR3 m_vMinVer; //육면체의 작은점
	D3DXVECTOR3 m_vMaxVer; //육면체의 큰점

	float	    m_fRadius;
	bool		m_bStatic; //정적이냐 정적이면 UpdatePos 를 할 필요가 없다.
public:
	cStaticCollision();
	cStaticCollision(IN const D3DXVECTOR3& _vPos,bool _bStatic);
	~cStaticCollision();

	void UpdatePos(IN const D3DXVECTOR3& _vPos);
	bool IsSphereCollision(IN const cStaticCollision*& _other);  //구 충돌
	bool IsAABBCollision(IN const cStaticCollision*& _other);	 //육면체 충돌
	bool IsCollision(IN const D3DXVECTOR3& _vPos,IN const cStaticCollision*& _other);
	bool IsCollision(IN const cStaticCollision*& _other); //m_bStatic 용
	D3DXVECTOR3 MakeSlideVec(IN const D3DXVECTOR3& _vPos); //미끄러짐 벡터만들자

	void Render();

	void SetRadius(float _f)
	{
		m_fRadius=_f;
	}
};

