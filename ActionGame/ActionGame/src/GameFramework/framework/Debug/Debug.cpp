#include	"Debug.h"

IDirect3DDevice9*	Debug::m_pDevice = NULL;
ID3DXFont*			Debug::m_pFont = NULL;
D3DXFONT_DESC		Debug::m_D3DFD;
int					Debug::m_Block_x = 0;
int					Debug::m_Block_y = 3;
#if	_UNICODE
	wstring		Debug::str;		// デバッグ文字列保持バッファ（UNICODE用）
#else
	string		Debug::str;		// デバッグ文字列保持バッファ（ANCI用）
#endif

