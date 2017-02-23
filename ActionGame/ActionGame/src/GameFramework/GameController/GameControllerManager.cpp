//////////////////////////////////////////////////////////////
// GameControllerManager �N���X
// 2016. 9. 27	�v���O�����쐬
// author		SyunMizuno
//////////////////////////////////////////////////////////////



#include	"GameControllerManager.h"
#include	"../framework/Utility/System/SystemUtility.h"
#include	"../Save/SaveGameFactory.h"
#include	"../framework/Debug/Debug.h"


const TCHAR* SAVE_NAME = _T("Controller.dat");



/*									//
//			�R���X�g���N�^			//
//									*/
GameControllerManager::GameControllerManager() {
	m_vecCont.clear();
}


/*									//
//			�f�X�g���N�^			//
//									*/
GameControllerManager::~GameControllerManager() {
	for (auto it = m_vecCont.begin(); it != m_vecCont.end(); ++it) {
		if (*it) {
			delete (*it);
			(*it) = NULL;
		}
	}
}


/*									//
//		�R���g���[���[�쐬			//
//									*/
void GameControllerManager::Create(unsigned int num) {
	for (unsigned int i = 0; i < num; ++i) {
		// �ڑ����I�[�o�[
		if (m_vecCont.size() >= CONTROLLER_MAX_CREATE) {
			Debug::Log("GameControllerManager : �R���g���[���[�̐ڑ������z���܂���");
			return;
		}

		new GameController;
	}
}


/*									//
//		�R���g���[���[�ǉ�			//
//									*/
void GameControllerManager::Add(GameController* pobj) {
	for (auto it = m_vecCont.begin(); it != m_vecCont.end(); ++it) {
		// �o�^�ς�
		if ((*it) == pobj) {
			Debug::Log("�R���g���[���[�͂��łɓo�^�ς݂ł�");
			return;
		}
	}

	m_vecCont.push_back(pobj);
}


/*									//
//		�R���g���[���[�擾			//
//									*/
GameController* GameControllerManager::Get(unsigned int num) {
	if (num <= 0 || m_vecCont.size() < num) {
		Debug::LogError("���݂��Ȃ��R���g���[���[�ԍ����w�肳��܂���");
		return NULL;
	}

	return m_vecCont[num - 1];
}


/*									//
//				�Z�[�u				//
//									*/
void GameControllerManager::Save() {
	// ��O����
	if (m_vecCont.size() <= 0) {
		Debug::LogError("�R���g���[���[�����݂��Ȃ��̂ŕۑ��ł��܂���");
		return;
	}

	//CGameSaveManager mgr;
	//for (int i = 0; i < m_vecCont.size(); ++i) {
	//	mgr.AddSaveData(m_vecCont[i]);
	//}
	//mgr.Save(SAVE_NAME);
}