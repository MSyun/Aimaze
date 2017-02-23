// �N���X�t�B���^�[�N���X
// 2016. 3.19	�v���O�����쐬
// author		SyunMizuno


#include	"CrossFilter.h"
#include	<tchar.h>
#include	"../../Utility/System/SystemUtility.h"
#include	"../../../../Archives/Archives.h"
#include	"../../Debug/Debug.h"
#include	"../../Graphic/Graphics.h"


/*									//
//			�R���X�g���N�^			//
//									*/
CrossFilter::CrossFilter() :
	m_pOldBackBuffer(NULL),
	m_pOldZBuffer(NULL),
	m_pMapZ(NULL),
	m_pOriginalMap(NULL),
	m_pOriginalMapSurf(NULL),
	m_pTexSceneScaled(NULL),
	m_pSurfSceneScaled(NULL),
	m_pTexBrightPass(NULL),
	m_pSurfBrightPass(NULL),
	m_pTexStarSource(NULL),
	m_pSurfStarSource(NULL)
{
	for (int i = 0; i < NUM_STAR_TEXTURES; i ++) {
		m_apTexStar[i] = NULL;
		m_apSurfStar[i] = NULL;
	}
}


/*									//
//			�f�X�g���N�^			//
//									*/
CrossFilter::~CrossFilter() {
	Invalidate();
}


/*									//
//			�f�o�C�X���X�g			//
//									*/
// <return> S_OK. ���� : E_FAIL. ���s
HRESULT CrossFilter::Invalidate() {
	if (!IsOK())	return S_OK;

	if (FAILED(IHlslBase::Invalidate())) {
		Debug::LogError("Invalidate�Ɏ��s���܂���");
		return E_FAIL;
	}

	SAFE_RELEASE(m_pOldZBuffer);
	SAFE_RELEASE(m_pMapZ);
	SAFE_RELEASE(m_pOriginalMap);
	SAFE_RELEASE(m_pOriginalMapSurf);
	SAFE_RELEASE(m_pTexSceneScaled);
	SAFE_RELEASE(m_pSurfSceneScaled);
	SAFE_RELEASE(m_pTexBrightPass);
	SAFE_RELEASE(m_pSurfBrightPass);
	SAFE_RELEASE(m_pTexStarSource);
	SAFE_RELEASE(m_pSurfStarSource);
	for (int i = 0; i < NUM_STAR_TEXTURES; i ++) {
		SAFE_RELEASE(m_apTexStar[i]);
		SAFE_RELEASE(m_apSurfStar[i]);
	}

	return S_OK;
}


/*									//
//			�f�o�C�X���Z�b�g		//
//									*/
// <return> S_OK. ���� : E_FAIL. ���s
HRESULT CrossFilter::Restore() {
	if (!IsOK())	return S_OK;

	if (FAILED(IHlslBase::Restore())) {
		Debug::LogError("Restore�Ɏ��s���܂���");
		return E_FAIL;
	}

	if (FAILED(CreateRenderTarget())) {
		Debug::LogError("Restore�Ɏ��s���܂���");
		return E_FAIL;
	}

	return S_OK;
}


