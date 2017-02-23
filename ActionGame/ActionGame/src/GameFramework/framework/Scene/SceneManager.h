///////////////////////////////////////////////////////////////
// シーンマネージャークラス
// author		SyunMizuno
// 2015.12.07	プログラム作成
// 2016.01.26	vectorからmapへ変更
// 2016.01.27	フェード追加
// 2016.07.06	2シーン再生機能追加
///////////////////////////////////////////////////////////////

#pragma once

#include	"SceneBase.h"
#include	"SceneFadeBase.h"
#include	<d3dx9.h>
#include	<stack>
#include	<map>
#include	<string>
#include	"../Singleton/Singleton.h"

using namespace std;


/*
	memo:
		各自機能追加等は列挙型に追加しコマンドを作ること
*/

class SceneManagerBase {
protected:
#pragma region enum

	// 列挙型
	typedef enum {
		SCENEMANAGER_NO = 0,	// 変更しない

		SCENEMANAGER_POP,		// popする
		SCENEMANAGER_PUSH,		// pushする
		SCENEMANAGER_CHANGE,	// 変更する
		SCENEMANAGER_RESTART,	// 再初期化

		// ↓クラス継承先で使用↓
		SCENEMANAGER_LOADING_POP,		// ロード画面ありpop
		SCENEMANAGER_LOADING_PUSH,		// ロード画面ありpush
		SCENEMANAGER_LOADING_CHANGE,	// ロード画面あり変更
		SCENEMANAGER_LOADING_RESTART,	// ロード画面あり再初期化
	} _command;

#pragma endregion

#pragma region variable

	map< string, SceneBase* >	m_MapScene;		// 登録されたシーン
	stack<SceneBase*>			m_Stack;		// シーンのスタック
	string						m_TmpSceneName;	// 変更先シーン
	int							m_nCommand;		// シーン制御のコマンド
	SceneFadeBase*				m_pFade;		// フェード用シーン

#pragma endregion

public:
#pragma region method

	SceneManagerBase();
	~SceneManagerBase() { Release(); }
	void Release(void);

	// シーンの追加・削除
	bool RegistScene(SceneBase* pScene, const string SceneName);
	template<typename T>bool RegistSceneMakeInstance(string SceneName) { T* data = new T; return RegistScene(data, SceneName); }
	void DeleteScene(const string SceneName);
	bool RegistFadeScene(SceneFadeBase* pScene);
	void DeleteFadeScene();

#pragma region Command

	void PopCommand(void) { m_nCommand = SCENEMANAGER_POP; }
	void PushCommand(const string SceneName) { m_TmpSceneName.clear(); m_TmpSceneName = SceneName; m_nCommand = SCENEMANAGER_PUSH; }
	void ChangeCommand(const string SceneName) { m_TmpSceneName.clear(); m_TmpSceneName = SceneName; m_nCommand = SCENEMANAGER_CHANGE; }
	void RestartCommand(void) { m_nCommand = SCENEMANAGER_RESTART; }
	void CancelCommand(void) { m_nCommand = SCENEMANAGER_NO; }
	void PopLoadCommand(void) { m_nCommand = SCENEMANAGER_LOADING_POP; }
	void PushLoadCommand(const string SceneName) { m_TmpSceneName = SceneName; m_nCommand = SCENEMANAGER_LOADING_PUSH; }
	void ChangeLoadCommand(const string SceneName) { m_TmpSceneName = SceneName; m_nCommand = SCENEMANAGER_LOADING_CHANGE; }
	void RestartLoadCommand(void) { m_nCommand = SCENEMANAGER_LOADING_RESTART; }

#pragma endregion

	void Draw() { if (m_pFade) m_pFade->Draw(); }

	SceneBase* GetScene(void);

	SceneFadeBase* GetFade(void) { return m_pFade; }

#pragma endregion

protected:
#pragma region conceal method

	SceneBase* GetTopScene(void) { return ((m_Stack.empty()) ? NULL : m_Stack.top()); }
	void PopScene(void);
	bool PushScene(SceneBase* pScene);
	bool ChangeScene(SceneBase* pScene);
	void SceneAllDelete(void);

#pragma endregion
};

class SceneManager : public SceneManagerBase, public Singleton<SceneManager> {};

// これで使用しやすくなる
inline SceneManager* GetSceneManager(void) {
	return SceneManager::Instance();
}