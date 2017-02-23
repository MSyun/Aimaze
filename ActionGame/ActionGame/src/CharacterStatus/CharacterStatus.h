// �X�e�[�^�X
// 2017. 1. 3	�v���O�����쐬
// Author		SyunMizuno


#pragma once



class CharacterStatus {
private:
#pragma region variable

	int		m_nMaxLife;	//	�̗͍ő�l
	int		m_nLife;	// �̗�
	int		m_nPower;	// �U����

#pragma endregion

public:
#pragma region method

	CharacterStatus();
	virtual ~CharacterStatus();

#pragma region Life
	// �ő�̗͐ݒ�
	void SetMaxLife(int max);
	// �ő�̗͎擾
	int GetMaxLife();
	// �̗͐ݒ�
	void SetLife(int life);
	// �̗͎擾
	int GetLife();
#pragma endregion

#pragma region Power
	// �U���͐ݒ�
	void SetPower(int power);
	// �U���͎擾
	int GetPower();
#pragma endregion

	// �_���[�W
	void Damage(int attack);
	// ��
	void Recovery(int heal);

#pragma endregion

private:
#pragma region conceal method



#pragma endregion
};