// ���@�g���p�A�j���[�^�[
// 2017. 1.24	�v���O�����쐬
// Author		SyunMizuno

#include	"WolfAnimator.h"


/*									//
//			�R���X�g���N�^			//
//									*/
WolfAnimator::WolfAnimator() :
	m_bWalk(false),
	m_tAttack(false)
{
	AddBool("Walk", &m_bWalk);
	AddTrigger("Attack", &m_tAttack);
}


/*									//
//			�f�X�g���N�^			//
//									*/
WolfAnimator::~WolfAnimator() {

}


/*									//
//			�J�ڎ��Ԑݒ�			//
//									*/
void WolfAnimator::ShiftTime() {
	m_Controller->SetShiftTime("Idle", 0.1f);
	m_Controller->SetShiftTime("Threat", 0.2f);
	m_Controller->SetShiftTime("Attack", 0.1f);
	m_Controller->SetShiftTime("Walk", 0.1f);
}


/*									//
//		�A�j���[�V���������x�ݒ�	//
//									*/
void WolfAnimator::LoopTime() {
	m_Controller->SetLoopTime("Idle", 1.0f);
	m_Controller->SetLoopTime("Threat", 1.0f);
	m_Controller->SetLoopTime("Attack", 2.0f);
	m_Controller->SetLoopTime("Walk", 2.0f);
}


/*									//
//			���[�v�ېݒ�			//
//									*/
void WolfAnimator::Loop() {
	m_Controller->SetLoop("Idle", true);
	m_Controller->SetLoop("Threat", false);
	m_Controller->SetLoop("Attack", false);
	m_Controller->SetLoop("Walk", true);
}


/*									//
//			�I���t���[���ݒ�		//
//									*/
void WolfAnimator::Period() {
	//m_Controller->SetPeriod("Idle", 50.0);
	//m_Controller->SetPeriod("Threat", 50.0);
	m_Controller->SetPeriod("Attack", 52.0);
//	m_Controller->SetPeriod("Walk", 50.0);
}


/*									//
//			�X�e�[�g�ݒ�			//
//									*/
void WolfAnimator::State() {
	m_Controller->GetOnUpdateAnim("Idle")->AddListener([&](double time) {
		if (m_tAttack) {
			m_tAttack = false;
			m_Controller->ChangeAnim("Threat");
		}
		if (m_bWalk) {
			m_Controller->ChangeAnim("Walk");
		}
	});

	m_Controller->GetOnUpdateAnim("Walk")->AddListener([&](double time) {
		if (!m_bWalk)
			m_Controller->ChangeAnim("Idle");
		if (m_tAttack) {
			m_tAttack = false;
			m_Controller->ChangeAnim("Threat");
		}
	});

	m_Controller->GetOnEndAnim("Threat")->AddListener([&]() {
		m_Controller->ChangeAnim("Attack");
	});

	m_Controller->GetOnEnterAnim("Attack")->AddListener([&]() {
		ChangeTime(0.5f, "Idle");
	});
}


/*									//
//			�����A�j���[�V����		//
//									*/
void WolfAnimator::StartState() {
	m_Controller->ChangeAnim("Idle");
}