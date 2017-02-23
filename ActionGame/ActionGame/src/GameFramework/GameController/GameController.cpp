//////////////////////////////////////////////////////////////
// ゲームコントローラークラス
// 2016. 8.12	プログラム作成
// author		SyunMizuno
//////////////////////////////////////////////////////////////


#include	"GameController.h"
#include	"../Save/SaveGameFactory.h"			// IDが欲しいため
#include	"GameControllerManager.h"
#include	"../framework/Debug/Debug.h"


/*
	memo	:
			列挙ではゲームで使用するボタンを宣言すること
*/


//----- セーブしたいオブジェクト
DATARECORD GameController::m_DataRecord[] = {
	DATA_LOCAL(GameController, m_tKeyInfo[0].JoyNum),
	DATA_LOCAL(GameController, m_tKeyInfo[1].JoyNum),
	DATA_LOCAL(GameController, m_tKeyInfo[2].JoyNum),
	DATA_LOCAL(GameController, m_tKeyInfo[3].JoyNum),
	DATA_LOCAL(GameController, m_tKeyInfo[4].JoyNum),
	DATA_LOCAL(GameController, m_tKeyInfo[5].JoyNum),
	DATA_LOCAL(GameController, m_tKeyInfo[6].JoyNum),
	DATA_LOCAL(GameController, m_tKeyInfo[7].JoyNum),
	DATA_LOCAL(GameController, m_tKeyInfo[8].JoyNum),
	DATA_LOCAL(GameController, m_tKeyInfo[9].JoyNum),
	DATA_LOCAL(GameController, m_tKeyInfo[10].JoyNum),
	DATA_LOCAL(GameController, m_tKeyInfo[11].JoyNum),
	DATA_LOCAL(GameController, m_tKeyInfo[12].JoyNum),
	DATA_LOCAL(GameController, m_tKeyInfo[0].KeyNum),
	DATA_LOCAL(GameController, m_tKeyInfo[1].KeyNum),
	DATA_LOCAL(GameController, m_tKeyInfo[2].KeyNum),
	DATA_LOCAL(GameController, m_tKeyInfo[3].KeyNum),
	DATA_LOCAL(GameController, m_tKeyInfo[4].KeyNum),
	DATA_LOCAL(GameController, m_tKeyInfo[5].KeyNum),
	DATA_LOCAL(GameController, m_tKeyInfo[6].KeyNum),
	DATA_LOCAL(GameController, m_tKeyInfo[7].KeyNum),
	DATA_LOCAL(GameController, m_tKeyInfo[8].KeyNum),
	DATA_LOCAL(GameController, m_tKeyInfo[9].KeyNum),
	DATA_LOCAL(GameController, m_tKeyInfo[10].KeyNum),
	DATA_LOCAL(GameController, m_tKeyInfo[11].KeyNum),
	DATA_LOCAL(GameController, m_tKeyInfo[12].KeyNum),
	DATA_END
};



/*									//
//			コンストラクタ			//
//									*/
GameController::GameController() {
	Reset();
	// 生成されたらマネージャがあれば登録
	if (GetContManager())
		GetContManager()->Add(this);
}


/*									//
//			デストラクタ			//
//									*/
GameController::~GameController() {
}


/*									//
//				リセット			//
//									*/
void GameController::Reset() {
	SetDefaultKey();
	SetDefaultJoy();
}


/*									//
//				データ取得			//
//									*/
GameController::_button GameController::GetKeyInfo(int _key) {
	return m_tKeyInfo[_key];
}


/*									//
//		対応したキー番号取得		//
//									*/
int GameController::GetKeyNum(int _button) {
	if (_button < 0 || BUTTON_MAX <= _button) {
		Debug::LogError("存在しないキーが投げられました");
		return -1;
	}

	return GetKeyInfo(_button).KeyNum;
}


/*									//
//		対応したジョイ番号取得		//
//									*/
int GameController::GetJoyNum(int _button) {
	if (_button < 0 || BUTTON_MAX <= _button) {
		Debug::LogError("存在しないキーが投げられました");
		return -1;
	}
	return GetKeyInfo(_button).JoyNum;
}


/*									//
//		対応したボタン番号を保存	//
//		（キーボード				*/
void GameController::SetKeyNum(int _button, int _key) {
	if (_button < 0 || BUTTON_MAX <= _button) {
		Debug::LogError("存在しないキーが投げられました");
		return;
	}
	// キーの設定
	m_tKeyInfo[_button].KeyNum = _key;
}


/*									//
//		対応したボタン番号を保存	//
//		（ジョイパッド				*/
void GameController::SetJoyNum(int _button, int _key) {
	if (_button < 0 || BUTTON_MAX <= _button) {
		Debug::LogError("存在しないキーが投げられました");
		return;
	}
	// キーの設定
	m_tKeyInfo[_button].JoyNum = _key;
}


/*									//
//			キー番号初期化			//
//									*/
void GameController::SetDefaultKey() {
	m_tKeyInfo[BUTTON_START].KeyNum		= DIK_SPACE;
	m_tKeyInfo[BUTTON_UP].KeyNum		= DIK_W;
	m_tKeyInfo[BUTTON_RIGHT].KeyNum		= DIK_D;
	m_tKeyInfo[BUTTON_DOWN].KeyNum		= DIK_S;
	m_tKeyInfo[BUTTON_LEFT].KeyNum		= DIK_A;
	m_tKeyInfo[BUTTON_SANKAKU].KeyNum	= DIK_I;
	m_tKeyInfo[BUTTON_MARU].KeyNum		= DIK_L;
	m_tKeyInfo[BUTTON_BATU].KeyNum		= DIK_K;
	m_tKeyInfo[BUTTON_SIKAKU].KeyNum	= DIK_J;
	m_tKeyInfo[BUTTON_L1].KeyNum		= DIK_E;
	m_tKeyInfo[BUTTON_L2].KeyNum		= DIK_Q;
	m_tKeyInfo[BUTTON_R1].KeyNum		= DIK_U;
	m_tKeyInfo[BUTTON_R2].KeyNum		= DIK_O;
}


/*									//
//			ジョイ番号初期化		//
//									*/
void GameController::SetDefaultJoy() {
	m_tKeyInfo[BUTTON_START].JoyNum		= 9;
	// 十字
	m_tKeyInfo[BUTTON_UP].JoyNum		= -1;
	m_tKeyInfo[BUTTON_RIGHT].JoyNum		= 1;
	m_tKeyInfo[BUTTON_DOWN].JoyNum		= 1;
	m_tKeyInfo[BUTTON_LEFT].JoyNum		= -1;
	// プリミティブ
	m_tKeyInfo[BUTTON_SANKAKU].JoyNum	= 3;
	m_tKeyInfo[BUTTON_MARU].JoyNum		= 2;
	m_tKeyInfo[BUTTON_BATU].JoyNum		= 1;
	m_tKeyInfo[BUTTON_SIKAKU].JoyNum	= 0;
	// L,R
	m_tKeyInfo[BUTTON_L1].JoyNum		= 4;
	m_tKeyInfo[BUTTON_L2].JoyNum		= 6;
	m_tKeyInfo[BUTTON_R1].JoyNum		= 5;
	m_tKeyInfo[BUTTON_R2].JoyNum		= 7;
}


/*									//
//		ファクトリ用クラスID		//
//									*/
int GameController::GetClassID() {
	return SAVE_GAME_FACT_CONTROLLER;
}