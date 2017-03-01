// ライトマネージャークラス
// 2016. 3.27	プログラム作成
// 2017. 1.22	デバッグログ追加
// author		SyunMizuno


#include	"LightManager.h"
#include	"../Debug/Debug.h"

/*									//
//			コンストラクタ			//
//									*/
LightManagerBase::LightManagerBase() :
	m_bCustom(false),
	m_bHelper(true)
{
	m_MapLight.clear();
}


/*									//
//			デストラクタ			//
//									*/
LightManagerBase::~LightManagerBase() {
	if (!m_bHelper)		return;

	m_MapLight.clear();
}


/*									//
//			全ライトを更新			//
//									*/
void LightManagerBase::AllSet() {
	map< string, Light* >::iterator p = m_MapLight.begin();

	while (p != m_MapLight.end()) {
		p->second->Set(m_bCustom);
		++p;
	}
}


/*									//
//			ライトを作成			//
//									*/
Light* LightManagerBase::Create(const string Name) {
	//----- 既に登録されているかCheck
	if (m_MapLight.end() != m_MapLight.find(Name)) {
		Debug::Log("ライト名" + Name + "はすでに生成されています");
		return Get(Name);
	}

	//----- 作成
	Light*	light = new Light;

	// リストに登録
	m_MapLight.insert(pair<string, Light*>(Name, light));

	return light;
}


/*									//
//			ライトの削除			//
//									*/
// <return> S_OK. 成功 : E_FAIL. 失敗
HRESULT LightManagerBase::Delete(const string Name) {
	//----- 初期化
	map< string, Light* >::iterator p = m_MapLight.find(Name);

	// 登録されていない場合
	if (p == m_MapLight.end()) {
		Debug::Log("ライト名" + Name + "は登録されていないため削除できません");
		return E_FAIL;
	}

	// 要素を削除
	delete p->second;
	p->second = NULL;

	m_MapLight.erase(Name);

	return S_OK;
}


/*									//
//			ライトの取得			//
//									*/
Light* LightManagerBase::Get(const string Name) {
	map< string, Light* >::iterator p = m_MapLight.find(Name);

	if (p == m_MapLight.end()) {
		return Create(Name);
	}

	return p->second;
}