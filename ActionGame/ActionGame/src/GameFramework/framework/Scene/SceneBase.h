///////////////////////////////////////////////////////////////
// シーンベースクラス
// author		SyunMizuno
// 2015.12.05	プログラム作成
// 2016. 1. 3	ちょっち改変
///////////////////////////////////////////////////////////////

#pragma once

/*
	memo	:
		シーンはこのクラスを継承すること
		コンストラクタ、デストラクタでInit,Releaseを呼び出さないこと
*/
class SceneBase {
private:
#pragma region variable

	bool m_bMakeFlg;	// Initが呼び出されたか

#pragma endregion

protected:
#pragma region conceal method

	// リソース系はここ
	virtual bool Init() = 0;
	virtual void Release() = 0;

#pragma endregion

public:
#pragma region method

	virtual void Update() = 0;
	virtual void Draw() = 0;
	virtual bool ReStart() { return true; }

	virtual bool InitMain() { m_bMakeFlg = true;	if (!Init()) { return false; } return true; }
	virtual void ReleaseMain() { m_bMakeFlg = false;	Release(); }

	SceneBase() { m_bMakeFlg = false; }
	virtual ~SceneBase() {}
	bool GetMakeFlg(void) { return m_bMakeFlg; }	// SceneManagerクラスで使用

#pragma endregion
};