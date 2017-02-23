//////////////////////////////////////////////////////////////
// GameControllerManager クラス
// 2016. 9. 27	プログラム作成
// author		SyunMizuno
//////////////////////////////////////////////////////////////

#pragma once


#include	"GameController.h"
#include	"../framework/Singleton/Singleton.h"
#include	"../framework/Save/SaveObjBase.h"

#include	<vector>


// 最大生成数
#define		CONTROLLER_MAX_CREATE	( 1 )


class	GameControllerManager : public	Singleton<GameControllerManager> {
protected:
#pragma region variable

	// コントローラー配列(1～4だと思う
	vector< GameController* >	m_vecCont;

#pragma endregion

public:
#pragma region method

	GameControllerManager();
	virtual ~GameControllerManager();

	// コントローラー作成
	void Create(unsigned int num = 1);

	// コントローラー追加
	void Add(GameController* pobj);

	// コントローラー取得
	GameController* Get(unsigned int num = 1);

	void Save();

#pragma endregion
};

inline GameControllerManager* GetContManager() {
	return GameControllerManager::Instance();
}