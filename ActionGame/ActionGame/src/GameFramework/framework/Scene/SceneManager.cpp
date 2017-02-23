///////////////////////////////////////////////////////////////
// シーンマネージャークラス
// author		SyunMizuno
// 2015.12.05	プログラム作成
// 2016.01.26	vectorからmapへ変更
// 2017. 1.22	デバッグログを追加
///////////////////////////////////////////////////////////////

#include	"SceneManager.h"
#include	"../Debug/Debug.h"



////////////////////////////////////////////////////////////////////////////
//	コンストラクタ
////////////////////////////////////////////////////////////////////////////
SceneManagerBase::SceneManagerBase() {
	while (!m_Stack.empty()) {
		m_Stack.pop();
	}
	m_TmpSceneName.clear();
	m_nCommand = SCENEMANAGER_NO;
	m_MapScene.clear();
	m_pFade = NULL;
}


////////////////////////////////////////////////////////////////////////////
//	関数名	：	Release
//	処理概要：	開放関数
//	入力	：
//	出力	：
////////////////////////////////////////////////////////////////////////////
void SceneManagerBase::Release() {
	// スタックの解放処理
	while (!m_Stack.empty()) {
		m_Stack.pop();
	}
	DeleteFadeScene();
	// 登録したシーンの削除
	SceneAllDelete();
	m_nCommand = SCENEMANAGER_NO;
	m_TmpSceneName.clear();
}


////////////////////////////////////////////////////////////////////////////
//	関数名	：	RegistScene
//	処理概要：	シーンオブジェクトを登録する
//	入力	：	pScene		登録するシーンオブジェクト
//				SceneName	登録するシーン名
//	出力	：	成功したらtrue,失敗したらfalse
////////////////////////////////////////////////////////////////////////////
bool SceneManagerBase::RegistScene(SceneBase* pScene, const string SceneName) {
	if (!pScene) {
		Debug::LogError("シーン名" + SceneName + "の実態がありません");
		return false;
	}

	if (m_MapScene.end() != m_MapScene.find(SceneName)) {
		Debug::LogError("シーン名" + SceneName + "は登録済みです");
		return false;
	}

	m_MapScene.insert(pair<string, SceneBase*>(SceneName, pScene));

	return true;
}


////////////////////////////////////////////////////////////////////////////
//	関数名	：	DeleteRegistedScene
//	処理概要：	シーンオブジェクトを削除する
//	入力	：	SceneName	シーン名
//	出力	：	
////////////////////////////////////////////////////////////////////////////
void SceneManagerBase::DeleteScene(const string SceneName) {
	map<string, SceneBase*>::iterator it = m_MapScene.find(SceneName);

	if (it == m_MapScene.end()) {
		Debug::Log("シーン名" + SceneName + "は登録されていないため削除できません");
		return;
	}

	it->second->ReleaseMain();
	delete it->second;
	it->second = NULL;

	m_MapScene.erase(SceneName);
}


////////////////////////////////////////////////////////////////////////////
//	関数名	：	RegistFadeScene
//	処理概要：	Fadeを行うシーンを登録
//	入力	：	pScene		登録するシーンオブジェクト
//				*pScene
//	出力	：	成功したらtrue,失敗したらfalse
////////////////////////////////////////////////////////////////////////////
bool SceneManagerBase::RegistFadeScene(SceneFadeBase* pScene) {
	if (!pScene) {
		Debug::LogError("フェード用シーンの実態がありません");
		return false;
	}

	if (m_pFade) {
		m_pFade->ReleaseMain();
		delete m_pFade;
		m_pFade = NULL;
	}

	m_pFade = pScene;
	return m_pFade->InitMain();
}


////////////////////////////////////////////////////////////////////////////
//	関数名	：	DeleteFadeScene
//	処理概要：	Fade開放関数
//	入力	：
//	出力	：
////////////////////////////////////////////////////////////////////////////
void SceneManagerBase::DeleteFadeScene() {
	if (!m_pFade)	return;

	m_pFade->ReleaseMain();
	delete m_pFade;
	m_pFade = NULL;
}


