// Windows用SceneManagerクラス
// 2015.12.16	プログラム作成
// 2016. 1. 3	ちょっち改変
// 2016.01.26	vectorからmapへ変更
// 2017. 1.22	デバッグログを追加


#include	"SceneManagerWin.h"
#include	"../Input/Input.h"
#include	"../Debug/Debug.h"

/*
	memo:
		ここで読み込み終了時のキー入力を変更すること
		GetScene関数はもっと継承してください
*/


#define		LOAD_END_KEY	(DIK_RETURN)



// ChangeSceneThreadの引数用
typedef struct {
	SceneBase *top;
	SceneBase *cScene;
} _ChangeData;

// ロードフラグ
typedef enum {
	LOADING_NO = 0,	// No処理
	LOADING_NOW,	// 読み込み中
	LOADING_MAX,	// 読み込み完了
	LOADING_END,	// 読み込み終了
} _Loading;


////////////////////////////////////////////////////////////////////////////
//	関数名	：	InitSceneThread
//	処理概要：	読み込み処理
//	入力	：
//	出力	：
////////////////////////////////////////////////////////////////////////////
unsigned _stdcall InitSceneThread(void* pThreadParam) {
	//----- シーンマネージャのインスタンス取得
	SceneManagerWins* Si = GetSceneManagerWins();
	if (!Si) _endthreadex(0);// インスタンスがない場合

	// 引数を取得
	SceneBase* tmp = (SceneBase*)pThreadParam;

	//----- 初期化
	if (tmp->InitMain()) {
		Si->SetLoadSuc(true);
	} else {
		Si->SetLoadSuc(false);
	}
	//----- ロード終了
	Si->SetLoadingFlag(LOADING_END);
	_endthreadex(0);
	return 0;
}


////////////////////////////////////////////////////////////////////////////
//	関数名	：	ReleaseSceneThread
//	処理概要：	読み込み処理
//	入力	：
//	出力	：
////////////////////////////////////////////////////////////////////////////
unsigned _stdcall ReleaseSceneThread(void* pThreadParam) {
	//----- シーンマネージャのインスタンス取得
	SceneManagerWins* Si = GetSceneManagerWins();
	if (!Si) _endthreadex(0);// インスタンスがない場合

	// 引数を取得
	SceneBase* tmp = (SceneBase*)pThreadParam;

	//----- 解放
	tmp->ReleaseMain();

	//----- ロード終了
	Si->SetLoadSuc(true);
	Si->SetLoadingFlag(LOADING_END);
	_endthreadex(0);
	return 0;
}


////////////////////////////////////////////////////////////////////////////
//	関数名	：	ChangeSceneThread
//	処理概要：	フレーム変更処理
//	入力	：
//	出力	：
////////////////////////////////////////////////////////////////////////////
unsigned _stdcall ChangeSceneThread(void* pThreadParam) {
	//----- シーンマネージャのインスタンス取得
	SceneManagerWins* Si = GetSceneManagerWins();
	if (!Si) _endthreadex(0);// インスタンスがない場合

	// 引数を取得
	_ChangeData* tmp = (_ChangeData*)pThreadParam;

	//----- 解放
	tmp->top->ReleaseMain();

	//----- 初期化
	if (tmp->cScene->InitMain()) {
		Si->SetLoadSuc(true);
	} else {
		Si->SetLoadSuc(false);
	}
	delete tmp;

	//----- ロード終了
	Si->SetLoadingFlag(LOADING_END);
	_endthreadex(0);
	return 0;
}



////////////////////////////////////////////////////////////////////////////
//	コンストラクタ
////////////////////////////////////////////////////////////////////////////
SceneManagerWin::SceneManagerWin() {
	m_Loading		= NULL;			// ロードシーン
	m_nLoadingFlag	= LOADING_NO;	// ロードフラグ
	m_hThread		= NULL;			// スレッドハンドル
	m_bLoadSuc		= true;			// ロード成功
	m_uLoadWaitTime	= 10;			// ロード開始ウェイト時間
	m_uLoadRate		= 0;			// 読み込み率
	m_bLoadKey		= false;
	m_bLoadEndAfter = false;
}


////////////////////////////////////////////////////////////////////////////
//	デストラクタ
////////////////////////////////////////////////////////////////////////////
SceneManagerWin::~SceneManagerWin() {
	Release();
}


////////////////////////////////////////////////////////////////////////////
//	関数名	：	Release
//	処理概要：	開放関数
//	入力	：
//	出力	：
////////////////////////////////////////////////////////////////////////////
void SceneManagerWin::Release(void) {
	//----- ロード中
	while (m_nLoadingFlag == LOADING_NOW || m_nLoadingFlag == LOADING_END) {
		if (m_nLoadingFlag == LOADING_END) {
			// スレッドを閉じる
			CloseHandle(m_hThread);
			break;
		}
	}

	DeleteLoadingScene();
	// 親クラスのリリース処理
	SceneManagerBase::Release();
}


