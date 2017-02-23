// ドラゴン用アニメーター
// 2017. 1.25	プログラム作成
// Author		SyunMizuno


#include	"DragonAnimator.h"



/*									//
//			コンストラクタ			//
//									*/
DragonAnimator::DragonAnimator() :
	m_bWalk(false),
	m_nAttack(-1),
	m_nRotate(-1),
	m_tHit(false),
	m_tPlay(false),
	m_bWing(false),
	m_tDeath(false)
{
	AddBool("Walk", &m_bWalk);
	AddInt("Attack", &m_nAttack);
	AddInt("Rotate", &m_nRotate);
	AddTrigger("Hit", &m_tHit);
	AddTrigger("Play", &m_tPlay);
	AddBool("Wing", &m_bWing);
	AddTrigger("Death", &m_tDeath);
}


/*									//
//			デストラクタ			//
//									*/
DragonAnimator::~DragonAnimator() {

}


/*									//
//			遷移時間設定			//
//									*/
void DragonAnimator::ShiftTime() {
	m_Controller->SetShiftTime("Idle", 0.5f);
	//m_Controller->SetShiftTime("Walk", 0.2f);
	//m_Controller->SetShiftTime("Rotate_R", 0.2f);
	//m_Controller->SetShiftTime("Rotate_L", 0.2f);
	//m_Controller->SetShiftTime("Tail", 0.2f);
	m_Controller->SetShiftTime("Wing", 0.5f);
	m_Controller->SetShiftTime("Glide", 0.2f);
	//m_Controller->SetShiftTime("Slash", 0.2f);
	m_Controller->SetShiftTime("Breath", 0.2f);
	m_Controller->SetShiftTime("Flame", 0.2f);
	m_Controller->SetShiftTime("KnockBack", 0.2f);
	//m_Controller->SetShiftTime("Play", 0.2f);
	m_Controller->SetShiftTime("Death", 0.2f);
	m_Controller->SetShiftTime("Rush", 0.2f);
}


/*									//
//		アニメーション加速度設定	//
//									*/
void DragonAnimator::LoopTime() {
	m_Controller->SetLoopTime("Idle", 1.0f);
	//m_Controller->SetLoopTime("Walk", 0.2f);
	//m_Controller->SetLoopTime("Rotate_R", 0.2f);
	//m_Controller->SetLoopTime("Rotate_L", 0.2f);
	//m_Controller->SetLoopTime("Tail", 0.2f);
	m_Controller->SetLoopTime("Wing", 1.0f);
	m_Controller->SetLoopTime("Glide", 0.7f);
	//m_Controller->SetLoopTime("Slash", 0.2f);
	m_Controller->SetLoopTime("Breath", 1.0f);
	m_Controller->SetLoopTime("Flame", 0.2f);
	m_Controller->SetLoopTime("KnockBack", 0.6f);
	//m_Controller->SetLoopTime("Play", 0.2f);
	m_Controller->SetLoopTime("Death", 0.5f);
	m_Controller->SetLoopTime("Rush", 1.0f);
}


/*									//
//			ループ可否設定			//
//									*/
void DragonAnimator::Loop() {
	m_Controller->SetLoop("Idle", true);
	//m_Controller->SetLoop("Walk", true);
	//m_Controller->SetLoop("Rotate_R", false);
	//m_Controller->SetLoop("Rotate_L", false);
	//m_Controller->SetLoop("Tail", false);
	m_Controller->SetLoop("Wing", true);
	m_Controller->SetLoop("Glide", true);
	//m_Controller->SetLoop("Slash", false);
	m_Controller->SetLoop("Breath", false);
	m_Controller->SetLoop("Flame", false);
	m_Controller->SetLoop("KnockBack", false);
	//m_Controller->SetLoop("Play", false);
	m_Controller->SetLoop("Death", false);
	m_Controller->SetLoop("Rush", false);
}


/*									//
//			終了フレーム設定		//
//									*/
void DragonAnimator::Period() {
	//m_Controller->SetPeriod("Idle", 50.0);
	//m_Controller->SetPeriod("Walk", 50.0);
	//m_Controller->SetPeriod("Rotate_R", 50.0);
	//m_Controller->SetPeriod("Rotate_L", 50.0);
	//m_Controller->SetPeriod("Tail", 50.0);
	//m_Controller->SetPeriod("Wing", 50.0);
	//m_Controller->SetPeriod("Glide", 50.0);
	//m_Controller->SetPeriod("Slash", 50.0);
	m_Controller->SetPeriod("Breath", 50.0);
	m_Controller->SetPeriod("Flame", 45.0);
	//m_Controller->SetPeriod("KnockBack", 50.0);
	//m_Controller->SetPeriod("Play", 50.0);
	m_Controller->SetPeriod("Death", 59.0);
	m_Controller->SetPeriod("Rush", 57.0);
}


