// �t�F�[�h�J���[�X�v���C�g
// 2016. 1. 4	�v���O�����쐬
// author		SyunMizuno



#include	"FadeColorSprite.h"
#include	"../../framework/Time/Time.h"
#include	<math.h>
#include	"../../framework/Utility/System/SystemUtility.h"
#include	"../../framework/Debug/Debug.h"


/*									//
//			�R���X�g���N�^			//
//									*/
FadeColorSprite::FadeColorSprite(
	Color begin,
	Color end,
	float time,
	bool reverse,
	bool awake) :
	m_BeginColor(begin),
	m_EndColor(end),
	m_fChangeTime(time),
	m_bReverse(reverse),
	m_bPlay(awake),
	m_bTimeScale(true),
	m_EaseType(EaseLiner)
{
	Reset(reverse);

	m_onEndPlayReverse = new CPPEvent<>;
	m_onEndPlay = new CPPEvent<>;
	SetName("FadeColorSprite");
	m_AnimationCurve.SetEase(m_EaseType);
}


/*									//
//			�f�X�g���N�^			//
//									*/
FadeColorSprite::~FadeColorSprite() {
	SAFE_DELETE(m_onEndPlayReverse);
	SAFE_DELETE(m_onEndPlay);
}


/*									//
//				�X�V				//
//									*/
void FadeColorSprite::Update() {
	Sprite::Update();

	if (!m_bPlay)	return;

	if (!m_bReverse) {
		FadeOut();
	} else {
		FadeIn();
	}

	float time = m_fCnt / m_fChangeTime;
	Color color;
	SetColor(*D3DXColorLerp(&color, &m_BeginColor, &m_EndColor, m_AnimationCurve.Evaluate(time)));
}


/*									//
//			�J�n�F�ݒ�				//
//									*/
void FadeColorSprite::SetBeginColor(Color color) {
	m_BeginColor = color;
}


/*									//
//			�ړI�F�ݒ�				//
//									*/
void FadeColorSprite::SetEndColor(Color color) {
	m_EndColor = color;
}


/*									//
//				�Đ�				//
//									*/
void FadeColorSprite::Play(bool reverse, bool init) {
	m_bPlay = true;
	m_bReverse = reverse;

	if (!init)	return;
	Reset(reverse);
}


/*									//
//				������				//
//									*/
void FadeColorSprite::Reset(bool reverse) {
	if (!reverse) {
		m_fCnt = 0.0f;
		SetColor(m_BeginColor);
	} else {
		m_fCnt = m_fChangeTime;
		SetColor(m_EndColor);
	}
	m_bReverse = reverse;
}


/*									//
//				�ꎞ��~			//
//									*/
void FadeColorSprite::Pause() {
	m_bPlay = false;
}


/*									//
//			�ꎞ��~����			//
//									*/
void FadeColorSprite::Resume() {
	m_bPlay = true;
}


/*									//
//			�ύX���Ԃ̐ݒ�			//
//									*/
void FadeColorSprite::SetChangeTime(float time) {
	m_fChangeTime = time;
}


/*									//
//		TimeScale�Ɉˑ����邩�ݒ�	//
//									*/
void FadeColorSprite::DependTimeScale(bool depend) {
	m_bTimeScale = depend;
}


/*									//
//			Ease�̕ύX				//
//									*/
void FadeColorSprite::ChangeEase(EaseType ease) {
	if (m_EaseType == ease)	return;

	m_EaseType = ease;
	m_AnimationCurve.SetEase(m_EaseType);
}

#pragma region conceal method
/*									//
//			�t�F�[�h�C��			//
//									*/
void FadeColorSprite::FadeIn() {
	m_fCnt -= GetDelta();
	if (m_fCnt <= 0.0f) {
		m_fCnt = 0.0f;
		m_bPlay = false;
		m_bReverse ^= true;
		m_onEndPlayReverse->Invoke();
	}
}


/*									//
//			�t�F�[�h�A�E�g			//
//									*/
void FadeColorSprite::FadeOut() {
	m_fCnt += GetDelta();
	if (m_fCnt >= m_fChangeTime) {
		m_fCnt = m_fChangeTime;
		m_bPlay = false;
		m_bReverse ^= true;
		m_onEndPlay->Invoke();
	}
}


/*									//
//			�X�V���Ԃ̎擾			//
//									*/
float FadeColorSprite::GetDelta() {
	if (m_bTimeScale)
		return Time::GetDeltaTime();
	else
		return Time::GetUnScaledDeltaTime();
}
#pragma endregion