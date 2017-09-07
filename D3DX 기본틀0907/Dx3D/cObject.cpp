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
		assert(false && "����� ����ؼ� ��ü�� ���� �ϼ���~");
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