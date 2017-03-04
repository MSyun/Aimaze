//	�����o�[�g�N���X
// 2016. 3.12	�v���O�����쐬
// author		SyunMizuno


#include	"Phong.h"
#include	<tchar.h>
#include	"../../Utility/System/SystemUtility.h"
#include	"../../../../Archives/Archives.h"
#include	"../../Graphic/Graphics.h"


/*									//
//			�R���X�g���N�^			//
//									*/
Phong::Phong() {
}


/*									//
//			�f�X�g���N�^			//
//									*/
Phong::~Phong() {
}


/*									//
//				������				//
//									*/
// <return> S_OK. ���� : E_FAIL. ���s
HRESULT Phong::Load() {
	HRESULT hr;
	D3DCAPS9 caps;
	LPDIRECT3DDEVICE9 pDevice = GetGraphics()->GetDevice();

	//----- �T�|�[�g�̃o�[�W�����m�F
	pDevice->GetDeviceCaps(&caps);
	if (caps.VertexShaderVersion < D3DVS_VERSION(1, 1) &&
		caps.PixelShaderVersion < D3DPS_VERSION(2, 0)) {
		return MessageBox(NULL, _T("�O���t�B�b�N�{�[�h�͑Ή����Ă��܂���"), _T("Phong"), MB_OK);
	}

	//----- �t�@�C���ǂݍ���
	LPD3DXBUFFER pErr = NULL;
	hr = D3DXCreateEffectFromFile(
		pDevice,
		Archives::Shader("Phong").c_str(),
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
	m_hCameraView = m_pEffect->GetParameterByName(NULL, "matView");
	m_hCameraProj = m_pEffect->GetParameterByName(NULL, "matProj");
	m_hBlendNum = m_pEffect->GetParameterByName(NULL, "iBlendNum");
	m_hTexture = m_pEffect->GetParameterByName(NULL, "tex");
	m_hLightDir = m_pEffect->GetParameterByName(NULL, "vLightDir");
	m_hEyePos = m_pEffect->GetParameterByName(NULL, "vEyePos");
	m_hAmbient = m_pEffect->GetParameterByName(NULL, "vColor");
	m_hSpecular = m_pEffect->GetParameterByName(NULL, "Specular");
	m_hSpecPow = m_pEffect->GetParameterByName(NULL, "SpecularPower");
	m_hLightBloomPow = m_pEffect->GetParameterByName(NULL, "LightBloomPower");

	m_hLightView = m_pEffect->GetParameterByName(NULL, "matLightView");
	m_hLightProj = m_pEffect->GetParameterByName(NULL, "matLightProj");
	m_hBias = m_pEffect->GetParameterByName(NULL, "fBias");
	m_hScaleBias = m_pEffect->GetParameterByName(NULL, "matScaleBias");
	m_hShadowMap = m_pEffect->GetParameterByName(NULL, "ShadowMap");

	m_pEffect->SetTechnique(m_hTechnique);

	return S_OK;
}


/*									//
//		�X�y�L�����[�͈̔͂�ݒ�	//
//									*/
void Phong::SetSpecular(float Specular) {
	if (IsOK())
		m_pEffect->SetFloat(m_hSpecular, Specular);
}


/*									//
//		�X�y�L�����[�̋�����ݒ�	//
//									*/
void Phong::SetSpecularPower(float SpecPow) {
	if (IsOK())
		m_pEffect->SetFloat(m_hSpecPow, SpecPow);
}


/*									//
//		�X�y�L�����[�̋�����ݒ�	//
//									*/
void Phong::SetLightBloomPower(float LightBloomPow) {
	if (IsOK())
		m_pEffect->SetFloat(m_hLightBloomPow, LightBloomPow);
}


/*									//
//				�s��̐ݒ�			//
//									*/
void Phong::SetMatrix() {
	if( IsOK() ) {
		D3DXMATRIX	m1;
		D3DXVECTOR4	v;

		// ���[���h �~ �r���[ �~ �ˉe
		m_pEffect->SetMatrixArray( m_hWorld, m_mtxWorld, 4 );
		m_pEffect->SetMatrix(m_hCameraView, &m_mtxView);
		m_pEffect->SetMatrix(m_hCameraProj, &m_mtxProj);
		m_pEffect->SetMatrix(m_hLightView, &m_mtxLightView);
		m_pEffect->SetMatrix(m_hLightProj, &m_mtxLightProj);

		//�J�����ʒu
		m1 = m_mtxWorld[0] * m_mtxView;
		D3DXMatrixInverse( &m1, NULL, &m1 );
		D3DXVec4Transform( &v, &m_vPosCamera, &m1 );
		m_pEffect->SetVector( m_hEyePos, &v );
	}

	// �V�F�[�_�[���g�p�ł��Ȃ��Ƃ��́A�Œ�@�\�p�C�v���C���̃}�g���b�N�X��ݒ肷��
	else
		GetGraphics()->GetDevice()->SetTransform( D3DTS_WORLD, &m_mtxWorld[0] );
}


/*									//
//			�J�����̐ݒ�			//
//									*/
void Phong::SetCamera(const Vector4* pCameraPos) {
	// �J�����ʒu
	if (!IsOK())	return;

	m_vPosCamera = *pCameraPos;
}

void Phong::SetCamera(const Vector3* pCameraPos) {
	SetCamera(&Vector4(*pCameraPos, 1.0f));
}


/*									//
//			�����̐ݒ�				//
//									*/
void Phong::SetLight(const Vector4* pLightDir) {
	if (!IsOK())	return;

	Matrix	m1;
	Vector4	v;
	Vector4	LightDir;
	// ���s�����̕����x�N�g����ݒ肷��
	LightDir = *pLightDir;

	D3DXMatrixInverse(&m1, NULL, &m_mtxWorld[0]);
	D3DXVec4Transform(&v, &LightDir, &m1);
	// ���K������
	D3DXVec3Normalize((Vector3*)&v, (Vector3*)&v);
	m_pEffect->SetVector(m_hLightDir, &v);
}