/*									//
//			ステート設定			//
//									*/
void DragonAnimator::State() {
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
	m_Controller->GetOnUpdateAnim("Idle")->AddListener([&](double time) {
		if (m_bWalk) {
			m_Controller->ChangeAnim("Walk");
		}
		if (m_nAttack != -1) {
			AttackPack();
		}
		if (m_nRotate != -1) {
			RotatePack();
		}
		if (m_tPlay) {
			m_Controller->ChangeAnim("Play");
			m_tPlay = false;
		}
		if (m_bWing) {
			m_Controller->ChangeAnim("Wing");
		}
	});
	//m_Controller->GetOnUpdateAnim("Walk")->AddListener([&]() {
	//	if (!m_bWalk) {
	//		m_Controller->ChangeAnim("Idle");
	//	}
	//	if (m_nAttack != -1) {
	//		AttackPack();
	//	}
	//	if (m_nRotate != -1) {
	//		RotatePack();
	//	}
	//});
	//m_Controller->GetOnEndAnim("Rotate_R")->AddListener([&]() {
	//	if (m_bWalk)
	//		m_Controller->ChangeAnim("Walk");
	//	else
	//		m_Controller->ChangeAnim("Idle");
	//});
	//m_Controller->GetOnEndAnim("Rotate_L")->AddListener([&]() {
	//	if (m_bWalk)
	//		m_Controller->ChangeAnim("Walk");
	//	else
	//		m_Controller->ChangeAnim("Idle");
	//});
	//m_Controller->GetOnEndAnim("Tail")->AddListener([&]() {
	//	if (m_bWalk)
	//		m_Controller->ChangeAnim("Walk");
	//	else
	//		m_Controller->ChangeAnim("Idle");
	//});
	//m_Controller->GetOnEndAnim("Slash")->AddListener([&]() {
	//	if (m_bWalk)
	//		m_Controller->ChangeAnim("Walk");
	//	else
	//		m_Controller->ChangeAnim("Idle");
	//});
	m_Controller->GetOnEndAnim("Breath")->AddListener([&]() {
		m_nAttack = -1;
		if (m_bWalk)
			m_Controller->ChangeAnim("Walk");
		else
			m_Controller->ChangeAnim("Idle");
	});
	m_Controller->GetOnEndAnim("Flame")->AddListener([&]() {
		m_nAttack = -1;
		if (m_bWalk)
			m_Controller->ChangeAnim("Walk");
		else
			m_Controller->ChangeAnim("Idle");
	});
	m_Controller->GetOnUpdateAnim("Wing")->AddListener([&](double time) {
		if (!m_bWing) {
			m_Controller->ChangeAnim("Idle");
		}
		if (m_bWalk) {
			m_Controller->ChangeAnim("Glide");
		}
	});
	m_Controller->GetOnUpdateAnim("Glide")->AddListener([&](double time) {
		if (!m_bWalk) {
			m_Controller->ChangeAnim("Wing");
		}
	});
	m_Controller->GetOnEndAnim("KnockBack")->AddListener([&]() {
		m_Controller->ChangeAnim("Idle");
		m_tHit = false;
	});
	m_Controller->GetOnExitAnim("KnockBack")->AddListener([&]() {
		m_tHit = false;
	});
	//m_Controller->GetOnEndAnim("Play")->AddListener([&]() {
	//	m_Controller->ChangeAnim("Idle");
	//});
	m_Controller->GetOnEndAnim("Rush")->AddListener([&]() {
		ChangeTime(1.0f, "Idle");
	});
	m_Controller->GetOnExitAnim("Rush")->AddListener([&]() {
		m_nAttack = -1;
	});
}


/*									//
//			初期アニメーション		//
//									*/
void DragonAnimator::StartState() {
	m_Controller->ChangeAnim("Idle");
}


/*									//
//				攻撃分木			//
//									*/
void DragonAnimator::AttackPack() {
	switch (m_nAttack) {
	case 0:
//		m_Controller->ChangeAnim("Tail");
		break;

	case 1:
//		m_Controller->ChangeAnim("Slash");
		m_Controller->ChangeAnim("Rush");
		break;

	case 2:
		m_Controller->ChangeAnim("Breath");
		break;

	case 3:
		m_Controller->ChangeAnim("Flame");
		break;
	}
}


/*									//
//				回転分木			//
//									*/
void DragonAnimator::RotatePack() {
	switch (m_nRotate) {
	case 0:
		m_Controller->ChangeAnim("Rotate_R");
		break;

	case 1:
		m_Controller->ChangeAnim("Rotate_L");
		break;
	}
}