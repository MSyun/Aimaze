// プレイヤー用アニメーター
// 2017. 1.23	プログラム作成
// Author		SyunMizuno


#include	"PlayerAnimator.h"



/*									//
//			コンストラクタ			//
//									*/
PlayerAnimator::PlayerAnimator() :
	m_bWeapon(false),
	m_bRun(false),
	m_tHit(false),
	m_tAttack(false)
{
	AddBool("Weapon", &m_bWeapon);
	AddBool("Run", &m_bRun);
	AddTrigger("Hit", &m_tHit);
	AddTrigger("Attack", &m_tAttack);
	AddTrigger("Death", &m_tDeath);
}


/*									//
//			デストラクタ			//
//									*/
PlayerAnimator::~PlayerAnimator() {

}


/*									//
//			遷移時間設定			//
//									*/
void PlayerAnimator::ShiftTime() {
	m_Controller->SetShiftTime("Idle", 0.05f);
	m_Controller->SetShiftTime("Run_Weapon", 0.05f);
	m_Controller->SetShiftTime("Run", 0.05f);
	m_Controller->SetShiftTime("Swing1", 0.0f);
	m_Controller->SetShiftTime("Swing2", 0.0f);
	m_Controller->SetShiftTime("Death", 0.0f);
	m_Controller->SetShiftTime("KnockBack", 0.05f);
}


/*									//
//		アニメーション加速度設定	//
//									*/
void PlayerAnimator::LoopTime() {
	m_Controller->SetLoopTime("Idle", 1.0f);
	m_Controller->SetLoopTime("Run_Weapon", 2.0f);
	m_Controller->SetLoopTime("Run", 2.0f);
	m_Controller->SetLoopTime("Swing1", 2.0f);
	m_Controller->SetLoopTime("Swing2", 3.0f);
	m_Controller->SetLoopTime("Death", 0.7f);
	m_Controller->SetLoopTime("KnockBack", 1.0f);
}


/*									//
//			ループ可否設定			//
//									*/
void PlayerAnimator::Loop() {
	m_Controller->SetLoop("Idle", true);
	m_Controller->SetLoop("Run_Weapon", true);
	m_Controller->SetLoop("Run", true);
	m_Controller->SetLoop("Swing1", false);
	m_Controller->SetLoop("Swing2", false);
	m_Controller->SetLoop("Death", false);
	m_Controller->SetLoop("KnockBack", false);
}


/*									//
//			終了フレーム設定		//
//									*/
void PlayerAnimator::Period() {
	m_Controller->SetPeriod("Swing1", 50.0);
	m_Controller->SetPeriod("Swing2", 50.0);
	m_Controller->SetPeriod("Death", 59.0);
	m_Controller->SetPeriod("KnockBack", 30.0);
}


/*									//
//			ステート設定			//
//									*/
void PlayerAnimator::State() {
	m_Controller->GetOnUpdateAnim("Idle")->AddListener([&](double time) {
		if (m_bRun) {
			if (m_bWeapon)
				m_Controller->ChangeAnim("Run_Weapon");
			else
				m_Controller->ChangeAnim("Run");
		}
		if (m_bWeapon && m_tAttack) {
			m_tAttack = false;
			m_Controller->ChangeAnim("Swing1");
		}
	});

	m_Controller->GetOnUpdateAnim("Run")->AddListener([&](double time) {
		if (!m_bRun) {
			m_Controller->ChangeAnim("Idle");
		}
		if (m_bWeapon) {
			m_Controller->ChangeAnim("Run_Weapon");
		}
	});

	m_Controller->GetOnUpdateAnim("Run_Weapon")->AddListener([&](double time) {
		if (m_tAttack) {
			m_tAttack = false;
			m_Controller->ChangeAnim("Swing1");
		}
		if (!m_bRun) {
			m_Controller->ChangeAnim("Idle");
		}
		if (!m_bWeapon) {
			m_Controller->ChangeAnim("Run");
		}
	});

	m_Controller->GetOnEndAnim("Swing1")->AddListener([&]() {
		if (m_tAttack) {
			m_tAttack = false;
			m_Controller->ChangeAnim("Swing2");
		} else {
			if (m_bRun)
				m_Controller->ChangeAnim("Run_Weapon");
			else
				m_Controller->ChangeAnim("Idle");
		}
	});

	m_Controller->GetOnEndAnim("Swing2")->AddListener([&]() {
		if (m_bRun)
			m_Controller->ChangeAnim("Run_Weapon");
		else
			m_Controller->ChangeAnim("Idle");
	});

	m_Controller->GetOnEndAnim("KnockBack")->AddListener([&]() {
		if (m_bRun) {
			if (m_bWeapon)
				m_Controller->ChangeAnim("Run_Weapon");
			else
				m_Controller->ChangeAnim("Run");
		} else {
			m_Controller->ChangeAnim("Idle");
		}
	});

	m_Controller->GetOnAnyState()->AddListener([&]() {
		if (m_tHit) {
			m_Controller->ChangeAnim("KnockBack");
			m_tHit = false;
		}
		if (m_tDeath) {
			m_Controller->ChangeAnim("Death");
			m_tDeath = false;
		}
	});
}


/*									//
//			初期アニメーション		//
//									*/
void PlayerAnimator::StartState() {
	m_Controller->ChangeAnim("Idle");
}