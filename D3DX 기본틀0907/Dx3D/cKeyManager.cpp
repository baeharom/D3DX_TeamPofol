#include "stdafx.h"
#include "cKeyManager.h"


cKeyManager::cKeyManager()
{
	for (int i = 0; i < KEYMAX; i++)
	{
		_keyUp.set(i, false);
		_keyDown.set(i, false);
	}
}


cKeyManager::~cKeyManager()
{
}


bool cKeyManager::IsOnceKeyDown(int key)
{
	if (GetAsyncKeyState(key) & 0x8000)
	{
		if (!_keyDown[key])
		{
			_keyDown.set(key, true);
			return true;
		}
	}
	else
	{
		_keyDown.set(key, false);
	}
	return false;
}

bool cKeyManager::IsOnceKeyUp(int key)
{
	if (GetAsyncKeyState(key) & 0x8000)
	{
		_keyUp.set(key, true);
	}
	else
	{
		if (_keyUp[key])
		{
			_keyUp.set(key, false);
			return true;
		}
	}
	return false;
}

bool cKeyManager::IsStayKeyDown(int key)
{
	if (GetAsyncKeyState(key) & 0x8000) return true;
	return false;
}

bool cKeyManager::IsToggleKey(int key)
{
	if (GetKeyState(key) & 0x0001) return true;
	return false;
}