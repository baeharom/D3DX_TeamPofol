#pragma once
//���� �浹 ���浹 �� AABB �浹üũ�� �Ѵ�.
//ȸ�� ���� ��������.  ������  �ǹ� ���� ����������Ʈ��
//�����̴� ��ü�� �̰����� 1�� �浹üũ�� �Ѵ�.
class cStaticCollision
{
private:
	SYNTHESIZE(D3DXVECTOR3, m_vScale, vScale);//����ü ũ���
private:
	D3DXMATRIXA16 m_matScale; 
	D3DXVECTOR3 m_vPos;
	D3DXVECTOR3 m_vMinVer; //����ü�� ������
	D3DXVECTOR3 m_vMaxVer; //����ü�� ū��

	float	    m_fRadius;
	bool		m_bStatic; //�����̳� �����̸� UpdatePos �� �� �ʿ䰡 ����.
public:
	cStaticCollision();
	cStaticCollision(IN const D3DXVECTOR3& _vPos,bool _bStatic);
	~cStaticCollision();

	void UpdatePos(IN const D3DXVECTOR3& _vPos);
	bool IsSphereCollision(IN const cStaticCollision*& _other);  //�� �浹
	bool IsAABBCollision(IN const cStaticCollision*& _other);	 //����ü �浹
	bool IsCollision(IN const D3DXVECTOR3& _vPos,IN const cStaticCollision*& _other);
	bool IsCollision(IN const cStaticCollision*& _other); //m_bStatic ��
	D3DXVECTOR3 MakeSlideVec(IN const D3DXVECTOR3& _vPos); //�̲����� ���͸�����

	void Render();

	void SetRadius(float _f)
	{
		m_fRadius=_f;
	}
};

