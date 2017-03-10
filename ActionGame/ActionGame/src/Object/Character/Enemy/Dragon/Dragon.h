// ドラゴン
// 2017. 2. 6	プログラム作成
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
		ATTACK_STAMP = 0,	// 踏みつぶし攻撃
		ATTACK_FLAME,		// 火炎放射
		ATTACK_BREATH,		// 火球
		ATTACK_RUSH,		// 突っ込み

		ATTACK_MAX
	};

#pragma endregion

#pragma region variable

	BossInformation*	m_pInfo;

	Delegate<>	m_ActionDelegate;

	// 休憩関連
	float	m_fIntervalCnt;

	bool	m_bAttack;	// 攻撃中（速度を早くするため
	bool	m_bStamp;	// 踏みつぶし攻撃
	bool	m_bBreath;	// 火球使用フラグ
	bool	m_bDeath;	// 死亡確認
	bool	m_bClear;	// クリア生成確認

	float	m_fStampTime;	// 踏みつぶし時間
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

	// 解析
	void NearAnalyze();
	void MiddleAnalyze();
	void FarAnalyze();

	// 攻撃関数(デリゲートに登録して使用
	void AddListenAttack(_attack anim);
	void AttackStamp();
	void AttackBreath();
	void AttackFlame();
	void AttackRush();

	// 攻撃中確認
	// true. 攻撃中 : false. 非攻撃中
	bool CheckAttack();

	// 死亡時の設定
	void Death();

#pragma endregion
};