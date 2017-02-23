//////////////////////////////////////////////////////////////
// wave読み込みクラス
// 2016. 9. 5	作成
// author		SyunMizuno
//////////////////////////////////////////////////////////////

#pragma once


#include	"xaudio2.h"
#include	<string>
using namespace std;

class	WaveLoader {
public:
	HRESULT Load(
		const string FileName,
		IXAudio2* Xaudio2,
		IXAudio2SourceVoice** pSource,
		BYTE** pDataAudio,
		DWORD* dwSizeAudio);

private:
	// サウンドデータファイルの生成
	HRESULT CreateSoundData(HANDLE& hFile, const string FileName);
	// WAVEファイルのチェック
	HRESULT CheckWave(HANDLE hFile);
	// フォーマットチェック
	HRESULT CheckFormat(HANDLE hFile, WAVEFORMATEXTENSIBLE& wfx);
	// オーディオデータ読み込み
	HRESULT ReadAudioData(HANDLE hFile, BYTE** pDataAudio, DWORD* dwSizeAudio);
	//----- チャンク
	// チェック
	HRESULT CheckChunk(HANDLE hFile, DWORD format, DWORD* pChunkSize, DWORD* pChunkDataPosition);
	// データの読み込み
	HRESULT ReadChunkData(HANDLE hFile, void* pBuffer, DWORD dwBuffersize, DWORD dwBufferoffset);
};