#pragma once
class cSkinnedMesh : public cObject
{
private:
	LPD3DXFRAME					m_pRoot;
	LPD3DXANIMATIONCONTROLLER	m_pAnimController;

	bool m_bBlending;
	float m_fActionTime;
	float m_fBlendTime;
public:
	cSkinnedMesh();
	~cSkinnedMesh();

	void Load(char* szFolder, char* szFile);
	void SetupBoneMatrixPtrs(LPD3DXFRAME pFrame);	// ������ �ִ� ��Ʈ���� ������ ����
	void Update();
	void Update(LPD3DXFRAME pFrame, LPD3DXFRAME pParent);
	void UpdateSkinnedMesh(LPD3DXFRAME pFrame);
	void Render(LPD3DXFRAME pFrame = NULL);

	void AnimationBlending();
};

