//	���C�g�N���X
// 2016. 3.27	�v���O�����쐬
// author		SyunMizuno

#include	"Light.h"
#include	<tchar.h>
#include	"../Graphic/Graphics.h"
#include	"../Screen/Screen.h"


/*									//
//			�R���X�g���N�^			//
//									*/
Light::Light() {
	SetName("Light");
	SetShadow(false);

	m_fLookLength = 10.0f;
	D3DXMatrixIdentity(&m_mtxView);
	D3DXMatrixIdentity(&m_mtxProj);
	m_fNear		= 1.f;
	m_fFar		= 10.0f;
	m_fSpeclar	= 100.0f;
	m_vColor	= Color( 1.0f, 1.0f, 1.0f, 1.0f );

	GetTransform()->SetPos(0.0f, 10.0f, 0.0f);
	GetTransform()->Rotate(0.0f, -90.0f, 0.0f);
	GetTransform()->Rotate(90.0f, 0.0f, 0.0f);
}


/*									//
//			�f�X�g���N�^			//
//									*/
Light::~Light() {
}


/*									//
//				�X�V				//
//									*/
void Light::Set(bool _custom) {
	Transform* transform = GetTransform();
	Point3 pos = transform->GetPos();
	CreateView();
	CreateProj();

	// �V�F�[�_���g�p���Ȃ��ꍇ�Œ�@�\�d�l
	if (!_custom) {
		D3DLIGHT9 light;
		ZeroMemory(&light, sizeof(D3DLIGHT9));		// ������
		light.Type = D3DLIGHT_DIRECTIONAL;			// ���s����
		// �g�U��
		light.Diffuse = m_vColor;
		light.Direction = GetDirection3();
		light.Range = 1000.0f;
		GetGraphics()->GetDevice()->SetLight(0, &light);		// ���C�g�ݒ�
		GetGraphics()->GetDevice()->LightEnable(0, TRUE);		// ���C�g���Ă�

		GetGraphics()->GetDevice()->SetRenderState(D3DRS_AMBIENT, 0x00444444);	// ���l�������قǖ��邢
	} else
		GetGraphics()->GetDevice()->LightEnable(0, FALSE);


	Debug::Print("\n���C�g���W :");
	Debug::Print("X���W�@�F");
	Debug::Print(m_pTransform->GetPos().x);
	Debug::Print("Y���W�@�F");
	Debug::Print(m_pTransform->GetPos().y);
	Debug::Print("Z���W�@�F");
	Debug::Print(m_pTransform->GetPos().z);

	Point3 point = m_pTransform->GetPos() + GetDirection3();
	Debug::Print("\n���C�g�����_ :");
	Debug::Print("X���W�@�F");
	Debug::Print(point.x);
	Debug::Print("Y���W�@�F");
	Debug::Print(point.y);
	Debug::Print("Z���W�@�F");
	Debug::Print(point.z);
}

void Light::CreateView() {
	Transform* trans = GetTransform();
	Point3 Pos = trans->GetPos();
	D3DXMatrixLookAtLH(
		&m_mtxView,
		&Pos,
		&(Pos + GetDirection3()),
		&trans->GetUp());
}


void Light::CreateProj() {
	D3DXMatrixPerspectiveFovLH(
		&m_mtxProj,
		D3DXToRadian(90),
		(float)Screen::GetWidth() / (float)Screen::GetHeight(),
		m_fNear,
		m_fFar);
}


Vector3 Light::GetDirection3() {
	return GetTransform()->GetForward() * m_fLookLength;
}


Vector4 Light::GetDirection4() {
	Vector3 forward = GetTransform()->GetForward() * m_fLookLength;

	return Vector4(
		forward.x,
		forward.y,
		forward.z,
		1.0f
	);
}