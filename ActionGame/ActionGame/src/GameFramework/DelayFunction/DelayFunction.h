//	遅延型関数クラス
// 2017. 3.22	プログラム作成
// author		SyunMizuno


#pragma once


#include	<functional>
#include	"../framework/Delegate/Delegate.h"


class DelayFunction {
private:
	double			m_dExeTime;		// 実行時間
	Delegate<void>	m_Function;		// 登録関数
	bool			m_bTimeScale;	// TimeScaleの依存

public:
	DelayFunction(
		std::function<void(void)> func,
		double time,
		bool timeScale = true);
	virtual ~DelayFunction();

	// 時間計測
	// return : true 実行終了. false 実行中
	bool TimeMate();

private:
	// 登録
	void Regist(std::function<void(void)> func, double time);

	// 実行終了確認
	// return : true 実行終了. false 実行中
	bool CheckEnd();

	// TimeScaleに依存する計測
	double MateScale();
	// TimeScaleに依存しない計測
	double MateNoScale();
};

// 使用
void DelayFunc(std::function<void(void)> func, double time, bool timeScale = true);