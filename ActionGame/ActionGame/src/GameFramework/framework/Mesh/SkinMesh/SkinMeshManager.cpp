// スキンメッシュマネージャークラス
// 2015.11.28	プロジェクト作成
// 2016. 1.13	Singletonへ変更
// 2016. 1.30	Flyweightパターンへ変更
// 2016. 2. 6	完成
// author		SyunMizuno

#pragma warning( disable : 4996 )

#include	"SkinMeshManager.h"
#include	<tchar.h>
#include	"MyAllocHierarchy.h"
#include	"HighLevelAnimController.h"
#include	"../../Debug/Debug.h"
#include	"../../Graphic/Graphics.h"


/*									//
//			コンストラクタ			//
//									*/
SkinMeshManagerBase::SkinMeshManagerBase() :
	m_bHelper(true)
{
	m_MapSkin.clear();
}


/*									//
//			デストラクタ			//
//									*/
SkinMeshManagerBase::~SkinMeshManagerBase() {
	if (!m_bHelper)	return;

	map< string, _tSkin* >::iterator p = m_MapSkin.begin();

	//----- 全要素削除
	while (p != m_MapSkin.end()) {
		Release(p->second);
		++p;
	}
	m_MapSkin.clear();
}


/*									//
//			メッシュを作成			//
//									*/
_tSkin* SkinMeshManagerBase::Create(const string FileName) {
	if (m_MapSkin.end() != m_MapSkin.find(FileName)) {
		Debug::Log("メッシュファイル名" + FileName + "はすでに生成されています");
		return Get(FileName);
	}

	//----- メッシュを作成
	// 変数宣言
	_tSkin *SkinMesh = new _tSkin;
	MyAllocHierarchy	Hierarchy;

	// ディレクトリを抽出
	TCHAR szDir[_MAX_PATH];
	TCHAR szDirWk[_MAX_DIR];
	_tsplitpath(FileName.c_str(), szDir, szDirWk, NULL, NULL);
	lstrcat(szDir, szDirWk);
	Hierarchy.SetDirectory(szDir);

	// メッシュ読み込み
	if (FAILED(D3DXLoadMeshHierarchyFromX(
		FileName.c_str(),
		D3DXMESH_MANAGED,
		GetGraphics()->GetDevice(),
		&Hierarchy,
		NULL,
		&SkinMesh->pFrameRoot,
		&SkinMesh->pController)))
	{
		MessageBox(NULL, _T("メッシュの読み込みに失敗しました"), FileName.c_str(), MB_OK);
		delete SkinMesh;
		return NULL;
	}

	// フレーム内の生成
	Hierarchy.FindSkin(SkinMesh->pFrameRoot, SkinMesh->pFrameRoot);

	// リストに登録
	m_MapSkin.insert(pair<string, _tSkin*>(FileName, SkinMesh));

	return SkinMesh;
}


/*									//
//			メッシュの削除			//
//									*/
HRESULT SkinMeshManagerBase::Delete(const string Name) {
	//----- 初期化
	map< string, _tSkin* >::iterator p = m_MapSkin.find(Name);

	if (p == m_MapSkin.end()) {
		Debug::Log("メッシュファイル名" + Name + "は登録されていないため削除できません");
		return E_FAIL;
	}

	Release(p->second);

	m_MapSkin.erase(Name);

	return S_OK;
}


/*									//
//			メッシュの取得			//
//									*/
_tSkin* SkinMeshManagerBase::Get(const string Name) {
	map< string, _tSkin* >::iterator p = m_MapSkin.find(Name);

	if (p == m_MapSkin.end()) {
		return Create(Name);
	}

	return p->second;
}


/*									//
//			指定メッシュの削除		//
//									*/
void SkinMeshManagerBase::Release(_tSkin* skin) {
	MyAllocHierarchy	Hierarchy;

	if (skin->pFrameRoot != NULL)
		Hierarchy.DestroyFrame(skin->pFrameRoot);

	if (skin->pController != NULL) {
		skin->pController->Release();
		skin->pController = NULL;
	}

	delete skin;
	skin = NULL;
}