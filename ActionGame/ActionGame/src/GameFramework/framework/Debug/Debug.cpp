#include	"Debug.h"

IDirect3DDevice9*	Debug::m_pDevice = NULL;
ID3DXFont*			Debug::m_pFont = NULL;
D3DXFONT_DESC		Debug::m_D3DFD;
int					Debug::m_Block_x = 0;
int					Debug::m_Block_y = 3;
#if	_UNICODE
	wstring		Debug::str;		// �f�o�b�O������ێ��o�b�t�@�iUNICODE�p�j
#else
	string		Debug::str;		// �f�o�b�O������ێ��o�b�t�@�iANCI�p�j
#endif

