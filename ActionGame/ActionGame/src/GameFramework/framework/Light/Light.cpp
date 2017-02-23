//	���C�g�N���X
// 2016. 3.27	�v���O�����쐬
// author		SyunMizuno

#include	"Light.h"
#include	<tchar.h>
#include	"../Graphic/Graphics.h"


/*									//
//			�R���X�g���N�^			//
//									*/
Light::Light() {
	m_vPos		= Vector3( 0.f, 10.f, 0.f );
	m_vLookAt	= Vector3( 0.f, 0.f, 0.f );
	m_vUp		= Vector3( 0.f, 1.f, 0.f );
	D3DXMatrixIdentity( &m_mtxView );
	D3DXMatrixIdentity( &m_mtxProj );
	m_fNear		= 1.f;
	m_fFar		= 20.f;
	m_fSpeclar	= 100.0f;
	m_vColor	= Color( 1.0f, 1.0f, 1.0f, 1.0f );
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
	D3DXMatrixLookAtLH(&m_mtxView, &m_vPos, &m_vLookAt, &m_vUp);
	D3DXMatrixPerspectiveFovLH(&m_mtxProj, D3DXToRadian(90), 1.f, m_fNear, m_fFar);

	// �V�F�[�_���g�p���Ȃ��ꍇ�Œ�@�\�d�l
	if (!_custom) {
		D3DLIGHT9 light;
		ZeroMemory(&light, sizeof(D3DLIGHT9));		// ������
		light.Type = D3DLIGHT_DIRECTIONAL;			// ���s����
		// �g�U��
		light.Diffuse = m_vColor;
		light.Direction = (Vector3)GetDir();
		light.Range = 1000.0f;
		GetGraphics()->GetDevice()->SetLight(0, &light);		// ���C�g�ݒ�
		GetGraphics()->GetDevice()->LightEnable(0, TRUE);		// ���C�g���Ă�

		GetGraphics()->GetDevice()->SetRenderState(D3DRS_AMBIENT, 0x00444444);	// ���l�������قǖ��邢
	} else
		GetGraphics()->GetDevice()->LightEnable(0, FALSE);
}