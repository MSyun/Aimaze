//////////////////////////////////////////////////////////////
// �Q�[���p�Z�[�u�}�l�[�W���[
// 2016.12.31	�v���O�����쐬
// author		SyunMizuno
//////////////////////////////////////////////////////////////

#pragma once


#include	"../framework/Save/SaveManagerBase.h"


class SaveGameManager	:	public	SaveManagerBase {
public:
	SaveGameManager() {};
	virtual ~SaveGameManager() {};

private:
	virtual ISaveObjBase* CreateObj(int classID);
};