//	�K�E�X�t�B���^�N���X
// 2016. 4.25	�v���O�����쐬
// 2016. 4.27	�ǉ�
// 2016. 4.28	���Ŋ���
// author		SyunMizuno

#include	"Gaussian.h"
#include	<tchar.h>
#include	"../../Debug/Debug.h"
#include	"../../Utility/System/SystemUtility.h"
#include	"../../../../Archives/Archives.h"
#include	"../../Graphic/Graphics.h"


/*
		memo	:
				Invalidate����Restore���Ƀe�N�X�`���̐������������Ȃ���
				�T�C�Y�ύX�����Ƃ��ɑΉ��ł��Ȃ��͂��i�v�C��
*/


/*									//
//			�R���X�g���N�^			//
//									*/
Gaussian::Gaussian() :
	m_pOldBackBuffer(NULL),
	m_pOldZBuffer(NULL),
	m_pMapZ(NULL),
	m_pOriginalMap(NULL),
	m_pOriginalMapSurf(NULL),
	m_pXMap(NULL),
	m_pXMapSurf(NULL),
	m_pXYMap(NULL),
	m_pXYMapSurf(NULL),
	m_fDispersion_sq(10.0f)
{
}


/*									//
//			�f�X�g���N�^			//
//									*/
Gaussian::~Gaussian() {
	Invalidate();
}


/*									//
//			�f�o�C�X���X�g			//
//									*/
// <return> S_OK. ���� : E_FAIL. ���s
HRESULT Gaussian::Invalidate() {
	if (!IsOK())	return S_OK;

	if (FAILED(IHlslBase::Invalidate())) {
		Debug::LogError("Invalidate�Ɏ��s���܂���");
		return E_FAIL;
	}

	SAFE_RELEASE(m_pOldBackBuffer);
	SAFE_RELEASE(m_pOldZBuffer);
	SAFE_RELEASE(m_pMapZ);
	SAFE_RELEASE(m_pOriginalMap);
	SAFE_RELEASE(m_pOriginalMapSurf);
	SAFE_RELEASE(m_pXMap);
	SAFE_RELEASE(m_pXMapSurf);
	SAFE_RELEASE(m_pXYMap);
	SAFE_RELEASE(m_pXYMapSurf);

	return S_OK;
}


