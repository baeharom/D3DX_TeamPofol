#include "stdafx.h"
#include "cMainGame.h"
#include "cSceneTest.h"

cMainGame::cMainGame()
{
	srand(time(NULL));
	rand();
}


cMainGame::~cMainGame()
{
}

void cMainGame::Init()
{
	g_pD3DDevice->SetRenderState(D3DRS_NORMALIZENORMALS, true);
	ManagerInit(); //���� ������ ���� ���ش�
	Loader();
	AddScene();
	AddTexture();
	AddImage();
	AddSound();
	
}

void cMainGame::Release()
{
	ManagerDestory();
}

void cMainGame::Update()
{
	ManagerUpdate();


}

void cMainGame::Render()
{
	g_pD3DDevice->Clear(NULL,
		NULL,
		D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER,
		D3DCOLOR_XRGB(47, 121, 112),
		1.0f, 0);
	g_pD3DDevice->BeginScene();
	//=========================================
	ManagerRender();
	//=========================================
	g_pD3DDevice->EndScene();
	g_pD3DDevice->Present(NULL, NULL, NULL, NULL);
}

void cMainGame::ManagerInit()
{
	g_pImageManager->Init();
	g_pSoundManager->Init();
	g_pFontManager->Init();
	g_pCamera->Init();

	D3DLIGHT9 light;
	D3DXVECTOR3 dir(0, -1, 0);
	D3DXVec3Normalize(&dir, &dir);
	D3DXCOLOR c = WHITE;

	light = InitDirectional(&dir, &c);

	g_pD3DDevice->SetLight(0, &light);	// ���� ����
	g_pD3DDevice->LightEnable(0, true);	// ���� ��� ����
}

void cMainGame::ManagerUpdate()
{
	g_pSceneManager->Update();
	g_pSoundManager->Update();
	//g_pCamera->Update(m_pCharacterController->GetPosition());
}

void cMainGame::ManagerRender()
{
	g_pSceneManager->Render();
}

void cMainGame::ManagerDestory()
{
	g_pImageManager->Release();
	g_pSceneManager->Destroy();
	g_pSoundManager->Release();

	g_pTextureManager->Destroy();
	g_pFontManager->Destroy();

	//============================//
	g_pAutoReleasePool->Drain();
	g_pObjectManager->Destroy();
	g_pDeviceManager->Destroy();
}

void cMainGame::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	g_pCamera->WndProc(hWnd, message,wParam,lParam);
}

void cMainGame::Loader()
{
}

void cMainGame::AddScene()
{
	g_pSceneManager->addScene("�׽�Ʈ", new cSceneTest);
	g_pSceneManager->changeScene("�׽�Ʈ");
}

void cMainGame::AddTexture()
{
}

void cMainGame::AddImage()
{
	g_pImageManager->AddImage("�׽�Ʈ", "Image/����.png",100,100,200,200);
}

void cMainGame::AddSound()
{
	g_pSoundManager->AddSound("ť��", "sound/ť��.mp3", true, true);
	g_pSoundManager->AddSound("�޴�����", "sound/menuselect.mp3", false, false);
	//g_pSoundManager->PlayBGM("ť��");
}

