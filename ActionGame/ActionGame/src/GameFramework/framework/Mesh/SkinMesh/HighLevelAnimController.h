// 2015.11.23	�v���O�����쐬
// 2016. 4.19	�A�j���[�V�������ł����s�ł���悤�֐��ǉ�
// 2016. 8.20	�{�������ǉ�
// 2016. 8.28	���[�v�t���O�ǉ�
// 2017. 1.17	�A�j���[�V�����J�n�E�I���C�x���g�ǉ�
// 2017. 1.23	�C�x���g�ǉ�
// author		SyunMizuno

#pragma once

#include	<d3dx9.h>
#include	<vector>
#include	<string>
#include	"../../CPPEvent/CPPEvent.h"
using namespace std;



/*
	memo :
			�]�T������΃C�x���g�ł͂Ȃ��N���X�ɋ@�\��u�����������������H
*/




// �A�j���[�V������ʃ��x���\����
struct HLANIMATION_DESC {
	UINT	uiAnimID;				// �A�j���[�V����ID
	LPCSTR	Name;					// �A�j���[�V������
	ID3DXAnimationSet*	pAnimSet;	// �A�j���[�V�����Z�b�g
	FLOAT	fLoopTime;				// 1���[�v�̔{��
	FLOAT	fTrackSpeed;			// �g���b�N�X�s�[�h�����l
	FLOAT	fShiftTime;				// �V�t�g����̂ɂ����鎞��
	FLOAT	fCurWeightTime;			// ���݂̃E�F�C�g����
	bool	bLoop;
	double	dPeriodTime;			// �A�j���[�V�����I���t���[��
};

// �A�j���[�V������ʃ��x���C���^�[�t�F�C�X
interface IHighLevelAnimController {
public:
	// �A�j���[�V�����R���g���[����ݒ�
	bool SetAnimCont(ID3DXAnimationController* pAnimCont);

	// �A�j���[�V�����R���g���[�����擾
	bool GetAnimCont(ID3DXAnimationController** ppAnimCont);

	// ���[�v���Ԃ�ݒ�
	bool SetLoopTime(UINT animID, FLOAT time);

	// ����ɂ����鎞�Ԃ�ݒ�
	bool SetShiftTime(UINT animID, FLOAT interval);

	// �A�j���[�V������؂�ւ�
	bool ChangeAnim(UINT animID);

	// �A�j���[�V�������X�V
	bool AdvanceTime(FLOAT time);

	// �A�j���[�V������1�񃋁[�v
	bool OneLoopCheck();

	// ���[�v�m�F�ݒ�
	bool SetLoop(UINT animID, bool loop);

	// �I���t���[���ݒ�
	bool SetPeriod(UINT animID, double period);
};

class HighLevelAnimController : public	IHighLevelAnimController {
private:
#pragma region variable

	vector<HLANIMATION_DESC*>	m_Anim;				// �A�j���[�V�����\���̔z��
	LPD3DXANIMATIONCONTROLLER	m_pAnimCont;		// �R���g���[���[
	UINT						m_CurAnimID;		// ���݂̃A�j���[�V����
	UINT						m_PreAnimID;		// �O��̃A�j���[�V����

	CPPEvent<>*					m_onAnyAnimState;	// �ǂ̃X�e�[�g����ł��Ăׂ�C�x���g
	vector<CPPEvent<>*>			m_onEnterAnimState;	// ���g�̃X�e�[�g�̊J�n�C�x���g
	vector<CPPEvent<>*>			m_onStartAnimState;	// �A�j���[�V�����J�n�C�x���g
	vector<CPPEvent<double>*>	m_onUpdateAnimState;// �A�j���[�V�����X�V�C�x���g
	vector<CPPEvent<>*>			m_onEndAnimState;	// �A�j���[�V�����I���C�x���g
	vector<CPPEvent<>*>			m_onExitAnimState;	// ���g�̃X�e�[�g�̏I���C�x���g

#pragma endregion

public:
#pragma region method

	HighLevelAnimController();		// �R���X�g���N�^
	~HighLevelAnimController();	// �f�X�g���N�^
	void Release();

	// �A�j���[�V�����R���g���[����ݒ�
	bool SetAnimCont(ID3DXAnimationController *pAnimCont);

	// �A�j���[�V�����R���g���[�����擾
	bool GetAnimCont(ID3DXAnimationController **ppAnimCont);

	// ���[�v���Ԃ�ݒ�
	bool SetLoopTime(UINT animID, FLOAT time);
	bool SetLoopTime(string name, FLOAT time);

	// ����ɂ����鎞�Ԃ�ݒ�
	bool SetShiftTime(UINT animID, FLOAT interval);
	bool SetShiftTime(string name, FLOAT interval);

	// �A�j���[�V������؂�ւ�
	bool ChangeAnim(UINT animID);
	bool ChangeAnim(string name);

	// �A�j���[�V�������X�V
	bool AdvanceTime(FLOAT time);

	// �A�j���[�V������1�񃋁[�v
	bool OneLoopCheck();

	// ���[�v�m�F�ݒ�
	bool SetLoop(UINT animID, bool loop);
	bool SetLoop(string name, bool loop);

	// �I���t���[���ݒ�
	bool SetPeriod(UINT animID, double period);
	bool SetPeriod(string name, double period);

#pragma region Event
	// �ǂ̃X�e�[�g�ł��Ă΂��C�x���g
	CPPEvent<>* GetOnAnyState();
	// �X�e�[�g�J�n�C�x���g
	CPPEvent<>* GetOnEnterAnim(UINT animID);
	CPPEvent<>* GetOnEnterAnim(string name);
	// �A�j���[�V�����J�n�C�x���g
	CPPEvent<>* GetOnStartAnim(UINT animID);
	CPPEvent<>* GetOnStartAnim(string name);
	// �A�j���[�V�����X�V�C�x���g
	CPPEvent<double>* GetOnUpdateAnim(UINT animID);
	CPPEvent<double>* GetOnUpdateAnim(string name);
	// �A�j���[�V�����I���C�x���g
	CPPEvent<>* GetOnEndAnim(UINT animID);
	CPPEvent<>* GetOnEndAnim(string name);
	// �X�e�[�g�I���C�x���g
	CPPEvent<>* GetOnExitAnim(UINT animID);
	CPPEvent<>* GetOnExitAnim(string name);
#pragma endregion

#pragma endregion
};