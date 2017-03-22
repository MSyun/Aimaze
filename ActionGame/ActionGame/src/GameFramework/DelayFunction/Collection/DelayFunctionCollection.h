//	遅延型関数管理クラス
// 2017. 3.22	プログラム作成
// author		SyunMizuno


#pragma once

#include	"../DelayFunction.h"
#include	<vector>


class DelayFunctionCollection {
private:
	static std::vector<DelayFunction*> m_aCollection;

public:
	DelayFunctionCollection();
	virtual ~DelayFunctionCollection();

	// 追加
	static void Add(DelayFunction* func);

	// 時間計測
	static void Update();

	// 全要素削除
	static void AllDelete();
};