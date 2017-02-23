// カメラマネージャークラス
// 2016. 3.27	プログラム作成
// 2017. 1. 4	データ削除をObjManagerへ一任
// 製作者		SyunMizuno


#include	"CameraManager.h"
#include	"../Debug/Debug.h"

/*									//
//			コンストラクタ			//
//									*/
CameraManagerBase::CameraManagerBase() :
	m_bHelper(true),
	m_CurCameraName("")
{
	m_MapCamera.clear();
}


/*									//
//			デストラクタ			//
//									*/
CameraManagerBase::~CameraManagerBase() {
	if (!m_bHelper)		return;

	// 要素解放
	m_MapCamera.clear();
}


/*									//
//			カメラを作成			//
//									*/
Camera* CameraManagerBase::Create(const string Name) {
	if (m_MapCamera.end() != m_MapCamera.find(Name)) {
		Debug::Log("カメラ名" + Name + "はすでに生成されています");
		return Get(Name);
	}

	Camera* camera = new Camera;
	camera->SetName(Name + "Camera");
	m_MapCamera.insert(pair<string, Camera*>(Name, camera));

	return camera;
}


/*									//
//			カメラの削除			//
//									*/
HRESULT CameraManagerBase::Delete(const string Name) {
	//----- 初期化
	map< string, Camera* >::iterator p = m_MapCamera.find(Name);

	if (p == m_MapCamera.end()) {
		Debug::Log("カメラ名" + Name + "は登録されていないため削除できません");
		return E_FAIL;
	}

	delete p->second;
	p->second = NULL;

	m_MapCamera.erase(Name);

	return S_OK;
}


/*									//
//			カメラの取得			//
//									*/
Camera* CameraManagerBase::Get(const string Name) {
	map< string, Camera* >::iterator p = m_MapCamera.find(Name);

	if (p == m_MapCamera.end()) {
		return Create(Name);
	}

	return p->second;
}


/*									//
//			カメラの設定			//
//									*/
bool CameraManagerBase::Set(const string Name) {
	if (Name.empty()) {
		Debug::LogError("カメラの名前がありません");
		return false;
	}

	Camera* camera = Get(Name);
	if (!camera) {
		Debug::LogError("カメラ名" + Name + "は存在しませんでした");
		return false;
	}

	camera->Set();
	m_CurCameraName = Name;
	return true;
}