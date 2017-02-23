//	�g�V�F�[�_
// 2016. 7.15	�v���O�����쐬
// Author		SyunMizuno


#include	"Wave.h"
#include	<tchar.h>
#include	"../../Utility/System/SystemUtility.h"
#include	"../../Debug/Debug.h"
#include	"../../../../Archives/Archives.h"
#include	"../../Graphic/Graphics.h"


/*									//
//			�R���X�g���N�^			//
//									*/
Wave::Wave() :
	m_pBumpMapTexture(NULL),
	m_pBumpMapSurface(NULL),
	m_nRenderTargetIndex(0)
{
	for (int i = 0; i < 2; i ++) {
		m_pWaveMapTexture[i] = NULL;
		m_pWaveMapSurface[i] = NULL;
	}
}


/*									//
//			�f�X�g���N�^			//
//									*/
Wave::~Wave() {
	this->Invalidate();
}


/*									//
//			�f�o�C�X���X�g			//
//									*/
// <return> S_OK. ���� : E_FAIL. ���s
HRESULT Wave::Invalidate() {
	HRESULT hr = S_OK;
	hr = IHlslBase::Invalidate();

	if (FAILED(hr)) {
		Debug::LogError("Invalidate�Ɏ��s���܂���");
		return hr;
	}

	// �g�}�b�v
	for (int i = 0; i < 2; i ++) {
		SAFE_RELEASE(m_pWaveMapTexture[i]);
		SAFE_RELEASE(m_pWaveMapSurface[i]);
	}

	// �@���}�b�v
	SAFE_RELEASE(m_pBumpMapTexture);
	SAFE_RELEASE(m_pBumpMapSurface);

	m_nRenderTargetIndex = 0;

	return hr;
}


/*									//
//			�f�o�C�X���Z�b�g		//
//									*/
// <return> S_OK. ���� : E_FAIL. ���s
HRESULT Wave::Restore() {
	if (!IsOK())	return S_OK;
	LPDIRECT3DDEVICE9 pDevice = GetGraphics()->GetDevice();

	if (FAILED(IHlslBase::Restore())) {
		Debug::LogError("Restore�Ɏ��s���܂���");
		return E_FAIL;
	}

	//----- �g�}�b�v�쐬
	for (int i = 0; i < 2; i ++) {
		if (FAILED(D3DXCreateTexture(
			pDevice,
			(int)MAP_WIDTH,
			(int)MAP_HEIGHT,
			1,
			D3DUSAGE_RENDERTARGET,
			D3DFMT_G16R16F,
			D3DPOOL_DEFAULT,
			&m_pWaveMapTexture[i])))
			return E_FAIL;

		if (FAILED(m_pWaveMapTexture[i]->GetSurfaceLevel(0, &m_pWaveMapSurface[i])))
			return E_FAIL;
	}

	// �g�}�b�v�����ŏ�����
	LPDIRECT3DSURFACE9 OldSurface = NULL;
	pDevice->GetRenderTarget(0, &OldSurface);

	LPDIRECT3DSURFACE9 OldDepthStencilSurface = NULL;
	pDevice->GetDepthStencilSurface(&OldDepthStencilSurface);

	// �f�v�X�o�b�t�@�͎g�p���Ȃ��̂Ŗ����ɂ���
	pDevice->SetDepthStencilSurface(NULL);

	for (int i = 0; i < 2; i ++) {
		pDevice->SetRenderTarget(0, m_pWaveMapSurface[i]);
		pDevice->Clear(
			0L,
			NULL,
			D3DCLEAR_TARGET,
			0x0,
			1.0f,
			0L);
	}

	pDevice->SetRenderTarget(0, OldSurface);
	SAFE_RELEASE(OldSurface);

	pDevice->SetDepthStencilSurface(OldDepthStencilSurface);
	SAFE_RELEASE(OldDepthStencilSurface);

	//----- �o���v�}�b�v�쐻
	if (FAILED(D3DXCreateTexture(
		pDevice,
		(DWORD)MAP_WIDTH,
		(DWORD)MAP_HEIGHT,
		1,
		D3DUSAGE_RENDERTARGET,
		D3DFMT_A8R8G8B8,
		D3DPOOL_DEFAULT,
		&m_pBumpMapTexture)))
		return E_FAIL;

	if (FAILED(m_pBumpMapTexture->GetSurfaceLevel(0, &m_pBumpMapSurface)))
		return E_FAIL;

	return S_OK;
}


