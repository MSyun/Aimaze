// テクスチャマネージャークラス
// 2015.11.14	プロジェクト作成
// 2016. 1.13	Singletonへ変更
// 2016. 1.30	Flyweightパターンへ変更
// 製作者		SyunMizuno
// 参照サイト	http://www.c3.club.kyutech.ac.jp/gamewiki/index.php?%A5%B2%A1%BC%A5%E0%C5%AA%A5%AF%A5%E9%A5%B9%C0%DF%B7%D7

#include	"TexManager.h"
#include	<tchar.h>
#include	"../Utility/System/SystemUtility.h"
#include	"../Debug/Debug.h"
#include	"../Graphic/Graphics.h"


/*									//
//			コンストラクタ			//
//									*/
TexManagerBase::TexManagerBase() :
	m_bHelper(true)
{
	m_MapTexture.clear();
}


/*									//
//			デストラクタ			//
//									*/
TexManagerBase::~TexManagerBase() {
	if (!m_bHelper)		return;

	map< string, LPDIRECT3DTEXTURE9 >::iterator p = m_MapTexture.begin();

	//----- 全要素削除
	while (p != m_MapTexture.end()) {
		SAFE_RELEASE(p->second);
		++p;
	}

	m_MapTexture.clear();
}


/*									//
//			テクスチャを作成		//
//									*/
LPDIRECT3DTEXTURE9 TexManagerBase::Create(const string FileName) {
	if (FileName.empty()) {
		Debug::LogError("ファイル名" + FileName + "がありません");
		return NULL;
	}

	//----- 既に登録されているかCheck
	if (m_MapTexture.end() != m_MapTexture.find(FileName)) {
		Debug::Log("テクスチャ" + FileName + "既に登録されています");
		return Get(FileName);
	}

	//----- 変数宣言
	LPDIRECT3DTEXTURE9	texture;

	//----- テクスチャを作成
	HRESULT hr;
	hr = D3DXCreateTextureFromFile(GetGraphics()->GetDevice(), FileName.c_str(), &texture);
	if (hr != S_OK) {
		MessageBox(NULL, _T("テクスチャの読み込みに失敗しました"), FileName.c_str(), MB_OK);
		return NULL;
	}
	// リストに登録
	m_MapTexture.insert(pair<string, LPDIRECT3DTEXTURE9>(FileName, texture));

	return texture;
}


/*									//
//			テクスチャを削除		//
//									*/
HRESULT TexManagerBase::Delete(const string Name) {
	//----- 初期化
	map< string, LPDIRECT3DTEXTURE9 >::iterator p = m_MapTexture.find(Name);

	// 登録されていない場合
	if (p == m_MapTexture.end()) {
		Debug::Log("テクスチャ" + Name + "は登録されていないため削除できません");
		return E_FAIL;
	}

	// 要素を削除
	p->second->Release();
	p->second = NULL;

	// リストから解放
	m_MapTexture.erase(Name);

	return S_OK;
}


/*									//
//			テクスチャを取得		//
//									*/
LPDIRECT3DTEXTURE9 TexManagerBase::Get(const string Name) {
	// イテレータの作成
	map< string, LPDIRECT3DTEXTURE9 >::iterator p = m_MapTexture.find(Name);

	// 登録されてない場合
	if (p == m_MapTexture.end()) {
		return Create(Name);
	}

	return p->second;
}