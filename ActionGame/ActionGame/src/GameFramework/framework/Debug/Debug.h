// Direct3D�f�o�b�O�t�H���g�N���X�錾��
// �쐬��		:	SyunMizuno
// 2015.11.28	:	�v���O�����쐬
// 2016. 1. 2	:	InvalidateDeviceObjects,RestoreDeviceObjects��ǉ�
// 2016.12.31	:	Log�֐��ǉ�
// 2017. 3.26	:	����������.cpp�ֈړ�
//  HP    : http://www.h6.dion.ne.jp/~game296o/index.html

#pragma once

#pragma warning( disable : 4996 )

#include	<d3dx9.h>
#include	<string>
#include	<stdlib.h>
#include	"../Convert.h"


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

	Debug();
	~Debug();

	// �f�o�C�X�̐ݒ�
	static void SetDevice(IDirect3DDevice9* _dev);

	// �`��̐ݒ�
	static void SetDraw(bool draw);

	// �`��̉ۂ�ύX
	static void ChangeDraw();

	// �ۑ��������������ʂɕ`��
	static void Draw();

#pragma region Print
	// ��ʂɕ\������f�o�b�O�������ǉ�
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
	// �G�f�B�^�[��Ƀ��O��\��
	static void Log(const string& message);
	static void Log(const Vector2& val);
	static void Log(const Vector3& val);
	static void Log(const Vector4& val);
	static void Log(const Quaternion& qua);
#pragma endregion

	// �G���[���O�̕\��
	// �G�f�B�^�[�������Ŏ~�܂�̂�F10�ŏꏊ���m�F���Ă�������
	static void LogError(const string& message);

	// �f�o�C�X���X�g
	static HRESULT InvalidateDeviceObjects();

	// �f�o�C�X���Z�b�g
	static HRESULT RestoreDeviceObjects();

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