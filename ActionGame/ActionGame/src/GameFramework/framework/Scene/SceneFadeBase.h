///////////////////////////////////////////////////////////////
// シーンフェードベースクラス
// author		SyunMizuno
// 2016. 1.27	プログラム作成
// 2016. 4.18	関数追加
// 2017. 1. 4	Event追加
///////////////////////////////////////////////////////////////

#pragma once

#include	"SceneBase.h"
#include	<d3dx9.h>
#include	"../../Obj/2D/Sprite.h"
#include	"../Convert.h"

#include	"../CPPEvent/CPPEvent.h"

/*
	memo	:
		フェード用シーンはこのクラスを継承すること
		コンストラクタ、デストラクタでInit,Releaseを呼び出さないこと
*/


class	SceneFadeBase : public	SceneBase {
private:
#pragma region variable

	float	m_fAlpha;	// アルファ値
	Vector2	m_vSize;

protected:
	bool	m_bExe;		// 実行フラグ
	bool	m_bIn;		// フェードイン・フェードアウト
	float	m_fTime;	// フェード時間
	bool	m_bChange;	// 変更可能フラグ
	IDirect3DDevice9*	m_pDevice;

	// 描画用クラス
	Sprite*			m_pSprite;

	// Event
	CPPEvent<>*	m_onBeginFadeIn;
	CPPEvent<>*	m_onEndFadeIn;
	CPPEvent<>*	m_onBeginFadeOut;
	CPPEvent<>*	m_onEndFadeOut;

#pragma endregion

public:
#pragma region method

	SceneFadeBase(
		IDirect3DDevice9* _Dev,
		bool _awake = false,
		int _width = 1280,
		int _height = 720,
		float _time = 1.0f);
	virtual ~SceneFadeBase() {};
	void Update();
	virtual void Draw();

	// 再初期化
	virtual bool ReStart(void);
	// 実行
	void Exe(void);
	bool Change(void);

	void SetFadeTime(float _time) { if (_time > 0.0f) m_fTime = _time; }
	void SetSize(int width, int height);

	// Event
	CPPEvent<>* GetOnBeginFadeIn() { return m_onBeginFadeIn; }
	CPPEvent<>* GetOnEndFadeIn() { return m_onEndFadeIn; }
	CPPEvent<>* GetOnBeginFadeOut() { return m_onBeginFadeOut; }
	CPPEvent<>* GetOnEndFadeOut() { return m_onEndFadeOut; }

#pragma endregion

protected:
#pragma region conceal method

	virtual bool Init();
	virtual void Release();
	// フェード処理
	virtual void FadeIn();
	virtual void FadeOut();

	virtual void OnAwake();

#pragma endregion
};