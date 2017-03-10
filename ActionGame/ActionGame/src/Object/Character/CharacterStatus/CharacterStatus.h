// ステータス
// 2017. 1. 3	プログラム作成
// Author		SyunMizuno


#pragma once



class CharacterStatus {
private:
#pragma region variable

	int		m_nMaxLife;	//	体力最大値
	int		m_nLife;	// 体力
	int		m_nPower;	// 攻撃力

#pragma endregion

public:
#pragma region method

	CharacterStatus();
	virtual ~CharacterStatus();

#pragma region Life
	// 最大体力設定
	void SetMaxLife(int max);
	// 最大体力取得
	int GetMaxLife();
	// 体力設定
	void SetLife(int life);
	// 体力取得
	int GetLife();
#pragma endregion

#pragma region Power
	// 攻撃力設定
	void SetPower(int power);
	// 攻撃力取得
	int GetPower();
#pragma endregion

	// ダメージ
	void Damage(int attack);
	// 回復
	void Recovery(int heal);

#pragma endregion

private:
#pragma region conceal method



#pragma endregion
};