////////////////////////////////////////////////////////////////////////////
//	関数名	：	GetScene
//	処理概要：	シーンの更新後、topのシーンを取得
//	入力	：	
//	出力	：	topのシーンのポインタ、なければNULLを返す
////////////////////////////////////////////////////////////////////////////
SceneBase* SceneManagerBase::GetScene(void) {
	//----- フェード
	if (m_pFade)
		m_pFade->Update();

	//----- 変数宣言
	bool bSuccess = true;

	//----- コマンド変更確認
	if (m_nCommand == SCENEMANAGER_NO) {
		return GetTopScene();
	} else {
		// フェード
		if (m_pFade) {
			// 実行
			m_pFade->Exe();
			if (!m_pFade->Change()) {
				return GetTopScene();
			}
		}

		map< string, SceneBase* >::iterator it = m_MapScene.find(m_TmpSceneName);

		if (it == m_MapScene.end()) {
			m_nCommand = SCENEMANAGER_NO;
			Debug::LogError("シーン名" + m_TmpSceneName + "は存在しません");
			return NULL;
		}

		// コマンド処理
		switch (m_nCommand) {
		case SCENEMANAGER_POP:
			PopScene();
			break;

		case SCENEMANAGER_CHANGE:
			bSuccess = ChangeScene(it->second);
			break;

		case SCENEMANAGER_PUSH:
			bSuccess = PushScene(it->second);
			break;

		case SCENEMANAGER_RESTART:
			bSuccess = GetTopScene()->ReStart();
			break;

		default:
			break;
		}

		// 変更なしで上書き
		m_nCommand = SCENEMANAGER_NO;

		// 失敗時の処理
		if (!bSuccess) {
			Debug::LogError("シーンの変更に失敗しました");
			return NULL;
		}
	}

	// 最新のシーンを返す
	return GetTopScene();
}


////////////////////////////////////////////////////////////////////////////
//	関数名	：	PopScene
//	処理概要：	シーンをpopする
//	入力	：	
//	出力	：	
////////////////////////////////////////////////////////////////////////////
void SceneManagerBase::PopScene(void) {
	if (m_Stack.empty())
		return;

	m_Stack.top()->ReleaseMain();
	m_Stack.pop();
}


////////////////////////////////////////////////////////////////////////////
//	関数名	：	PushScene
//	処理概要：	シーンをpushする
//	入力	：	pScene	pushするシーンのポインタ
//	出力	：	
////////////////////////////////////////////////////////////////////////////
bool SceneManagerBase::PushScene(SceneBase* pScene) {
	if (!pScene) {
		Debug::LogError("Pushしたシーンの実態がありません");
		return false;
	}

	// 初期化確認
	if (pScene->GetMakeFlg()) {
		if (!pScene->ReStart()) {
			Debug::LogError("シーンの再初期化に失敗しました");
			pScene->ReleaseMain();
			return false;
		}
	} else {
		if (!pScene->InitMain()) {
			Debug::LogError("シーンの初期化に失敗しました");
			pScene->ReleaseMain();
			return false;
		}
	}

	m_Stack.push(pScene);
	return true;
}


////////////////////////////////////////////////////////////////////////////
//	関数名	：	ChangeScene
//	処理概要：	topシーンを変更する
//	入力	：	pScene	変更するシーン
//	出力	：	存在すればtrue存在しなければfalse
////////////////////////////////////////////////////////////////////////////
bool SceneManagerBase::ChangeScene(SceneBase* pScene) {
	if (!pScene) {
		Debug::LogError("Changeしたいシーンの実態がありません");
		return false;
	}

	PopScene();
	if (!PushScene(pScene)) {
		Debug::LogError("シーンのPushに失敗しました");
		return false;
	}

	return true;
}


////////////////////////////////////////////////////////////////////////////
//	関数名	：	SceneAllDelete
//	処理概要：	登録されたシーンをすべて開放
//	入力	：	
//	出力	：	
////////////////////////////////////////////////////////////////////////////
void SceneManagerBase::SceneAllDelete(void) {
	map<string, SceneBase*>::iterator it = m_MapScene.begin();
	while (it != m_MapScene.end()) {
		if (it->second->GetMakeFlg())
			it->second->ReleaseMain();
		delete it->second;
		it->second = NULL;
		it++;
	}
	m_MapScene.clear();
}