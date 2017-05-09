//////////////////////////////////////////////////////////////
// サウンドマネージャクラス
// 2016. 9.26	クラス作成
// 2016. 9.27	セーブ関数取得
// author		SyunMizuno
//////////////////////////////////////////////////////////////

#pragma once


#include	"SoundBase.h"
#include	"../Singleton/Singleton.h"
#include	"../Save/Obj/SaveObjBase.h"


/*
	memo	:
			セーブ関数は継承先で作成したほうが良いかも
			その場合ベースに書き換えてそれを継承するときにシングルトンを使用すること
*/


typedef enum {
	SOUND_BGM = 0,
	SOUND_SE,
	SOUND_VOICE,
	SOUND_MAX,
} _SOUND_TYPE;


class	SoundManager : public	Singleton<SoundManager>, public	ISaveObjBase {
public:
#pragma region method

	SoundManager();
	virtual ~SoundManager();

	HRESULT Init(HWND hWnd);
	HRESULT Load(
		_SOUND_TYPE type,
		const TCHAR* filename,
		const TCHAR* name);
	void Delete(_SOUND_TYPE type, const TCHAR* name);
	HRESULT Play(
		_SOUND_TYPE type,
		const TCHAR* name,
		int loop = 0);
	void Stop(_SOUND_TYPE type);
	void Stop(_SOUND_TYPE type, const TCHAR* name);
	void SetVolum(_SOUND_TYPE type, float volum);
	float GetVolum(_SOUND_TYPE type);

	// セーブ
	void Save();

	// セーブデータ
	virtual DATARECORD* GetDataRecord() { return m_DataRecord; }
	virtual int GetClassID();

#pragma endregion

protected:
#pragma region variable

	IXAudio2*				m_pXaudio2;			// インターフェース
	IXAudio2MasteringVoice*	m_pMasteringVoice;	// マスターボイス

	// サウンド
	SoundBase*	m_pBGM;
	SoundBase*	m_pSE;
	SoundBase*	m_pVoice;

	// セーブオブジェクト
	static DATARECORD m_DataRecord[];

#pragma endregion

#pragma region conceal method

	// サウンドタイプ
	SoundBase* CheckSound(_SOUND_TYPE type);
	void Release();

#pragma endregion
};

inline SoundManager* GetSoundManager() {
	return SoundManager::Instance();
}