/*									//
//				������				//
//									*/
// <return> S_OK. ���� : E_FAIL. ���s
HRESULT CrossFilter::Load() {
	//----- �ϐ��錾
	HRESULT hr;
	D3DCAPS9 caps;

	//----- �T�|�[�g�̃o�[�W�����m�F
	LPDIRECT3DDEVICE9 pDevice = GetGraphics()->GetDevice();
	pDevice->GetDeviceCaps(&caps);
	if (caps.VertexShaderVersion < D3DVS_VERSION(1, 1) ||
		caps.PixelShaderVersion < D3DPS_VERSION(2, 0)) {
		return MessageBox(NULL, _T("����PC�̃O���t�B�b�N�{�[�h�͑Ή����Ă��܂���"), _T("CrossFilter"), MB_OK);
	}

	//----- �t�@�C���ǂݍ���
	LPD3DXBUFFER pErr = NULL;	// �G���[�p
	hr = D3DXCreateEffectFromFile(
		pDevice,
		Archives::Shader("CrossFilter").c_str(),
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

	// fx�t�@�C�����Ő錾���Ă���ϐ��̃n���h�����擾����
	m_hTechnique = m_pEffect->GetTechniqueByName("tDownScale4x4");

	m_pEffect->SetTechnique(m_hTechnique);

	return S_OK;
}


/*									//
//			�T�C�Y�̐ݒ�			//
//									*/
// <return> S_OK. ���� : E_FAIL. ���s
HRESULT CrossFilter::SetSize(float x, float y) {
	if( !IsOK() )	return S_OK;

	m_dwWidth	= (DWORD)x;
	m_dwHeight	= (DWORD)y;

	// �����_�[�^�[�Q�b�g���̍쐬
	if (FAILED(this->CreateRenderTarget())) {
		Debug::LogError("�e�N�X�`���E�T�[�t�F�C�X�̍쐬�Ɏ��s���܂���");
		return E_FAIL;
	}

	return S_OK;
}


/*									//
//	�e�N�X�`���E�T�[�t�F�X�̍쐬	//
//									*/
// <return> S_OK. ���� : E_FAIL. ���s
HRESULT CrossFilter::CreateRenderTarget() {
	if (!IsOK())	return S_OK;

	HRESULT hr;
	LPDIRECT3DDEVICE9 pDevice = GetGraphics()->GetDevice();


	// �[�x�o�b�t�@�̕ۑ�
	hr = pDevice->CreateDepthStencilSurface(
		m_dwWidth,
		m_dwHeight,
		D3DFMT_D16,
		D3DMULTISAMPLE_NONE,
		0,
		TRUE,
		&m_pMapZ,
		NULL);
	if (FAILED(hr))	return hr;

	//----- �V�[����`�悷��HDR
	hr = pDevice->CreateTexture(
		m_dwWidth,
		m_dwHeight,
		1,
		D3DUSAGE_RENDERTARGET,
		D3DFMT_A16B16G16R16F,
		D3DPOOL_DEFAULT,
		&m_pOriginalMap,
		NULL);
	if (FAILED(hr)) return hr;

	hr = m_pOriginalMap->GetSurfaceLevel(0, &m_pOriginalMapSurf);
	if (FAILED(hr)) return hr;

	//----- �k���o�b�t�@
	// �k���o�b�t�@�̊�{�T�C�Y�iFB��؂�̂Ă�4�̔{���̑傫���j
	m_dwCropWidth = m_dwWidth - m_dwWidth % 4;
	m_dwCropHeight = m_dwHeight - m_dwHeight % 4;

	hr = pDevice->CreateTexture(
		m_dwCropWidth / 4,
		m_dwCropHeight / 4,
		1,
		D3DUSAGE_RENDERTARGET,
		D3DFMT_A16B16G16R16F,
		D3DPOOL_DEFAULT,
		&m_pTexSceneScaled,
		NULL);
	if (FAILED(hr)) return hr;

	hr = m_pTexSceneScaled->GetSurfaceLevel(0, &m_pSurfSceneScaled);
	if (FAILED(hr)) return hr;

	//----- �P�x�̒��o
	hr = pDevice->CreateTexture(
		m_dwCropWidth / 4 + 2,
		m_dwCropHeight / 4 + 2,
		1,
		D3DUSAGE_RENDERTARGET,
		D3DFMT_A8R8G8B8,
		D3DPOOL_DEFAULT,
		&m_pTexBrightPass,
		NULL);
	if (FAILED(hr)) return hr;

	hr = m_pTexBrightPass->GetSurfaceLevel(0, &m_pSurfBrightPass);
	if (FAILED(hr)) return hr;
	// �ӂ��������h��Ԃ��Ă���
	pDevice->ColorFill(m_pSurfBrightPass, NULL, D3DCOLOR_ARGB(0, 0, 0, 0));

	//----- ������Ȃ��悤�ɂڂ������߂̃e�N�X�`��
	hr = pDevice->CreateTexture(
		m_dwCropWidth / 4 + 2,
		m_dwCropHeight / 4 + 2,
		1,
		D3DUSAGE_RENDERTARGET,
		D3DFMT_A8R8G8B8,
		D3DPOOL_DEFAULT,
		&m_pTexStarSource,
		NULL);
	if (FAILED(hr)) return hr;

	hr = m_pTexStarSource->GetSurfaceLevel(0, &m_pSurfStarSource);
	if (FAILED(hr)) return hr;
	// �ӂ��������h��Ԃ��Ă���
	pDevice->ColorFill(m_pSurfStarSource, NULL, D3DCOLOR_ARGB(0, 0, 0, 0));

	//----- ��䊂̂��߂̃e�N�X�`��
	for (int i = 0; i < NUM_STAR_TEXTURES; i++) {
		hr = pDevice->CreateTexture(
			m_dwCropWidth / 4,
			m_dwCropHeight / 4,
			1,
			D3DUSAGE_RENDERTARGET,
			D3DFMT_A16B16G16R16F,
			D3DPOOL_DEFAULT,
			&m_apTexStar[i],
			NULL);
		if (FAILED(hr)) return hr;

		hr = m_apTexStar[i]->GetSurfaceLevel(0, &m_apSurfStar[i]);
		if (FAILED(hr)) return hr;
	}

	return hr;
}


/*									//
//		�V�F�[�_�[�����J�n			//
//									*/
void CrossFilter::Begin() {
	if (!IsOK())	return;
	LPDIRECT3DDEVICE9 pDevice = GetGraphics()->GetDevice();


	// �`���̐؂�ւ�
	pDevice->GetRenderTarget(0, &m_pOldBackBuffer);
	pDevice->GetDepthStencilSurface(&m_pOldZBuffer);
	pDevice->SetRenderTarget(0, m_pOriginalMapSurf);
	pDevice->SetDepthStencilSurface(m_pMapZ);

	// �����_�����O�^�[�Q�b�g�̃N���A
	pDevice->Clear(0L, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DCOLOR_RGBA(0, 0, 0, 0), 1.0f, 0L);
	// �A���t�@�u�����h�𒆎~
	pDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
}


/*									//
//		�V�F�[�_�[�����I��			//
//									*/
void CrossFilter::End() {
	if (!IsOK())	return;
	LPDIRECT3DDEVICE9 pDevice = GetGraphics()->GetDevice();

	// �k���e�N�X�`���쐬
	this->Scene_To_SceneScaled();
	// �A���t�@�u�����h���s��
	pDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	pDevice->SetDepthStencilSurface(NULL);
	// �P�x���o
	this->SceneScaled_To_BrightPass();
	// �e�N�X�`�������L�΂�
	this->BrightPass_To_StarSource();
	// ����
	this->RenderStar();

	// �����_�����O�^�[�Q�b�g��߂�
	pDevice->SetRenderTarget(0, m_pOldBackBuffer);	// �����T�[�t�F�X
	pDevice->SetDepthStencilSurface(m_pOldZBuffer);
}


/*									//
//		1/4�ɂ��āA					//
//		m_pTexSceneScaled�ɓ����	*/
HRESULT CrossFilter::Scene_To_SceneScaled() {
	if (!IsOK())	return S_OK;
	LPDIRECT3DDEVICE9 pDevice = GetGraphics()->GetDevice();

	// �͂ݏo�����ꍇ�ɒ��S�������R�s�[����
	CoordRect coords;
	RECT rectSrc;
	rectSrc.left = (m_dwWidth - m_dwCropWidth) / 2;
	rectSrc.top = (m_dwHeight - m_dwCropHeight) / 2;
	rectSrc.right = rectSrc.left + m_dwCropWidth;
	rectSrc.bottom = rectSrc.top + m_dwCropHeight;
	// �����_�����O�^�[�Q�b�g�ɂ������e�N�X�`�����W���v�Z����
	GetTextureCoords(m_pOriginalMap, &rectSrc, m_pTexSceneScaled, NULL, &coords);

	// ���ӂ�16�e�N�Z�����T���v�����O�_�Ƃ���
	// �v�Z����0.5�͒��S�ɍ��킹�邽�߂̕␳
	int index = 0;
	D3DXVECTOR2 offsets[MAX_SAMPLES];

	for (int y = 0; y < 4; y ++) {
		for (int x = 0; x < 4; x ++) {
			offsets[index].x = (x - 1.5f) / m_dwWidth;
			offsets[index].y = (y - 1.5f) / m_dwHeight;
			index ++;
		}
	}
	m_pEffect->SetValue("g_avSampleOffsets", offsets, sizeof(offsets));

	// 16�e�N�Z�����T���v�����O���Ă��̕��ς��k���o�b�t�@�ɏo�͂���
	pDevice->SetRenderTarget(0, m_pSurfSceneScaled);
	m_pEffect->SetTechnique("tDownScale4x4");
	m_pEffect->Begin(NULL, 0);
	m_pEffect->BeginPass(0);
	pDevice->SetTexture(0, m_pOriginalMap);
	DrawFullScreenQuad(coords.u0, coords.v0, coords.u1, coords.v1);
	m_pEffect->EndPass();
	m_pEffect->End();

	return S_OK;
}


/*									//
//			�P�x�̒��o				//
//									*/
HRESULT CrossFilter::SceneScaled_To_BrightPass() {
	if (!IsOK())	return S_OK;
	LPDIRECT3DDEVICE9 pDevice = GetGraphics()->GetDevice();

	// �o�͐�̏�񂩂�`���̃T�C�Y��ݒ肷��
	D3DSURFACE_DESC desc;
	m_pTexBrightPass->GetLevelDesc(0, &desc);
	RECT rectDest = { 0, 0, (LONG)desc.Width, (LONG)desc.Height };
	InflateRect(&rectDest, -1, -1);	// �o�͐�̑傫�������菬��������
	DWORD scissor;
	pDevice->GetRenderState(D3DRS_SCISSORTESTENABLE, &scissor);
	pDevice->SetRenderState(D3DRS_SCISSORTESTENABLE, TRUE);
	pDevice->SetScissorRect(&rectDest);

	// �S��ʃR�s�[
	pDevice->SetRenderTarget(0, m_pSurfBrightPass);
	pDevice->Clear(0L, NULL, D3DCLEAR_TARGET, D3DCOLOR_RGBA(0, 0, 0, 0), 1.0f, 0L);
	m_pEffect->SetTechnique("tBrightPassFilter");
	m_pEffect->Begin(NULL, 0);
	m_pEffect->BeginPass(0);
	pDevice->SetTexture(0, m_pTexSceneScaled);
	DrawFullScreenQuad(0.0f, 0.0f, 1.0f, 1.0f);
	m_pEffect->EndPass();
	m_pEffect->End();

	pDevice->SetRenderState(D3DRS_SCISSORTESTENABLE, scissor);

	return S_OK;
}


/*									//
//		�K�E�X�^�̃{�J�V��������	//
//									*/
HRESULT CrossFilter::BrightPass_To_StarSource() {
	if (!IsOK())	return S_OK;
	LPDIRECT3DDEVICE9 pDevice = GetGraphics()->GetDevice();

	// �o�͐�̏�񂩂�`���̃T�C�Y��ݒ肷��
	D3DSURFACE_DESC desc;
	m_pTexStarSource->GetLevelDesc(0, &desc);
	RECT rectDest = { 0, 0, (LONG)desc.Width, (LONG)desc.Height };
	InflateRect(&rectDest, -1, -1);
	DWORD scissor;
	pDevice->GetRenderState(D3DRS_SCISSORTESTENABLE, &scissor);
	pDevice->SetRenderState(D3DRS_SCISSORTESTENABLE, TRUE);
	pDevice->SetScissorRect(&rectDest);

	// �e�N�X�`�����W���Z�o����
	CoordRect coords;
	GetTextureCoords(m_pTexBrightPass, NULL, m_pTexStarSource, &rectDest, &coords);

	// ���摜�̑傫������K�E�X���z�̌W�����v�Z����
	D3DXVECTOR2 offsets[MAX_SAMPLES];
	D3DXVECTOR4 weights[MAX_SAMPLES];
	m_pTexBrightPass->GetLevelDesc(0, &desc);
	GetGaussBlur5x5(desc.Width, desc.Height, offsets, weights);
	m_pEffect->SetValue("g_avSampleOffsets", offsets, sizeof(offsets));
	m_pEffect->SetValue("g_avSampleWeights", weights, sizeof(weights));

	// �K�E�X�ڂ����������Ȃ�
	m_pEffect->SetTechnique("tGaussBlur5x5");
	pDevice->SetRenderTarget(0, m_pSurfStarSource);
	pDevice->Clear(0L, NULL, D3DCLEAR_TARGET, D3DCOLOR_RGBA(0, 0, 0, 0), 1.0f, 0L);
	m_pEffect->Begin(NULL, 0);
	m_pEffect->BeginPass(0);
	pDevice->SetTexture(0, m_pTexBrightPass);
	DrawFullScreenQuad(coords.u0, coords.v0, coords.u1, coords.v1);
	m_pEffect->EndPass();
	m_pEffect->End();

	pDevice->SetRenderState(D3DRS_SCISSORTESTENABLE, scissor);

	return S_OK;
}


/*									//
//			�{�J�V��������			//
//									*/
HRESULT CrossFilter::GetGaussBlur5x5(
	DWORD dwD3DTexWidth,
	DWORD dwD3DTexHeight,
	D3DXVECTOR2* avTexCoordOffset,
	D3DXVECTOR4* avSampleWeight)
{
	float tu = 1.0f / (float)dwD3DTexWidth;
	float tv = 1.0f / (float)dwD3DTexHeight;

	float totalWeight = 0.0f;
	int index = 0;
	for (int x = -2; x <= 2; x++) {
		for (int y = -2; y <= 2; y++) {
			// �W�����������Ȃ镔���͏���
			if (2 < abs(x) + abs(y)) continue;

			avTexCoordOffset[index] = D3DXVECTOR2(x * tu, y * tv);
			float fx = (FLOAT)x;
			float fy = (FLOAT)y;
			avSampleWeight[index].x =
			avSampleWeight[index].y =
			avSampleWeight[index].z =
			avSampleWeight[index].w = expf(-(fx*fx + fy*fy) / (2 * 1.0f));
			totalWeight += avSampleWeight[index].x;

			index++;
		}
	}

	// �d�݂̍��v��1.0f�ɂ���
	for (int i = 0; i < index; i++) avSampleWeight[i] *= 1.0f / totalWeight;

	return S_OK;
}


/*									//
//			��䊂�������			//
//		�����͏�肭�����Ă��Ȃ�	*/
HRESULT CrossFilter::RenderStar() {
	if (!IsOK())	return S_OK;
	LPDIRECT3DDEVICE9 pDevice = GetGraphics()->GetDevice();

	// �G�t�F�N�g�Ŏg���萔�̐ݒ�
	static const int s_maxPasses = 1;
	static const int nSamples = 8;
	// ��䊂̐F
	static const D3DXCOLOR s_colorWhite(0.63f, 0.63f, 0.63f, 0.0f);
	static const D3DXCOLOR s_ChromaticAberrationColor[8] = {
		D3DXCOLOR(0.5f, 0.5f, 0.5f, 0.0f),	// ��
		D3DXCOLOR(0.8f, 0.3f, 0.3f, 0.0f),	// ��
		D3DXCOLOR(1.0f, 0.2f, 0.2f, 0.0f),	// ��
		D3DXCOLOR(0.5f, 0.2f, 0.6f, 0.0f),	// ��
		D3DXCOLOR(0.2f, 0.2f, 1.0f, 0.0f),	// ��
		D3DXCOLOR(0.2f, 0.3f, 0.7f, 0.0f),	// ��
		D3DXCOLOR(0.2f, 0.6f, 0.2f, 0.0f),	// ��
		D3DXCOLOR(0.3f, 0.5f, 0.3f, 0.0f),	// ��
	};

	static D3DXVECTOR4 s_aaColor[s_maxPasses][nSamples];

	for (int p = 0; p < s_maxPasses; p++) {
		// ���S����̋����ɉ����Č�䊂̐F������
		float ratio = (float)(p + 1) / (float)s_maxPasses;
		// ���ꂼ��̃T���v�����O�œK���ɐF������
		for (int s = 0; s < nSamples; s++) {
			D3DXCOLOR chromaticAberrColor;
			D3DXColorLerp(&chromaticAberrColor,
				&(s_ChromaticAberrationColor[s]),
				&s_colorWhite,
				ratio);
			// �S�̓I�ȐF�̕ω��𒲐�����
			D3DXColorLerp((D3DXCOLOR*)&(s_aaColor[p][s]),
				&s_colorWhite,
				&chromaticAberrColor,
				0.7f);
		}
	}

	float radOffset = 10.f;	// ���_�ɉ����ĉ�]����
	HRESULT hr;
	// ���摜�̕��ƍ����𒲂ׂ�
	D3DSURFACE_DESC desc;
	m_pSurfStarSource->GetDesc(&desc);
	float srcW = (FLOAT)desc.Width;
	float srcH = (FLOAT)desc.Height;

	int nStarLines = 6;	// ��䊂̖{��
	for (int d = 0; d < nStarLines; d ++) {	// �����ɉ��������[�v
		LPDIRECT3DTEXTURE9 pTexSource = m_pTexStarSource;
		float rad = radOffset + 2 * d * D3DX_PI / (FLOAT)nStarLines;	// �p�x
		float sn = sinf(rad);
		float cs = cosf(rad);
		D3DXVECTOR2 vtStepUV = D3DXVECTOR2(0.3f * sn / srcW,
			0.3f * cs / srcH);

		float attnPowScale = (atanf(D3DX_PI / 4) + 0.1f) * (160.0f + 120.0f) / (srcW + srcH);

		int iWorkTexture = 0;
		for (int p = 0; p < s_maxPasses; p++) {
			// �`���̌���
			LPDIRECT3DSURFACE9 pSurfDest = NULL;
			if (p == s_maxPasses - 1) {
				// �Ō�̃p�X�́A�ۑ��p�̃o�b�t�@�Ɋm�ۂ���
				pSurfDest = m_apSurfStar[d + 2];
			} else {
				pSurfDest = m_apSurfStar[iWorkTexture];
			}
			pDevice->SetRenderTarget(0, pSurfDest);

			// �e�N�X�`�����W�⍇������Ƃ��̏d�݂��v�Z����
			D3DXVECTOR4 avSampleWeights[MAX_SAMPLES];
			D3DXVECTOR2 avSampleOffSets[MAX_SAMPLES];

			for (int i = 0; i < nSamples; i ++) {
				// ���ꂼ��̏d��
				float lum = powf(0.95f, attnPowScale * i);
				avSampleWeights[i] = s_aaColor[s_maxPasses - 1 - p][i] * lum * (p + 1.0f) * 0.5f;

				// �e�N�X�`�����W�����炷��
				avSampleOffSets[i].x = vtStepUV.x * i;
				avSampleOffSets[i].y = vtStepUV.y * i;
				if (0.9f <= fabs(avSampleOffSets[i].x) ||
					0.9f <= fabs(avSampleOffSets[i].y)) {
					avSampleOffSets[i].x = 0.0f;
					avSampleOffSets[i].y = 0.0f;
					avSampleWeights[i] *= 0.0f;
				}
			}

			hr = m_pEffect->SetValue("g_avSampleOffsets", avSampleOffSets, sizeof(avSampleOffSets));
			hr = m_pEffect->SetVectorArray("g_avSampleWeights", avSampleWeights, nSamples);

			// �S��ʃR�s�[
			hr = m_pEffect->SetTechnique("tStar");
			hr = m_pEffect->Begin(NULL, 0);
			hr = m_pEffect->BeginPass(0);
			pDevice->SetTexture(0, pTexSource);
			DrawFullScreenQuad(0.0f, 0.0f, 1.0f, 1.0f);
			m_pEffect->EndPass();
			m_pEffect->End();

			// ���̃p�X�̂��߂Ƀp�����[�^��ݒ肷��
			vtStepUV *= (float)nSamples;
			attnPowScale *= nSamples;

			// �����_�����O�����o�͂����̃e�N�X�`���ɂ���
			pTexSource = m_apTexStar[iWorkTexture];

			iWorkTexture ^= 1;
			//iWorkTexture += 1;
		}
	}

	// �S�Ă̌�䊂���������
	pDevice->SetRenderTarget(0, m_apSurfStar[0]);

	hr = m_pEffect->SetTechnique("tMergeTextures");
	hr = m_pEffect->Begin(NULL, 0);
	hr = m_pEffect->BeginPass(0);
	hr = pDevice->SetTexture(0, m_apTexStar[0 + 2]);
	hr = pDevice->SetTexture(1, m_apTexStar[1 + 2]);
	hr = pDevice->SetTexture(2, m_apTexStar[2 + 2]);
	hr = pDevice->SetTexture(3, m_apTexStar[3 + 2]);
	hr = pDevice->SetTexture(4, m_apTexStar[4 + 2]);
	hr = pDevice->SetTexture(5, m_apTexStar[5 + 2]);

	DrawFullScreenQuad(0.0f, 0.0f, 1.0f, 1.0f);

	m_pEffect->EndPass();
	m_pEffect->End();


	return S_OK;
}


/*									//
//		�e�N�X�`�����W�̐ݒ�		//
//									*/
HRESULT CrossFilter::GetTextureCoords(
	LPDIRECT3DTEXTURE9 pTexSrc,
	RECT* pRectSrc,
	LPDIRECT3DTEXTURE9 pTexDest,
	RECT* pRectDest,
	CoordRect* pCoords)
{
	D3DSURFACE_DESC desc;

	// �Ó����̌���
	if (pTexSrc == NULL || pTexDest == NULL || pCoords == NULL)
		return E_INVALIDARG;

	// ��{�I�ɂ́A�e�N�X�`�����W�����̂܂܏o�͂���
	pCoords->u0 = 0.0f;
	pCoords->v0 = 0.0f;
	pCoords->u1 = 1.0f;
	pCoords->v1 = 1.0f;

	// ���͌��̃T�[�t�F�X�Ɋւ��ĕ␳����
	if (pRectSrc != NULL) {
		pTexSrc->GetLevelDesc(0, &desc);	// �e�N�X�`���̏����擾����
		// �]�����̋�`�ɉ����ăe�N�X�`�����W�����킹��
		pCoords->u0 += pRectSrc->left / desc.Width;
		pCoords->v0 += pRectSrc->top / desc.Height;
		pCoords->u1 -= (desc.Width - pRectSrc->right) / desc.Width;
		pCoords->v1 -= (desc.Height - pRectSrc->bottom) / desc.Height;
	}

	// �o�͐�̃T�[�t�F�X�Ɋւ��ĕ␳����
	if (pRectDest != NULL) {
		pTexDest->GetLevelDesc(0, &desc);	// �e�N�X�`���̏����擾����
		// �o�͐�̋�`�ɉ����ăe�N�X�`�����W�����킹��
		pCoords->u0 -= pRectDest->left / desc.Width;
		pCoords->v0 -= pRectDest->top / desc.Height;
		pCoords->u1 += (desc.Width - pRectDest->right) / desc.Width;
		pCoords->v1 += (desc.Height - pRectDest->bottom) / desc.Height;
	}

	return S_OK;
}


/*									//
//		�e�N�X�`���̕`��			//
//									*/
void CrossFilter::DrawFullScreenQuad(
	float fLeftU,
	float fTopV,
	float fRightU,
	float fBottomV)
{
	D3DSURFACE_DESC desc;
	PDIRECT3DSURFACE9 pSurf;
	DWORD zenable;
	LPDIRECT3DDEVICE9 pDevice = GetGraphics()->GetDevice();

	// �����_�����O�^�[�Q�b�g�̏��i���ƍ����j���擾����
	pDevice->GetRenderTarget(0, &pSurf);
	pSurf->GetDesc(&desc);
	pSurf->Release();
	FLOAT	w = (FLOAT)desc.Width;
	FLOAT	h = (FLOAT)desc.Height;

	typedef struct {
		float p[4];	// �ʒu���W
		float t[2];	// �e�N�X�`�����W
	} ScreenVertex;
	ScreenVertex svQuad[4] = {
		// x		y		  z		w		u		v
		{0 - 0.5f, 0 - 0.5f, 0.5f, 1.0f, fLeftU,  fTopV,},
		{w - 0.5f, 0 - 0.5f, 0.5f, 1.0f, fRightU, fTopV,},
		{0 - 0.5f, h - 0.5f, 0.5f, 1.0f, fLeftU,  fBottomV,},
		{w - 0.5f, h - 0.5f, 0.5f, 1.0f, fRightU, fBottomV,},
	};

	pDevice->GetRenderState(D3DRS_ZENABLE, &zenable);
	pDevice->SetRenderState(D3DRS_ZENABLE, FALSE);
	pDevice->SetFVF(D3DFVF_XYZRHW | D3DFVF_TEX1);
	pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, 2, svQuad, sizeof(ScreenVertex));
	pDevice->SetRenderState(D3DRS_ZENABLE, zenable);
}