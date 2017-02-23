// ライトマネージャークラス
// 2016. 3.27	プログラム作成
// 2016. 9. 3	解放確認用ヘルパー変数追加
// 2017. 2.23	メンバからデバイスを排除
// author		SyunMizuno

#pragma once

#include	<map>
#include	<string>
#include	"Light.h"
#include	"../Singleton/Singleton.h"

using namespace std;

class LightManagerBase {
protected:
	map< string, Light* >	m_MapLight;

	// 固定機能使用フラグ
	bool m_bCustom;

	bool m_bHelper;

public:
	LightManagerBase();
	virtual ~LightManagerBase();

	// 全更新
	void AllSet();

	// 作成
	Light* Create(const string Name);

	// 削除
	HRESULT Delete(const string Name);

	// 取得
	Light* Get(const string Name);

	void SetCustom(bool _custom) { m_bCustom = _custom; }

	void SetHelper(bool helper) { m_bHelper = helper; }
};

class LightManager : public	LightManagerBase, public	Singleton<LightManager> {
public:
	LightManager() : LightManagerBase() {}
};

// これで使用しやすくなる
inline LightManager* GetLightManager(void) {
	return LightManager::Instance();
}