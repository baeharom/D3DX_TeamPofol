#pragma once

class cMainGame
{
private:


public:
	cMainGame();
	~cMainGame();

	void Init();
	void Release();
	void Update();
	void Render();
public:
	void ManagerInit();
	void ManagerUpdate();
	void ManagerRender();
	void ManagerDestory();
	void WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

	void Loader();  //���ϵ� �ε�
	void AddScene(); //Scene �߰�
	void AddTexture(); //3DTexture �߰�
	void AddImage();  //2DImg �߰�
	void AddSound(); //���� �߰�


};

