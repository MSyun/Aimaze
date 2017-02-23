// デリゲートインターフェース
// 2016.12.30	プログラム作成
// author		SyunMizuno


#pragma once


template<typename... Args>
class IDelegate {
public:
#pragma region method

	IDelegate() {}
	virtual ~IDelegate() {}

	// 実行関数
	virtual void Invoke(Args...) = 0;

#pragma endregion
};