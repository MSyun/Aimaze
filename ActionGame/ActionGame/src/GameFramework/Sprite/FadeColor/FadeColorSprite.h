// �t�F�[�h�J���[�X�v���C�g
// 2017. 1. 4	�v���O�����쐬
// 2017. 2.13	TimeScale�Ɉˑ����邩�ݒ�\��
// 2017. 2.21	AnimationCurve�ǉ�
// author		SyunMizuno


#pragma once


#include	"../../Obj/2D/Sprite.h"
#include	"../../framework/CPPEvent/CPPEvent.h"
#include	"../../framework/AnimationCurve/AnimationCurve.h"



class FadeColorSprite	:	public	Sprite {
private:
#pragma region variable

	Color	m_BeginColor;	// �J�n�F
	Color	m_EndColor;		// �ړI�F
	float	m_fChangeTime;	// �t�F�[�h����
	float	m_fCnt;			// �v���J�E���^
	bool	m_bPlay;		// �Đ�
	bool	m_bReverse;		// �t�Đ�
	bool	m_bTimeScale;	// TimeScale�Ɉˑ��m�F

	AnimationCurve	m_AnimationCurve;
	EaseType	m_EaseType;

	CPPEvent<>*	m_onEndPlayReverse;
	CPPEvent<>*	m_onEndPlay;

#pragma endregion

public:
#pragma region method

	FadeColorSprite(
		Color begin = Color(1.0f, 1.0f, 1.0f, 1.0f),
		Color end = Color(0.0f, 0.0f, 0.0f, 1.0f),
		float time = 1.0f,
		bool reverse = false,
		bool awake = false);
	virtual ~FadeColorSprite();

	// �J�n�F�ݒ�
	void SetBeginColor(Color color);
	// �ړI�F�ݒ�
	void SetEndColor(Color color);
	// �Đ�
	void Play(bool reverse = false, bool init = false);
	// ������
	void Reset(bool reverse);
	// �ꎞ��~
	void Pause();
	// �ꎞ��~����
	void Resume();
	// �ύX���Ԃ̐ݒ�
	void SetChangeTime(float time);
	// TimeScale�Ɉˑ����邩�ݒ�
	void DependTimeScale(bool depend);
	// Ease�̕ύX
	void ChangeEase(EaseType ease);

	CPPEvent<>* GetOnEndPlayReverse() { return m_onEndPlayReverse; }
	CPPEvent<>* GetOnEndPlay() { return m_onEndPlay; }

#pragma endregion

private:
#pragma region conceal method

	virtual void Update();

	void FadeIn();
	void FadeOut();

	// �X�V���Ԃ̎擾
	float GetDelta();

#pragma endregion
};