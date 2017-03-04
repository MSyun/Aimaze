//	�����o�[�g�N���X
// 2016. 3.12	�v���O�����쐬
// author		SyunMizuno


#include	"Lambert.h"
#include	<tchar.h>
#include	"../../Utility/System/SystemUtility.h"
#include	"../../../../Archives/Archives.h"
#include	"../../Graphic/Graphics.h"


/*									//
//			�R���X�g���N�^			//
//									*/
Lambert::Lambert() {
}


/*									//
//			�f�X�g���N�^			//
//									*/
Lambert::~Lambert() {
}


/*									//
//				������				//
//									*/
// <return> S_OK. ���� : E_FAIL. ���s
HRESULT Lambert::Load() {
	HRESULT hr;
	D3DCAPS9 caps;
	LPDIRECT3DDEVICE9 pDevice = GetGraphics()->GetDevice();

	//----- �T�|�[�g�̃o�[�W�����m�F
	pDevice->GetDeviceCaps(&caps);
	if (caps.VertexShaderVersion < D3DVS_VERSION(1, 1) &&
		caps.PixelShaderVersion < D3DPS_VERSION(2, 0)) {
		return MessageBox(NULL, _T("����PC�̃O���t�B�b�N�{�[�h�͑Ή����Ă��܂���"), _T("Lambert"), MB_OK);
	}

	//----- �t�@�C���ǂݍ���
	LPD3DXBUFFER pErr = NULL;
	hr = D3DXCreateEffectFromFile(
		pDevice,
		Archives::Shader("Lambert").c_str(),
		NULL,
		NULL,
		0,
		NULL,
		&m_pEffect,
		&pErr);

	if (FAILED(hr)) {
		MessageBox(NULL, (LPCTSTR)pErr->GetBufferPointer(), "ERROR", MB_OK);
		SAFE_RELEASE(pErr);
		return E_FAIL;
	}
	SAFE_RELEASE(pErr);

	//fx�t�@�C�����Ő錾���Ă���ϐ��̃n���h�����擾����
	m_hTechnique = m_pEffect->GetTechniqueByName("TShader");
	m_hWorld = m_pEffect->GetParameterByName(NULL, "matWorld");
	m_hCameraView = m_pEffect->GetParameterByName(NULL, "matView");
	m_hCameraProj = m_pEffect->GetParameterByName(NULL, "matProj");
	m_hBlendNum = m_pEffect->GetParameterByName(NULL, "iBlendNum");
	m_hLightDir = m_pEffect->GetParameterByName(NULL, "vLightDir");
	m_hAmbient = m_pEffect->GetParameterByName(NULL, "vColor");
	m_hTexture = m_pEffect->GetParameterByName(NULL, "tex");

	m_hLightView = m_pEffect->GetParameterByName(NULL, "matLightView");
	m_hLightProj = m_pEffect->GetParameterByName(NULL, "matLightProj");
	m_hBias = m_pEffect->GetParameterByName(NULL, "fBias");
	m_hScaleBias = m_pEffect->GetParameterByName(NULL, "matScaleBias");
	m_hShadowMap = m_pEffect->GetParameterByName(NULL, "ShadowMap");

	m_pEffect->SetTechnique(m_hTechnique);

	return S_OK;
}