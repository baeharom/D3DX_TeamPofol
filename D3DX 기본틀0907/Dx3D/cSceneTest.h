#pragma once
#include "cScene.h"
class cGrid;
class cSkyBox;
class cHeightMap;
class cSkinnedMesh;

class cSceneTest:public cScene
{
private:
	cSkyBox* m_pSkyBox;
	cGrid* m_pGrid;
	cHeightMap* m_pHeightMap;
	cSkinnedMesh* m_pX;


public:
	cSceneTest();
	~cSceneTest();

	virtual void Init()		override;
	virtual void Update()   override;
	virtual void Render()	override;
	virtual void Release()	override;
};

