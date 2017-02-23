//////////////////////////////////////////////////////////////
// �J�����N���X
// 2016. 7. 3	�v���O�����쐬
// Author		SyunMizuno
//////////////////////////////////////////////////////////////


#include	"Camera.h"
#include	"../Screen/Screen.h"
#include	"../Utility/System/SystemUtility.h"
#include	"../Graphic/Graphics.h"

/*										//
//			�R���X�g���N�^				//
//										*/
Camera::Camera() :
	m_fFovy(60.0f),
	m_fAspect((float)Screen::GetWidth() / (float)Screen::GetHeight()),
	m_fNear(1.0f),
	m_fFar(1000.0f)
{
	GetTransform()->SetPos(0.0f, 5.0f, -10.0f);
	m_fLookLength = 5.0f;
	CreateSky();
	SetName("Camera");
}


/*										//
//			�f�X�g���N�^				//
//										*/
Camera::~Camera() {
	DeleteSky();
}


/*										//
//				�X�V					//
//										*/
void Camera::Update() {
	Obj3D::Update();

	// �X�J�C�{�b�N�X�X�V
	if (m_pSky) {
		m_pSky->GetTransform()->SetPos(GetTransform()->GetPos());
	}
}


/*										//
//				�Z�b�g					//
//										*/
void Camera::Set() {
	Transform*	transform = GetTransform();
	// �r���[�}�g���N�X�̐ݒ�
	D3DXMatrixLookAtLH(
		&m_matView,
		&transform->GetPos(),
		&(transform->GetPos() + transform->GetForward() * m_fLookLength),
		&transform->GetUp());
	GetGraphics()->SetView(m_matView);

	// �v���W�F�N�V�����}�g���N�X�̐ݒ�
	D3DXMatrixPerspectiveFovLH(
		&m_matProj,
		D3DXToRadian(m_fFovy),
		m_fAspect,
		m_fNear,
		m_fFar);
	GetGraphics()->SetProjection(m_matProj);
}


/*										//
//			�X�J�C�{�b�N�X�쐬			//
//										*/
void Camera::CreateSky() {
	if (!m_pSky) {
		m_pSky = new Skydome;
	}
}


/*										//
//			�X�J�C�{�b�N�X�폜			//
//										*/
void Camera::DeleteSky() {
	SAFE_DELETE(m_pSky);
}