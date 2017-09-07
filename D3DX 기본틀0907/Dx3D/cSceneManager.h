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
	//�� �߰� 
	void addScene(string sceneName, cScene* scene);
	//�� ����
	HRESULT changeScene(string sceneName);
};

