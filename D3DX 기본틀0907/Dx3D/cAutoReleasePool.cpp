#include "stdafx.h"
#include "cAutoReleasePool.h"


cAutoReleasePool::cAutoReleasePool()
{
	m_lstObject.clear();
}


cAutoReleasePool::~cAutoReleasePool()
{
}

void cAutoReleasePool::AddObject(cObject* pObject)
{
	m_lstObject.push_back(pObject);
}

void cAutoReleasePool::Drain()
{
	for each (auto p in m_lstObject)
		SAFE_RELEASE(p);

	m_lstObject.clear();
}