#include "stdafx.h"
#include "cSceneTest.h"
#include "cGrid.h"
#include "cHeightMap.h"
#include "cSkyBox.h"
#include "cSkinnedMesh.h"
cSceneTest::cSceneTest()
	:m_pHeightMap(nullptr), m_pSkyBox(nullptr),m_pX(nullptr)
{
}


cSceneTest::~cSceneTest()
{
	Release();
}

void cSceneTest::Init()
{
	m_pGrid = new cGrid;
	m_pGrid->AutoRelease();
	m_pGrid->Setup();

	D3DXVECTOR3 vScaling(1, 1, 1);
	m_pHeightMap = new cHeightMap;
	m_pHeightMap->Init("HeightMapData/HeightMap.raw", 257, 257, 0.05f, "HeightMapData/terrain.jpg", &vScaling);

	m_pSkyBox = new cSkyBox;
	LPCSTR	sky[6] = { "Image/SkyBox1/Top.bmp", "Image/SkyBox1/Front.bmp", "Image/SkyBox1/Back.bmp",
		"Image/SkyBox1/Right.bmp", "Image/SkyBox1/Left.bmp", "Image/SkyBox1/Bottom.bmp" };
	m_pSkyBox->Init(sky);

	m_pX = new cSkinnedMesh;
	m_pX->AutoRelease();
	m_pX->Load("Xfile/Lethita", "Lethita.x");
}

void cSceneTest::Update()
{
	if (g_pKeyManager->IsOnceKeyDown('1'))
	{
		g_bFillRender = !g_bFillRender;
	}
	if (g_pKeyManager->IsOnceKeyDown('2'))
	{
		g_pSoundManager->PlayEffectSound("¸Þ´º¼¿·º", D3DXVECTOR3(0, 0, 0));
	}
	m_pX->Update();
	g_pCamera->Update();
}

void cSceneTest::Render()
{
	m_pSkyBox->Render();

	g_pD3DDevice->SetRenderState(D3DRS_FILLMODE, g_bFillRender ? D3DFILL_WIREFRAME : D3DFILL_SOLID);
	m_pGrid->Render();
	if (m_pHeightMap)
		m_pHeightMap->Render();
	m_pX->Render();

}

void cSceneTest::Release()
{
	SAFE_DELETE(m_pSkyBox);
	SAFE_RELEASE(m_pHeightMap);
}
