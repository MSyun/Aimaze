//////////////////////////////////////////////////////////////
// wave読み込みクラス
// 2016. 9. 5	作成
// author		SyunMizuno
//////////////////////////////////////////////////////////////


#include	"WaveLoader.h"
#include	<tchar.h>


/*									//
//		WAVEファイルの読み込み		//
//									*/
HRESULT WaveLoader::Load(
	const string FileName,
	IXAudio2* Xaudio2,
	IXAudio2SourceVoice** pSource,
	BYTE** pDataAudio,
	DWORD* dwSizeAudio)
{
	HANDLE hFile;
	WAVEFORMATEXTENSIBLE wfx;

	// バッファのクリア
	memset(&wfx, 0, sizeof(WAVEFORMATEXTENSIBLE));

	if (FAILED(CreateSoundData(hFile, FileName))) {
		MessageBox(NULL, _T("サウンドデータファイルの生成に失敗"), _T("警告"), MB_ICONWARNING);
		return HRESULT_FROM_WIN32(GetLastError());
	}

	if (FAILED(CheckWave(hFile))) {
		MessageBox(NULL, _T("WAVEファイルのチェックに失敗"), _T("警告"), MB_ICONWARNING);
		return S_FALSE;
	}

	if (FAILED(CheckFormat(hFile, wfx))) {
		MessageBox(NULL, _T("フォーマットチェックに失敗"), _T("警告"), MB_ICONWARNING);
		return S_FALSE;
	}

	if (FAILED(ReadAudioData(hFile, pDataAudio, dwSizeAudio))) {
		MessageBox(NULL, _T("オーディオデータ読み込みに失敗"), _T("警告"), MB_ICONWARNING);
		return S_FALSE;
	}

	if (FAILED(Xaudio2->CreateSourceVoice(pSource, &(wfx.Format)))) {
		MessageBox(NULL, _T("ソースボイスの生成に失敗"), _T("警告"), MB_ICONWARNING);
		return S_FALSE;
	}

	return S_OK;
}


/*									//
//	サウンドデータファイルの生成	//
//									*/
HRESULT WaveLoader::CreateSoundData(HANDLE& hFile, const string FileName) {
	hFile = CreateFile(FileName.c_str(), GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, 0, NULL);
	if (hFile == INVALID_HANDLE_VALUE) {
		return HRESULT_FROM_WIN32(GetLastError());
	}
	if (SetFilePointer(hFile, 0, NULL, FILE_BEGIN) == INVALID_SET_FILE_POINTER) {
		// ファイルポインタを先頭に移動
		return HRESULT_FROM_WIN32(GetLastError());
	}

	return S_OK;
}


/*									//
//		WAVEファイルのチェック		//
//									*/
HRESULT WaveLoader::CheckWave(HANDLE hFile) {
	DWORD dwChunkSize = 0;
	DWORD dwChunkPosition = 0;
	DWORD dwFiletype;
	HRESULT hr;

	hr = CheckChunk(hFile, 'FFIR', &dwChunkSize, &dwChunkPosition);
	if (FAILED(hr)) {
		return hr;
	}
	hr = ReadChunkData(hFile, &dwFiletype, sizeof(DWORD), dwChunkPosition);
	if (FAILED(hr)) {
		return hr;
	}
	if (dwFiletype != 'EVAW') {
		return hr;
	}

	return hr;
}


/*									//
//		フォーマットチェック		//
//									*/
HRESULT WaveLoader::CheckFormat(HANDLE hFile, WAVEFORMATEXTENSIBLE& wfx) {
	DWORD dwChunkSize = 0;
	DWORD dwChunkPosition = 0;
	HRESULT hr;

	hr = CheckChunk(hFile, ' tmf', &dwChunkSize, &dwChunkPosition);
	if (FAILED(hr)) {
		return hr;
	}
	hr = ReadChunkData(hFile, &wfx, dwChunkSize, dwChunkPosition);
	if (FAILED(hr)) {
		return hr;
	}

	return hr;
}


