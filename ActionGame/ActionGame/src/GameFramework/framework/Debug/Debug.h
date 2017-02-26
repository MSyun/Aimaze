// Direct3Dデバッグフォントクラス宣言部
// 作成者		:	SyunMizuno
// 2015.11.28	:	プログラム作成
// 2016. 1. 2	:	InvalidateDeviceObjects,RestoreDeviceObjectsを追加
// 2016.12.31	:	Log関数追加
//  HP    : http://www.h6.dion.ne.jp/~game296o/index.html

#pragma once

#pragma warning( disable : 4996 )

#include	<d3dx9.h>
#include	<tchar.h>
#include	<string>
#include	<stdlib.h>


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

	Debug() {}

	~Debug() {
		if (m_pDevice)	m_pDevice->Release();
		if (m_pFont)	m_pFont->Release();
	}

	static void SetDevice(IDirect3DDevice9* _dev) {
		m_pDevice = _dev;
		Create();
	}

	static void SetDraw(bool draw) {
		m_bDraw = draw;
		str = _T("");
	}
	static void ChangeDraw() {
		m_bDraw ^= true;
		str = _T("");
	}

#pragma region Print

	// 文字代入
	static void Print(const TCHAR* c) {
		str += c;
	}
	static void Print(unsigned char val) {
		str += val;
	}
	static void Print(short val) {
		TCHAR* m_c;
		_stprintf(m_c, _T("%d"), val);
		str += m_c;
	}
	static void Print(unsigned short val) {
		TCHAR* m_c;
		_stprintf(m_c, _T("%d"), val);
		str += m_c;
	}
	static void Print(int val) {
		TCHAR m_c[20];
		_stprintf(m_c, _T("%d"), val);
		str += m_c;
	}
	static void Print(unsigned int val) {
		TCHAR m_c[20];
		_stprintf(m_c, _T("%d"), val);
		str += m_c;
	}
	static void Print(unsigned long val) {
		TCHAR m_c[20];
		_stprintf(m_c, _T("%d"), val);
		str += m_c;
	}
	static void Print(float val) {
		TCHAR m_c[20];
		_stprintf(m_c, _T("%8.3f"), val);
		str += m_c;
	}
	static void Print(double val) {
		TCHAR m_c[20];
		_stprintf(m_c, _T("%8.3lf"), val);
		str += m_c;
	}
	static void Print(bool val) {
		if (val)
			str += _T("true");
		else
			str += _T("false");
	}
	//static void Print( void* ptr ) {
	//	_stprintf( m_c, _T("%08x"), ptr );
	//	str += m_c;
	//}

#pragma endregion

	// 描画
	static void Render() {
		if (!m_bDraw)	return;

		// 現在蓄えられている文字列を描画
		if (!m_pFont) {
			str = _T("");
			return;
		}

		RECT rect;
		SetRect(&rect, 0, 0, 0, 0);
		m_pFont->DrawText(NULL, str.c_str(), -1, &rect, DT_LEFT | DT_CALCRECT, 0xffffffff);
		OffsetRect(&rect, m_Block_x * m_D3DFD.Width, m_Block_y * m_D3DFD.Height);
		m_pFont->DrawText(NULL, str.c_str(), -1, &rect, DT_LEFT, 0xffffffff);

		// 文字バッファをクリア
		str = _T("");
	}

	// ログの表示
	static void Log(string message) {
		message += "\n";
		OutputDebugString(message.c_str());
	}
	// エラーログの表示
	static void LogError(string message) {
#ifdef _DEBUG
		message = "Error : " + message;
		Log(message);
		DebugBreak();
#endif // DEBUG
	}

	static HRESULT InvalidateDeviceObjects() {
		if (!m_pFont->OnLostDevice()) return false;
		return true;
	}
	static HRESULT RestoreDeviceObjects() {
		if (!m_pFont->OnResetDevice()) return false;
		return true;
	}


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