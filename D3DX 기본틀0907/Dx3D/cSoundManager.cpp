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
	//���� �ý��� ����
	System_Create(&_system);

	//���� ä�μ� ����
	_system->init(TOTALSOUNDBUFFER, FMOD_INIT_NORMAL, 0);

	//���� ä�� �����Ҵ�
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

	//�ý��� �ݱ�
	if (_system != nullptr)
	{
		_system->release();
		_system->close();
	}
}

void cSoundManager::Update()
{/*
	���� �ý����� ��� ������Ʈ ����� ������ �ٲ�ų�
	����� ���� ���带 ä�ο��� ������ �� �پ��� �۾��� �ڵ��������ش�.
	*/
	//���ΰ��ӿ��� ���� �Ŵ����� ������Ʈ ���ش�.
	_system->update();



}


void cSoundManager::AddSound(string keyName, string soundName, bool bgm, bool loop)
{
	if (loop)
	{
		//���
		if (bgm)
		{
			_system->createStream(soundName.c_str(), FMOD_LOOP_NORMAL, nullptr, &_sound[_mTotalSounds.size()]);
		}

		else//����Ʈ
		{
			_system->createSound(soundName.c_str(), FMOD_LOOP_NORMAL, nullptr, &_sound[_mTotalSounds.size()]);
		}
	}
	//����x ����Ʈ
	else
	{
		_system->createSound(soundName.c_str(), FMOD_DEFAULT, nullptr, &_sound[_mTotalSounds.size()]);
	}

	//�ʿ� ���带 Ű���� �Բ� ����ش�.
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
			//���� �÷���
			_system->playSound(FMOD_CHANNEL_FREE, *iter->second, false, &_channel[count]);
			//��������
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
			//���� �÷���
			_system->playSound(FMOD_CHANNEL_FREE, *iter->second, false, &_channel[count]);
			//��������
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
			//���� �÷���
			_system->playSound(FMOD_CHANNEL_FREE, *iter->second, false, &_channel[count]);
			//��������
			float dis=GetDistance(g_pCamera->GetvPos(), _vPos);
			dis*=m_fDistanceRatio;
			_channel[count]->setVolume(m_fESCurVolume-dis);
			//�¿� �� ����
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

