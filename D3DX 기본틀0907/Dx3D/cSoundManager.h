#pragma once

#include "fmod/inc/fmod.hpp"
#pragma comment(lib, "fmod/lib/fmodex_vc.lib")	//lib 링크
using namespace FMOD;

//채널버퍼, 사운드버퍼
#define EXTRACHANNERBUFFER 5
#define SOUNDBUFFER 20

//총사운드 버퍼 설정: 버퍼가 작아서 씹히는 문제를 해결해주기 위함
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
	System* _system;	//fmod의 시스템 클래스
	Sound** _sound;		//fmod의 사운드 클래스
	Channel** _channel;	//fmod의 채널 클래스

	arrSounds _mTotalSounds;	//맵에 담긴 사운드 목록

	float m_fDistanceRatio;  //거리에 따른 소리비율
	//이펙트 사운드
	float m_fESCurVolume; 
	float m_fESMaxVolume;
	//비쥐엠 사운드
	float m_fBGMCurVolume;
	float m_fBGMMaxVolume;
public:
	void Init();
	void Release();
	void Update();
	
public:
	//사운드 추가(키값, 파일이름, BGM? 루프?)
	void AddSound(string keyName, string soundName, bool bgm = FALSE, bool loop = FALSE);
	//사운드 플레이(키값, 볼륨) 0.0f ~ 1.0f
	void PlayBGM(string keyName);			//BGM 용
	void PlayEffectSound(string keyName);
	void PlayEffectSound(string keyName, const D3DXVECTOR3& _vPos); //3D 사운드용 
	//사운드 정지
	void Stop(string keyName);
	//사운드 일시정지
	void Pause(string keyName);
	//사운드 다시 재생
	void Resume(string keyName);
	//플레이 중이냐?
	bool IsPlaySound(string keyName);
	//일시정지 중이냐?
	bool IsPauseSound(string keyName);
};

