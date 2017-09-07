#pragma once

#define g_pSceneManager cSceneManager::GetInstance()
class cScene;

class cSceneManager
{
	SINGLETON(cSceneManager);
private:
	map<string, cScene*> m_mapSceneList;
	cScene*				m_pCurScene;
public:
	void Update();
	void Render();
	void Destroy();
public:
	//¾À Ãß°¡ 
	void addScene(string sceneName, cScene* scene);
	//¾À º¯°æ
	HRESULT changeScene(string sceneName);
};

