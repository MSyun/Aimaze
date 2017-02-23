//////////////////////////////////////////////////////////////
// サウンドベースクラス
// 2016. 9.26	クラス作成
// author		SyunMizuno
//////////////////////////////////////////////////////////////

#pragma once


#include	<Windows.h>
#include	"xaudio2.h"
#include	<map>
#include	<string>

#include	"../Save/SaveObjBase.h"

using namespace std;


class	SoundBase : public	ISaveObjBase {
protected:
#pragma region struct

	// サウンド構造体
	struct _tSound {
		IXAudio2SourceVoice*	pSource;
		BYTE*					pDataAudio;
		DWORD					dwSizeAudio;
	};

#pragma endregion

#pragma region variable

	// サウンド
	map< string, _tSound* >	m_mapSound;

	// 音量
	float	m_fVolum;

	// セーブオブジェクト
	static DATARECORD m_DataRecord[];

#pragma endregion

public:
#pragma region method

	SoundBase();
	virtual ~SoundBase();

	// 読み込み
	HRESULT Load(
		const string	FileName,
		const string	name,
		IXAudio2*		ppXaudio2);
	void Delete(const string name);					// 削除
	HRESULT Play(const string name, int loop = 0);	// 再生
	void Stop(const string name);					// 停止
	void Stop();									// 全停止
	void SetVolum(float volum);						// ボリューム設定
	float GetVolum();								// ボリューム取得

	// セーブデータ
	virtual DATARECORD* GetDataRecord() { return m_DataRecord; }
	virtual int GetClassID();

#pragma endregion

protected:
	// 解放
	void Release(_tSound* sound);
};