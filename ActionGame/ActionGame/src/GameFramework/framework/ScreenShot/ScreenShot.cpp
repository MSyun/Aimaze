// �X�N���[���V���b�g�N���X
// 2015.11.14	�v���W�F�N�g�쐬
// author		SyunMizuno

#include	"ScreenShot.h"


/*
	MEMO	:
			�Ⴄ�X���b�h�Ŏ��s���������������������Ȃ��Ȃ��Ă悢����
*/


/*									//
//			�R���X�g���N�^			//
//									*/
ScreenShot::ScreenShot(IDirect3DDevice9* pDevice, HWND hWnd) :
	m_pDevice(pDevice),
	m_hWnd(hWnd)
{}


/*									//
//			�f�X�g���N�^			//
//									*/
ScreenShot::~ScreenShot() {
}


/*									//
//				�B�e				//
//									*/
void ScreenShot::Snap(const string filename, bool window) {
	IDirect3DSurface9*	pSurface = NULL;

	this->CreateSurface(pSurface);

	RECT rect = this->CreateRect(window);

	// �T�[�t�F�C�X���摜�ɏo��
	D3DXSaveSurfaceToFile(
		filename.c_str(),	// �o�͂���t�@�C����
		D3DXIFF_BMP,		// �o�͂���t�@�C���`��BITMAP�Ƃ�
		pSurface,			// ���͂���T�[�t�F�C�X
		NULL,				// 256 �F�̃p���b�g���܂� PALETTEENTRY �\���̂ւ̃|�C���^�@�ʏ��NULL
		&rect);				// �o�͂���摜�͈̔� NULL���Ɖ摜�S��

	pSurface->Release();
	pSurface = NULL;
}


/*									//
//			�T�[�t�F�C�X�̍쐬		//
//									*/
void ScreenShot::CreateSurface(IDirect3DSurface9* pSurface) {
	m_pDevice->CreateOffscreenPlainSurface(
		GetSystemMetrics(SM_CXSCREEN),	// �T�[�t�F�C�X�̕�
		GetSystemMetrics(SM_CYSCREEN),	// �T�[�t�F�C�X�̍���
		D3DFMT_A8R8G8B8,				// �v���C�}���o�b�t�@�Ɠ���D3DFORMAT
		D3DPOOL_SYSTEMMEM,				// �������̃A�N�Z�X�`��
		&pSurface,						// �쐬����T�[�t�F�C�X
		NULL);							// �\�񂳂�Ă������	�ʏ��NULL

	m_pDevice->GetFrontBufferData(
		0,				// �ʏ��0
		pSurface);		// �T�[�t�F�C�X�Ƀf�[�^���o��
}


/*									//
//		��ʃT�C�Y�̋�`���쐬		//
//									*/
RECT ScreenShot::CreateRect(bool window) {
	RECT rect;
	GetWindowRect(m_hWnd, &rect);

	// �E�C���h�E�̏ꍇ
	if (window) {// �E�C���h�E�T�C�Y�ɍ��킹��
		rect.bottom -= GetSystemMetrics(SM_CYDLGFRAME);
		rect.top += GetSystemMetrics(SM_CYDLGFRAME) + GetSystemMetrics(SM_CYCAPTION);
		rect.left += GetSystemMetrics(SM_CXDLGFRAME);
		rect.right -= GetSystemMetrics(SM_CXDLGFRAME);
	}

	return rect;
}