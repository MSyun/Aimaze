//////////////////////////////////////////////////////////////
// DirectX9�p�N���X
// 2016. 2. 7	�v���O�����쐬
// author		SyunMizuno
//////////////////////////////////////////////////////////////

#pragma once


#include	"DXBase.h"
#include	<d3dx9.h>
#include	"../Graphic/Graphics.h"


#if 1
#pragma comment ( lib, "d3d9.lib" )		// �`�揈���ɕK�v
#pragma comment ( lib, "d3dx9.lib" )	// [d3d9.lib]�̊g�����C�u����
#pragma comment ( lib, "dxguid.lib" )	// DirectX�R���|�[�l���g�g�p�ɕK�v
#endif

/*
	memo	:
		DirectX10 or 11�Ŏg�����킩��Ȃ������o������̂ł킩�莟��ׁ[�X�܂ߕύX
*/

/*								//
//		DirectX9�p�x�[�X		//
//								*/
class	DX9Base : public	IDXBase {
protected:
#pragma region variable

	D3DPRESENT_PARAMETERS	m_D3Dpp;		// ����
	D3DPRESENT_PARAMETERS	m_D3DppWindow;	// �E�C���h�E�E���[�h�p
	D3DPRESENT_PARAMETERS	m_D3DppFull;	// �t���X�N���[���E���[�h�p
	LPDIRECT3DDEVICE9		m_pDevice;		// �`��f�o�C�X
	LPDIRECT3D9				m_pD3Direct;

	bool					m_bDeviceLost;	// �f�o�C�X�����t���O
	RECT					m_rectWindow;	// �E�C���h�E�E���[�h�ł̍Ō�̈ʒu
	HMENU					m_hMenu;		// ���j���[�E�n���h��
	bool					m_bActive;		// �A�N�e�B�u���

	Graphics*				m_pGraphics;	// �O���t�B�b�N

#pragma endregion

public:
#pragma region method

	DX9Base(Application* app);
	virtual ~DX9Base();

	// ������
	virtual HRESULT Init();

	// �X�e�b�v�֐�
	virtual HRESULT Step();

	// �I������
	virtual HRESULT Release();

	virtual HRESULT ChangeWindowSize();	// �T�C�Y�̕ύX
	virtual void ChangeDisplayMode();	// ���[�h�̕ύX

#pragma endregion

protected:
#pragma region conceal method

	virtual HRESULT InvalidateDeviceObjects() { return S_OK; };	// OnLostDevice
	virtual HRESULT RestoreDeviceObjects() { return S_OK; };	// OnResetDevice

	// ���[�h�ݒ�
	virtual void PreWindow();	// �E�C���h�E
	virtual void PreScreen();	// �t���X�N���[��

	// �X�e�C�g�ݒ�
	virtual void CreateState();

	// �r���[�|�[�g�ݒ�
	virtual HRESULT CreateViewport();

	// �f�o�C�X�̐ݒ�
	virtual HRESULT CreateDevice();

	virtual HRESULT InitApp();
	virtual HRESULT ReleaseApp();
	virtual void Update();
	virtual void Draw();

#pragma endregion
};