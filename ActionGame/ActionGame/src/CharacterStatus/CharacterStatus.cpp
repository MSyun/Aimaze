// ステータス
// 2017. 1. 3	プログラム作成
// Author		SyunMizuno



#include	"CharacterStatus.h"
#include	"../GameFramework/framework/Utility/Math/MathUtility.h"
#include	"../GameFramework/framework/Debug/Debug.h"


/*									//
//			コンストラクタ			//
//									*/
CharacterStatus::CharacterStatus() :
	m_nMaxLife(30),
	m_nLife(30),
	m_nPower(3)
{
}


/*									//
//			デストラクタ			//
//									*/
CharacterStatus::~CharacterStatus() {

}

#pragma region Life
/*									//
//			最大体力設定			//
//									*/
void CharacterStatus::SetMaxLife(int max) {
	m_nMaxLife = max;
	m_nLife = Clamp(m_nLife, 0, m_nMaxLife);
}


/*									//
//			最大体力取得			//
//									*/
int CharacterStatus::GetMaxLife() {
	return m_nMaxLife;
}


/*									//
//				体力設定			//
//									*/
void CharacterStatus::SetLife(int life) {
	m_nLife = life;
}


/*									//
//				体力取得			//
//									*/
int CharacterStatus::GetLife() {
	return m_nLife;
}
#pragma endregion

#pragma region Power
/*									//
//				攻撃力設定			//
//									*/
void CharacterStatus::SetPower(int power) {
	m_nPower = power;
}


/*									//
//				攻撃力取得			//
//									*/
int CharacterStatus::GetPower() {
	return m_nPower;
}
#pragma endregion

/*									//
//				ダメージ			//
//									*/
void CharacterStatus::Damage(int attack) {
	if (attack < 0) {
		Debug::Log("攻撃力がマイナス値です");
		return;
	}

	m_nLife -= attack;
	if (m_nLife < 0)
		m_nLife = 0;
}


/*									//
//				回復				//
//									*/
void CharacterStatus::Recovery(int heal) {
	if (heal < 0) {
		Debug::Log("回復がマイナス値です");
		return;
	}

	m_nLife += heal;
	if (m_nLife > m_nMaxLife)
		m_nLife = m_nMaxLife;
}