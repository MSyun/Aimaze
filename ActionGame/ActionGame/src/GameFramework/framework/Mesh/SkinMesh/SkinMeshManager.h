// スキンメッシュマネージャークラス
// 2015.11.28	プロジェクト作成
// 2016. 1.13	Singletonへ変更
// 2016. 9. 3	解放確認用ヘルパー変数追加
// author		SyunMizuno

#pragma once

#include	<map>
#include	<string>
#include	"../../Singleton/Singleton.h"
#include	"SkinMesh.h"

using namespace std;

/*
	memo:
		ボーン合成行列		コピー
		マテリアル			コピー
		コントローラー		コピー
		フレーム			参照
		コンテナ			コピー
		頂点（ポリゴン）	参照
		スキン				参照
		テクスチャ			参照
*/



class SkinMeshManagerBase {
protected:
	map< string, _tSkin* >	m_MapSkin;
	bool m_bHelper;

public:
	SkinMeshManagerBase();
	virtual ~SkinMeshManagerBase();

	// 作成
	_tSkin* Create(const string FileName);

	// 削除
	HRESULT Delete(const string Name);

	// 取得
	_tSkin* Get(const string Name);

	void SetHelper(bool helper) { m_bHelper = helper; }

protected:
	// 指定メッシュの削除
	void Release(_tSkin* skin);
};

class SkinMeshManager : public	SkinMeshManagerBase, public	Singleton<SkinMeshManager> {
public:
	SkinMeshManager() : SkinMeshManagerBase() {}
};

// これで使用しやすくなる
inline SkinMeshManager* GetSkinMeshManager(void) {
	return SkinMeshManager::Instance();
}