/*									//
//			�f�o�C�X���Z�b�g		//
//									*/
// <return> S_OK. ���� : E_FAIL. ���s
HRESULT Gaussian::Restore() {
	if (!IsOK())	return S_OK;

	if (FAILED(IHlslBase::Restore())) {
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
//				������				//
//									*/
// <return> S_OK. ���� : E_FAIL. ���s
HRESULT Gaussian::Load() {
	HRESULT hr;
	D3DCAPS9 caps;
	LPDIRECT3DDEVICE9 pDevice = GetGraphics()->GetDevice();

	//----- �T�|�[�g�̃o�[�W�����m�F
	pDevice->GetDeviceCaps(&caps);
	if (caps.VertexShaderVersion < D3DVS_VERSION(1, 1) &&
		caps.PixelShaderVersion < D3DPS_VERSION(2, 0)) {
		return MessageBox(NULL, _T("����PC�̃O���t�B�b�N�{�[�h�͑Ή����Ă��܂���"), _T("Gaussian"), MB_OK);
	}

	//----- �t�@�C���ǂݍ���
	LPD3DXBUFFER pErr = NULL;
	hr = D3DXCreateEffectFromFile(
		pDevice,
		Archives::Shader("Gaussian").c_str(),
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

	// �d�݂̐ݒ�
	this->UpdateWeight(m_fDispersion_sq*m_fDispersion_sq);

	m_pEffect->SetTechnique(m_hTechnique);

	return S_OK;
}


/*									//
//			�n���h���̐ڑ�			//
//									*/
void Gaussian::ConnectHandle() {
	m_hTechnique = m_pEffect->GetTechniqueByName("TShader");
	m_hWeight = m_pEffect->GetParameterByName(NULL, "weight");
	m_hSrcMap = m_pEffect->GetParameterByName(NULL, "SrcMap");
}


/*									//
//			�T�C�Y�̐ݒ�			//
//									*/
// <return> S_OK. ���� : E_FAIL. ���s
HRESULT Gaussian::SetSize(float x, float y) {
	if (!IsOK())	return S_OK;

	MAP_WIDTH = x;
	MAP_HEIGHT = y;

	m_pEffect->SetFloat("MAP_WIDTH", MAP_WIDTH);	// ���̐ݒ�
	m_pEffect->SetFloat("MAP_HEIGHT", MAP_HEIGHT);	// �����̐ݒ�

	 //�I�t�Z�b�g�l��ݒ�
	D3DXVECTOR4 offset;
	offset.x = 16.0f / MAP_WIDTH;  offset.y = 0.0f / MAP_HEIGHT;
	m_pEffect->SetVector("offsetX", &offset);
	offset.x = 0.0f / MAP_WIDTH;  offset.y = 16.0f / MAP_HEIGHT;
	m_pEffect->SetVector("offsetY", &offset);

	// �����_�[�^�[�Q�b�g���̍쐬
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
HRESULT Gaussian::CreateRenderTarget() {
	if (!IsOK())	return S_OK;
	LPDIRECT3DDEVICE9 pDevice = GetGraphics()->GetDevice();

	// �[�x�o�b�t�@�̕ۑ�
	if (FAILED(pDevice->CreateDepthStencilSurface(
		(int)MAP_WIDTH,
		(int)MAP_HEIGHT,
		D3DFMT_D16,
		D3DMULTISAMPLE_NONE,
		0,
		TRUE,
		&m_pMapZ,
		NULL)))
		return E_FAIL;

	if (FAILED(pDevice->CreateTexture(
		(int)MAP_WIDTH,
		(int)MAP_HEIGHT,
		1,
		D3DUSAGE_RENDERTARGET,
		D3DFMT_A8R8G8B8,
		D3DPOOL_DEFAULT,
		&m_pOriginalMap,
		NULL)))
		return E_FAIL;

	if (FAILED(m_pOriginalMap->GetSurfaceLevel(0, &m_pOriginalMapSurf)))
		return E_FAIL;

	// �G�b�W
	if (FAILED(pDevice->CreateTexture(
		(int)MAP_WIDTH,
		(int)MAP_HEIGHT,
		1,
		D3DUSAGE_RENDERTARGET,
		D3DFMT_A8R8G8B8,
		D3DPOOL_DEFAULT,
		&m_pXMap,
		NULL)))
		return E_FAIL;

	if (FAILED(m_pXMap->GetSurfaceLevel(0, &m_pXMapSurf)))
		return E_FAIL;

	// �G�b�W���ڂ������}�b�v
	if (FAILED(pDevice->CreateTexture(
		(int)MAP_WIDTH,
		(int)MAP_HEIGHT,
		1,
		D3DUSAGE_RENDERTARGET,
		D3DFMT_A8R8G8B8,
		D3DPOOL_DEFAULT,
		&m_pXYMap,
		NULL)))
		return E_FAIL;

	if (FAILED(m_pXYMap->GetSurfaceLevel(0, &m_pXYMapSurf)))
		return E_FAIL;

	return S_OK;
}


/*									//
//		�e�N�X�`���쐬����			//
//									*/
void Gaussian::Begin() {
	if (!IsOK())	return;
	LPDIRECT3DDEVICE9 pDevice = GetGraphics()->GetDevice();

	pDevice->GetRenderTarget(0, &m_pOldBackBuffer);
	pDevice->GetDepthStencilSurface(&m_pOldZBuffer);
	pDevice->SetRenderTarget(0, m_pOriginalMapSurf);
	pDevice->SetDepthStencilSurface(m_pMapZ);

	// �e�N�X�`�����N���A
	pDevice->Clear(0L, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DCOLOR_RGBA(0, 0, 0, 0), 1.0f, 0L);
}


/*									//
//			�{�J�V�����J�n			//
//									*/
void Gaussian::End() {
	if (!IsOK())	return;
	LPDIRECT3DDEVICE9 pDevice = GetGraphics()->GetDevice();

	m_pEffect->Begin(NULL, 0);

	// �ڂ���x
	pDevice->SetRenderTarget(0, m_pXMapSurf);
	DWORD Z;
	pDevice->GetRenderState(D3DRS_ZENABLE, &Z);
	pDevice->SetRenderState(D3DRS_ZENABLE, FALSE);

	typedef struct { FLOAT p[3]; FLOAT tu, tv; } VERTEX;

	VERTEX Vertex1[4] = {
		//   x      y     z      tu tv
		{{  1.0f, -1.0f, 0.1f},   1, 1,},
		{{ -1.0f, -1.0f, 0.1f},   0, 1,},
		{{ -1.0f,  1.0f, 0.1f},   0, 0,},
		{{  1.0f,  1.0f, 0.1f},   1, 0,},
	};
	pDevice->SetFVF(D3DFVF_XYZ | D3DFVF_TEX1);
	m_pEffect->SetTexture(m_hSrcMap, m_pOriginalMap);
	m_pEffect->BeginPass(0);
	pDevice->DrawPrimitiveUP(D3DPT_TRIANGLEFAN, 2, Vertex1, sizeof(VERTEX));
	m_pEffect->EndPass();

	// �ڂ���y
	pDevice->SetRenderTarget(0, m_pXYMapSurf);
	m_pEffect->SetTexture(m_hSrcMap, m_pXMap);
	m_pEffect->BeginPass(1);
	pDevice->DrawPrimitiveUP(D3DPT_TRIANGLEFAN, 2, Vertex1, sizeof(VERTEX));
	m_pEffect->EndPass();

	m_pEffect->End();

	// ���Ƃɖ߂�
	pDevice->SetRenderTarget(0, m_pOldBackBuffer);
	pDevice->SetDepthStencilSurface(m_pOldZBuffer);

	pDevice->SetRenderState(D3DRS_ZENABLE, Z);
}


/*									//
//				�d�݂̌v�Z			//
//									*/
void Gaussian::UpdateWeight(float dispersion) {
	DWORD i;

	float total = 0;
	for (i = 0; i < WEIGHT_MUN; i++) {
		float pos = 1.0f + 2.0f * (float)i;
		m_fWeight[i] = expf(-0.5f * (float)(pos*pos) / dispersion);
		total += 2.0f * m_fWeight[i];
	}
	// �K�i��
	for (i = 0; i < WEIGHT_MUN; i++)
		m_fWeight[i] /= total;

	// �V�F�[�_�ɕύX��ʒm
	if (IsOK())
		m_pEffect->SetFloatArray(m_hWeight, m_fWeight, WEIGHT_MUN);
}