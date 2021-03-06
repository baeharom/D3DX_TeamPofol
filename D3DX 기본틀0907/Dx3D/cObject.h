#pragma once
class cObject
{
private:
	int m_nRefCount;

public:
	cObject();
	virtual ~cObject();

	virtual void AddRef();
	virtual void Release();
	virtual void AutoRelease();

	int GetRefCount() { return m_nRefCount; }
};

