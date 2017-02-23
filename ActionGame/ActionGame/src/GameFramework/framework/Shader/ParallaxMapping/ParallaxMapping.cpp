//	�����}�b�s���O�V�F�[�_
// 2016. 7.16	�v���O�����쐬
// Author		SyunMizuno


#include	"ParallaxMapping.h"
#include	<tchar.h>
#include	"../../Utility/System/SystemUtility.h"
#include	"../../../../Archives/Archives.h"
#include	"../../Graphic/Graphics.h"


/*									//
//			�R���X�g���N�^			//
//									*/
ParallaxMapping::ParallaxMapping() {
}


/*									//
//			�f�X�g���N�^			//
//									*/
ParallaxMapping::~ParallaxMapping() {
}


/*									//
//				������				//
//									*/
// <return> S_OK. ���� : E_FAIL. ���s
HRESULT ParallaxMapping::Load() {
	HRESULT hr;
	D3DCAPS9 caps;
	LPDIRECT3DDEVICE9 pDevice = GetGraphics()->GetDevice();

	//----- �T�|�[�g�̃o�[�W�����m�F
	pDevice->GetDeviceCaps(&caps);
	if (caps.VertexShaderVersion < D3DVS_VERSION(1, 1) &&
		caps.PixelShaderVersion < D3DPS_VERSION(2, 0)) {
		return MessageBox(NULL, _T("����PC�̃O���t�B�b�N�{�[�h�͑Ή����Ă��܂���"), _T("ParallaxMapping"), MB_OK);
	}

	//----- �t�@�C���ǂݍ���
	LPD3DXBUFFER pErr = NULL;
	hr = D3DXCreateEffectFromFile(
		pDevice,
		Archives::Shader("ParallaxMapping").c_str(),
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

	//fx�t�@�C�����Ő錾���Ă���ϐ��̃n���h�����擾����
	m_hTechnique = m_pEffect->GetTechniqueByName("TShader");
	m_hWorld = m_pEffect->GetParameterByName(NULL, "matWorld");
	m_hView = m_pEffect->GetParameterByName(NULL, "matView");
	m_hProj = m_pEffect->GetParameterByName(NULL, "matProj");
	m_hBlendNum = m_pEffect->GetParameterByName(NULL, "iBlendNum");
	m_hTexture = m_pEffect->GetParameterByName(NULL, "tex");
	m_hLightDir = m_pEffect->GetParameterByName(NULL, "vLightDir");
	m_hEyePos = m_pEffect->GetParameterByName(NULL, "vEyePos");
	m_hAmbient = m_pEffect->GetParameterByName(NULL, "vColor");
	m_hSpecular = m_pEffect->GetParameterByName(NULL, "Specular");
	m_hSpecPow = m_pEffect->GetParameterByName(NULL, "SpecularPower");
	m_hBumpTex = m_pEffect->GetParameterByName(NULL, "Bumptex");
	m_hHLimit = m_pEffect->GetParameterByName(NULL, "fHLimit");

	m_pEffect->SetTechnique(m_hTechnique);

	return S_OK;
}


/*									//
//		�X�y�L�����[�͈̔͂�ݒ�	//
//									*/
void ParallaxMapping::SetSpecular(float Specular) {
	if (!IsOK())	return;

	m_pEffect->SetFloat(m_hSpecular, Specular);
}


/*									//
//		�X�y�L�����[�̋�����ݒ�	//
//									*/
void ParallaxMapping::SetSpecularPower(float SpecPow) {
	if (!IsOK())	return;

	m_pEffect->SetFloat(m_hSpecPow, SpecPow);
}


/*									//
//				�s��̐ݒ�			//
//									*/
void ParallaxMapping::SetMatrix() {
	if (IsOK()) {
		// ���[���h �~ �r���[ �~ �ˉe
		m_pEffect->SetMatrixArray(m_hWorld, m_mtxWorld, 4);
		m_pEffect->SetMatrix(m_hView, &m_mtxView);
		m_pEffect->SetMatrix(m_hProj, &m_mtxProj);

		//�J�����ʒu
		m_pEffect->SetVector(m_hEyePos, &m_vPosCamera);
	}

	// �V�F�[�_�[���g�p�ł��Ȃ��Ƃ��́A�Œ�@�\�p�C�v���C���̃}�g���b�N�X��ݒ肷��
	else
		GetGraphics()->GetDevice()->SetTransform(D3DTS_WORLD, &m_mtxWorld[0]);
}


/*									//
//			�����̍ő�l�̐ݒ�		//
//									*/
void ParallaxMapping::SetHeightLimit(float limit) {
	if (!IsOK())	return;

	m_pEffect->SetFloat(m_hHLimit, limit);
}


/*									//
//			�J�����̐ݒ�			//
//									*/
void ParallaxMapping::SetCamera(const D3DXVECTOR4* pCameraPos) {
	if (!IsOK())	return;

	m_vPosCamera = *pCameraPos;
}

void ParallaxMapping::SetCamera(const D3DXVECTOR3* pCameraPos) {
	SetCamera(&D3DXVECTOR4(*pCameraPos, 1.0f));
}

/*									//
//			�����̐ݒ�				//
//									*/
void ParallaxMapping::SetLight(const Vector4* pLightDir) {
	if (!IsOK())	return;

	// ���s�����̕����x�N�g����ݒ肷��
	Vector4 LightDir = *pLightDir;
	// ���K������
	D3DXVec3Normalize((D3DXVECTOR3*)&LightDir, (D3DXVECTOR3*)&LightDir);
	m_pEffect->SetVector(m_hLightDir, &LightDir);
}