// �A�j���[�^�[�C���^�[�t�F�[�X
// 2017. 1.23	�v���O�����쐬
// Author		SyunMizuno

#pragma once


#include	"../framework/Mesh/SkinMesh/HighLevelAnimController.h"
#include	"../framework/Debug/Debug.h"
#include	"../framework/Time/Time.h"
#include	<map>
using namespace std;

/*
	Memo	:
			�A�j���[�V�����̐ݒ���Ǘ�
*/


class IAnimator {
protected:
	typedef bool trigger;
	HighLevelAnimController* m_Controller;

private:
	map<string, bool*>	m_Bool;
	map<string, int*>	m_Int;
	map<string, float*>	m_Float;
	map<string, trigger*>	m_Trigger;

	// �ύX�\��p
	float m_ChangeTime;
	string m_ChangeAnim;

public:
#pragma region method

	IAnimator() : m_ChangeTime(-1.0f) {};
	virtual ~IAnimator() {};

	// �Z�b�e�B���O
	void AnimatorSetting(HighLevelAnimController* Cont) {
		m_Controller = Cont;
		ShiftTime();
		LoopTime();
		Loop();
		Period();
		State();
		StartState();
	}

	void Update() {
		if (m_ChangeTime >= 0.0f) {
			m_ChangeTime -= Time::GetDeltaTime();
			if (m_ChangeTime < 0.0f)
				m_Controller->ChangeAnim(m_ChangeAnim);
		}
	}

#pragma region Getter/Setter
	bool GetBool(string name) {
		auto it = m_Bool.find(name);
		if (it == m_Bool.end()) {
			Debug::LogError("Animator::GetBool��" + name + "�͌�����܂���");
			return false;
		}
		return *it->second;
	}
	int GetInt(string name) {
		auto it = m_Int.find(name);
		if (it == m_Int.end()) {
			Debug::LogError("Animator::GetInt��" + name + "�͌�����܂���");
			return -1;
		}
		return *it->second;
	}
	float GetFloat(string name) {
		auto it = m_Float.find(name);
		if (it == m_Float.end()) {
			Debug::LogError("Animator::GetFloat��" + name + "�͌�����܂���");
			return -1.0f;
		}
		return *it->second;
	}
	void SetBool(string name, bool value) {
		auto it = m_Bool.find(name);
		if (it == m_Bool.end()) {
			Debug::LogError("Animator::SetBool��" + name + "�͌�����܂���");
			return;
		}
		*it->second = value;
	}
	void SetInt(string name, int value) {
		auto it = m_Int.find(name);
		if (it == m_Int.end()) {
			Debug::LogError("Animator::SetInt��" + name + "�͌�����܂���");
			return;
		}
		*it->second = value;
	}
	void SetFloat(string name, float value) {
		auto it = m_Float.find(name);
		if (it == m_Float.end()) {
			Debug::LogError("Animator::SetFloat��" + name + "�͌�����܂���");
			return;
		}
		*it->second = value;
	}
	void SetTrigger(string name) {
		auto it = m_Trigger.find(name);
		if (it == m_Trigger.end()) {
			Debug::LogError("Animator::SetTrigger��" + name + "�͌�����܂���");
			return;
		}
		*it->second = true;
	}

	// �A�j���[�V�����̗\��
	void ChangeTime(float time, string anim) {
		if (m_ChangeTime >= 0.0f)	return;
		m_ChangeTime = time;
		m_ChangeAnim = anim;
	}
#pragma endregion

#pragma endregion

protected:
#pragma region conceal method

	virtual void ShiftTime() {}
	virtual void LoopTime() {}
	virtual void Loop() {}
	virtual void Period() {}
	virtual void State() = 0;
	virtual void StartState() {}

#pragma region Add
	void AddBool(string name, bool* value) {
		if (!value)	return;
		m_Bool.insert(pair<string, bool*>(name, value));
	}
	void AddInt(string name, int* value) {
		if (!value)	return;
		m_Int.insert(pair<string, int*>(name, value));
	}
	void AddFloat(string name, float* value) {
		if (!value)	return;
		m_Float.insert(pair<string, float*>(name, value));
	}
	void AddTrigger(string name, trigger* value) {
		if (!value)	return;
		m_Trigger.insert(pair<string, trigger*>(name, value));
	}
#pragma endregion

#pragma endregion
};