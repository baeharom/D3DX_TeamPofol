#pragma once
class cOBBBox
{
private:
	SYNTHESIZE(D3DXVECTOR3, m_vExtent, vExtent);//X Y Z ���̸� �����Ѵ�. �����ϰ� ó�� ����
private:
	D3DXMATRIXA16* m_pmatRT; /*ȸ������ �ǽð����� �ٲ��ֱ� ���� ������ ���� 
								�����ϰ��� ��Ʈ������ ����Ǿ� ������, �и��࿡ ���� �浹�˻簡 ����� ���� �ʴ´�
								���� RT ��Ʈ������ ����Ѵ�		 */
	D3DXVECTOR3 m_CenterPos; //RT ��Ʈ������ ������ ��(T)�� �����Ѵ�
	D3DXVECTOR3 m_Axis[3]; //R ��Ʈ������ �� X Y Z
public:
	cOBBBox(D3DXMATRIXA16* _matRT);
	~cOBBBox();
	void UpdateBox();
	bool Collision(const cOBBBox* _other);
	void Render();
};

