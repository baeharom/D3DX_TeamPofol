// stdafx.h : 자주 사용하지만 자주 변경되지는 않는
// 표준 시스템 포함 파일 및 프로젝트 관련 포함 파일이
// 들어 있는 포함 파일입니다.
//

#pragma once

#include "targetver.h"

#define WIN32_LEAN_AND_MEAN             // 거의 사용되지 않는 내용은 Windows 헤더에서 제외합니다.
// Windows 헤더 파일:
#include <windows.h>

// C 런타임 헤더 파일입니다.
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>


// TODO: 프로그램에 필요한 추가 헤더는 여기에서 참조합니다.
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
//============디파인=============================//
#include "Defines.h"
//=====================================================//
#include "D3DUtility.h"
#include "cObject.h"
#include "cRay.h"
//========================매니저=============================//
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
//		## 전역변수 ##
//====================================================

extern HWND g_hWnd;
extern LPD3DXMESH	g_pMeshDebug;
extern bool		g_bFillRender;
