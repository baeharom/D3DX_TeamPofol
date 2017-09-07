#pragma once
#include "stdafx.h"

interface iMap
{
public:
	virtual void Load(IN char* szFilePath, IN D3DXMATRIXA16* pMat) = 0;
	virtual bool GetHeight(IN const float& x, OUT float& y, IN const float& z) PURE;
	virtual vector<D3DXVECTOR3>& GetVertex() PURE;
	virtual vector<DWORD>& GetIndex() PURE;
};