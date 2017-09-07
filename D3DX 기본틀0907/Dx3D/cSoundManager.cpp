#include "stdafx.h"
#include "cSoundManager.h"


cSoundManager::cSoundManager()
	:m_fESCurVolume(1.f), m_fESMaxVolume(1.f), m_fBGMCurVolume(1.f), m_fBGMMaxVolume(1.f)
	, m_fDistanceRatio(0.003f)
{
}


cSoundManager::~cSoundManager()
{
}

void cSoundManager::Init()
{
	//사운드 시스템 생성
	System_Create(&_system);

	//사운드 채널수 생성
	_system->init(TOTALSOUNDBUFFER, FMOD_INIT_NORMAL, 0);

	//사운드 채널 동적할당
	_sound = new Sound *[TOTALSOUNDBUFFER];
	_channel = new Channel *[TOTALSOUNDBUFFER];

	memset(_sound, 0, sizeof(Sound*)* (TOTALSOUNDBUFFER));
	memset(_channel, 0, sizeof(Channel*)* (TOTALSOUNDBUFFER));

}

void cSoundManager::Release()
{
	if (_channel != nullptr || _sound != nullptr)
	{
		for (int i = 0; i < TOTALSOUNDBUFFER; i++)
		{
			if (_channel != nullptr)
			{
				if (_channel[i]) _channel[i]->stop();
			}

			if (_sound != nullptr)
			{
				if (_sound[i]) _sound[i]->release();
			}
		}
	}

	SAFE_DELETE(_channel);
	SAFE_DELETE(_sound);

	//시스템 닫기
	if (_system != nullptr)
	{
		_system->release();
		_system->close();
	}
}

void cSoundManager::Update()
{/*
	사운드 시스템을 계속 업데이트 해줘야 볼륨이 바뀌거나
	재생이 끝난 사운드를 채널에서 빼내는 등 다양한 작업을 자동으로해준다.
	*/
	//메인게임에서 사운드 매니져를 업데이트 해준다.
	_system->update();



}


void cSoundManager::AddSound(string keyName, string soundName, bool bgm, bool loop)
{
	if (loop)
	{
		//브금
		if (bgm)
		{
			_system->createStream(soundName.c_str(), FMOD_LOOP_NORMAL, nullptr, &_sound[_mTotalSounds.size()]);
		}

		else//이펙트
		{
			_system->createSound(soundName.c_str(), FMOD_LOOP_NORMAL, nullptr, &_sound[_mTotalSounds.size()]);
		}
	}
	//루프x 이펙트
	else
	{
		_system->createSound(soundName.c_str(), FMOD_DEFAULT, nullptr, &_sound[_mTotalSounds.size()]);
	}

	//맵에 사운드를 키값과 함꼐 담아준다.
	_mTotalSounds.insert(make_pair(keyName, &_sound[_mTotalSounds.size()]));
}

void cSoundManager::PlayBGM(string keyName)
{
	int count = 0;
	arrSoundIter iter = _mTotalSounds.begin();
	for (iter; iter != _mTotalSounds.end(); ++iter, count++)
	{
		if (keyName == iter->first)
		{
			//사운드 플레이
			_system->playSound(FMOD_CHANNEL_FREE, *iter->second, false, &_channel[count]);
			//볼륨셋팅
				_channel[count]->setVolume(m_fBGMCurVolume);
				break;
		}
	}
}

void cSoundManager::PlayEffectSound(string keyName)
{
	int count = 0;
	arrSoundIter iter = _mTotalSounds.begin();
	for (iter; iter != _mTotalSounds.end(); ++iter, count++)
	{
		if (keyName == iter->first)
		{
			//사운드 플레이
			_system->playSound(FMOD_CHANNEL_FREE, *iter->second, false, &_channel[count]);
			//볼륨셋팅
			_channel[count]->setVolume(m_fESCurVolume);
			break;
		}
	}
}

void cSoundManager::PlayEffectSound(string keyName,const D3DXVECTOR3& _vPos)
{
	int count = 0;
	arrSoundIter iter = _mTotalSounds.begin();
	for (iter; iter != _mTotalSounds.end(); ++iter, count++)
	{
		if (keyName == iter->first)
		{
			//사운드 플레이
			_system->playSound(FMOD_CHANNEL_FREE, *iter->second, false, &_channel[count]);
			//볼륨셋팅
			float dis=GetDistance(g_pCamera->GetvPos(), _vPos);
			dis*=m_fDistanceRatio;
			_channel[count]->setVolume(m_fESCurVolume-dis);
			//좌우 팬 셋팅
			float angle = GetAngle(g_pCamera->GetvPos(), _vPos);
			if (angle > D3DX_PI/2)
				_channel[count]->setPan(1);
			else if (angle == 0)
				_channel[count]->setPan(0);
			else
				_channel[count]->setPan(-1);
			break;
		}
	}
}

void cSoundManager::Stop(string keyName)
{
	int count = 0;
	arrSoundIter iter = _mTotalSounds.begin();
	for (iter; iter != _mTotalSounds.end(); ++iter, count++)
	{
		if (keyName == iter->first)
		{
			_channel[count]->stop();
			break;
		}
	}
}

void cSoundManager::Pause(string keyName)
{
	int count = 0;
	arrSoundIter iter = _mTotalSounds.begin();
	for (iter; iter != _mTotalSounds.end(); ++iter, count++)
	{
		if (keyName == iter->first)
		{
			_channel[count]->setPaused(true);
			break;
		}
	}
}

void cSoundManager::Resume(string keyName)
{
	int count = 0;
	arrSoundIter iter = _mTotalSounds.begin();
	for (iter; iter != _mTotalSounds.end(); ++iter, count++)
	{
		if (keyName == iter->first)
		{
			_channel[count]->setPaused(false);
			break;
		}
	}
}

bool cSoundManager::IsPlaySound(string keyName)
{
	bool isPlay;
	int count = 0;
	arrSoundIter iter = _mTotalSounds.begin();
	for (iter; iter != _mTotalSounds.end(); ++iter, count++)
	{
		if (keyName == iter->first)
		{
			_channel[count]->isPlaying(&isPlay);
			break;
		}
	}

	return isPlay;
}

bool cSoundManager::IsPauseSound(string keyName)
{
	bool isPause;
	int count = 0;
	arrSoundIter iter = _mTotalSounds.begin();
	for (iter; iter != _mTotalSounds.end(); ++iter, count++)
	{
		if (keyName == iter->first)
		{
			_channel[count]->getPaused(&isPause);
			break;
		}
	}

	return isPause;
}

