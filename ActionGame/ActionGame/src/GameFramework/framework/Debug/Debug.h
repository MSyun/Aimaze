// Direct3D�f�o�b�O�t�H���g�N���X�錾��
// �쐬��		:	SyunMizuno
// 2015.11.28	:	�v���O�����쐬
// 2016. 1. 2	:	InvalidateDeviceObjects,RestoreDeviceObjects��ǉ�
// 2016.12.31	:	Log�֐��ǉ�
//  HP    : http://www.h6.dion.ne.jp/~game296o/index.html

#pragma once

#pragma warning( disable : 4996 )

#include	<d3dx9.h>
#include	<tchar.h>
#include	<string>
#include	<stdlib.h>


/*
	memo:
		_stprintf��Unicode,ANCI�̗��Ή��A�L�\
		�C���^�[�t�F�[�X����鎖
*/


using namespace std;


class Debug {
private:
#pragma region variable

	static IDirect3DDevice9*	m_pDevice;	// �`��f�o�C�X
	static ID3DXFont*			m_pFont;	// �t�H���g�I�u�W�F�N�g�|�C���^
	static D3DXFONT_DESC		m_D3DFD;	// �t�H���g����
	static int					m_Block_x;	// �\���ʒu
	static int					m_Block_y;
	static bool					m_bDraw;

#if	_UNICODE
	static  wstring		str;		// �f�o�b�O������ێ��o�b�t�@�iUNICODE�p�j
#else
	static string		str;		// �f�o�b�O������ێ��o�b�t�@�iANCI�p�j
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

	// �������
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

	// �`��
	static void Render() {
		if (!m_bDraw)	return;

		// ���ݒ~�����Ă��镶�����`��
		if (!m_pFont) {
			str = _T("");
			return;
		}

		RECT rect;
		SetRect(&rect, 0, 0, 0, 0);
		m_pFont->DrawText(NULL, str.c_str(), -1, &rect, DT_LEFT | DT_CALCRECT, 0xffffffff);
		OffsetRect(&rect, m_Block_x * m_D3DFD.Width, m_Block_y * m_D3DFD.Height);
		m_pFont->DrawText(NULL, str.c_str(), -1, &rect, DT_LEFT, 0xffffffff);

		// �����o�b�t�@���N���A
		str = _T("");
	}

	// ���O�̕\��
	static void Log(string message) {
		message += "\n";
		OutputDebugString(message.c_str());
	}
	// �G���[���O�̕\��
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


	// �ʒu�ݒ�ȈՎw��p���Z�q
	//  debugfont( 10, 20 )�ƈʒu�w��ł��܂�
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
			// �f�t�H���g�t�H���g�쐬
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