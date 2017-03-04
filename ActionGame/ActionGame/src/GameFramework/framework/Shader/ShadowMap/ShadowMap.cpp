//	�e�p�e�N�X�`���N���X
// 2017. 2.26	�v���O�����쐬
// author		SyunMizuno



#include	"ShadowMap.h"
#include	"../../Graphic/Graphics.h"
#include	<tchar.h>
#include	"../../../../Archives/Archives.h"
#include	"../../Utility/System/SystemUtility.h"
#include	"../../Debug/Debug.h"
#include	"../../Screen/Screen.h"
#include	"../../Light/LightManager.h"


ShadowMap::ShadowMap() :
	m_unSizeX(Screen::GetWidth()),
	m_unSizeY(Screen::GetHeight()),
	m_pZTex(NULL),
	m_pZSurf(NULL),
	m_pZ(NULL),
	m_pOldBackBuffer(NULL),
	m_pOldZ(NULL)
{
}

ShadowMap::~ShadowMap() {
	Invalidate();
}


/*									//
//				������				//
//									*/
// <return> S_OK. ���� : E_FAIL. ���s
HRESULT ShadowMap::Load() {
	HRESULT hr;
	D3DCAPS9 caps;
	LPDIRECT3DDEVICE9 pDevice = GetGraphics()->GetDevice();

	//----- �T�|�[�g�̃o�[�W�����m�F
	pDevice->GetDeviceCaps(&caps);
	if (caps.VertexShaderVersion < D3DVS_VERSION(1, 1) &&
		caps.PixelShaderVersion < D3DPS_VERSION(2, 0)) {
		return MessageBox(NULL, _T("����PC�̃O���t�B�b�N�{�[�h�͑Ή����Ă��܂���"), _T("ShadowMap"), MB_OK);
	}

	//----- �t�@�C���ǂݍ���
	LPD3DXBUFFER pErr = NULL;
	hr = D3DXCreateEffectFromFile(
		pDevice,
		Archives::Shader("ShadowMap").c_str(),
		NULL,
		NULL,
		0,
		NULL,
		&m_pEffect,
		&pErr);

	if (FAILED(hr)) {
		MessageBox(NULL, (LPCTSTR)pErr->GetBufferPointer(), _T("ERROR"), MB_OK);
		SAFE_RELEASE(pErr);
		return E_FAIL;
	}
	SAFE_RELEASE(pErr);

	ConnectHandle();
	m_pEffect->SetTechnique(m_hTechnique);

	return S_OK;
}


/*									//
//			�n���h���̐ڑ�			//
//									*/
void ShadowMap::ConnectHandle() {
	m_hTechnique = m_pEffect->GetTechniqueByName("TShader");
	m_hWorld = m_pEffect->GetParameterByName(NULL, "matWorld");
	m_hCameraProj = m_pEffect->GetParameterByName(NULL, "matCameraProj");
	m_hCameraView = m_pEffect->GetParameterByName(NULL, "matCameraView");
	m_hBlendNum = m_pEffect->GetParameterByName(NULL, "iBlendNum");
}


/*									//
//			�f�o�C�X���X�g			//
//									*/
// <return> S_OK. ���� : E_FAIL. ���s
HRESULT ShadowMap::Invalidate() {
	if (!IsOK())	return S_OK;

	if (FAILED(IHlslDrawBase::Invalidate())) {
		Debug::LogError("Invalidate�Ɏ��s���܂���");
		return E_FAIL;
	}

	SAFE_RELEASE(m_pZTex);
	SAFE_RELEASE(m_pZSurf);
	SAFE_RELEASE(m_pZ);
	SAFE_RELEASE(m_pOldBackBuffer);
	SAFE_RELEASE(m_pOldZ);

	return S_OK;
}


/*									//
//			�f�o�C�X���Z�b�g		//
//									*/
// <return> S_OK. ���� : E_FAIL. ���s
HRESULT ShadowMap::Restore() {
	if (!IsOK())	return S_OK;

	if (FAILED(IHlslDrawBase::Restore())) {
		Debug::LogError("Restore�Ɏ��s���܂���");
		return E_FAIL;
	}

	if (FAILED(CreateRenderTarget())) {
		Debug::LogError("CreateRenderTarget�Ɏ��s���܂���");
		return E_FAIL;
	}

	return S_OK;
}


