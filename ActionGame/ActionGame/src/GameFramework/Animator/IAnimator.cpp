// �A�j���[�^�[�C���^�[�t�F�[�X
// 2017. 1.23	�v���O�����쐬
// Author		SyunMizuno


#include	"IAnimator.h"


/*									//
//			�R���X�g���N�^			//
//									*/
IAnimator::IAnimator() :
	m_ChangeTime(-1.0f)
{
}


/*									//
//			�f�X�g���N�^			//
//									*/
IAnimator::~IAnimator() {

}


/*									//
//			�Z�b�e�B���O			//
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
//				�X�V				//
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
//				bool�̎擾			//
//									*/
bool IAnimator::GetBool(string name) {
	auto it = m_Bool.find(name);
	if (it == m_Bool.end()) {
		Debug::LogError("Animator::GetBool��" + name + "�͌�����܂���");
		return false;
	}
	return *it->second;
}


/*									//
//				int�̎擾			//
//									*/
int IAnimator::GetInt(string name) {
	auto it = m_Int.find(name);
	if (it == m_Int.end()) {
		Debug::LogError("Animator::GetInt��" + name + "�͌�����܂���");
		return -1;
	}
	return *it->second;
}


/*									//
//				float�̎擾			//
//									*/
float IAnimator::GetFloat(string name) {
	auto it = m_Float.find(name);
	if (it == m_Float.end()) {
		Debug::LogError("Animator::GetFloat��" + name + "�͌�����܂���");
		return -1.0f;
	}
	return *it->second;
}


/*									//
//				bool�̐ݒ�			//
//									*/
void IAnimator::SetBool(string name, bool value) {
	auto it = m_Bool.find(name);
	if (it == m_Bool.end()) {
		Debug::LogError("Animator::SetBool��" + name + "�͌�����܂���");
		return;
	}
	*it->second = value;
}


/*									//
//				int�̐ݒ�			//
//									*/
void IAnimator::SetInt(string name, int value) {
	auto it = m_Int.find(name);
	if (it == m_Int.end()) {
		Debug::LogError("Animator::SetInt��" + name + "�͌�����܂���");
		return;
	}
	*it->second = value;
}


/*									//
//				float�̐ݒ�			//
//									*/
void IAnimator::SetFloat(string name, float value) {
	auto it = m_Float.find(name);
	if (it == m_Float.end()) {
		Debug::LogError("Animator::SetFloat��" + name + "�͌�����܂���");
		return;
	}
	*it->second = value;
}


/*									//
//			trigger�̐ݒ�			//
//									*/
void IAnimator::SetTrigger(string name) {
	auto it = m_Trigger.find(name);
	if (it == m_Trigger.end()) {
		Debug::LogError("Animator::SetTrigger��" + name + "�͌�����܂���");
		return;
	}
	*it->second = true;
}
#pragma endregion


/*									//
//		�A�j���[�V�����̗\��		//
//									*/
void IAnimator::ChangeTime(float time, string anim) {
	if (m_ChangeTime >= 0.0f)	return;
	m_ChangeTime = time;
	m_ChangeAnim = anim;
}


#pragma region Add
/*									//
//				bool�̒ǉ�			//
//									*/
void IAnimator::AddBool(string name, bool* value) {
	if (!value)	return;
	m_Bool.insert(pair<string, bool*>(name, value));
}


/*									//
//				int�̒ǉ�			//
//									*/
void IAnimator::AddInt(string name, int* value) {
	if (!value)	return;
	m_Int.insert(pair<string, int*>(name, value));
}


/*									//
//				float�̒ǉ�			//
//									*/
void IAnimator::AddFloat(string name, float* value) {
	if (!value)	return;
	m_Float.insert(pair<string, float*>(name, value));
}


/*									//
//			trigger�̒ǉ�			//
//									*/
void IAnimator::AddTrigger(string name, trigger* value) {
	if (!value)	return;
	m_Trigger.insert(pair<string, trigger*>(name, value));
}
#pragma endregion