// アニメーターインターフェース
// 2017. 1.23	プログラム作成
// Author		SyunMizuno

#pragma once


#include	"../framework/Mesh/SkinMesh/HighLevelAnimController.h"
#include	"../framework/Debug/Debug.h"
#include	"../framework/Time/Time.h"
#include	<map>
using namespace std;

/*
	Memo	:
			アニメーションの設定を管理
*/


class IAnimator {
protected:
	typedef bool trigger;
	HighLevelAnimController* m_Controller;

private:
	map<string, bool*>	m_Bool;
	map<string, int*>	m_Int;
	map<string, float*>	m_Float;
	map<string, trigger*>	m_Trigger;

	// 変更予約用
	float m_ChangeTime;
	string m_ChangeAnim;

public:
#pragma region method

	IAnimator();
	virtual ~IAnimator();

	// セッティング
	void AnimatorSetting(HighLevelAnimController* Cont);

	// 更新
	void Update();

#pragma region Getter/Setter
	// boolの取得
	bool GetBool(string name);

	// intの取得
	int GetInt(string name);

	// floatの取得
	float GetFloat(string name);

	// boolの設定
	void SetBool(string name, bool value);

	// intの設定
	void SetInt(string name, int value);

	// floatの設定
	void SetFloat(string name, float value);

	// triggerの設定
	void SetTrigger(string name);
#pragma endregion

	// アニメーションの予約
	void ChangeTime(float time, string anim);

#pragma endregion

protected:
#pragma region conceal method

	virtual void ShiftTime() {}
	virtual void LoopTime() {}
	virtual void Loop() {}
	virtual void Period() {}
	virtual void State() = 0;
	virtual void StartState() {}

#pragma region Add
	// boolの追加
	void AddBool(string name, bool* value);

	// intの追加
	void AddInt(string name, int* value);

	// floatの追加
	void AddFloat(string name, float* value);

	// triggerの追加
	void AddTrigger(string name, trigger* value);
#pragma endregion

#pragma endregion
};