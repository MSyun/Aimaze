// �h���S��
// 2017. 2. 6	�v���O�����쐬
// Author		SyunMizuno


#pragma once


#include	"../Enemy.h"
#include	"../../../../GameFramework/framework/CPPEvent/CPPEvent.h"
#include	"../../../Attack/DragonFlame/DragonFlame.h"
#include	"../../../Attack/DragonBreath/DragonBreath.h"
#include	"../../../Attack/DragonRush/DragonRush.h"
#include	"../../../../Object/Character/Enemy/BossInformation/BossInformation.h"
#include	"../../../Attack/DragonStamp/DragonStamp.h"


class Dragon	:	public	Enemy {
private:
#pragma region struct

	enum _attack {
		ATTACK_STAMP = 0,	// ���݂Ԃ��U��
		ATTACK_FLAME,		// �Ή�����
		ATTACK_BREATH,		// �΋�
		ATTACK_RUSH,		// �˂�����

		ATTACK_MAX
	};

#pragma endregion

#pragma region variable

	BossInformation*	m_pInfo;

	Delegate<>	m_ActionDelegate;

	// �x�e�֘A
	float	m_fIntervalCnt;

	bool	m_bAttack;	// �U�����i���x�𑁂����邽��
	bool	m_bStamp;	// ���݂Ԃ��U��
	bool	m_bBreath;	// �΋��g�p�t���O
	bool	m_bDeath;	// ���S�m�F
	bool	m_bClear;	// �N���A�����m�F

	float	m_fStampTime;	// ���݂Ԃ�����
	int		m_nStampState;
	float	m_fStampSpeed;

	DragonFlame*	m_pFlame;
	DragonBreath*	m_pBreath;
	DragonRush*		m_pRush;
	DragonStamp*	m_pStamp;

	int		m_nBreathCnt;

#pragma endregion

public:
	Dragon(int max);
	virtual ~Dragon();

	virtual void Damage(int attack);

private:
#pragma region conceal method

	virtual void Init();
	virtual void Update();

	void Action();

	// ���
	void NearAnalyze();
	void MiddleAnalyze();
	void FarAnalyze();

	// �U���֐�(�f���Q�[�g�ɓo�^���Ďg�p
	void AddListenAttack(_attack anim);
	void AttackStamp();
	void AttackBreath();
	void AttackFlame();
	void AttackRush();

	// �U�����m�F
	// true. �U���� : false. ��U����
	bool CheckAttack();

	// ���S���̐ݒ�
	void Death();

#pragma endregion
};