//////////////////////////////////////////////////////////////
// GameControllerManager クラス
// 2016. 9. 27	プログラム作成
// author		SyunMizuno
//////////////////////////////////////////////////////////////



#include	"GameControllerManager.h"
#include	"../framework/Utility/System/SystemUtility.h"
#include	"../Save/SaveGameFactory.h"
#include	"../framework/Debug/Debug.h"


const TCHAR* SAVE_NAME = _T("Controller.dat");



/*									//
//			コンストラクタ			//
//									*/
GameControllerManager::GameControllerManager() {
	m_vecCont.clear();
}


/*									//
//			デストラクタ			//
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
//		コントローラー作成			//
//									*/
void GameControllerManager::Create(unsigned int num) {
	for (unsigned int i = 0; i < num; ++i) {
		// 接続数オーバー
		if (m_vecCont.size() >= CONTROLLER_MAX_CREATE) {
			Debug::Log("GameControllerManager : コントローラーの接続数を越えました");
			return;
		}

		new GameController;
	}
}


/*									//
//		コントローラー追加			//
//									*/
void GameControllerManager::Add(GameController* pobj) {
	for (auto it = m_vecCont.begin(); it != m_vecCont.end(); ++it) {
		// 登録済み
		if ((*it) == pobj) {
			Debug::Log("コントローラーはすでに登録済みです");
			return;
		}
	}

	m_vecCont.push_back(pobj);
}


/*									//
//		コントローラー取得			//
//									*/
GameController* GameControllerManager::Get(unsigned int num) {
	if (num <= 0 || m_vecCont.size() < num) {
		Debug::LogError("存在しないコントローラー番号を指定されました");
		return NULL;
	}

	return m_vecCont[num - 1];
}


/*									//
//				セーブ				//
//									*/
void GameControllerManager::Save() {
	// 例外処理
	if (m_vecCont.size() <= 0) {
		Debug::LogError("コントローラーが存在しないので保存できません");
		return;
	}

	//CGameSaveManager mgr;
	//for (int i = 0; i < m_vecCont.size(); ++i) {
	//	mgr.AddSaveData(m_vecCont[i]);
	//}
	//mgr.Save(SAVE_NAME);
}