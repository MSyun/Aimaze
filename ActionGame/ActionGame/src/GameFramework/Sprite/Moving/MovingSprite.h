// �ړ��X�v���C�g
// 2017. 1. 4	�v���O�����쐬
// 2017. 2.13	TimeScale�Ɉˑ����邩�ݒ�\��
// author		SyunMizuno


#pragma once


#include	"../../Obj/2D/Sprite.h"
#include	"../../framework/CPPEvent/CPPEvent.h"
#include	"../../framework/AnimationCurve/AnimationCurve.h"



class MovingSprite	:	public	Sprite {
private:
#pragma region variable

	Point3	m_BeginPos;		// �J�n���W
	Point3	m_EndPos;		// �ړI���W
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

	MovingSprite(
		Point3 begin = Point3(0.0f, 0.0f, 0.0f),
		Point3 end = Point3(100.0f, 100.0f, 0.0f),
		float time = 1.0f,
		bool reverse = false,
		bool awake = false);
	virtual ~MovingSprite();

	// �J�n���W�ݒ�
	void SetBeginPos(Point3 pos);
	void SetBeginPos(float x, float y, float z);
	// �ړI���W�ݒ�
	void SetEndPos(Point3 pos);
	void SetEndPos(float x, float y, float z);
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

protected:
#pragma region conceal method

	virtual void Update();

	void FadeIn();
	void FadeOut();

	// �X�V���Ԃ̎擾
	float GetDelta();

#pragma endregion
};