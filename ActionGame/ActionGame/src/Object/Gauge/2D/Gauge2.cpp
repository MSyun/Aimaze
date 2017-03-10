// 2D�Q�[�W�N���X
// 2017. 2. 4	�v���O�����쐬
// Author		SyunMizuno


#include	"Gauge2.h"
#include	"../../../GameFramework/framework/Utility/Math/MathUtility.h"
#include	"../../../GameFramework/framework/Time/Time.h"


/*									//
//			�R���X�g���N�^			//
//									*/
Gauge2::Gauge2(int _max) :
	m_fMax((float)_max),
	m_fCur((float)_max),
	m_fTarget((float)_max)
{
	SetName("Gauge2");
}


/*									//
//			�f�X�g���N�^			//
//									*/
Gauge2::~Gauge2() {

}


/*									//
//				�X�V				//
//									*/
void Gauge2::Update() {
	Sprite::Update();

	if (m_fTarget > m_fCur) {
		m_fCur += (Time::GetDeltaTime() * m_fTarget);
		if (m_fTarget < m_fCur)
			m_fCur = m_fTarget;
	} else
	if (m_fTarget < m_fCur) {
		m_fCur -= (Time::GetDeltaTime() * m_fTarget);
		if (m_fTarget > m_fCur)
			m_fCur = m_fTarget;
	}
}


/*									//
//			�ő�̐��l��ݒ�		//
//									*/
void Gauge2::SetMaxNum(float _max) {
	m_fMax = _max;

	if (m_fMax < m_fCur) {
		m_fCur = m_fTarget = m_fMax;
	}
}


/*									//
//			���݂̐��l��ݒ�		//
//									*/
void Gauge2::SetCurrentNum(float _num) {
	m_fTarget = m_fCur = Clamp(_num, 0.0f, m_fMax);
}


/*									//
//			�ړI�̒l��ݒ�			//
//									*/
void Gauge2::SetNum(float _num) {
	m_fTarget = _num;
}


/*									//
//			���_���W�̐ݒ�			//
//									*/
void Gauge2::SetVtx() {
	RectTransform* trans = GetRectTransform();
	Vector3* Vtx = trans->GetVertex();
	float sizeX = trans->GetSize().x / m_fMax * m_fCur;
	float Angle = trans->GetAngle();
	float fSin = sinf(D3DXToRadian(Angle));
	float fCos = cosf(D3DXToRadian(Angle));
	Vector3 scale = trans->GetScale();
	Vector2 size = trans->GetSize();
	Vector3 pos = trans->GetPos();

	m_aVtx[0].pos = Vtx[0];

	m_aVtx[1].pos = Vector3(
		m_aVtx[0].pos.x + (sizeX * fCos * scale.x) - (-sizeX * fSin * scale.x),
		pos.y - (size.y * 0.5f * fSin * scale.y) + (-size.y * 0.5f * fCos * scale.y),
		pos.z);

	m_aVtx[2].pos = Vtx[2];

	m_aVtx[3].pos = Vector3(
		m_aVtx[2].pos.x + (sizeX * fCos * scale.x) - (sizeX * fSin * scale.x),
		pos.y - (size.y * 0.5f * fSin * scale.y) + (size.y * 0.5f * fCos * scale.y),
		0.0f);
}


/*									//
//		�e�N�X�`�����W�̐ݒ�		//
//									*/
void Gauge2::SetTex() {
	// �e�N�X�`�����W
	float texX = 1.0f / m_fMax * m_fCur;

	// �e�N�X�`�����W�̐ݒ�
	m_aVtx[0].tex = Vector2(0.0f, 0.0f);
	m_aVtx[1].tex = Vector2(texX, 0.0f);
	m_aVtx[2].tex = Vector2(0.0f, 1.0f);
	m_aVtx[3].tex = Vector2(texX, 1.0f);
}