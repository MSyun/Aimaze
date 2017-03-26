// Direct3Dデバッグフォントクラス宣言部
// 作成者		:	SyunMizuno
// 2015.11.28	:	プログラム作成
// 2016. 1. 2	:	InvalidateDeviceObjects,RestoreDeviceObjectsを追加
// 2016.12.31	:	Log関数追加
// 2017. 3.26	:	実装処理を.cppへ移動
//  HP    : http://www.h6.dion.ne.jp/~game296o/index.html

#pragma once

#pragma warning( disable : 4996 )

#include	<d3dx9.h>
#include	<string>
#include	<stdlib.h>
#include	"../Convert.h"


/*
	memo:
		_stprintfはUnicode,ANCIの両対応、有能
		インターフェースを作る事
*/


using namespace std;


class Debug {
private:
#pragma region variable

	static IDirect3DDevice9*	m_pDevice;	// 描画デバイス
	static ID3DXFont*			m_pFont;	// フォントオブジェクトポインタ
	static D3DXFONT_DESC		m_D3DFD;	// フォント属性
	static int					m_Block_x;	// 表示位置
	static int					m_Block_y;
	static bool					m_bDraw;

#if	_UNICODE
	static  wstring		str;		// デバッグ文字列保持バッファ（UNICODE用）
#else
	static string		str;		// デバッグ文字列保持バッファ（ANCI用）
#endif

#pragma endregion

public:
#pragma region method

	Debug();
	~Debug();

	// デバイスの設定
	static void SetDevice(IDirect3DDevice9* _dev);

	// 描画の設定
	static void SetDraw(bool draw);

	// 描画の可否を変更
	static void ChangeDraw();

	// 保存した文字列を画面に描画
	static void Draw();

#pragma region Print
	// 画面に表示するデバッグ文字列を追加
	static void Print(const TCHAR* c);
	static void Print(unsigned char val);
	static void Print(short val);
	static void Print(unsigned short val);
	static void Print(int val);
	static void Print(unsigned int val);
	static void Print(unsigned long val);
	static void Print(float val);
	static void Print(double val);
	static void Print(const Vector2& val);
	static void Print(const Vector3& val);
	static void Print(const Vector4& val);
	static void Print(const Quaternion& qua);
#pragma endregion

#pragma region Log
	// エディター上にログを表示
	static void Log(const string& message);
	static void Log(const Vector2& val);
	static void Log(const Vector3& val);
	static void Log(const Vector4& val);
	static void Log(const Quaternion& qua);
#pragma endregion

	// エラーログの表示
	// エディターがここで止まるのでF10で場所を確認してください
	static void LogError(const string& message);

	// デバイスロスト
	static HRESULT InvalidateDeviceObjects();

	// デバイスリセット
	static HRESULT RestoreDeviceObjects();

	// 位置設定簡易指定用演算子
	//  debugfont( 10, 20 )と位置指定できます
	Debug& operator ()(int x, int y) {
		return SetPos(x, y);
	}

#pragma endregion

protected:
#pragma region conceal method

	Debug& SetPos(int x, int y) {
		m_Block_x = x;
		m_Block_y = y;
		return *this;
	}

	static void Create() {
		if (m_pDevice) {
			// デフォルトフォント作成
			m_pDevice->AddRef();
			m_D3DFD.Height = 14;
			m_D3DFD.Width = 7;
			m_D3DFD.Weight = 500;
			m_D3DFD.MipLevels = D3DX_DEFAULT;
			m_D3DFD.Italic = false;
			m_D3DFD.CharSet = DEFAULT_CHARSET;
			m_D3DFD.OutputPrecision = OUT_DEFAULT_PRECIS;
			m_D3DFD.Quality = DEFAULT_QUALITY;
			m_D3DFD.PitchAndFamily = FIXED_PITCH | FF_MODERN;
			memset(m_D3DFD.FaceName, 0, sizeof(m_D3DFD.FaceName));

			D3DXCreateFontIndirect(m_pDevice, &m_D3DFD, &m_pFont);
		}
	}

#pragma endregion
};