#pragma once
//게임 신(Scene)의 기본 베이스 클래스 
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

