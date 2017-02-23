// 移動スプライト
// 2016. 1. 4	プログラム作成
// author		SyunMizuno


#include	"MovingSprite.h"
#include	"../../framework/Time/Time.h"
#include	<math.h>
#include	"../../framework/Utility/System/SystemUtility.h"


/*									//
//			コンストラクタ			//
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
//			デストラクタ			//
//									*/
MovingSprite::~MovingSprite() {
	SAFE_DELETE(m_onEndPlayReverse);
	SAFE_DELETE(m_onEndPlay);
}


/*									//
//				更新				//
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
//			開始座標設定			//
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
//			目的座標設定			//
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
//				再生				//
//									*/
void MovingSprite::Play(bool reverse, bool init) {
	m_bPlay = true;
	m_bReverse = reverse;

	if (!init)	return;
	Reset(reverse);
}


/*									//
//				初期化				//
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
//				一時停止			//
//									*/
void MovingSprite::Pause() {
	m_bPlay = false;
}


/*									//
//			一時停止解除			//
//									*/
void MovingSprite::Resume() {
	m_bPlay = true;
}


/*									//
//			変更時間の設定			//
//									*/
void MovingSprite::SetChangeTime(float time) {
	m_fChangeTime = time;
}


/*									//
//		TimeScaleに依存するか設定	//
//									*/
void MovingSprite::DependTimeScale(bool depend) {
	m_bTimeScale = depend;
}


/*									//
//			Easeの変更				//
//									*/
void MovingSprite::ChangeEase(EaseType ease) {
	if (m_EaseType == ease)	return;

	m_EaseType = ease;
	m_AnimationCurve.SetEase(m_EaseType);
}

#pragma region conceal method
/*									//
//			フェードイン			//
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
//			フェードアウト			//
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
//			更新時間の取得			//
//									*/
float MovingSprite::GetDelta() {
	if (m_bTimeScale)
		return Time::GetDeltaTime();
	else
		return Time::GetUnScaledDeltaTime();
}
#pragma endregion