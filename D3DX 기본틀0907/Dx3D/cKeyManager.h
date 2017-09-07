#pragma once
#include <bitset>
#define g_pKeyManager cKeyManager::GetInstance()
#define KEYMAX 256

class cKeyManager
{
private:
	SINGLETON(cKeyManager);
private:
	bitset<KEYMAX> _keyUp;
	bitset<KEYMAX> _keyDown;
public:
	//키가 한번만 눌렷냐?
	bool IsOnceKeyDown(int key);
	//키가 한번 눌렀다 띄었냐?
	bool IsOnceKeyUp(int key);
	//키가 계속 눌러있냐?
	bool IsStayKeyDown(int key);
	//토글키냐?(캡스락,넘버락)
	bool IsToggleKey(int key);
};

