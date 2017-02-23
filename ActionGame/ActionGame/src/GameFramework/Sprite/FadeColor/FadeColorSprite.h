// フェードカラースプライト
// 2017. 1. 4	プログラム作成
// 2017. 2.13	TimeScaleに依存するか設定可能へ
// 2017. 2.21	AnimationCurve追加
// author		SyunMizuno


#pragma once


#include	"../../Obj/2D/Sprite.h"
#include	"../../framework/CPPEvent/CPPEvent.h"
#include	"../../framework/AnimationCurve/AnimationCurve.h"



class FadeColorSprite	:	public	Sprite {
private:
#pragma region variable

	Color	m_BeginColor;	// 開始色
	Color	m_EndColor;		// 目的色
	float	m_fChangeTime;	// フェード時間
	float	m_fCnt;			// 計測カウンタ
	bool	m_bPlay;		// 再生
	bool	m_bReverse;		// 逆再生
	bool	m_bTimeScale;	// TimeScaleに依存確認

	AnimationCurve	m_AnimationCurve;
	EaseType	m_EaseType;

	CPPEvent<>*	m_onEndPlayReverse;
	CPPEvent<>*	m_onEndPlay;

#pragma endregion

public:
#pragma region method

	FadeColorSprite(
		Color begin = Color(1.0f, 1.0f, 1.0f, 1.0f),
		Color end = Color(0.0f, 0.0f, 0.0f, 1.0f),
		float time = 1.0f,
		bool reverse = false,
		bool awake = false);
	virtual ~FadeColorSprite();

	// 開始色設定
	void SetBeginColor(Color color);
	// 目的色設定
	void SetEndColor(Color color);
	// 再生
	void Play(bool reverse = false, bool init = false);
	// 初期化
	void Reset(bool reverse);
	// 一時停止
	void Pause();
	// 一時停止解除
	void Resume();
	// 変更時間の設定
	void SetChangeTime(float time);
	// TimeScaleに依存するか設定
	void DependTimeScale(bool depend);
	// Easeの変更
	void ChangeEase(EaseType ease);

	CPPEvent<>* GetOnEndPlayReverse() { return m_onEndPlayReverse; }
	CPPEvent<>* GetOnEndPlay() { return m_onEndPlay; }

#pragma endregion

private:
#pragma region conceal method

	virtual void Update();

	void FadeIn();
	void FadeOut();

	// 更新時間の取得
	float GetDelta();

#pragma endregion
};