// ���@�g���p�A�j���[�^�[
// 2017. 1.24	�v���O�����쐬
// Author		SyunMizuno



#include	"WizardAnimator.h"




/*									//
//			�R���X�g���N�^			//
//									*/
WizardAnimator::WizardAnimator() :
	m_bWalk(false),
	m_tAttack(false)
{
	AddBool("Walk", &m_bWalk);
	AddTrigger("Attack", &m_tAttack);
}


/*									//
//			�f�X�g���N�^			//
//									*/
WizardAnimator::~WizardAnimator() {

}


/*									//
//			�J�ڎ��Ԑݒ�			//
//									*/
void WizardAnimator::ShiftTime() {
	m_Controller->SetShiftTime("Idle", 0.2f);
	m_Controller->SetShiftTime("Attack", 0.2f);
	m_Controller->SetShiftTime("Threat", 0.2f);
}


/*									//
//		�A�j���[�V���������x�ݒ�	//
//									*/
void WizardAnimator::LoopTime() {
	m_Controller->SetLoopTime("Idle", 0.5f);
	m_Controller->SetLoopTime("Attack", 1.0f);
	m_Controller->SetLoopTime("Threat", 1.0f);
}


/*									//
//			���[�v�ېݒ�			//
//									*/
void WizardAnimator::Loop() {
	m_Controller->SetLoop("Idle", true);
	m_Controller->SetLoop("Attack", false);
	m_Controller->SetLoop("Threat", false);
}


/*									//
//			�I���t���[���ݒ�		//
//									*/
void WizardAnimator::Period() {
	//m_Controller->SetPeriod("Idle", 50.0);
	m_Controller->SetPeriod("Attack", 50.0);
	m_Controller->SetPeriod("Threat", 53.0);
}


/*									//
//			�X�e�[�g�ݒ�			//
//									*/
void WizardAnimator::State() {
	m_Controller->GetOnUpdateAnim("Idle")->AddListener([&](double time) {
		if (m_tAttack) {
			m_tAttack = false;
			m_Controller->ChangeAnim("Threat");
		}
	});

	m_Controller->GetOnEndAnim("Threat")->AddListener([&]() {
		ChangeTime(2.0f, "Attack");
	});

	m_Controller->GetOnEndAnim("Attack")->AddListener([&]() {
		ChangeTime(1.0f, "Idle");
	});
}


/*									//
//			�����A�j���[�V����		//
//									*/
void WizardAnimator::StartState() {
	m_Controller->ChangeAnim("Idle");
}