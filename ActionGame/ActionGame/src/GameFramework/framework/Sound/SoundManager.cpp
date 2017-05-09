//////////////////////////////////////////////////////////////
// サウンドマネージャクラス
// 2016. 9.26	クラス作成
// author		SyunMizuno
//////////////////////////////////////////////////////////////


#include	"SoundManager.h"
#include	<tchar.h>
#include	"../Save/Factory/SaveFactory.h"
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
//			コンストラクタ			//
//									*/
SoundManager::SoundManager() :
	m_pXaudio2(NULL),
	m_pMasteringVoice(NULL),
	m_pBGM(NULL),
	m_pSE(NULL),
	m_pVoice(NULL)
{}


/*									//
//			デストラクタ			//
//									*/
SoundManager::~SoundManager() {
	Release();

	// COMライブラリの終了処理
	CoUninitialize();
}


/*									//
//				初期化				//
//									*/
// 引数 : ウィンドウハンドル
// <return> S_OK. 成功 : E_FAIL. 失敗
HRESULT SoundManager::Init(HWND hWnd) {
	HRESULT hr;

	// COMライブラリの初期化
	CoInitializeEx(NULL, COINIT_MULTITHREADED);

	// XAudio2インターフェース初期化
	UINT flag = 0;
#ifdef _DEBUG
	flag |= XAUDIO2_DEBUG_ENGINE;
#endif
	hr = XAudio2Create(&m_pXaudio2, flag);
	if (FAILED(hr)) {
		MessageBox(hWnd, _T("XAudio2インターフェースの作成に失敗"), _T("警告"), MB_ICONWARNING);

		// COMライブラリの終了
		CoUninitialize();

		return E_FAIL;
	}

	// マスターボイスの作成
	hr = m_pXaudio2->CreateMasteringVoice(&m_pMasteringVoice);
	if (FAILED(hr)) {
		MessageBox(hWnd, _T("マスターボイスの生成に失敗"), _T("警告"), MB_ICONWARNING);

		if (m_pXaudio2) {
			m_pXaudio2->Release();
			m_pXaudio2 = NULL;
		}

		// COMライブラリの終了
		CoUninitialize();

		return E_FAIL;
	}

	SaveManagerBase mgr;
	if (mgr.Load(SaveName) == NO_SAVEFILE) {
		// サウンドの作成
		m_pBGM = new SoundBase;
		m_pSE = new SoundBase;
		m_pVoice = new SoundBase;
	}

	return S_OK;
}


/*									//
//				解放				//
//									*/
void SoundManager::Release() {
	SAFE_DELETE(m_pBGM);
	SAFE_DELETE(m_pSE);
	SAFE_DELETE(m_pVoice);

	// マスターボイスの破棄
	if (m_pMasteringVoice) {
		m_pMasteringVoice->DestroyVoice();
		m_pMasteringVoice = NULL;
	}

	if (m_pXaudio2) {
		// XAudio2インターフェースの解放
		m_pXaudio2->Release();
		m_pXaudio2 = NULL;
	}
}


/*									//
//				読み込み			//
//									*/
HRESULT SoundManager::Load(_SOUND_TYPE type, const TCHAR* filename, const TCHAR* name) {
	// タイプチェック
	SoundBase* Sound = CheckSound(type);
	if (!Sound)	return E_FAIL;

	return Sound->Load(filename, name, m_pXaudio2);
}


/*									//
//				削除				//
//									*/
void SoundManager::Delete(_SOUND_TYPE type, const TCHAR* name) {
	// タイプチェック
	SoundBase* Sound = CheckSound(type);
	if (!Sound)	return;

	Sound->Delete(name);
}


/*									//
//				再生				//
//									*/
HRESULT SoundManager::Play(_SOUND_TYPE type, const TCHAR* name, int loop) {
	// タイプチェック
	SoundBase* Sound = CheckSound(type);
	if (!Sound)	return E_FAIL;

	// BGMはループ
	if (type == SOUND_BGM)
		return Sound->Play(name, XAUDIO2_LOOP_INFINITE);

	return Sound->Play(name, loop);
}


/*									//
//				全停止				//
//									*/
void SoundManager::Stop(_SOUND_TYPE type) {
	// タイプチェック
	SoundBase* Sound = CheckSound(type);
	if (!Sound)	return;

	Sound->Stop();
}


/*									//
//				停止				//
//									*/
void SoundManager::Stop(_SOUND_TYPE type, const TCHAR* name) {
	// タイプチェック
	SoundBase* Sound = CheckSound(type);
	if (!Sound)	return;

	Sound->Stop(name);
}


/*									//
//				音量設定			//
//									*/
void SoundManager::SetVolum(_SOUND_TYPE type, float volum) {
	// タイプチェック
	SoundBase* Sound = CheckSound(type);
	if (!Sound)	return;

	Sound->SetVolum(volum);
}


/*									//
//				音量取得			//
//									*/
float SoundManager::GetVolum(_SOUND_TYPE type) {
	// タイプチェック
	SoundBase* Sound = CheckSound(type);
	if (!Sound)	return -1.0f;

	return Sound->GetVolum();
}


/*									//
//			タイプのチェック		//
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

	Debug::LogError("サウンドの種類が存在しません");
	return NULL;
}


/*									//
//				クラスID			//
//									*/
int SoundManager::GetClassID() {
	return _SAVE_FACTORY::SAVE_FACT_SOUNDMANAGER;
}


/*									//
//				セーブ				//
//									*/
void SoundManager::Save() {
	SaveManagerBase mgr;
	mgr.AddSaveData(this->m_pBGM);
	mgr.AddSaveData(this->m_pSE);
	mgr.AddSaveData(this->m_pVoice);
	mgr.AddSaveData(this);
	mgr.Save(SaveName);
	Debug::Log("サウンドを保存しました");
}