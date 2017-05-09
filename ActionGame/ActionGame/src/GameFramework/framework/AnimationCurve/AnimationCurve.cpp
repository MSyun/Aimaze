//	アニメーションカーブ
// 2017. 2.20	プログラム作成
// 2017. 2.21	仮完成
// Author		SyunMizuno


#include	"AnimationCurve.h"
#include	"../Utility/System/SystemUtility.h"
#include	"../Debug/Debug.h"
#include	"../Utility/Math/MathUtility.h"


/*
	Memo	:
			Easeを変えると生成しなおすので要修正
*/



/*									//
//			コンストラクタ			//
//									*/
AnimationCurve::AnimationCurve() :
	m_CurrentEase(EaseLiner)
{
	ChangeEase(m_CurrentEase);
}


/*									//
//			デストラクタ			//
//									*/
AnimationCurve::~AnimationCurve() {
	for (auto it = m_Keys.begin(); it != m_Keys.end(); ++it) {
		SAFE_DELETE(*it);
	}
}


/*									//
//				補完設定			//
//									*/
void AnimationCurve::SetEase(EaseType type) {
	if (m_CurrentEase == type)	return;

	m_CurrentEase = type;
	ChangeEase(type);
}


/*									//
//				補完設定			//
//									*/
void AnimationCurve::ChangeEase(EaseType type) {
	for (auto it = m_Keys.begin(); it != m_Keys.end(); ++it) {
		SAFE_DELETE(*it);
	}
	m_Keys.clear();

	switch (type) {
	case EaseInQuad:
		CreateInQuad();
		break;

	case EaseOutQuad:
		CreateOutQuad();
		break;

	case EaseInOutQuad:
		CreateInOutQuad();
		break;

	case EaseInCubic:
		CreateInCubic();
		break;

	case EaseOutCubic:
		CreateOutCubic();
		break;

	case EaseInOutCubic:
		CreateInOutCubic();
		break;

	case EaseInQuart:
		CreateInQuart();
		break;

	case EaseOutQuart:
		CreateOutQuart();
		break;

	case EaseInOutQuart:
		CreateInOutQuart();
		break;

	case EaseInQuint:
		CreateInQuint();
		break;

	case EaseOutQuint:
		CreateOutQuint();
		break;

	case EaseInOutQuint:
		CreateInOutQuint();
		break;

	case EaseInExpo:
		CreateInExpo();
		break;

	case EaseOutExpo:
		CreateOutExpo();
		break;

	case EaseInOutExpo:
		CreateInOutExpo();
		break;

	case EaseInCirc:
		CreateInCirc();
		break;

	case EaseOutCirc:
		CreateOutCirc();
		break;

	case EaseInOutCirc:
		CreateInOutCirc();
		break;

	case EaseInSine:
		CreateInSine();
		break;

	case EaseOutSine:
		CreateOutSine();
		break;

	case EaseInOutSine:
		CreateInOutSine();
		break;

	case EaseLiner:
		CreateLiner();
		break;

	case EaseInBounce:
		CreateInBounce();
		break;

	case EaseOutBounce:
		CreateOutBounce();
		break;

	case EaseInOutBounce:
		CreateInOutBounce();
		break;

	case EaseInBack:
		CreateInBack();
		break;

	case EaseOutBack:
		CreateOutBack();
		break;

	case EaseInOutBack:
		CreateInOutBack();
		break;

	case EaseInElastic:
		CreateInElastic();
		break;

	case EaseOutElastic:
		CreateOutElastic();
		break;

	case EaseInOutElastic:
		CreateInOutElastic();
		break;
	};
}


/*									//
//				補完取得			//
//									*/
EaseType AnimationCurve::GetEase() {
	return m_CurrentEase;
}


/*									//
//			キーの数を取得			//
//									*/
int AnimationCurve::Length() {
	return m_Keys.size();
}


/*									//
//				キーの追加			//
//									*/
int AnimationCurve::AddKey(float time, float value) {
	for (auto it = m_Keys.begin(); it != m_Keys.end(); ++it) {
		if (time == (*it)->fTime)	return -1;
	}

	tKeyframe* Key = new tKeyframe(time, value);
	m_Keys.push_back(Key);

	return Length() - 1;
}

int AnimationCurve::AddKey(tKeyframe& key) {
	for (auto it = m_Keys.begin(); it != m_Keys.end(); ++it) {
		if (key.fTime == (*it)->fTime)	return -1;
	}

	tKeyframe* Key = new tKeyframe(key);
	m_Keys.push_back(Key);

	return Length() - 1;
}


