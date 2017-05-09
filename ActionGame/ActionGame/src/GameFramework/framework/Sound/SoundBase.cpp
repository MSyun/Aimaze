//////////////////////////////////////////////////////////////
// サウンドベースクラス
// 2016. 9.26	クラス作成
// 作成者		SyunMizuno
//////////////////////////////////////////////////////////////



#include	"SoundBase.h"
#include	"Wave/WaveLoader.h"

#include	"../Save/Factory/SaveFactory.h"
#include	"../Debug/Debug.h"



DATARECORD SoundBase::m_DataRecord[] =
{
	DATA_LOCAL(SoundBase, m_fVolum),
	DATA_END,
};


/*									//
//			コンストラクタ			//
//									*/
SoundBase::SoundBase() :
	m_fVolum(1.0f)
{
	m_mapSound.clear();
}


/*									//
//			デストラクタ			//
//									*/
SoundBase::~SoundBase() {
	for (auto it = m_mapSound.begin(); it != m_mapSound.end(); ++ it) {
		Release(it->second);
	}
	m_mapSound.clear();
}


/*									//
//				読み込み			//
//									*/
HRESULT SoundBase::Load(
	const string filename,
	const string name,
	IXAudio2* pXaudio2)
{
	//----- 登録済みか確認
	if (m_mapSound.end() != m_mapSound.find(name)) {
		Debug::Log("サウンド名" + name + "はすでに生成されています");
		return E_FAIL;
	}

	HRESULT hr;
	WaveLoader Wave;

	_tSound* Sound = new _tSound;
	Sound->pDataAudio = NULL;
	Sound->pSource = NULL;

	//----- Wave読み込み
	hr = Wave.Load(
		filename,
		pXaudio2,
		&Sound->pSource,
		&Sound->pDataAudio,
		&Sound->dwSizeAudio);
	if (hr != S_OK) {
		Debug::LogError("サウンド名" + name + "の生成に失敗しました");
		Release(Sound);
		return hr;
	}

	//----- サウンドの登録
	Sound->pSource->SetVolume(m_fVolum);
	m_mapSound.insert(pair< string, _tSound* >(name, Sound));

	return S_OK;
}


/*									//
//				削除				//
//									*/
void SoundBase::Delete(const string name) {
	//----- 初期化
	auto it = m_mapSound.find(name);

	// 登録確認
	if (it == m_mapSound.end()) {
		Debug::Log("サウンド名" + name + "は登録されていないため削除できません");
		return;
	}

	Release(it->second);

	m_mapSound.erase(name);
}


/*									//
//				再生				//
//									*/
HRESULT SoundBase::Play(const string name, int loop) {
	//----- 登録済み確認
	auto it = m_mapSound.find(name);

	// 登録されていない場合
	if (it == m_mapSound.end()) {
		Debug::LogError("サウンド名" + name + "は登録されていません");
		return E_FAIL;
	}

	//----- 状態取得
	XAUDIO2_VOICE_STATE xa2state;
	it->second->pSource->GetState(&xa2state);
	if (xa2state.BuffersQueued != 0) {
		// 一時停止
		it->second->pSource->Stop(0);

		// オーディオバッファの削除
		it->second->pSource->FlushSourceBuffers();
	}

	//----- 値の設定
	XAUDIO2_BUFFER buffer;
	memset(&buffer, 0, sizeof(XAUDIO2_BUFFER));
	buffer.AudioBytes = it->second->dwSizeAudio;
	buffer.pAudioData = it->second->pDataAudio;
	buffer.Flags = XAUDIO2_END_OF_STREAM;
	buffer.LoopCount = loop;

	// オーディオバッファの登録
	it->second->pSource->SubmitSourceBuffer(&buffer);

	// 再生
	it->second->pSource->Start(0);

	return S_OK;
}


/*									//
//				停止				//
//									*/
void SoundBase::Stop(const string name) {
	//----- 登録済み確認
	auto it = m_mapSound.find(name);

	// 登録されていない場合
	if (it == m_mapSound.end()) {
		Debug::Log("サウンド名" + name + "は登録されていないため停止できません");
		return;
	}

	XAUDIO2_VOICE_STATE xa2state;

	//----- 状態取得
	it->second->pSource->GetState(&xa2state);
	if (xa2state.BuffersQueued != 0) {
		// 一時停止
		it->second->pSource->Stop(0);

		// オーディオバッファの削除
		it->second->pSource->FlushSourceBuffers();
	}
}


/*									//
//				全停止				//
//									*/
void SoundBase::Stop() {
	for (auto it = m_mapSound.begin(); it != m_mapSound.end(); ++ it) {
		if (it->second->pSource) {
			it->second->pSource->Stop(0);
		}
	}
}


/*									//
//				音量設定			//
//									*/
void SoundBase::SetVolum(float volum) {
	m_fVolum = volum;

	for (auto it = m_mapSound.begin(); it != m_mapSound.end(); ++ it) {
		it->second->pSource->SetVolume(volum);
	}
}


/*									//
//				音量取得			//
//									*/
float SoundBase::GetVolum() {
	return m_fVolum;
}


/*									//
//				クラスID			//
//									*/
int SoundBase::GetClassID() {
	return _SAVE_FACTORY::SAVE_FACT_SOUND;
}


/*									//
//			指定サウンドを開放		//
//									*/
void SoundBase::Release(_tSound* sound) {
	// 確認
	if (!sound)	return;

	// オーディオデータの解放
	if (sound->pDataAudio) {
		delete[] sound->pDataAudio;
		sound->pDataAudio = NULL;
	}

	// ソースボイスの廃棄
	if (sound->pSource) {
		// 一時停止
		sound->pSource->Stop(0);

		sound->pSource->DestroyVoice();
		sound->pSource = NULL;
	}

	delete sound;
	sound = NULL;
}