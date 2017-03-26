// アニメーターインターフェース
// 2017. 1.23	プログラム作成
// Author		SyunMizuno


#include	"IAnimator.h"


/*									//
//			コンストラクタ			//
//									*/
IAnimator::IAnimator() :
	m_ChangeTime(-1.0f)
{
}


/*									//
//			デストラクタ			//
//									*/
IAnimator::~IAnimator() {

}


/*									//
//			セッティング			//
//									*/
void IAnimator::AnimatorSetting(HighLevelAnimController* Cont) {
	m_Controller = Cont;
	ShiftTime();
	LoopTime();
	Loop();
	Period();
	State();
	StartState();
}


/*									//
//				更新				//
//									*/
void IAnimator::Update() {
	if (m_ChangeTime >= 0.0f) {
		m_ChangeTime -= Time::GetDeltaTime();
		if (m_ChangeTime < 0.0f)
			m_Controller->ChangeAnim(m_ChangeAnim);
	}
}


#pragma region Getter/Setter
/*									//
//				boolの取得			//
//									*/
bool IAnimator::GetBool(string name) {
	auto it = m_Bool.find(name);
	if (it == m_Bool.end()) {
		Debug::LogError("Animator::GetBoolに" + name + "は見つかりません");
		return false;
	}
	return *it->second;
}


/*									//
//				intの取得			//
//									*/
int IAnimator::GetInt(string name) {
	auto it = m_Int.find(name);
	if (it == m_Int.end()) {
		Debug::LogError("Animator::GetIntに" + name + "は見つかりません");
		return -1;
	}
	return *it->second;
}


/*									//
//				floatの取得			//
//									*/
float IAnimator::GetFloat(string name) {
	auto it = m_Float.find(name);
	if (it == m_Float.end()) {
		Debug::LogError("Animator::GetFloatに" + name + "は見つかりません");
		return -1.0f;
	}
	return *it->second;
}


/*									//
//				boolの設定			//
//									*/
void IAnimator::SetBool(string name, bool value) {
	auto it = m_Bool.find(name);
	if (it == m_Bool.end()) {
		Debug::LogError("Animator::SetBoolに" + name + "は見つかりません");
		return;
	}
	*it->second = value;
}


/*									//
//				intの設定			//
//									*/
void IAnimator::SetInt(string name, int value) {
	auto it = m_Int.find(name);
	if (it == m_Int.end()) {
		Debug::LogError("Animator::SetIntに" + name + "は見つかりません");
		return;
	}
	*it->second = value;
}


/*									//
//				floatの設定			//
//									*/
void IAnimator::SetFloat(string name, float value) {
	auto it = m_Float.find(name);
	if (it == m_Float.end()) {
		Debug::LogError("Animator::SetFloatに" + name + "は見つかりません");
		return;
	}
	*it->second = value;
}


/*									//
//			triggerの設定			//
//									*/
void IAnimator::SetTrigger(string name) {
	auto it = m_Trigger.find(name);
	if (it == m_Trigger.end()) {
		Debug::LogError("Animator::SetTriggerに" + name + "は見つかりません");
		return;
	}
	*it->second = true;
}
#pragma endregion


/*									//
//		アニメーションの予約		//
//									*/
void IAnimator::ChangeTime(float time, string anim) {
	if (m_ChangeTime >= 0.0f)	return;
	m_ChangeTime = time;
	m_ChangeAnim = anim;
}


#pragma region Add
/*									//
//				boolの追加			//
//									*/
void IAnimator::AddBool(string name, bool* value) {
	if (!value)	return;
	m_Bool.insert(pair<string, bool*>(name, value));
}


/*									//
//				intの追加			//
//									*/
void IAnimator::AddInt(string name, int* value) {
	if (!value)	return;
	m_Int.insert(pair<string, int*>(name, value));
}


/*									//
//				floatの追加			//
//									*/
void IAnimator::AddFloat(string name, float* value) {
	if (!value)	return;
	m_Float.insert(pair<string, float*>(name, value));
}


/*									//
//			triggerの追加			//
//									*/
void IAnimator::AddTrigger(string name, trigger* value) {
	if (!value)	return;
	m_Trigger.insert(pair<string, trigger*>(name, value));
}
#pragma endregion