#pragma once
class cImage:public cObject
{
private:
	SYNTHESIZE(RECT, m_rcDraw, RC);
	SYNTHESIZE(D3DXMATRIX, m_matWorld, matW);
	SYNTHESIZE(float, m_fCurW, CurW);
	SYNTHESIZE(float, m_fCurH, CurH);
	SYNTHESIZE_ADD_REF(LPDIRECT3DTEXTURE9, m_pTexture, Texture); 
private:
	D3DCOLOR m_dwColor; 
	float m_fOrgW;
	float m_fOrgH;
public:
	cImage();
	~cImage();

	void Init(string _strKey, float _x, float _y);
	void Init(string _strKey, float _x, float _y, float _w, float _h);
	void Render(LPD3DXSPRITE pSprite,int _a = 255);  //기본 렌더
	void RenderAndPos(LPD3DXSPRITE pSprite,float _x, float _y, int _a=255); //위치 변경하면서 렌더
	void RenderAndPosCenter(LPD3DXSPRITE pSprite, float _x, float _y, int _a = 255);
	void RenderAndSize(LPD3DXSPRITE pSprite,float _w, float _h, int _a = 255); //사이즈 변경하면서 렌더
	void Render(LPD3DXSPRITE pSprite,float _x, float _y, float _w, float _h, int _a = 255); //위치 사이즈 변경하면서 렌더
};

