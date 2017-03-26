#include	"Debug.h"
#include	<tchar.h>


IDirect3DDevice9*	Debug::m_pDevice = NULL;
ID3DXFont*			Debug::m_pFont = NULL;
D3DXFONT_DESC		Debug::m_D3DFD;
int					Debug::m_Block_x = 0;
int					Debug::m_Block_y = 3;

#ifdef _DEBUG
bool				Debug::m_bDraw = true;
#else
bool				Debug::m_bDraw = false;
#endif // _DEBUG

#if	_UNICODE
	wstring		Debug::str;		// デバッグ文字列保持バッファ（UNICODE用）
#else
	string		Debug::str;		// デバッグ文字列保持バッファ（ANCI用）
#endif


/*									//
//			コンストラクタ			//
//									*/
Debug::Debug() {

}


/*									//
//			デストラクタ			//
//									*/
Debug::~Debug() {
	if (m_pDevice)	m_pDevice->Release();
	if (m_pFont)	m_pFont->Release();
}


/*									//
//			デバイスの設定			//
//									*/
void Debug::SetDevice(IDirect3DDevice9* _dev) {
	m_pDevice = _dev;
	Create();
}


/*									//
//				描画の設定			//
//									*/
void Debug::SetDraw(bool draw) {
	m_bDraw = draw;
	str = _T("");
}


/*									//
//			描画の可否を設定		//
//									*/
void Debug::ChangeDraw() {
	m_bDraw ^= true;
	str = _T("");
}


#pragma region Print
/*									//
//画面に表示するデバッグ文字列を追加//
//									*/
void Debug::Print(const TCHAR* c) {
	str += c;
}
void Debug::Print(unsigned char val) {
	str += val;
}
void Debug::Print(short val) {
	TCHAR m_c[20];
	_stprintf(m_c, _T("%d"), val);
	str += m_c;
}
void Debug::Print(unsigned short val) {
	TCHAR m_c[20];
	_stprintf(m_c, _T("%d"), val);
	str += m_c;
}
void Debug::Print(int val) {
	TCHAR m_c[20];
	_stprintf(m_c, _T("%d"), val);
	str += m_c;
}
void Debug::Print(unsigned int val) {
	TCHAR m_c[20];
	_stprintf(m_c, _T("%d"), val);
	str += m_c;
}
void Debug::Print(unsigned long val) {
	TCHAR m_c[20];
	_stprintf(m_c, _T("%d"), val);
	str += m_c;
}
void Debug::Print(float val) {
	TCHAR m_c[20];
	_stprintf(m_c, _T("%8.3f"), val);
	str += m_c;
}
void Debug::Print(double val) {
	TCHAR m_c[20];
	_stprintf(m_c, _T("%8.3lf"), val);
	str += m_c;
}
void Debug::Print(const Vector2& val) {
	str +=
		"( " +
		to_string(val.x) + ", " +
		to_string(val.y) +
		" )";
}
void Debug::Print(const Vector3& val) {
	str += 
		"( " +
		to_string(val.x) + ", " +
		to_string(val.y) + ", " +
		to_string(val.z) +
		" )";
}
void Debug::Print(const Vector4& val) {
	str +=
		"( " +
		to_string(val.x) + ", " +
		to_string(val.y) + ", " +
		to_string(val.z) + ", " +
		to_string(val.w) +
		" )";
}
void Debug::Print(const Quaternion& qua) {
	str +=
		"( " +
		to_string(qua.x) + ", " +
		to_string(qua.y) + ", " +
		to_string(qua.z) + ", " +
		to_string(qua.w) +
		" )";
}
#pragma endregion


#pragma region Log
/*									//
//		エディター上にログを表示	//
//									*/
void Debug::Log(const string& message) {
	string st(message);
	st += "\n";
	OutputDebugString(st.c_str());
}
void Debug::Log(const Vector2& val) {
	string st = "( " +
		to_string(val.x) + ", " +
		to_string(val.y) +
		" )\n";
	OutputDebugString(st.c_str());
}
void Debug::Log(const Vector3& val) {
	string st =  "( " +
		to_string(val.x) + ", " +
		to_string(val.y) + ", " +
		to_string(val.z) +
		" )\n";
	OutputDebugString(st.c_str());
}
void Debug::Log(const Vector4& val) {
	string st = "( " +
		to_string(val.x) + ", " +
		to_string(val.y) + ", " +
		to_string(val.z) + ", " +
		to_string(val.w) +
		" )\n";
	OutputDebugString(st.c_str());
}
void Debug::Log(const Quaternion& qua) {
	string st = "( " +
		to_string(qua.x) + ", " +
		to_string(qua.y) + ", " +
		to_string(qua.z) + ", " +
		to_string(qua.w) +
		" )\n";
	OutputDebugString(st.c_str());
}
#pragma endregion


/*									//
//			エラーログの表示		//
//									*/
// エディターがここで止まるのでF10で場所を確認してください
void Debug::LogError(const string& message) {
#ifdef _DEBUG
	string st(message);
	st = "Error : " + st;
	Log(st);
	DebugBreak();
#endif // DEBUG
}


/*									//
//	保存した文字列を画面に描画		//
//									*/
void Debug::Draw() {
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


/*									//
//			デバイスロスト			//
//									*/
HRESULT Debug::InvalidateDeviceObjects() {
	if (!m_pFont->OnLostDevice()) return false;
	return true;
}


/*									//
//			デバイスリセット		//
//									*/
HRESULT Debug::RestoreDeviceObjects() {
	if (!m_pFont->OnResetDevice()) return false;
	return true;
}