/*									//
//		オーディオデータ読み込み	//
//									*/
HRESULT WaveLoader::ReadAudioData(HANDLE hFile, BYTE** pDataAudio, DWORD* dwSizeAudio) {
	DWORD dwChunkPosition = 0;
	HRESULT hr;

	hr = CheckChunk(hFile, 'atad', dwSizeAudio, &dwChunkPosition);
	if (FAILED(hr)) {
		return hr;
	}
	*pDataAudio = new BYTE[*dwSizeAudio];
	hr = ReadChunkData(hFile, *pDataAudio, *dwSizeAudio, dwChunkPosition);
	if (FAILED(hr)) {
		return hr;
	}

	return hr;
}


/*									//
//			チャンクのチェック		//
//									*/
HRESULT WaveLoader::CheckChunk(HANDLE hFile, DWORD format, DWORD* pChunkSize, DWORD* pChunkDataPosition) {
	HRESULT hr = S_OK;
	DWORD dwRead;
	DWORD dwChunkType;
	DWORD dwChunkDataSize;
	DWORD dwRIFFDataSize = 0;
	DWORD dwFileType;
	DWORD dwBytesRead = 0;
	DWORD dwOffset = 0;

	if (SetFilePointer(hFile, 0, NULL, FILE_BEGIN) == INVALID_SET_FILE_POINTER) {
		// ファイルポインタを先頭に移動
		return HRESULT_FROM_WIN32(GetLastError());
	}

	while (hr == S_OK) {
		if (ReadFile(hFile, &dwChunkType, sizeof(DWORD), &dwRead, NULL) == 0) {
			// チャンクの読み込み
			hr = HRESULT_FROM_WIN32(GetLastError());
		}

		if (ReadFile(hFile, &dwChunkDataSize, sizeof(DWORD), &dwRead, NULL) == 0) {
			// チャンクデータの読み込み
			hr = HRESULT_FROM_WIN32(GetLastError());
		}

		switch (dwChunkType) {
		case 'FFIR':
			dwRIFFDataSize = dwChunkDataSize;
			dwChunkDataSize = 4;
			if (ReadFile(hFile, &dwFileType, sizeof(DWORD), &dwRead, NULL) == 0) {
				// チャンクタイプの読み込み
				hr = HRESULT_FROM_WIN32(GetLastError());
			}
			break;

		default:
			if (SetFilePointer(hFile, dwChunkDataSize, NULL, FILE_CURRENT) == INVALID_SET_FILE_POINTER) {
				// ファイルポインタをチャンクデータ分移動
				return HRESULT_FROM_WIN32(GetLastError());
			}
		}

		dwOffset += sizeof(DWORD) * 2;
		if (dwChunkType == format) {
			*pChunkSize = dwChunkDataSize;
			*pChunkDataPosition = dwOffset;

			return S_OK;
		}

		dwOffset += dwChunkDataSize;
		if (dwBytesRead >= dwRIFFDataSize) {
			return S_FALSE;
		}
	}

	return S_OK;
}


/*									//
//		チャンクデータの読み込み	//
//									*/
HRESULT WaveLoader::ReadChunkData(HANDLE hFile, void* pBuffer, DWORD dwBuffersize, DWORD dwBufferoffset) {
	DWORD dwRead;

	if (SetFilePointer(hFile, dwBufferoffset, NULL, FILE_BEGIN) == INVALID_SET_FILE_POINTER) {
		// ファイルポインタを指定位置まで移動
		return HRESULT_FROM_WIN32(GetLastError());
	}

	if (ReadFile(hFile, pBuffer, dwBuffersize, &dwRead, NULL) == 0) {
		// データの読み込み
		return HRESULT_FROM_WIN32(GetLastError());
	}

	return S_OK;
}