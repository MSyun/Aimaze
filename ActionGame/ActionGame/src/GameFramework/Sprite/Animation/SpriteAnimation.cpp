// author		SyunMizuno
// 2016. 2. 4	�v���O�����쐬


#include	"SpriteAnimation.h"
#include	"../../framework/Time/Time.h"


/*									//
//			�R���X�g���N�^			//
//									*/
SpriteAnimation::SpriteAnimation() :
	m_fCounter(0.0f),
	m_nPattern(0),
	m_nDivideX(1),
	m_nDivideY(1),
	m_fAnimationSpeed(0.1f)
{
	SetName("SpriteAnimation");
}


/*									//
//			�f�X�g���N�^			//
//									*/
SpriteAnimation::~SpriteAnimation() {
}


/*									//
//				������				//
//									*/
HRESULT SpriteAnimation::Init(
	IDirect3DTexture9* _pTex,
	Vector2 _size,
	int		_divX,
	int		_divY,
	float	_count)
{
	Sprite::SetTexture(_pTex);
	GetRectTransform()->SetSize(_size);
	m_fCounter = _count;
	m_nDivideX = _divX;
	m_nDivideY = _divY;

	return S_OK;
}


/*									//
//				�X�V				//
//									*/
void SpriteAnimation::Update() {
	SetVtx();
	SetColor();
	Animation();
}


/*									//
//		�e�N�X�`�����W�̐ݒ�		//
//									*/
void SpriteAnimation::SetTex() {
	m_aVtx[0].tex.x = (float)(m_nPattern % m_nDivideX *(1.0f / m_nDivideX));
	m_aVtx[0].tex.y = (float)(m_nPattern / m_nDivideX *(1.0f / m_nDivideY));
	m_aVtx[1].tex.x = m_aVtx[0].tex.x + 1.0f / m_nDivideX;
	m_aVtx[1].tex.y = m_aVtx[0].tex.y;
	m_aVtx[2].tex.x = m_aVtx[0].tex.x;
	m_aVtx[2].tex.y = m_aVtx[0].tex.y + 1.0f / m_nDivideY;
	m_aVtx[3].tex.x = m_aVtx[1].tex.x;
	m_aVtx[3].tex.y = m_aVtx[2].tex.y;
}


/*									//
//		�A�j���[�V�����ԍ��̐ݒ�	//
//									*/
void SpriteAnimation::Animation() {
	m_fCounter += Time::GetDeltaTime();
	if(m_fCounter >= m_fAnimationSpeed) {
		m_fCounter = 0.0f;
		// �A�j���p�^�[���̐؂�ւ�
		++m_nPattern;
		SetTex();
	}
}


/*									//
//		�A�j���[�V�������x�̐ݒ�	//
//									*/
void SpriteAnimation::SetAnimationSpeed(float speed) {
	m_fAnimationSpeed = speed;
}