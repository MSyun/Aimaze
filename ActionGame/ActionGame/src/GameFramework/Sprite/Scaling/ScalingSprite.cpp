// �g�k�X�v���C�g
// 2017. 2. 7	�v���O�����쐬
// author		SyunMizuno



#include	"ScalingSprite.h"
#include	"../../framework/Time/Time.h"
#include	<math.h>
#include	"../../framework/Utility/System/SystemUtility.h"


/*									//
//			�R���X�g���N�^			//
//									*/
ScalingSprite::ScalingSprite(
	Vector3 begin,
	Vector3 end,
	float time,
	bool reverse,
	bool awake) :
	m_BeginScale(begin),
	m_EndScale(end),
	m_fChangeTime(time),
	m_bReverse(reverse),
	m_bPlay(awake),
	m_bTimeScale(true),
	m_EaseType(EaseLiner)
{
	Reset(reverse);

	m_onEndPlayReverse = new CPPEvent<>;
	m_onEndPlay = new CPPEvent<>;
	SetName("ScalingSprite");
	m_AnimationCurve.SetEase(m_EaseType);
}


/*									//
//			�f�X�g���N�^			//
//									*/
ScalingSprite::~ScalingSprite() {
	SAFE_DELETE(m_onEndPlayReverse);
	SAFE_DELETE(m_onEndPlay);
}


/*									//
//				�X�V				//
//									*/
void ScalingSprite::Update() {
	Sprite::Update();

	if (!m_bPlay)	return;

	if (!m_bReverse) {
		FadeOut();
	} else {
		FadeIn();
	}

	float time = m_fCnt / m_fChangeTime;
	Vector3 scale;
	GetRectTransform()->SetScale(*D3DXVec3Lerp(&scale, &m_BeginScale, &m_EndScale, m_AnimationCurve.Evaluate(time)));
}


/*									//
//			�J�n���W�ݒ�			//
//									*/
void ScalingSprite::SetBeginScale(Vector3 scale) {
	m_BeginScale = scale;
}
void ScalingSprite::SetBeginScale(float x, float y, float z) {
	m_BeginScale.x = x;
	m_BeginScale.y = y;
	m_BeginScale.z = z;
}


/*									//
//			�ړI���W�ݒ�			//
//									*/
void ScalingSprite::SetEndScale(Vector3 scale) {
	m_EndScale = scale;
}
void ScalingSprite::SetEndScale(float x, float y, float z) {
	m_EndScale.x = x;
	m_EndScale.y = y;
	m_EndScale.z = z;
}


/*									//
//				�Đ�				//
//									*/
void ScalingSprite::Play(bool reverse, bool init) {
	m_bPlay = true;
	m_bReverse = reverse;

	if (!init)	return;
	Reset(reverse);
}


/*									//
//				������				//
//									*/
void ScalingSprite::Reset(bool reverse) {
	if (!reverse) {
		m_fCnt = 0.0f;
		GetRectTransform()->SetScale(m_BeginScale);
	} else {
		m_fCnt = m_fChangeTime;
		GetRectTransform()->SetScale(m_EndScale);
	}
	m_bReverse = reverse;
}


/*									//
//				�ꎞ��~			//
//									*/
void ScalingSprite::Pause() {
	m_bPlay = false;
}


/*									//
//			�ꎞ��~����			//
//									*/
void ScalingSprite::Resume() {
	m_bPlay = true;
}


/*									//
//			�ύX���Ԃ̐ݒ�			//
//									*/
void ScalingSprite::SetChangeTime(float time) {
	m_fChangeTime = time;
}


/*									//
//		TimeScale�Ɉˑ����邩�ݒ�	//
//									*/
void ScalingSprite::DependTimeScale(bool depend) {
	m_bTimeScale = depend;
}


/*									//
//			Ease�̕ύX				//
//									*/
void ScalingSprite::ChangeEase(EaseType ease) {
	if (m_EaseType == ease)	return;

	m_EaseType = ease;
	m_AnimationCurve.SetEase(m_EaseType);
}

#pragma region conceal method
/*									//
//			�t�F�[�h�C��			//
//									*/
void ScalingSprite::FadeIn() {
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
void ScalingSprite::FadeOut() {
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
float ScalingSprite::GetDelta() {
	if (m_bTimeScale)
		return Time::GetDeltaTime();
	else
		return Time::GetUnScaledDeltaTime();
}
#pragma endregion