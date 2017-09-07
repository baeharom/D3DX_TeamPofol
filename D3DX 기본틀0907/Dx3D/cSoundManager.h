#pragma once

#include "fmod/inc/fmod.hpp"
#pragma comment(lib, "fmod/lib/fmodex_vc.lib")	//lib ��ũ
using namespace FMOD;

//ä�ι���, �������
#define EXTRACHANNERBUFFER 5
#define SOUNDBUFFER 20

//�ѻ��� ���� ����: ���۰� �۾Ƽ� ������ ������ �ذ����ֱ� ����
#define TOTALSOUNDBUFFER SOUNDBUFFER + EXTRACHANNERBUFFER

#define g_pSoundManager cSoundManager::GetInstance()

class cSoundManager
{
private:
	typedef map<string, Sound**> arrSounds;
	typedef map<string, Sound**>::iterator arrSoundIter;
private:
	SINGLETON(cSoundManager);
private:
	System* _system;	//fmod�� �ý��� Ŭ����
	Sound** _sound;		//fmod�� ���� Ŭ����
	Channel** _channel;	//fmod�� ä�� Ŭ����

	arrSounds _mTotalSounds;	//�ʿ� ��� ���� ���

	float m_fDistanceRatio;  //�Ÿ��� ���� �Ҹ�����
	//����Ʈ ����
	float m_fESCurVolume; 
	float m_fESMaxVolume;
	//���㿥 ����
	float m_fBGMCurVolume;
	float m_fBGMMaxVolume;
public:
	void Init();
	void Release();
	void Update();
	
public:
	//���� �߰�(Ű��, �����̸�, BGM? ����?)
	void AddSound(string keyName, string soundName, bool bgm = FALSE, bool loop = FALSE);
	//���� �÷���(Ű��, ����) 0.0f ~ 1.0f
	void PlayBGM(string keyName);			//BGM ��
	void PlayEffectSound(string keyName);
	void PlayEffectSound(string keyName, const D3DXVECTOR3& _vPos); //3D ����� 
	//���� ����
	void Stop(string keyName);
	//���� �Ͻ�����
	void Pause(string keyName);
	//���� �ٽ� ���
	void Resume(string keyName);
	//�÷��� ���̳�?
	bool IsPlaySound(string keyName);
	//�Ͻ����� ���̳�?
	bool IsPauseSound(string keyName);
};

