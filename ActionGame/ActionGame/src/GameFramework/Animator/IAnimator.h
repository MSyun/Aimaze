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

	IAnimator();
	virtual ~IAnimator();

	// �Z�b�e�B���O
	void AnimatorSetting(HighLevelAnimController* Cont);

	// �X�V
	void Update();

#pragma region Getter/Setter
	// bool�̎擾
	bool GetBool(string name);

	// int�̎擾
	int GetInt(string name);

	// float�̎擾
	float GetFloat(string name);

	// bool�̐ݒ�
	void SetBool(string name, bool value);

	// int�̐ݒ�
	void SetInt(string name, int value);

	// float�̐ݒ�
	void SetFloat(string name, float value);

	// trigger�̐ݒ�
	void SetTrigger(string name);
#pragma endregion

	// �A�j���[�V�����̗\��
	void ChangeTime(float time, string anim);

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
	// bool�̒ǉ�
	void AddBool(string name, bool* value);

	// int�̒ǉ�
	void AddInt(string name, int* value);

	// float�̒ǉ�
	void AddFloat(string name, float* value);

	// trigger�̒ǉ�
	void AddTrigger(string name, trigger* value);
#pragma endregion

#pragma endregion
};