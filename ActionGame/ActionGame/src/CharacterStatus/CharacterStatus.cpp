// �X�e�[�^�X
// 2017. 1. 3	�v���O�����쐬
// Author		SyunMizuno



#include	"CharacterStatus.h"
#include	"../GameFramework/framework/Utility/Math/MathUtility.h"
#include	"../GameFramework/framework/Debug/Debug.h"


/*									//
//			�R���X�g���N�^			//
//									*/
CharacterStatus::CharacterStatus() :
	m_nMaxLife(30),
	m_nLife(30),
	m_nPower(3)
{
}


/*									//
//			�f�X�g���N�^			//
//									*/
CharacterStatus::~CharacterStatus() {

}

#pragma region Life
/*									//
//			�ő�̗͐ݒ�			//
//									*/
void CharacterStatus::SetMaxLife(int max) {
	m_nMaxLife = max;
	m_nLife = Clamp(m_nLife, 0, m_nMaxLife);
}


/*									//
//			�ő�̗͎擾			//
//									*/
int CharacterStatus::GetMaxLife() {
	return m_nMaxLife;
}


/*									//
//				�̗͐ݒ�			//
//									*/
void CharacterStatus::SetLife(int life) {
	m_nLife = life;
}


/*									//
//				�̗͎擾			//
//									*/
int CharacterStatus::GetLife() {
	return m_nLife;
}
#pragma endregion

#pragma region Power
/*									//
//				�U���͐ݒ�			//
//									*/
void CharacterStatus::SetPower(int power) {
	m_nPower = power;
}


/*									//
//				�U���͎擾			//
//									*/
int CharacterStatus::GetPower() {
	return m_nPower;
}
#pragma endregion

/*									//
//				�_���[�W			//
//									*/
void CharacterStatus::Damage(int attack) {
	if (attack < 0) {
		Debug::Log("�U���͂��}�C�i�X�l�ł�");
		return;
	}

	m_nLife -= attack;
	if (m_nLife < 0)
		m_nLife = 0;
}


/*									//
//				��				//
//									*/
void CharacterStatus::Recovery(int heal) {
	if (heal < 0) {
		Debug::Log("�񕜂��}�C�i�X�l�ł�");
		return;
	}

	m_nLife += heal;
	if (m_nLife > m_nMaxLife)
		m_nLife = m_nMaxLife;
}