////////////////////////////////////////////////////////////////////////////
//	関数名	：	RegistLoadingScene
//	処理概要：	NowLoading画面用シーン読み込みを行う
//	入力	：	*pScene
//	出力	：	成功すればtrue失敗すればfalse
////////////////////////////////////////////////////////////////////////////
bool SceneManagerWin::RegistLoadingScene(SceneBase* pScene) {
	if (!pScene) {
		Debug::LogError("Loading画面用シーンの実態がありません");
		return false;
	}

	if (m_Loading) {
		m_Loading->ReleaseMain();
		delete m_Loading;
		m_Loading = NULL;
	}

	m_Loading = pScene;
	return m_Loading->InitMain();
}


////////////////////////////////////////////////////////////////////////////
//	関数名	：	DeleteLoadingScene
//	処理概要：	読み込み画面開放関数
//	入力	：
//	出力	：
////////////////////////////////////////////////////////////////////////////
void SceneManagerWin::DeleteLoadingScene(void) {
	if (!m_Loading)
		return;

	m_Loading->ReleaseMain();
	delete m_Loading;
	m_Loading = NULL;
}


////////////////////////////////////////////////////////////////////////////
//	関数名	：	GetScene
//	処理概要：	シーンの更新後、topのシーンを取得
//	入力	：
//	出力	：	topシーンのポインタ。ない場合はNULLを返す
////////////////////////////////////////////////////////////////////////////
SceneBase* SceneManagerWin::GetScene() {
	//----- フェード
	if (m_pFade)
		m_pFade->Update();

	//----- ロード中
	if (m_nLoadingFlag != LOADING_NO) {
		Sleep(m_uLoadWaitTime);

		//----- 読み込み終了中
		if (m_nLoadingFlag == LOADING_MAX) {
			if (!m_bLoadKey) {
				// ボタン入力により遷移
				if(m_bLoadEndAfter)
					m_bLoadKey = true;
			} else {
				// フェード実行
				if (m_pFade) {
					m_pFade->Exe();
					m_bLoadEndAfter = false;
					if (!m_pFade->Change()) {
						return m_Loading;
					}
					// フラグを変更
					m_nLoadingFlag = LOADING_NO;
					m_uLoadRate = 0;
					m_bLoadKey = false;
				}
			}
		}

		//----- スレッド終了時
		if (m_nLoadingFlag == LOADING_END) {
			CloseHandle(m_hThread);	// スレッドを閉じる
			// 読み込み失敗
			if (!m_bLoadSuc) {
				return NULL;
			}
			// フラグを変更
			m_nLoadingFlag = LOADING_MAX;
		}

		m_nCommand = SCENEMANAGER_NO;
		return m_Loading;
	}

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

		// 成功確認
		bool bSuccess = true;

		// コマンド処理
		switch (m_nCommand) {
		case SCENEMANAGER_POP:
			PopScene();
			break;

		case SCENEMANAGER_PUSH:
			bSuccess = PushScene(it->second);
			break;

		case SCENEMANAGER_CHANGE:
			bSuccess = ChangeScene(it->second);
			break;

		case SCENEMANAGER_RESTART:
			bSuccess = GetTopScene()->ReStart();
			break;

		case SCENEMANAGER_LOADING_POP:
			PopLoadScene();
			m_nCommand = SCENEMANAGER_NO;
			return m_Loading;

		case SCENEMANAGER_LOADING_PUSH:
			PushLoadScene(it->second);
			m_nCommand = SCENEMANAGER_NO;
			return m_Loading;

		case SCENEMANAGER_LOADING_CHANGE:
			ChangeLoadScene(it->second);
			m_nCommand = SCENEMANAGER_NO;
			return m_Loading;

		case SCENEMANAGER_LOADING_RESTART:
			ChangeLoadScene(GetTopScene());
			m_nCommand = SCENEMANAGER_NO;
			return m_Loading;

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
//	関数名	：	ChangeLoadScene
//	処理概要：	topシーンを変更する。読み込み画面あり
//	入力	：	pScene	変更するシーンのポインタ
//	出力	：	成功すればtrue失敗すればfalse
////////////////////////////////////////////////////////////////////////////
bool SceneManagerWin::ChangeLoadScene(SceneBase* pScene) {
	if (!pScene) {
		Debug::LogError("Changeしたいシーンの実態がありません");
		return false;
	}

	//----- 読み込み開始
	m_nLoadingFlag = LOADING_NOW;
	if (!m_Loading->ReStart()) {
		Debug::LogError("ローディング画面用シーンの再初期化に失敗しました");
	}

	//----- 初期化の確認
	if (pScene->GetMakeFlg() && pScene != GetTopScene()) {
		if (!m_Stack.empty()) {
			m_hThread = (HANDLE)_beginthreadex(
				NULL,				// SECURITY_ATTRIBUTESへのポインタorNULL
				0,					// スタックサイズ
				ReleaseSceneThread,	// スレッド関数のアドレス
				GetTopScene(),		// スレッドに渡す引数
				0,					// 0（すぐ実行）、CREATE_SUSPENDED（一時停止）
				NULL);				// スレッド識別子。NULLでも可
			m_Stack.pop();
		}
		if (!pScene->ReStart()) {
			Debug::LogError("シーンの再初期化に失敗しました");
			return false;
		}
	} else {
		// 初期化されてないよ～
		if (!m_Stack.empty()) {
			// 積んである場合
			_ChangeData *Data;
			Data = new _ChangeData;
			Data->top = GetTopScene();
			Data->cScene = pScene;
			// スレッド開始
			m_hThread = (HANDLE)_beginthreadex(
				NULL,				// SECURITY_ATTRIBUTESへのポインタorNULL
				0,					// スタックサイズ
				ChangeSceneThread,	// スレッド関数のアドレス
				Data,				// スレッドに渡す引数
				0,					// 0（すぐ実行）、CREATE_SUSPENDED（一時停止）
				NULL);				// スレッド識別子。NULLでも可
			m_Stack.pop();
		} else {
			// 空の場合
			// スレッド開始
			m_hThread = (HANDLE)_beginthreadex(
				NULL,				// SECURITY_ATTRIBUTESへのポインタorNULL
				0,					// スタックサイズ
				InitSceneThread,	// スレッド関数のアドレス
				pScene,				// スレッドに渡す引数
				0,					// 0（すぐ実行）、CREATE_SUSPENDED（一時停止）
				NULL);				// スレッド識別子。NULLでも可
		}
	}

	m_Stack.push(pScene);
	return true;
}


////////////////////////////////////////////////////////////////////////////
//	関数名	：	PopLoadScene
//	処理概要：	フレームをpopする。読み込み画面あり
//	入力	：	
//	出力	：	
////////////////////////////////////////////////////////////////////////////
void SceneManagerWin::PopLoadScene(void) {
	if (m_Stack.empty()) {
		Debug::Log("popするシーンはありません");
		return;
	}

	SceneBase* pScene = m_Stack.top();

	//----- 読み込み開始
	m_Loading->ReStart();
	m_nLoadingFlag = LOADING_NOW;

	//----- スレッド開始
	m_hThread = (HANDLE)_beginthreadex(
		NULL,				// SECURITY_ATTRIBUTESへのポインタorNULL
		0,					// スタックサイズ
		ReleaseSceneThread,	// スレッド関数のアドレス
		pScene,				// スレッドに渡す引数
		0,					// 0（すぐ実行）、CREATE_SUSPENDED（一時停止）
		NULL);				// スレッド識別子。NULLでも可

	m_Stack.pop();
}


////////////////////////////////////////////////////////////////////////////
//	関数名	：	PushLoadScene
//	処理概要：	シーンをpushする。読み込み画面あり
//	入力	：	pScene	pushするシーンのポインタ
//	出力	：	成功すればtrue失敗すればfalse
////////////////////////////////////////////////////////////////////////////
bool SceneManagerWin::PushLoadScene(SceneBase* pScene) {
	if (!pScene) {
		Debug::LogError("Pushしたいシーンの実態がありません");
		return false;
	}

	//----- 読み込み開始
	if (!m_Loading->ReStart()) {
		Debug::LogError("ローディング画面用シーンの再初期化に失敗しました");
	}

	if (pScene->GetMakeFlg()) {
		// リソース確保済み
		if (!pScene->ReStart()) {
			Debug::LogError("Pushしたシーンの再初期化に失敗しました");
			pScene->ReleaseMain();
			return false;
		}
	} else {
		// リソース確保してねぇ
		m_nLoadingFlag = LOADING_NOW;
		//----- スレッド開始
		m_hThread = (HANDLE)_beginthreadex(
			NULL,				// SECURITY_ATTRIBUTESへのポインタorNULL
			0,					// スタックサイズ
			InitSceneThread,	// スレッド関数のアドレス
			pScene,				// スレッドに渡す引数
			0,					// 0（すぐ実行）、CREATE_SUSPENDED（一時停止）
			NULL);				// スレッド識別子。NULLでも可
	}

	m_Stack.push(pScene);
	return true;
}