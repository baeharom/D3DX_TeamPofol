#pragma once

class cPyramid;

class cGrid : public cObject
{
private:
	D3DXMATRIXA16			m_matWorld;
	vector<ST_PC_VERTEX>	m_vecPCVertex;

	vector<cPyramid*>		m_vecPyramid;

public:
	cGrid();
	~cGrid();

	void Setup();
	void Render();
};

