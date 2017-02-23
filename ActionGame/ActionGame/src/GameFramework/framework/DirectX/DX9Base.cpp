//////////////////////////////////////////////////////////////
// 2016. 2. 7	�v���O�����쐬
// author		SyunMizuno
//////////////////////////////////////////////////////////////

#include	"DX9Base.h"
#include	<tchar.h>
#include	"../Screen/Screen.h"
#include	"../Utility/System/SystemUtility.h"


/*									//
//			�R���X�g���N�^			//
//									*/
DX9Base::DX9Base(Application* app) :
	IDXBase(app),
	m_bDeviceLost(false),
	m_hMenu(NULL),
	m_bActive(false),
	m_pGraphics(NULL)
{}


/*									//
//			�f�X�g���N�^			//
//									*/
DX9Base::~DX9Base() {
	Release();
}


/*									//
//				������				//
//									*/
// <return> S_OK. ���� : E_FAIL. ���s
HRESULT DX9Base::Init() {
	D3DDISPLAYMODE	d3ddm;

	// Direct3D�I�u�W�F�N�g����
	m_pD3Direct = Direct3DCreate9(D3D_SDK_VERSION);
	if (m_pD3Direct == NULL) {
		return E_FAIL;
	}

	// ���݂̃f�B�X�v���C���[�h�擾
	if (FAILED(m_pD3Direct->GetAdapterDisplayMode(D3DADAPTER_DEFAULT, &d3ddm))) {
		return E_FAIL;
	}

	PreWindow();
	PreScreen();

	if (!Screen::GetFullScreen())	m_D3Dpp = m_D3DppWindow;
	else							m_D3Dpp = m_D3DppFull;

	if (FAILED(CreateDevice()))
		return E_FAIL;

	if (FAILED(CreateViewport()))
		return E_FAIL;

	CreateState();

	ShowCursor(TRUE);

	if (FAILED(InitApp()))
		return E_FAIL;

	return S_OK;
}


/*									//
//		�E�C���h�E���[�h�̐ݒ�		//
//									*/
void DX9Base::PreWindow() {
	HWND hWnd = m_pApp->GetHWND();

	ZeroMemory( &m_D3DppWindow, sizeof(m_D3DppWindow) );
	m_D3DppWindow.BackBufferWidth				= 0;
	m_D3DppWindow.BackBufferHeight				= 0;
	m_D3DppWindow.BackBufferFormat				= D3DFMT_UNKNOWN;
	m_D3DppWindow.BackBufferCount				= 1;
	m_D3DppWindow.MultiSampleType				= D3DMULTISAMPLE_NONE;
	m_D3DppWindow.MultiSampleQuality			= 0;
	m_D3DppWindow.SwapEffect					= D3DSWAPEFFECT_DISCARD;
	m_D3DppWindow.hDeviceWindow					= hWnd;
	m_D3DppWindow.Windowed						= TRUE;
	m_D3DppWindow.EnableAutoDepthStencil		= TRUE;								// �X�e���V���o�b�t�@�iTRUE:�g�p,FALSE:�s�g�p�j
	m_D3DppWindow.AutoDepthStencilFormat		= D3DFMT_D24S8;						// �f�v�X�Ƃ���24bit,�X�e���V���Ƃ���8bit���g��
	m_D3DppWindow.Flags							= 0;
	m_D3DppWindow.FullScreen_RefreshRateInHz	= 0;								// ���t���b�V�����[�g�i�w��ł��Ȃ��̂�0�Œ�j
	m_D3DppWindow.PresentationInterval			= D3DPRESENT_INTERVAL_IMMEDIATE;	// �C���^�[�o���iVSync��҂����ɕ`��j
}


/*									//
//		�t���X�N���[�����[�h�̐ݒ�	//
//									*/
void DX9Base::PreScreen() {
	HWND hWnd = m_pApp->GetHWND();

	ZeroMemory( &m_D3DppFull, sizeof(m_D3DppFull) );
	m_D3DppFull.BackBufferWidth				= Screen::GetWidth();
	m_D3DppFull.BackBufferHeight			= Screen::GetHeight();
	m_D3DppFull.BackBufferFormat			= D3DFMT_X8R8G8B8;
	m_D3DppFull.BackBufferCount				= 1;
	m_D3DppFull.MultiSampleType				= D3DMULTISAMPLE_NONE;
	m_D3DppFull.MultiSampleQuality			= 0;
	m_D3DppFull.SwapEffect					= D3DSWAPEFFECT_DISCARD;
	m_D3DppFull.hDeviceWindow				= hWnd;
	m_D3DppFull.Windowed					= FALSE;
	m_D3DppFull.EnableAutoDepthStencil		= TRUE;							// �X�e���V���o�b�t�@�iTRUE:�g�p,FALSE:�s�g�p�j
	m_D3DppFull.AutoDepthStencilFormat		= D3DFMT_D24S8;					// �f�v�X�Ƃ���24bit,�X�e���V���Ƃ���8bit���g��
	m_D3DppFull.Flags						= 0;
	m_D3DppFull.FullScreen_RefreshRateInHz	= D3DPRESENT_RATE_DEFAULT;		// ���t���b�V�����[�g�i���݂̑��x�ɍ��킹��j
	m_D3DppFull.PresentationInterval		= D3DPRESENT_INTERVAL_DEFAULT;	// �C���^�[�o���iVSync��҂��ĕ`��j
}


