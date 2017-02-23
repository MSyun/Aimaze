//////////////////////////////////////////////////////////////
// GameControllerManager �N���X
// 2016. 9. 27	�v���O�����쐬
// author		SyunMizuno
//////////////////////////////////////////////////////////////

#pragma once


#include	"GameController.h"
#include	"../framework/Singleton/Singleton.h"
#include	"../framework/Save/SaveObjBase.h"

#include	<vector>


// �ő吶����
#define		CONTROLLER_MAX_CREATE	( 1 )


class	GameControllerManager : public	Singleton<GameControllerManager> {
protected:
#pragma region variable

	// �R���g���[���[�z��(1�`4���Ǝv��
	vector< GameController* >	m_vecCont;

#pragma endregion

public:
#pragma region method

	GameControllerManager();
	virtual ~GameControllerManager();

	// �R���g���[���[�쐬
	void Create(unsigned int num = 1);

	// �R���g���[���[�ǉ�
	void Add(GameController* pobj);

	// �R���g���[���[�擾
	GameController* Get(unsigned int num = 1);

	void Save();

#pragma endregion
};

inline GameControllerManager* GetContManager() {
	return GameControllerManager::Instance();
}