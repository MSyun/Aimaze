//////////////////////////////////////////////////////////////
// �Q�[���p�Z�[�u�}�l�[�W���[
// 2016.12.31	�v���O�����쐬
// author		SyunMizuno
//////////////////////////////////////////////////////////////


#include	"SaveGameManager.h"
#include	"SaveGameFactory.h"


/*									//
//			�t�@�N�g���֐�			//
//		�i�����͌p�����邱�ƑO��j	*/
ISaveObjBase* SaveGameManager::CreateObj(int classID) {
	return SaveGameFactory::Create(classID);	// �w��̃N���X���Ȃ�
}