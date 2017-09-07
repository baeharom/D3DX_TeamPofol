#include "stdafx.h"
#include "cObject.h"


cObject::cObject()
	: m_nRefCount(1)
{
	g_pObjectManager->AddObject(this);
}


cObject::~cObject()
{
	if (m_nRefCount <= 0)
		g_pObjectManager->RemoveObject(this);
	else
		assert(false && "릴리즈를 사용해서 객체를 해제 하세요~");
}

void cObject::AddRef()
{
	++m_nRefCount;
	AutoRelease();
}

void cObject::Release()
{
	--m_nRefCount;

	if (m_nRefCount <= 0)
		delete this;
}

void cObject::AutoRelease()
{
	g_pAutoReleasePool->AddObject(this);
}