/*									//
//				キーの削除			//
//									*/
void AnimationCurve::RemoveKey(int index) {
	if (index < 0 || Length() <= index) {
		Debug::Log("AnimationCurve.RemoveKey(index) : 存在しないキーです");
		return;
	}

	auto it = m_Keys[index];
	SAFE_DELETE(it);
	m_Keys.erase(m_Keys.begin() + index);
}


/*									//
//			timeのカーブを評価		//
//									*/
float AnimationCurve::Evaluate(float time) {
	tKeyframe* begin = NULL;
	tKeyframe* end = NULL;

	// 対応するキーフレームを検索
	for (auto it = m_Keys.begin(); it != m_Keys.end(); ++it) {
		if ((*it)->fTime == time)	return (*it)->fValue;

		if ((*it)->fTime < time)
			begin = (*it);
		if ((*it)->fTime > time) {
			end = (*it);
			break;
		}
	}

	if (begin == NULL || end == NULL)
		return -1;

	// 時間調整
	float NTime = (time - begin->fTime) / (end->fTime - begin->fTime);

	return Bezier(
		Vector2(begin->fTime, begin->fValue),
		begin->fOutTangent,
		end->fInTangent,
		Vector2(end->fTime, end->fValue),
		NTime).y;
}


#pragma region EaseCreate
// Liner
void AnimationCurve::CreateLiner() {
	tKeyframe key1(0.0f, 0.0f, Vector2(-0.3f, 0.0f), Vector2(0.3f, 0.3f));
	tKeyframe key2(1.0f, 1.0f, Vector2(0.7f, 0.7f), Vector2(1.3f, 1.0));
	AddKey(key1);
	AddKey(key2);
}
// Quad
void AnimationCurve::CreateInQuad() {
	tKeyframe key1(0.0f, 0.0f, Vector2(-0.3f, 0.0f), Vector2(0.4f, 0.0f));
	tKeyframe key2(1.0f, 1.0f, Vector2(0.6f, 0.2f), Vector2(1.3f, 1.0));
	AddKey(key1);
	AddKey(key2);
}
void AnimationCurve::CreateOutQuad() {
	tKeyframe key1(0.0f, 0.0f, Vector2(-0.3f, 0.0f), Vector2(0.4f, 0.8f));
	tKeyframe key2(1.0f, 1.0f, Vector2(0.6f, 1.0f), Vector2(1.3f, 1.0));
	AddKey(key1);
	AddKey(key2);
}
void AnimationCurve::CreateInOutQuad() {
	tKeyframe key1(0.0f, 0.0f, Vector2(-0.3f, 0.0f), Vector2(0.4f, 0.0f));
	tKeyframe key2(1.0f, 1.0f, Vector2(0.6f, 1.0f), Vector2(1.3f, 1.0));
	AddKey(key1);
	AddKey(key2);
}
// Cubic
void AnimationCurve::CreateInCubic() {
	tKeyframe key1(0.0f, 0.0f, Vector2(-0.3f, 0.0f), Vector2(0.6f, 0.0f));
	tKeyframe key2(1.0f, 1.0f, Vector2(0.6f, 0.2f), Vector2(1.3f, 1.0));
	AddKey(key1);
	AddKey(key2);
}
void AnimationCurve::CreateOutCubic() {
	tKeyframe key1(0.0f, 0.0f, Vector2(-0.3f, 0.0f), Vector2(0.4f, 0.8f));
	tKeyframe key2(1.0f, 1.0f, Vector2(0.4f, 1.0f), Vector2(1.3f, 1.0));
	AddKey(key1);
	AddKey(key2);
}
void AnimationCurve::CreateInOutCubic() {
	tKeyframe key1(0.0f, 0.0f, Vector2(-0.3f, 0.0f), Vector2(0.6f, 0.0f));
	tKeyframe key2(1.0f, 1.0f, Vector2(0.4f, 1.0f), Vector2(1.3f, 1.0));
	AddKey(key1);
	AddKey(key2);
}
// Quart
void AnimationCurve::CreateInQuart() {
	tKeyframe key1(0.0f, 0.0f, Vector2(-0.3f, 0.0f), Vector2(0.8f, 0.0f));
	tKeyframe key2(1.0f, 1.0f, Vector2(0.6f, 0.2f), Vector2(1.3f, 1.0));
	AddKey(key1);
	AddKey(key2);
}
void AnimationCurve::CreateOutQuart() {
	tKeyframe key1(0.0f, 0.0f, Vector2(-0.3f, 0.0f), Vector2(0.4f, 0.8f));
	tKeyframe key2(1.0f, 1.0f, Vector2(0.2f, 1.0f), Vector2(1.3f, 1.0));
	AddKey(key1);
	AddKey(key2);
}
void AnimationCurve::CreateInOutQuart() {
	tKeyframe key1(0.0f, 0.0f, Vector2(-0.3f, 0.0f), Vector2(0.8f, 0.0f));
	tKeyframe key2(1.0f, 1.0f, Vector2(0.2f, 1.0f), Vector2(1.3f, 1.0));
	AddKey(key1);
	AddKey(key2);
}
// Quint
void AnimationCurve::CreateInQuint() {
	tKeyframe key1(0.0f, 0.0f, Vector2(-0.3f, 0.0f), Vector2(0.9f, 0.0f));
	tKeyframe key2(1.0f, 1.0f, Vector2(0.7f, 0.2f), Vector2(1.3f, 1.0));
	AddKey(key1);
	AddKey(key2);
}
void AnimationCurve::CreateOutQuint() {
	tKeyframe key1(0.0f, 0.0f, Vector2(-0.3f, 0.0f), Vector2(0.3f, 0.8f));
	tKeyframe key2(1.0f, 1.0f, Vector2(0.1f, 1.0f), Vector2(1.3f, 1.0));
	AddKey(key1);
	AddKey(key2);
}
void AnimationCurve::CreateInOutQuint() {
	tKeyframe key1(0.0f, 0.0f, Vector2(-0.3f, 0.0f), Vector2(0.9f, 0.0f));
	tKeyframe key2(1.0f, 1.0f, Vector2(0.1f, 1.0f), Vector2(1.3f, 1.0));
	AddKey(key1);
	AddKey(key2);
}
// Expo
void AnimationCurve::CreateInExpo() {
	tKeyframe key1(0.0f, 0.0f, Vector2(-0.3f, 0.0f), Vector2(1.0f, 0.0f));
	tKeyframe key2(1.0f, 1.0f, Vector2(0.8f, 0.1f), Vector2(1.3f, 1.0));
	AddKey(key1);
	AddKey(key2);
}
void AnimationCurve::CreateOutExpo() {
	tKeyframe key1(0.0f, 0.0f, Vector2(-0.3f, 0.0f), Vector2(0.2f, 0.9f));
	tKeyframe key2(1.0f, 1.0f, Vector2(0.0f, 1.0f), Vector2(1.3f, 1.0));
	AddKey(key1);
	AddKey(key2);
}
void AnimationCurve::CreateInOutExpo() {
	tKeyframe key1(0.0f, 0.0f, Vector2(-0.3f, 0.0f), Vector2(1.0f, 0.0f));
	tKeyframe key2(1.0f, 1.0f, Vector2(0.0f, 1.0f), Vector2(1.3f, 1.0));
	AddKey(key1);
	AddKey(key2);
}
// Circ
void AnimationCurve::CreateInCirc() {
	tKeyframe key1(0.0f, 0.0f, Vector2(-0.3f, 0.0f), Vector2(0.5f, 0.0f));
	tKeyframe key2(1.0f, 1.0f, Vector2(1.0f, 0.5f), Vector2(1.3f, 1.0));
	AddKey(key1);
	AddKey(key2);
}
void AnimationCurve::CreateOutCirc() {
	tKeyframe key1(0.0f, 0.0f, Vector2(-0.3f, 0.0f), Vector2(0.0f, 0.5f));
	tKeyframe key2(1.0f, 1.0f, Vector2(0.5f, 1.0f), Vector2(1.3f, 1.0));
	AddKey(key1);
	AddKey(key2);
}
void AnimationCurve::CreateInOutCirc() {
	tKeyframe key1(0.0f, 0.0f, Vector2(-0.3f, 0.0f), Vector2(0.25f, 0.0f));
	tKeyframe key2(0.5f, 0.5f, Vector2(0.5f, 0.25f), Vector2(0.5f, 0.75f));
	tKeyframe key3(1.0f, 1.0f, Vector2(0.0f, 1.0f), Vector2(1.3f, 1.0));
	AddKey(key1);
	AddKey(key2);
	AddKey(key3);
}
// Sine
void AnimationCurve::CreateInSine() {
	tKeyframe key1(0.0f, 0.0f, Vector2(-0.3f, 0.0f), Vector2(0.2f, 0.0f));
	tKeyframe key2(1.0f, 1.0f, Vector2(0.6f, 0.2f), Vector2(1.3f, 1.0));
	AddKey(key1);
	AddKey(key2);
}
void AnimationCurve::CreateOutSine() {
	tKeyframe key1(0.0f, 0.0f, Vector2(-0.3f, 0.0f), Vector2(0.4f, 0.8f));
	tKeyframe key2(1.0f, 1.0f, Vector2(0.8f, 1.0f), Vector2(1.3f, 1.0));
	AddKey(key1);
	AddKey(key2);
}
void AnimationCurve::CreateInOutSine() {
	tKeyframe key1(0.0f, 0.0f, Vector2(-0.3f, 0.0f), Vector2(0.2f, 0.0f));
	tKeyframe key2(1.0f, 1.0f, Vector2(0.8f, 1.0f), Vector2(1.3f, 1.0));
	AddKey(key1);
	AddKey(key2);
}
// Bounce
void AnimationCurve::CreateInBounce() {
	tKeyframe key1(0.0f, 0.0f, Vector2(-0.3f, 0.0f), Vector2(0.05f, 0.05f));
	tKeyframe key2(0.1f, 0.0f, Vector2(0.05f, 0.05f), Vector2(0.15f, 0.1f));
	tKeyframe key3(0.3f, 0.0f, Vector2(0.25f, 0.1f), Vector2(0.4f, 0.3f));
	tKeyframe key4(0.6f, 0.0f, Vector2(0.5f, 0.3f), Vector2(0.65f, 0.4f));
	tKeyframe key5(1.0f, 1.0f, Vector2(0.8f, 1.0f), Vector2(1.3f, 1.0));
	AddKey(key1);
	AddKey(key2);
	AddKey(key3);
	AddKey(key4);
	AddKey(key5);
}
void AnimationCurve::CreateOutBounce() {
	tKeyframe key1(0.0f, 0.0f, Vector2(-0.3f, 0.0f), Vector2(0.2f, 0.0f));
	tKeyframe key2(0.4f, 1.0f, Vector2(0.35f, 0.6f), Vector2(0.5f, 0.3f));
	tKeyframe key3(0.7f, 1.0f, Vector2(0.6f, 0.7f), Vector2(0.75f, 0.9f));
	tKeyframe key4(0.9f, 1.0f, Vector2(0.85f, 0.9f), Vector2(0.95f, 0.95f));
	tKeyframe key5(1.0f, 1.0f, Vector2(0.95f, 0.95f), Vector2(1.3f, 1.0));
	AddKey(key1);
	AddKey(key2);
	AddKey(key3);
	AddKey(key4);
	AddKey(key5);
}
void AnimationCurve::CreateInOutBounce() {
	tKeyframe key1(0.0f, 0.0f, Vector2(-0.3f, 0.0f), Vector2(0.025f, 0.025f));
	tKeyframe key2(0.05f, 0.0f, Vector2(0.025f, 0.025f), Vector2(0.1f, 0.05f));
	tKeyframe key3(0.15f, 0.0f, Vector2(0.1f, 0.05f), Vector2(0.2f, 0.15f));
	tKeyframe key4(0.3f, 0.0f, Vector2(0.25f, 0.15f), Vector2(0.35f, 0.2f));
	tKeyframe key5(0.5f, 0.5f, Vector2(0.4f, 0.5f), Vector2(0.6f, 0.5f));
	tKeyframe key6(0.7f, 1.0f, Vector2(0.65f, 0.8f), Vector2(0.75f, 0.85f));
	tKeyframe key7(0.85f, 1.0f, Vector2(0.8f, 0.85f), Vector2(0.9f, 0.95f));
	tKeyframe key8(0.95f, 1.0f, Vector2(0.9f, 0.95f), Vector2(0.975f, 0.975f));
	tKeyframe key9(1.0f, 1.0f, Vector2(0.975f, 0.975f), Vector2(1.3f, 1.0f));
	AddKey(key1);
	AddKey(key2);
	AddKey(key3);
	AddKey(key4);
	AddKey(key5);
	AddKey(key6);
	AddKey(key7);
	AddKey(key8);
	AddKey(key9);
}
// Back
void AnimationCurve::CreateInBack() {
	tKeyframe key1(0.0f, 0.0f, Vector2(-0.3f, 0.0f), Vector2(0.3f, 0.0f));
	tKeyframe key2(0.6f, 0.0f, Vector2(0.4f, -0.2f), Vector2(0.8f, 0.2f));
	tKeyframe key3(1.0f, 1.0f, Vector2(0.9f, 0.6f), Vector2(1.3f, 1.0f));
	AddKey(key1);
	AddKey(key2);
	AddKey(key3);
}
void AnimationCurve::CreateOutBack() {
	tKeyframe key1(0.0f, 0.0f, Vector2(-0.3f, 0.0f), Vector2(0.1f, 0.4f));
	tKeyframe key2(0.4f, 1.0f, Vector2(0.2f, 0.8f), Vector2(0.6f, 1.2f));
	tKeyframe key3(1.0f, 1.0f, Vector2(0.7f, 1.0f), Vector2(1.3f, 1.0f));
	AddKey(key1);
	AddKey(key2);
	AddKey(key3);
}
void AnimationCurve::CreateInOutBack() {
	tKeyframe key1(0.0f, 0.0f, Vector2(-0.3f, 0.0f), Vector2(0.2f, 0.0f));
	tKeyframe key2(0.4f, 0.0f, Vector2(0.3f, -0.2f), Vector2(0.5f, 0.2f));
	tKeyframe key3(0.6f, 1.0f, Vector2(0.5f, 0.8f), Vector2(0.7f, 1.2f));
	tKeyframe key4(1.0f, 1.0f, Vector2(0.8f, 1.0f), Vector2(1.3f, 1.0f));
	AddKey(key1);
	AddKey(key2);
	AddKey(key3);
	AddKey(key4);
}
// Elastic
void AnimationCurve::CreateInElastic() {
	tKeyframe key1(0.0f, 0.0f, Vector2(-0.3f, 0.0f), Vector2(0.2f, 0.025f));
	tKeyframe key2(0.3f, 0.0f, Vector2(0.25f, -0.025f), Vector2(0.4f, 0.05f));
	tKeyframe key3(0.5f, 0.0f, Vector2(0.45f, 0.025f), Vector2(0.55f, -0.025f));
	tKeyframe key4(0.65f, 0.0f, Vector2(0.6f, -0.1f), Vector2(0.7f, 0.2f));
	tKeyframe key5(0.8f, 0.0f, Vector2(0.75f, 0.2f), Vector2(0.825f, -0.1f));
	tKeyframe key6(0.9f, -0.3f, Vector2(0.85f, -0.2f), Vector2(0.95f, -0.3f));
	tKeyframe key7(1.0f, 1.0f, Vector2(0.975f, 0.95f), Vector2(1.3f, 1.0f));
	AddKey(key1);
	AddKey(key2);
	AddKey(key3);
	AddKey(key4);
	AddKey(key5);
	AddKey(key6);
	AddKey(key7);
}
void AnimationCurve::CreateOutElastic() {
	tKeyframe key1(0.0f, 0.0f, Vector2(-0.3f, 0.0f), Vector2(0.025f, 0.05f));
	tKeyframe key2(0.1f, 1.3f, Vector2(0.05f, 1.3f), Vector2(0.15f, 1.2f));
	tKeyframe key3(0.2f, 1.0f, Vector2(0.175f, 1.1f), Vector2(0.25f, 0.8f));
	tKeyframe key4(0.35f, 1.0f, Vector2(0.3f, 0.8f), Vector2(0.4f, 1.1f));
	tKeyframe key5(0.5f, 1.0f, Vector2(0.45f, 1.025f), Vector2(0.55f, 0.975f));
	tKeyframe key6(0.7f, 1.0f, Vector2(0.6f, 0.95f), Vector2(0.75f, 1.025f));
	tKeyframe key7(1.0f, 1.0f, Vector2(0.8f, 0.975f), Vector2(1.3f, 1.0f));
	AddKey(key1);
	AddKey(key2);
	AddKey(key3);
	AddKey(key4);
	AddKey(key5);
	AddKey(key6);
	AddKey(key7);
}
void AnimationCurve::CreateInOutElastic() {
	Debug::LogError("CreateInOutElasticは利用できません");
	//tKeyframe key1(0.0f, 0.0f, Vector2(-0.3f, 0.0f), Vector2(0.2f, 0.0f));
	//tKeyframe key2(0.4f, 0.0f, Vector2(0.3f, -0.2f), Vector2(0.5f, 0.2f));
	//tKeyframe key3(0.6f, 1.0f, Vector2(0.5f, 0.8f), Vector2(0.7f, 1.2f));
	//tKeyframe key4(1.0f, 1.0f, Vector2(0.8f, 1.0f), Vector2(1.3f, 1.0f));
	//AddKey(key1);
	//AddKey(key2);
	//AddKey(key3);
	//AddKey(key4);
}
#pragma endregion