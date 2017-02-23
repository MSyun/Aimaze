// フェードカラースプライト
// 2016. 1. 4	プログラム作成
// author		SyunMizuno



#include	"FadeColorSprite.h"
#include	"../../framework/Time/Time.h"
#include	<math.h>
#include	"../../framework/Utility/System/SystemUtility.h"
#include	"../../framework/Debug/Debug.h"


/*									//
//			コンストラクタ			//
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
//			デストラクタ			//
//									*/
FadeColorSprite::~FadeColorSprite() {
	SAFE_DELETE(m_onEndPlayReverse);
	SAFE_DELETE(m_onEndPlay);
}


/*									//
//				更新				//
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
//			開始色設定				//
//									*/
void FadeColorSprite::SetBeginColor(Color color) {
	m_BeginColor = color;
}


/*									//
//			目的色設定				//
//									*/
void FadeColorSprite::SetEndColor(Color color) {
	m_EndColor = color;
}


/*									//
//				再生				//
//									*/
void FadeColorSprite::Play(bool reverse, bool init) {
	m_bPlay = true;
	m_bReverse = reverse;

	if (!init)	return;
	Reset(reverse);
}


/*									//
//				初期化				//
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
//				一時停止			//
//									*/
void FadeColorSprite::Pause() {
	m_bPlay = false;
}


/*									//
//			一時停止解除			//
//									*/
void FadeColorSprite::Resume() {
	m_bPlay = true;
}


/*									//
//			変更時間の設定			//
//									*/
void FadeColorSprite::SetChangeTime(float time) {
	m_fChangeTime = time;
}


/*									//
//		TimeScaleに依存するか設定	//
//									*/
void FadeColorSprite::DependTimeScale(bool depend) {
	m_bTimeScale = depend;
}


/*									//
//			Easeの変更				//
//									*/
void FadeColorSprite::ChangeEase(EaseType ease) {
	if (m_EaseType == ease)	return;

	m_EaseType = ease;
	m_AnimationCurve.SetEase(m_EaseType);
}

#pragma region conceal method
/*									//
//			フェードイン			//
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
//			フェードアウト			//
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
//			更新時間の取得			//
//									*/
float FadeColorSprite::GetDelta() {
	if (m_bTimeScale)
		return Time::GetDeltaTime();
	else
		return Time::GetUnScaledDeltaTime();
}
#pragma endregion