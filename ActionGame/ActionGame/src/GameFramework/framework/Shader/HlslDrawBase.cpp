//	HLSL�`��n���N���X
// 2016. 7.19	�v���O�����쐬
// author		SyunMizuno


#include	"HlslDrawBase.h"
#include	<tchar.h>
#include	"../Graphic/Graphics.h"
#include	"../Screen/Screen.h"
#include	"../Camera/CameraManager.h"


/*									//
//			�R���X�g���N�^			//
//									*/
IHlslDrawBase::IHlslDrawBase() {
}


/*									//
//			�f�X�g���N�^			//
//									*/
IHlslDrawBase::~IHlslDrawBase() {
}


/*									//
//		�V�F�[�_�[�����J�n			//
//									*/
void IHlslDrawBase::Begin() {
	if (!IsOK())	return;

	LPDIRECT3DDEVICE9 pDevice = GetGraphics()->GetDevice();
	Camera* camera = GetCameraManager()->CurrentCamera();
	SetCameraView(camera->GetView());
	SetCameraProj(camera->GetProj());

	m_pEffect->Begin(NULL, 0);

	float fOffsetX = 0.5f + (0.5f / ((float)Screen::GetWidth() / 3.0f));
	float fOffsetY = 0.5f + (0.5f / ((float)Screen::GetHeight() / 3.0f));
	Matrix ScaleBias(
		0.5f, 0.0f, 0.0f, 0.0f,
		0.0f, -0.5f, 0.0f, 0.0f,
		0.0f, 0.0f, 0.0f, 0.0f,
		fOffsetX, fOffsetY, 0.0f, 1.0f);
	SetScaleBias(ScaleBias);
}


/*									//
//		�e�N�X�`����\��t��		//
//									*/
void IHlslDrawBase::SetDecalMap(LPDIRECT3DTEXTURE9 &pTex) {
	if (IsOK()) {
		m_pEffect->SetTexture(m_hTexture, pTex);
	} else {
		GetGraphics()->GetDevice()->SetTexture(0, pTex);
	}
}


/*									//
//			������ݒ�			//
//									*/
void IHlslDrawBase::SetAmbient(float Ambient) {
	if (IsOK()) {
		D3DXVECTOR4	A;
		A = Vector4(Ambient, Ambient, Ambient, Ambient);
		m_pEffect->SetVector(m_hAmbient, &A);
	}
	// �V�F�[�_�[���g�p�ł��Ȃ��Ƃ��́A�Œ�@�\�p�C�v���C���̃}�e���A����ݒ�
	else {
		D3DMATERIAL9 old_material;
		LPDIRECT3DDEVICE9 pDevice = GetGraphics()->GetDevice();
		pDevice->GetMaterial(&old_material);
		old_material.Ambient.r = Ambient;
		old_material.Ambient.g = Ambient;
		old_material.Ambient.b = Ambient;
		old_material.Ambient.a = 1.0f;
		pDevice->SetMaterial(&old_material);
	}
}


/*									//
//			������ݒ�			//
//									*/
void IHlslDrawBase::SetAmbient(const Vector4* pAmbient) {
	if (IsOK()) {
		m_pEffect->SetVector(m_hAmbient, pAmbient);
	}
	//�V�F�[�_�[���g�p�ł��Ȃ��Ƃ��́A�Œ�@�\�p�C�v���C���̃}�e���A����ݒ肷��
	else {
		D3DMATERIAL9 old_material;
		LPDIRECT3DDEVICE9 pDevice = GetGraphics()->GetDevice();
		pDevice->GetMaterial(&old_material);
		old_material.Ambient.r = pAmbient->x;
		old_material.Ambient.g = pAmbient->y;
		old_material.Ambient.b = pAmbient->z;
		old_material.Ambient.a = pAmbient->w;
		pDevice->SetMaterial(&old_material);
	}
}


/*									//
//			�u�����h����ݒ�		//
//									*/
void IHlslDrawBase::SetBlendNum(DWORD BlendNum) {
	if (IsOK()) {
		m_pEffect->SetInt(m_hBlendNum, BlendNum);
	} else
		GetGraphics()->GetDevice()->SetRenderState(D3DRS_VERTEXBLEND, BlendNum - 1);
}


/*									//
//			���[���h�s���ݒ�		//
//									*/
void IHlslDrawBase::SetWorldMatrix(const Matrix* world, int n) {
	if (IsOK()) {
		m_mtxWorld[n] = (*world);
	} else
		GetGraphics()->GetDevice()->SetTransform(D3DTS_WORLDMATRIX(n), world);
}


/*									//
//			�s��̐ݒ�				//
//									*/
void IHlslDrawBase::SetMatrix() {
	if (IsOK()) {
		// ���[���h �~ �r���[ �~ �ˉe
		m_pEffect->SetMatrixArray(m_hWorld, m_mtxWorld, 4);
		m_pEffect->SetMatrix(m_hCameraView, &m_mtxCameraView);
		m_pEffect->SetMatrix(m_hCameraProj, &m_mtxCameraProj);
		m_pEffect->SetMatrix(m_hLightView, &m_mtxLightView);
		m_pEffect->SetMatrix(m_hLightProj, &m_mtxLightProj);
	}

	// �V�F�[�_�[���g�p�ł��Ȃ��Ƃ��́A�Œ�@�\�p�C�v���C���̃}�g���b�N�X��ݒ肷��
	else
		GetGraphics()->GetDevice()->SetTransform(D3DTS_WORLD, &m_mtxWorld[0]);
}

/*									//
//			�����̐ݒ�				//
//									*/
void IHlslDrawBase::SetLight(const Vector4* pLightDir) {
	if (!IsOK())	return;

	Vector4 LightDir = *pLightDir;
	D3DXVec3Normalize((Vector3*)&LightDir, (Vector3*)&LightDir);
	m_pEffect->SetVector(m_hLightDir, &LightDir);
}


/*									//
//		�J������View�̐ݒ�			//
//									*/
void IHlslDrawBase::SetCameraView(Matrix& view) {
	if (!IsOK())	return;

	m_mtxCameraView = view;
}


/*									//
//		�J������Projoction�̐ݒ�	//
//									*/
void IHlslDrawBase::SetCameraProj(Matrix& proj) {
	if (!IsOK())	return;

	m_mtxCameraProj = proj;
}


/*									//
//		���C�g��View�̐ݒ�			//
//									*/
void IHlslDrawBase::SetLightView(Matrix& view) {
	if (!IsOK())	return;

	m_mtxLightView = view;
}


/*									//
//		���C�g��Projection�̐ݒ�	//
//									*/
void IHlslDrawBase::SetLightProj(Matrix& proj) {
	if (!IsOK())	return;

	m_mtxLightProj = proj;
}


/*									//
//			�o�C�A�X�̐ݒ�			//
//									*/
void IHlslDrawBase::SetBias(float bias) {
	if (!IsOK())	return;

	m_pEffect->SetFloat(m_hBias, bias);
}


/*									//
//			�e�e�N�X�`���̐ݒ�		//
//									*/
void IHlslDrawBase::SetShadowMap(LPDIRECT3DTEXTURE9 shadow) {
	if (!IsOK())	return;
	
	m_pEffect->SetTexture(m_hShadowMap, shadow);
}


void IHlslDrawBase::SetScaleBias(Matrix& mat) {
	if (!IsOK())	return;

	m_pEffect->SetMatrix(m_hScaleBias, &mat);
}