/*									//
//				������				//
//									*/
// <return> S_OK. ���� : E_FAIL. ���s
HRESULT Wave::Load() {
	HRESULT hr;
	D3DCAPS9 caps;
	LPDIRECT3DDEVICE9 pDevice = GetGraphics()->GetDevice();

	//----- �T�|�[�g�̃o�[�W�����m�F
	pDevice->GetDeviceCaps(&caps);
	if (caps.VertexShaderVersion < D3DVS_VERSION(1, 1) &&
		caps.PixelShaderVersion < D3DPS_VERSION(2, 0)) {
		return MessageBox(NULL, _T("����PC�̃O���t�B�b�N�{�[�h�͑Ή����Ă��܂���"), _T("Wave"), MB_OK);
	}

	// �t�@�C���ǂݍ���
	LPD3DXBUFFER pErr = NULL;
	hr = D3DXCreateEffectFromFile(
		pDevice,
		Archives::Shader("Wave").c_str(),
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

	// �n���h�����擾
	m_hTechnique = m_pEffect->GetTechniqueByName("TShader");
	m_hTexOffset = m_pEffect->GetParameterByName(NULL, "TexOffset");
	m_hSpringPower = m_pEffect->GetParameterByName(NULL, "SpringPower");
	m_hAddWavePos = m_pEffect->GetParameterByName(NULL, "AddWavePos");
	m_hAddWaveHeight = m_pEffect->GetParameterByName(NULL, "AddWaveHeight");
	m_hWaveTex = m_pEffect->GetParameterByName(NULL, "tex");

	m_pEffect->SetTechnique(m_hTechnique);

	return S_OK;
}


/*									//
//			�T�C�Y�̐ݒ�			//
//									*/
// <return> S_OK. ���� : E_FAIL. ���s
HRESULT Wave::SetSize(float x, float y) {
	if (!IsOK())	return S_OK;

	MAP_WIDTH = x;
	MAP_HEIGHT = y;

	//�I�t�Z�b�g�l��ݒ�
	D3DXVECTOR2 Size;
	Size.x = 1.0f / MAP_WIDTH;
	Size.y = 1.0f / MAP_HEIGHT;
	m_pEffect->SetFloatArray(m_hTexOffset, Size, sizeof(D3DXVECTOR2));

	// �����_�[�^�[�Q�b�g���̍쐬
	if (FAILED(this->Invalidate())) {
		Debug::LogError("Invalidate�Ɏ��s���܂���");
		return E_FAIL;
	}
	if (FAILED(this->Restore())) {
		Debug::LogError("Invalidate�Ɏ��s���܂���");
		return E_FAIL;
	}

	return S_OK;
}


/*									//
//			�΂ˋ��x�̐ݒ�			//
//									*/
void Wave::SetSpringPower(float SpringPower) {
	if (IsOK()) {
		m_pEffect->SetFloat(m_hSpringPower, SpringPower);
	}
}


/*									//
//				�g�̒ǉ�			//
//									*/
void Wave::AddWave(float posx, float posy, float height) {
	if (IsOK()) {
		// �g��ǉ�����ʒu��ݒ�
		D3DXVECTOR2 AddWavePos = D3DXVECTOR2(posx, posy);
		m_pEffect->SetValue(m_hAddWavePos, &AddWavePos, sizeof(D3DXVECTOR2));

		// �ǉ�����g�̍�����ݒ肷��
		m_pEffect->SetFloat(m_hAddWaveHeight, height);
	}
}


/*									//
//				�g�̒ǉ�			//
//									*/
LPDIRECT3DTEXTURE9 Wave::Render() {
	if (IsOK()) {
		D3DVIEWPORT9 OldViewport, NewViewport;
		LPDIRECT3DDEVICE9 pDevice = GetGraphics()->GetDevice();

		// �r���[�|�[�g��g�}�b�v�̃T�C�Y�ɍ��킹��
		pDevice->GetViewport(&OldViewport);
		CopyMemory(&NewViewport, &OldViewport, sizeof(D3DVIEWPORT9));
		NewViewport.Width = (DWORD)MAP_WIDTH;
		NewViewport.Height = (DWORD)MAP_HEIGHT;
		pDevice->SetViewport(&NewViewport);


		typedef struct { FLOAT p[3]; FLOAT tu, tv; } VERTEX;

		VERTEX Vertex1[4] = {
			//   x      y     z      tu tv
			{{  1.0f, -1.0f, 0.1f},   1, 1,},
			{{ -1.0f, -1.0f, 0.1f},   0, 1,},
			{{ -1.0f,  1.0f, 0.1f},   0, 0,},
			{{  1.0f,  1.0f, 0.1f},   1, 0,},
		};
		pDevice->SetFVF(D3DFVF_XYZ | D3DFVF_TEX1);


		// ���݂̃����_�[�^�[�Q�b�g�T�[�t�F�C�X���擾
		LPDIRECT3DSURFACE9 OldSurface = NULL;
		pDevice->GetRenderTarget(0, &OldSurface);

		m_nRenderTargetIndex = 1 - m_nRenderTargetIndex;

		// �����_�[�^�[�Q�b�g���Z�b�g����
		pDevice->SetRenderTarget(0, m_pWaveMapSurface[1 - m_nRenderTargetIndex]);

		LPDIRECT3DSURFACE9 OldDepthStencilSurface = NULL;
		pDevice->GetDepthStencilSurface(&OldDepthStencilSurface);

		// �f�v�X�o�b�t�@���g�p���Ȃ��̂Ŗ����ɂ���
		pDevice->SetDepthStencilSurface(NULL);

		// �g�}�b�v���e�N�X�`���\�X�e�[�W�O�ɃZ�b�g����
		m_pEffect->SetTexture(m_hWaveTex, m_pWaveMapTexture[m_nRenderTargetIndex]);

		// �g�}�b�v�̍X�V
		m_pEffect->Begin(NULL, 0);
		m_pEffect->BeginPass(0);

		// 2D�X�v���C�g�̕`��
		pDevice->DrawPrimitiveUP(D3DPT_TRIANGLEFAN, 2, Vertex1, sizeof(VERTEX));

		m_pEffect->EndPass();

		// �X�V�����g�}�b�v���Q�Ƃ����ʂ�ʉ��ɂ��ă����_�����O����
		pDevice->SetRenderTarget(0, m_pBumpMapSurface);

		m_pEffect->SetTexture(m_hWaveTex, m_pWaveMapTexture[1 - m_nRenderTargetIndex]);

		m_pEffect->BeginPass(1);

		// 2D�X�v���C�g�̕`��
		pDevice->DrawPrimitiveUP(D3DPT_TRIANGLEFAN, 2, Vertex1, sizeof(VERTEX));

		m_pEffect->EndPass();
		m_pEffect->End();

		// �߂�
		pDevice->SetRenderTarget(0, OldSurface);
		SAFE_RELEASE(OldSurface);

		pDevice->SetDepthStencilSurface(OldDepthStencilSurface);
		SAFE_RELEASE(OldDepthStencilSurface);

		pDevice->SetViewport(&OldViewport);
	}

	return m_pBumpMapTexture;
}