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
	//Ű�� �ѹ��� ���ǳ�?
	bool IsOnceKeyDown(int key);
	//Ű�� �ѹ� ������ �����?
	bool IsOnceKeyUp(int key);
	//Ű�� ��� �����ֳ�?
	bool IsStayKeyDown(int key);
	//���Ű��?(ĸ����,�ѹ���)
	bool IsToggleKey(int key);
};