/*									//
//		�e�N�X�`���T�C�Y�̐ݒ�		//
//									*/
HRESULT ShadowMap::SetSize(UINT x, UINT y) {
	if (x <= 0 || y <= 0) {
		Debug::LogError("�����̃e�N�X�`���T�C�Y�͐����ł��܂���");
		return E_FAIL;
	}

	m_unSizeX = x;
	m_unSizeY = y;

	if (FAILED(this->CreateRenderTarget())) {
		Debug::LogError("CreateRenderTarget�Ɏ��s���܂���");
		return E_FAIL;
	}

	return S_OK;
}


/*									//
//	�e�N�X�`���E�T�[�t�F�X�̍쐬	//
//									*/
// <return> S_OK. ���� : E_FAIL. ���s
HRESULT ShadowMap::CreateRenderTarget() {
	LPDIRECT3DDEVICE9 pDevice = GetGraphics()->GetDevice();

	// �w���Z�l�e�N�X�`���𐶐�
	if (FAILED(D3DXCreateTexture(
		pDevice,
		m_unSizeX,
		m_unSizeY,
		1,
		D3DUSAGE_RENDERTARGET,
		D3DFMT_A8R8G8B8,
		D3DPOOL_DEFAULT,
		&m_pZTex)))
		return E_FAIL;

	m_pZTex->GetSurfaceLevel(0, &m_pZSurf);	// �T�[�t�F�C�X�擾

											// �`��f�o�C�X�ɒ�`����Ă���o�b�t�@�̔\�͂��擾
	IDirect3DSurface9* pSurf;
	pDevice->GetDepthStencilSurface(&pSurf);
	D3DSURFACE_DESC Desc;
	pSurf->GetDesc(&Desc);
	pSurf->Release();

	// �Ǝ��[�x�o�b�t�@���쐬
	if (FAILED(pDevice->CreateDepthStencilSurface(
		m_unSizeX,
		m_unSizeY,
		Desc.Format,
		Desc.MultiSampleType,
		Desc.MultiSampleQuality,
		FALSE,
		&m_pZ,
		NULL
	)))
		return E_FAIL;

	return S_OK;
}


/*									//
//		�V�F�[�_�[�����J�n			//
//									*/
void ShadowMap::Begin() {
	if (!IsOK())	return;
	LPDIRECT3DDEVICE9 pDevice = GetGraphics()->GetDevice();

	// �f�o�C�X�������Ă���o�b�t�@���ꎞ�ۑ�
	pDevice->GetRenderTarget(0, &m_pOldBackBuffer);
	pDevice->GetDepthStencilSurface(&m_pOldZ);

	// �f�o�C�X��Z�l�e�N�X�`���T�[�t�F�C�X�Ɛ[�x�o�b�t�@��ݒ�
	pDevice->SetRenderTarget(0, m_pZSurf);
	pDevice->SetDepthStencilSurface(m_pZ);

	// �e�T�[�t�F�C�X��������
	pDevice->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DCOLOR_ARGB(255, 255, 255, 255), 1.0f, 0);

	Light* pLight = GetLightManager()->Get("Directional");
	SetCameraView(pLight->GetView());
	SetCameraProj(pLight->GetProj());
	m_pEffect->Begin(NULL, 0);
}


/*									//
//		�V�F�[�_�[�����I��			//
//									*/
void ShadowMap::End() {
	if (!IsOK())	return;
	LPDIRECT3DDEVICE9 pDevice = GetGraphics()->GetDevice();

	IHlslDrawBase::End();
	// �f�o�C�X�Ɍ��̃T�[�t�F�C�X��߂�
	pDevice->SetRenderTarget(0, m_pOldBackBuffer);
	pDevice->SetDepthStencilSurface(m_pOldZ);

	m_pOldBackBuffer = NULL;
	m_pOldZ = NULL;
}


/*									//
//		�e�p�e�N�X�`���̎擾		//
//									*/
LPDIRECT3DTEXTURE9 ShadowMap::GetShadowTex() {
	return m_pZTex;
}