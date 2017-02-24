//////////////////////////////////////////////////////////////
// �T�E���h�}�l�[�W���N���X
// 2016. 9.26	�N���X�쐬
// author		SyunMizuno
//////////////////////////////////////////////////////////////


#include	"SoundManager.h"
#include	<tchar.h>
#include	"../Save/SaveFactory.h"
#include	"../Utility/System/SystemUtility.h"
#include	"../Debug/Debug.h"


DATARECORD SoundManager::m_DataRecord[] =
{
	DATA_PTR(SoundManager, m_pBGM),
	DATA_PTR(SoundManager, m_pSE),
	DATA_PTR(SoundManager, m_pVoice),
	DATA_END,
};

const TCHAR* SaveName = _T("SoundVolum.dat");


/*									//
//			�R���X�g���N�^			//
//									*/
SoundManager::SoundManager() :
	m_pXaudio2(NULL),
	m_pMasteringVoice(NULL),
	m_pBGM(NULL),
	m_pSE(NULL),
	m_pVoice(NULL)
{}


/*									//
//			�f�X�g���N�^			//
//									*/
SoundManager::~SoundManager() {
	Release();

	// COM���C�u�����̏I������
	CoUninitialize();
}


/*									//
//				������				//
//									*/
// ���� : �E�B���h�E�n���h��
// <return> S_OK. ���� : E_FAIL. ���s
HRESULT SoundManager::Init(HWND hWnd) {
	HRESULT hr;

	// COM���C�u�����̏�����
	CoInitializeEx(NULL, COINIT_MULTITHREADED);

	// XAudio2�C���^�[�t�F�[�X������
	UINT flag = 0;
#ifdef _DEBUG
	flag |= XAUDIO2_DEBUG_ENGINE;
#endif
	hr = XAudio2Create(&m_pXaudio2, flag);
	if (FAILED(hr)) {
		MessageBox(hWnd, _T("XAudio2�C���^�[�t�F�[�X�̍쐬�Ɏ��s"), _T("�x��"), MB_ICONWARNING);

		// COM���C�u�����̏I��
		CoUninitialize();

		return E_FAIL;
	}

	// �}�X�^�[�{�C�X�̍쐬
	hr = m_pXaudio2->CreateMasteringVoice(&m_pMasteringVoice);
	if (FAILED(hr)) {
		MessageBox(hWnd, _T("�}�X�^�[�{�C�X�̐����Ɏ��s"), _T("�x��"), MB_ICONWARNING);

		if (m_pXaudio2) {
			m_pXaudio2->Release();
			m_pXaudio2 = NULL;
		}

		// COM���C�u�����̏I��
		CoUninitialize();

		return E_FAIL;
	}

	SaveManagerBase mgr;
	if (mgr.Load(SaveName) == NO_SAVEFILE) {
		// �T�E���h�̍쐬
		m_pBGM = new SoundBase;
		m_pSE = new SoundBase;
		m_pVoice = new SoundBase;
	}

	return S_OK;
}


/*									//
//				���				//
//									*/
void SoundManager::Release() {
	SAFE_DELETE(m_pBGM);
	SAFE_DELETE(m_pSE);
	SAFE_DELETE(m_pVoice);

	// �}�X�^�[�{�C�X�̔j��
	if (m_pMasteringVoice) {
		m_pMasteringVoice->DestroyVoice();
		m_pMasteringVoice = NULL;
	}

	if (m_pXaudio2) {
		// XAudio2�C���^�[�t�F�[�X�̉��
		m_pXaudio2->Release();
		m_pXaudio2 = NULL;
	}
}


/*									//
//				�ǂݍ���			//
//									*/
HRESULT SoundManager::Load(_SOUND_TYPE type, const TCHAR* filename, const TCHAR* name) {
	// �^�C�v�`�F�b�N
	SoundBase* Sound = CheckSound(type);
	if (!Sound)	return E_FAIL;

	return Sound->Load(filename, name, m_pXaudio2);
}


/*									//
//				�폜				//
//									*/
void SoundManager::Delete(_SOUND_TYPE type, const TCHAR* name) {
	// �^�C�v�`�F�b�N
	SoundBase* Sound = CheckSound(type);
	if (!Sound)	return;

	Sound->Delete(name);
}


/*									//
//				�Đ�				//
//									*/
HRESULT SoundManager::Play(_SOUND_TYPE type, const TCHAR* name, int loop) {
	// �^�C�v�`�F�b�N
	SoundBase* Sound = CheckSound(type);
	if (!Sound)	return E_FAIL;

	// BGM�̓��[�v
	if (type == SOUND_BGM)
		return Sound->Play(name, XAUDIO2_LOOP_INFINITE);

	return Sound->Play(name, loop);
}


/*									//
//				�S��~				//
//									*/
void SoundManager::Stop(_SOUND_TYPE type) {
	// �^�C�v�`�F�b�N
	SoundBase* Sound = CheckSound(type);
	if (!Sound)	return;

	Sound->Stop();
}


/*									//
//				��~				//
//									*/
void SoundManager::Stop(_SOUND_TYPE type, const TCHAR* name) {
	// �^�C�v�`�F�b�N
	SoundBase* Sound = CheckSound(type);
	if (!Sound)	return;

	Sound->Stop(name);
}


/*									//
//				���ʐݒ�			//
//									*/
void SoundManager::SetVolum(_SOUND_TYPE type, float volum) {
	// �^�C�v�`�F�b�N
	SoundBase* Sound = CheckSound(type);
	if (!Sound)	return;

	Sound->SetVolum(volum);
}


/*									//
//				���ʎ擾			//
//									*/
float SoundManager::GetVolum(_SOUND_TYPE type) {
	// �^�C�v�`�F�b�N
	SoundBase* Sound = CheckSound(type);
	if (!Sound)	return -1.0f;

	return Sound->GetVolum();
}


/*									//
//			�^�C�v�̃`�F�b�N		//
//									*/
SoundBase* SoundManager::CheckSound(_SOUND_TYPE type) {
	switch (type) {
	case _SOUND_TYPE::SOUND_BGM:
		return m_pBGM;

	case _SOUND_TYPE::SOUND_SE:
		return m_pSE;

	case _SOUND_TYPE::SOUND_VOICE:
		return m_pVoice;
	};

	Debug::LogError("�T�E���h�̎�ނ����݂��܂���");
	return NULL;
}


/*									//
//				�N���XID			//
//									*/
int SoundManager::GetClassID() {
	return _SAVE_FACTORY::SAVE_FACT_SOUNDMANAGER;
}


/*									//
//				�Z�[�u				//
//									*/
void SoundManager::Save() {
	SaveManagerBase mgr;
	mgr.AddSaveData(this->m_pBGM);
	mgr.AddSaveData(this->m_pSE);
	mgr.AddSaveData(this->m_pVoice);
	mgr.AddSaveData(this);
	mgr.Save(SaveName);
	Debug::Log("�T�E���h��ۑ����܂���");
}