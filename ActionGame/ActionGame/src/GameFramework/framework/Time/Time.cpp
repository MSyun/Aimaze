//////////////////////////////////////////////////////////////
// タイマークラス
// 2016. 7. 8	クラス作成
// 作成者		SyunMizuno
//////////////////////////////////////////////////////////////


#include	"Time.h"

/*
	MEMO	:
			//デルタタイムがリリースになると全く安定しないため
			//1.0fで強引に仮実装
*/


#pragma comment(lib, "winmm.lib")

DWORD	Time::m_dwCurrentFrameRate	= 0;		// 現在の1秒間のFPS
DWORD	Time::m_dwSettingFrameRate	= 60;		// 設定したFPS
float	Time::m_fDeltaTime			= 1.0f;		// フレーム更新の割合（基本1.0f
float	Time::m_fTimeScale			= 1.0f;		// 時間経過のファクタ（1.0で通常、2.0で2倍速
float	Time::m_fTime				= -1.0f;	// ゲーム開始からの時間
float	Time::m_fTimeUnScaled		= -1.0f;	// ゲーム開始からの時間(TimeScaleの影響なし
DWORD	Time::m_dwFrameCount		= 0;		// フレーム通過回数
DWORD	Time::m_dwLastTime			= 0;		// 前のフレームが実行された現実時間
float	Time::m_fFrameTime			= 0.0f;		// 1フレーム間のミリ秒
DWORD	Time::m_dwPrevMinutes		= 0;		// 前のフレーム時間


/*									//
//			コンストラクタ			//
//									*/
Time::Time() {
	timeBeginPeriod(1);

	SetFrameRate(m_dwSettingFrameRate);
}


/*									//
//			デストラクタ			//
//									*/
Time::~Time() {
	timeEndPeriod(1);
}


/*									//
//		フレームレートの設定		//
//									*/
void Time::SetFrameRate(DWORD frame) {
	m_dwSettingFrameRate = frame;
	m_fFrameTime = 1000.0f / frame;
}


/*									//
//		時間のスケールの設定		//
//									*/
void Time::SetTimeScale(float scale) {
	m_fTimeScale = scale;
}


/*									//
//				更新				//
//									*/
void Time::Update() {
	m_dwFrameCount++;

	// 計測開始
	if( m_fTime == -1.0f ) {
		m_dwLastTime = m_dwPrevMinutes = timeGetTime();
		m_fTime = 0.0f;
		m_fTimeUnScaled = 0.0f;
		return;
	}

	// 時間まで待って
	DWORD dwTime = (DWORD)(m_dwFrameCount * m_fFrameTime + m_dwLastTime);
//	DWORD hoge = timeGetTime();
////	while(timeGetTime() <= dwTime) {
//	while(hoge <= dwTime) {
//		hoge += (DWORD)m_fFrameTime;
////		Sleep(1);	// 休め
//	}
	DWORD time = timeGetTime();

	// 更新スピードの割合計算
	m_fDeltaTime = (time - m_dwPrevMinutes) / m_fFrameTime / m_dwSettingFrameRate;
	m_fTime += m_fDeltaTime * m_fTimeScale;
	m_fTimeUnScaled += m_fDeltaTime;
	m_dwPrevMinutes = time;

	// 1秒経過
	if( time - m_dwLastTime >= 1000 ) {
		m_dwLastTime = time;
		m_dwCurrentFrameRate = m_dwFrameCount;	// 最新フレーム数
		m_dwFrameCount		= 0;
	}
}


/*									//
//		現在の1秒間のFPSを取得		//
//									*/
DWORD Time::GetFrameRate() {
	return m_dwCurrentFrameRate;
}


/*									//
//			設定したFPSを取得		//
//									*/
DWORD Time::GetSettingFrameRate() {
	return m_dwSettingFrameRate;
}


/*									//
//			ファクタを取得			//
//									*/
// TimeScaleに依存する
float Time::GetDeltaTime() {
	return m_fDeltaTime * m_fTimeScale;
}
// TimeScaleに依存しない
float Time::GetUnScaledDeltaTime() {
	return m_fDeltaTime;
}


/*									//
//		ゲーム開始からの時間		//
//									*/
// TimeScaleに依存する
float Time::GetTime() {
	return m_fTime;
}
// TimeScaleに依存しない
float Time::GetUnScaledTime() {
	return m_fTimeUnScaled;
}