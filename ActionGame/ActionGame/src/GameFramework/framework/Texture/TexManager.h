// テクスチャマネージャークラス
// 2015.11.14	プロジェクト作成
// 2016. 1.13	Singletonへ変更
// 2016. 9. 3	解放確認用ヘルパー変数追加
// author		SyunMizuno
// 参照サイト	http://www.c3.club.kyutech.ac.jp/gamewiki/index.php?%A5%B2%A1%BC%A5%E0%C5%AA%A5%AF%A5%E9%A5%B9%C0%DF%B7%D7

#pragma once

#include	<map>
#include	<d3dx9.h>
#include	<string>
#include	"../Singleton/Singleton.h"

#pragma comment ( lib, "d3d9.lib" )		// 描画処理に必要

using namespace std;


class TexManagerBase {
protected:
	map< string, LPDIRECT3DTEXTURE9 >	m_MapTexture;
	bool m_bHelper;

public:
	TexManagerBase();
	virtual ~TexManagerBase();

	// 作成
	LPDIRECT3DTEXTURE9 Create(const string FileName);

	// 削除
	HRESULT Delete(const string Name);

	// 取得
	LPDIRECT3DTEXTURE9 Get(const string Name);

	void SetHelper(bool helper) { m_bHelper = helper; }
};

class TexManager : public	TexManagerBase, public	Singleton<TexManager> {
public:
	TexManager() : TexManagerBase() {}
};

// これで使用しやすくなる
inline TexManager* GetTexManager(void) {
	return TexManager::Instance();
}