/*									//
//			�f�o�C�X�ݒ�			//
//									*/
// <return> S_OK. ���� : E_FAIL. ���s
HRESULT DX9Base::CreateDevice() {
	HWND hWnd = m_pApp->GetHWND();

	// �f�o�C�X�I�u�W�F�N�g�̐���
	// [�f�o�C�X�쐬����]<�`��>��<���_����>���n�[�h�E�F�A�ōs��
	if (FAILED(m_pD3Direct->CreateDevice(
		D3DADAPTER_DEFAULT,						// �f�B�X�v���C�A�_�v�^
		D3DDEVTYPE_HAL,							// �f�o�C�X�^�C�v
		hWnd,									// �t�H�[�J�X����E�C���h�E�ւ̃n���h��
		D3DCREATE_HARDWARE_VERTEXPROCESSING |	// �f�o�C�X�쐬����̑g�ݍ��킹
		D3DCREATE_MULTITHREADED,				// �}���`�X���b�h�����S��
		&m_D3Dpp,								// �f�o�C�X�̃v���[���e�[�V�����p�����[�^
		&m_pDevice)))							// �f�o�C�X�C���^�[�t�F�[�X�ւ̃|�C���^
	{
		// [�f�o�C�X�쐬����]<�`��>���n�[�h�E�F�A�ōs���A<���_����>��CPU�ōs��
		if (FAILED(m_pD3Direct->CreateDevice(
			D3DADAPTER_DEFAULT,
			D3DDEVTYPE_HAL,
			hWnd,
			D3DCREATE_SOFTWARE_VERTEXPROCESSING |
			D3DCREATE_MULTITHREADED,
			&m_D3Dpp,
			&m_pDevice))) {
			// [�f�o�C�X�쐬����]<�`��>��<���_����>��CPU�ōs��
			if (FAILED(m_pD3Direct->CreateDevice(
				D3DADAPTER_DEFAULT,
				D3DDEVTYPE_REF,
				hWnd,
				D3DCREATE_SOFTWARE_VERTEXPROCESSING |
				D3DCREATE_MULTITHREADED,
				&m_D3Dpp,
				&m_pDevice))) {
				return E_FAIL;
			}
		}
	}

	return S_OK;
}


/*									//
//			�r���[�|�[�g�ݒ�		//
//									*/
// <return> S_OK. ���� : E_FAIL. ���s
HRESULT DX9Base::CreateViewport() {
	D3DVIEWPORT9 vp;
	vp.X = 0;
	vp.Y = 0;
	vp.Width = m_D3Dpp.BackBufferWidth;
	vp.Height = m_D3Dpp.BackBufferHeight;
	vp.MinZ = 0.0f;
	vp.MaxZ = 1.0f;

	HRESULT hr;
	hr = m_pDevice->SetViewport(&vp);
	if (FAILED(hr)) {
		HWND hWnd = m_pApp->GetHWND();
		MessageBox(hWnd, _T("ChangeWindowSize SetViewport"), _T("�r���[�|�[�g�̍쐬�Ɏ��s"), NULL);
		DestroyWindow(hWnd);
		return hr;
	}

	return S_OK;
}


/*									//
//			�X�e�C�g�ݒ�			//
//									*/
void DX9Base::CreateState() {
	m_pDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);
	m_pDevice->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);	// �ŏ��̃A���t�@����
	m_pDevice->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_CURRENT);	// �Q�Ԗڂ̃A���t�@����

	m_pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
	m_pDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	m_pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	m_pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
	m_pDevice->SetRenderState(D3DRS_ZENABLE, TRUE);

	m_pDevice->SetSamplerState(0, D3DSAMP_ADDRESSU, D3DTADDRESS_WRAP);
	m_pDevice->SetSamplerState(0, D3DSAMP_ADDRESSV, D3DTADDRESS_WRAP);
	m_pDevice->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
	m_pDevice->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);


	m_pGraphics = new Graphics;
	m_pGraphics->SetDevice(m_pDevice);

}


