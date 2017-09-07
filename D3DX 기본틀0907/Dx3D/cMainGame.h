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

	void Loader();  //파일들 로드
	void AddScene(); //Scene 추가
	void AddTexture(); //3DTexture 추가
	void AddImage();  //2DImg 추가
	void AddSound(); //사운드 추가


};

