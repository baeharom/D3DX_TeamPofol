// stdafx.h : ���� ��������� ���� ��������� �ʴ�
// ǥ�� �ý��� ���� ���� �� ������Ʈ ���� ���� ������
// ��� �ִ� ���� �����Դϴ�.
//

#pragma once

#include "targetver.h"

#define WIN32_LEAN_AND_MEAN             // ���� ������ �ʴ� ������ Windows ������� �����մϴ�.
// Windows ��� ����:
#include <windows.h>

// C ��Ÿ�� ��� �����Դϴ�.
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>


// TODO: ���α׷��� �ʿ��� �߰� ����� ���⿡�� �����մϴ�.
#include <d3dx9.h>
#include <vector>
#include <time.h>
#include <map>
#include <assert.h>
#include <set>
#include <list>

#pragma comment(lib, "d3d9.lib")
#pragma comment(lib, "d3dx9.lib")
using namespace std;
//============������=============================//
#include "Defines.h"
//=====================================================//
#include "D3DUtility.h"
#include "cObject.h"
#include "cRay.h"
//========================�Ŵ���=============================//
#include "cDeviceManager.h"
#include "cTextureManager.h"
#include "cTimeManager.h"
#include "cObjectManager.h"
#include "cAutoReleasePool.h"
#include "cFontManager.h"
#include "cSceneManager.h"
#include "cKeyManager.h"
#include "cSoundManager.h"
#include "cImageManager.h"
#include "cCamera.h"
#include "cFrustum.h"
//==============================================================//
//=========NAMESPACE======================//
using namespace D3DUTILITY;
//=======================================
//====================================================
//		## �������� ##
//====================================================

extern HWND g_hWnd;
extern LPD3DXMESH	g_pMeshDebug;
extern bool		g_bFillRender;