/*									//
//			�A�v���̏�����			//
//									*/
// <return> S_OK. ���� : E_FAIL. ���s
HRESULT DX9Base::InitApp() {
	return S_OK;
}


/*									//
//				�X�e�b�v			//
//									*/
// <return> S_OK. ���� : E_FAIL. ���s
HRESULT DX9Base::Step() {
	Update();
	Draw();

	return S_OK;
}


/*									//
//				�X�V				//
//									*/
void DX9Base::Update() {
}


/*									//
//				�`��				//
//									*/
void DX9Base::Draw() {
	m_pDevice->Clear(0, NULL, (D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER | D3DCLEAR_STENCIL), D3DCOLOR_RGBA(0, 0, 0, 0), 1.0f, 0);

	// Direct3D�ɂ��`��̊J�n
	if (SUCCEEDED(m_pDevice->BeginScene())) {

		// Direct3D�ɂ��`��̏I��
		m_pDevice->EndScene();
	}

	// �o�b�N�o�b�t�@�ƃt�����g�o�b�t�@�̓���ւ�
	m_pDevice->Present(NULL, NULL, NULL, NULL);
}


/*									//
//				���				//
//									*/
// <return> S_OK. ���� : E_FAIL. ���s
HRESULT DX9Base::Release() {
	if (FAILED(ReleaseApp()))
		return E_FAIL;

	if (m_hMenu)
		DestroyMenu(m_hMenu);

	SAFE_DELETE(m_pGraphics);

	return S_OK;
}


/*									//
//			�A�v���̉��			//
//									*/
// <return> S_OK. ���� : E_FAIL. ���s
HRESULT DX9Base::ReleaseApp() {
	return S_OK;
}


/*									//
//			�T�C�Y�̕ύX			//
//									*/
// <return> S_OK. ���� : E_FAIL. ���s
HRESULT DX9Base::ChangeWindowSize() {
	InvalidateDeviceObjects();

	HWND hWnd = m_pApp->GetHWND();

	// ���Z�b�g
	HRESULT hr = m_pDevice->Reset(&m_D3Dpp);
	if (FAILED(hr)) {
		if (hr == D3DERR_DEVICELOST)
			m_bDeviceLost = true;
		else
			DestroyWindow(hWnd);
		MessageBox(hWnd, _T("ChangeWindowSize ERROR"), _T("�E�C���h�E�T�C�Y�̕ύX�Ɏ��s"), NULL);
		return hr;
	}

	// �r���[�|�[�g�̐ݒ�
	if (FAILED(CreateViewport()))
		return E_FAIL;

	// �X�e�C�g�̏�����
	CreateState();

	RestoreDeviceObjects();

	return hr;
}


/*									//
//			���[�h�̕ύX			//
//									*/
void DX9Base::ChangeDisplayMode() {
	HWND hWnd = m_pApp->GetHWND();
	bool window = !Screen::GetFullScreen();

	// ���[�h�ύX
	window = !window;
	if (window) {
		m_D3Dpp = m_D3DppWindow;
	} else {
		m_D3Dpp = m_D3DppFull;
		GetWindowRect(hWnd, &m_rectWindow);
	}
	InvalidateDeviceObjects();

	HRESULT hr = m_pDevice->Reset(&m_D3Dpp);
	if (FAILED(hr)) {
		if (hr == D3DERR_DEVICELOST)
			m_bDeviceLost = true;
		else
			DestroyWindow(hWnd);
		MessageBox(hWnd, _T("ChangeDisplayMode Reset"), _T("�E�C���h�E�̕ύX�Ɏ��s���܂���"), NULL);
	}

	if (window) {
		SetWindowLong(hWnd, GWL_STYLE, WS_OVERLAPPEDWINDOW | WS_VISIBLE);
		if (m_hMenu != NULL) {
			SetMenu(hWnd, m_hMenu);
			m_hMenu = NULL;
		}
		SetWindowPos(
			hWnd,
			HWND_NOTOPMOST,
			m_rectWindow.left,
			m_rectWindow.top,
			m_rectWindow.right - m_rectWindow.left,
			m_rectWindow.bottom - m_rectWindow.top,
			SWP_SHOWWINDOW);
	} else {
		SetWindowLong(hWnd, GWL_STYLE, WS_POPUP | WS_VISIBLE);
		if (m_hMenu == NULL) {
			m_hMenu = GetMenu(hWnd);
			SetMenu(hWnd, NULL);
		}
	}
	// �X�e�C�g�̏�����
	CreateState();

	RestoreDeviceObjects();
	m_pApp->SetHWND(hWnd);
}