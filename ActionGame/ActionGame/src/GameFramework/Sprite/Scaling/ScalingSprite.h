// 拡縮スプライト
// 2017. 2. 7	プログラム作成
// 2017. 2.13	TimeScaleに依存するか設定可能へ
// author		SyunMizuno


#pragma once


#include	"../../Obj/2D/Sprite.h"
#include	"../../framework/CPPEvent/CPPEvent.h"
#include	"../../framework/AnimationCurve/AnimationCurve.h"



class ScalingSprite : public	Sprite {
private:
#pragma region variable

	Vector3	m_BeginScale;	// 開始サイズ
	Vector3	m_EndScale;		// 終了サイズ
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

	ScalingSprite(
		Vector3 begin = Vector3(0.0f, 0.0f, 0.0f),
		Vector3 end = Vector3(1.0f, 1.0f, 1.0f),
		float time = 1.0f,
		bool reverse = false,
		bool awake = false);
	virtual ~ScalingSprite();

	// 開始座標設定
	void SetBeginScale(Vector3 scale);
	void SetBeginScale(float x, float y, float z);
	// 目的座標設定
	void SetEndScale(Vector3 scale);
	void SetEndScale(float x, float y, float z);
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