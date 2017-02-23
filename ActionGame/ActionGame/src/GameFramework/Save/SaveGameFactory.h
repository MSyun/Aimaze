//////////////////////////////////////////////////////////////
// �Z�[�u�p�t�@�N�g���N���X(�Q�[���p)
// 2016. 12. 31	�v���O�����쐬
// author		SyunMizuno
//////////////////////////////////////////////////////////////

#pragma once


#include	"../framework/Save/SaveFactory.h"


typedef enum {
	SAVE_GAME_FACT_CONTROLLER = SAVE_FACT_MAX,
	SAVE_GAME_FACT_CONTROLLERMANAGER,

	SAVE_GAME_FACT_MAX,
}_SAVE_GAME_FACTORY;


class SaveGameFactory :	public	SaveFactory{
public:
	SaveGameFactory() {};
	virtual ~SaveGameFactory() {};
	static ISaveObjBase* Create(int classID);
};