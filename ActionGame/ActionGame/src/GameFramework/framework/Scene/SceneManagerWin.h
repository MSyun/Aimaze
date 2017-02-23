// Windows用SceneManagerクラス
// 2015.12.15	プログラム作成
// 2016.01.26	vectorからmapへ変更
// 2016.01.28	読み込み率追加
// 改変者		SyunMizuno

#pragma once

#include	"SceneBase.h"
#include	"SceneManager.h"
#include	<process.h>


class SceneManagerWin : public	SceneManagerBase {
protected:
#pragma region variable

	SceneBase*		m_Loading;		// ロードシーン
	int				m_nLoadingFlag;	// ロードフラグ。
	HANDLE			m_hThread;		// スレッドハンドル
	bool			m_bLoadSuc;		// ロードの成功
	unsigned int	m_uLoadWaitTime;// ロード中スレッドを開始するまでのウェイト
	unsigned char	m_uLoadRate;	// 読み込み率
	bool			m_bLoadKey;		// 読み込み終了確認
	bool			m_bLoadEndAfter;// 読み込み後のシーン遷移確認

#pragma endregion

public:
#pragma region method

	SceneManagerWin();
	~SceneManagerWin();
	void Release(void);

	void SetLoadingFlag(int flag) { m_nLoadingFlag = flag; }
	void SetLoadSuc(bool Success) { m_bLoadSuc = Success; }
	void SetLoadWait(unsigned int time) { m_uLoadWaitTime = time; }
	void SetLoadRate(unsigned char _rate) { m_uLoadRate = _rate; }
	unsigned char GetLoadRate(void) { return m_uLoadRate; }
	// 読み込み終了確認（true. 読み込み成功 : false. 読み込み中
	bool CheckLoadingEnd() { return !m_bLoadKey; }
	// 読み込み終了
	void LoadingEnd() { m_bLoadEndAfter = true; }

	// シーンの追加
	bool RegistLoadingScene(SceneBase* pScene);
	template<typename T> bool RegistLoadingSceneMakeInstance(void) { T *data = new T; return RegistLoadingScene(data); }
	void DeleteLoadingScene(void);

	SceneBase* GetScene(void);

#pragma endregion

protected:
#pragma region conceal method

	bool ChangeLoadScene(SceneBase* pScene);
	void PopLoadScene(void);
	bool PushLoadScene(SceneBase* pScene);

#pragma endregion
};

class SceneManagerWins : public	SceneManagerWin, public	Singleton<SceneManagerWins> {};

// これで使用しやすくなる
inline SceneManagerWins* GetSceneManagerWins(void) {
	return SceneManagerWins::Instance();
}