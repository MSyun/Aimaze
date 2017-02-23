//////////////////////////////////////////////////////////////
// タイマークラス
// 2016. 7. 8	クラス作成
// 作成者		SyunMizuno
//////////////////////////////////////////////////////////////

#pragma once

#include	<Windows.h>


class	Time {
private:
#pragma region variable

	static DWORD	m_dwCurrentFrameRate;	// 現在の1秒間のFPS
	static DWORD	m_dwSettingFrameRate;	// 設定したFPS
	static float	m_fDeltaTime;			// フレーム更新の割合（基本1.0f
	static float	m_fTimeScale;			// 時間経過のファクタ（1.0で通常、2.0で2倍速
	static float	m_fTime;				// ゲーム開始からの時間
	static float	m_fTimeUnScaled;		// ゲーム開始からの時間(TimeScaleの影響なし

#pragma endregion

#pragma region conceal variable

	static DWORD	m_dwFrameCount;		// フレーム通過回数
	static DWORD	m_dwLastTime;		// 前のフレームが実行された現実時間
	static float	m_fFrameTime;		// 1フレーム間のミリ秒
	static DWORD	m_dwPrevMinutes;	// 前のフレーム時間

#pragma endregion

public:
#pragma region method

	Time();
	virtual ~Time();

	// フレームレートの設定
	static void SetFrameRate(DWORD frame);
	// 時間のスケールの設定
	static void SetTimeScale(float scale);
	// 更新
	static void Update();
	// フレームレートの取得
	static DWORD GetFrameRate();
	// 設定したFPSを取得
	static DWORD GetSettingFrameRate();
	// TimeScaleに依存したファクタを取得
	static float GetDeltaTime();
	// TimeScaleに依存しないファクタを取得
	static float GetUnScaledDeltaTime();

	// TimeScaleに依存する時間
	static float GetTime();
	// TimeScaleに依存しない時間
	static float GetUnScaledTime();

#pragma endregion
};