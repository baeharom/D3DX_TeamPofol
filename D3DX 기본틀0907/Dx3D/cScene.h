#pragma once
//���� ��(Scene)�� �⺻ ���̽� Ŭ���� 
class cScene
{
public:
	cScene();
	virtual ~cScene();

	virtual void Init() PURE;
	virtual void Update() PURE;
	virtual void Render() PURE;
	virtual void Release() PURE;
};

