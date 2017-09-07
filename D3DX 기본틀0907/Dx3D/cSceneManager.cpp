#include "stdafx.h"
#include "cSceneManager.h"
#include "cScene.h"

cSceneManager::cSceneManager()
	:m_pCurScene(nullptr)
{
}


cSceneManager::~cSceneManager()
{
	Destroy();
}

void cSceneManager::Update()
{
	if (m_pCurScene)m_pCurScene->Update();
}

void cSceneManager::Render()
{
	if (m_pCurScene)m_pCurScene->Render();
}

void cSceneManager::Destroy()
{
	map<string, cScene*>::iterator iter;

	for (iter = m_mapSceneList.begin(); iter != m_mapSceneList.end(); ++iter)
		SAFE_DELETE(iter->second);

	m_mapSceneList.clear();
}

void cSceneManager::addScene(string sceneName, cScene * scene)
{
	if (m_mapSceneList[sceneName])return; //ÀÌ¹Ì ÀÖ´Â ¾ÀÀÌ¸é ¸®ÅÏ

	m_mapSceneList[sceneName] = scene;
}

HRESULT cSceneManager::changeScene(string sceneName)
{
	if(!m_mapSceneList[sceneName]) //¹Ù²Ü ¾ÀÀÌ ¾øÀ¸¸é ¸®ÅÏ
		return E_FAIL;

	cScene* changeNode = m_mapSceneList[sceneName];

	if (m_pCurScene == changeNode) //¹Ù²Ü ¾ÀÀÌ ÇöÀç ¾ÀÀÌ¸é ¸®ÅÏ
		return E_FAIL;

	if (m_pCurScene)m_pCurScene->Release(); //ÇöÀç ¾ÀÀÌ ÀÖ´Ù¸é ¸±¸®Áî
	changeNode->Release();					//¹Ù²Ü ¾À ¸±¸®Áî , ÀÎÀÕ
	changeNode->Init();
	m_pCurScene = changeNode;
	return S_OK;
}
