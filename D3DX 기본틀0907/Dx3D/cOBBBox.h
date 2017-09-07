#pragma once
class cOBBBox
{
private:
	SYNTHESIZE(D3DXVECTOR3, m_vExtent, vExtent);//X Y Z 길이를 저장한다. 스케일값 처럼 저장
private:
	D3DXMATRIXA16* m_pmatRT; /*회전값을 실시간으로 바꿔주기 위한 포인터 저장 
								스케일값이 매트릭스에 적용되어 있으면, 분리축에 대한 충돌검사가 제대로 되지 않는다
								따라서 RT 매트릭스를 사용한다		 */
	D3DXVECTOR3 m_CenterPos; //RT 매트릭스의 포지션 값(T)을 저장한다
	D3DXVECTOR3 m_Axis[3]; //R 매트릭스의 축 X Y Z
public:
	cOBBBox(D3DXMATRIXA16* _matRT);
	~cOBBBox();
	void UpdateBox();
	bool Collision(const cOBBBox* _other);
	void Render();
};

