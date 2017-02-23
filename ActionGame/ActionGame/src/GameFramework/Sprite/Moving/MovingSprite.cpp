// �ړ��X�v���C�g
// 2016. 1. 4	�v���O�����쐬
// author		SyunMizuno


#include	"MovingSprite.h"
#include	"../../framework/Time/Time.h"
#include	<math.h>
#include	"../../framework/Utility/System/SystemUtility.h"


/*									//
//			�R���X�g���N�^			//
//									*/
MovingSprite::MovingSprite(
	Point3 begin,
	Point3 end,
	float time,
	bool reverse,
	bool awake) :
	m_BeginPos(begin),
	m_EndPos(end),
	m_fChangeTime(time),
	m_bReverse(reverse),
	m_bPlay(awake),
	m_bTimeScale(true),
	m_EaseType(EaseLiner)
{
	Reset(reverse);

	m_onEndPlayReverse = new CPPEvent<>;
	m_onEndPlay = new CPPEvent<>;
	SetName("MovingSprite");
	m_AnimationCurve.SetEase(m_EaseType);
}


/*									//
//			�f�X�g���N�^			//
//									*/
MovingSprite::~MovingSprite() {
	SAFE_DELETE(m_onEndPlayReverse);
	SAFE_DELETE(m_onEndPlay);
}


/*									//
//				�X�V				//
//									*/
void MovingSprite::Update() {
	Sprite::Update();

	if (!m_bPlay)	return;

	if (!m_bReverse) {
		FadeOut();
	} else {
		FadeIn();
	}

	float time = m_fCnt / m_fChangeTime;
	Vector3 pos;
	GetRectTransform()->SetPos(*D3DXVec3Lerp(&pos, &m_BeginPos, &m_EndPos, m_AnimationCurve.Evaluate(time)));
}


/*									//
//			�J�n���W�ݒ�			//
//									*/
void MovingSprite::SetBeginPos(Point3 pos) {
	m_BeginPos = pos;
}
void MovingSprite::SetBeginPos(float x, float y, float z) {
	m_BeginPos.x = x;
	m_BeginPos.y = y;
	m_BeginPos.z = z;
}


/*									//
//			�ړI���W�ݒ�			//
//									*/
void MovingSprite::SetEndPos(Point3 pos) {
	m_EndPos = pos;
}
void MovingSprite::SetEndPos(float x, float y, float z) {
	m_EndPos.x = x;
	m_EndPos.y = y;
	m_EndPos.z = z;
}


/*									//
//				�Đ�				//
//									*/
void MovingSprite::Play(bool reverse, bool init) {
	m_bPlay = true;
	m_bReverse = reverse;

	if (!init)	return;
	Reset(reverse);
}


/*									//
//				������				//
//									*/
void MovingSprite::Reset(bool reverse) {
	if (!reverse) {
		m_fCnt = 0.0f;
		GetRectTransform()->SetPos(m_BeginPos);
	} else {
		m_fCnt = 1.0f;
		GetRectTransform()->SetPos(m_EndPos);
	}
	m_bReverse = reverse;
}


/*									//
//				�ꎞ��~			//
//									*/
void MovingSprite::Pause() {
	m_bPlay = false;
}


/*									//
//			�ꎞ��~����			//
//									*/
void MovingSprite::Resume() {
	m_bPlay = true;
}


/*									//
//			�ύX���Ԃ̐ݒ�			//
//									*/
void MovingSprite::SetChangeTime(float time) {
	m_fChangeTime = time;
}


/*									//
//		TimeScale�Ɉˑ����邩�ݒ�	//
//									*/
void MovingSprite::DependTimeScale(bool depend) {
	m_bTimeScale = depend;
}


/*									//
//			Ease�̕ύX				//
//									*/
void MovingSprite::ChangeEase(EaseType ease) {
	if (m_EaseType == ease)	return;

	m_EaseType = ease;
	m_AnimationCurve.SetEase(m_EaseType);
}

#pragma region conceal method
/*									//
//			�t�F�[�h�C��			//
//									*/
void MovingSprite::FadeIn() {
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
void MovingSprite::FadeOut() {
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
float MovingSprite::GetDelta() {
	if (m_bTimeScale)
		return Time::GetDeltaTime();
	else
		return Time::GetUnScaledDeltaTime();
}
#pragma endregion