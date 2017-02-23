// カメラマネージャークラス
// 2016. 4.14	プログラム作成
// 2016. 9. 3	解放確認用ヘルパー変数追加
// author		SyunMizuno

#pragma once


#include	<map>
#include	<string>
#include	"Camera.h"
#include	"../Singleton/Singleton.h"

using namespace std;


class CameraManagerBase {
protected:
	map< string, Camera* >	m_MapCamera;
	bool m_bHelper;

	string m_CurCameraName;

public:
	CameraManagerBase();
	virtual ~CameraManagerBase();

	// 作成
	Camera* Create(const string Name);

	// 削除
	HRESULT Delete(const string Name);

	// 取得
	Camera* Get(const string Name);

	// セット
	bool Set(const string Name);

	// 最後に使用したものを取得
	Camera* CurrentCamera() { return Get(m_CurCameraName); }

	void SetHelper(bool helper) { m_bHelper = helper; }
};

class CameraManager : public	CameraManagerBase, public	Singleton<CameraManager> {
public:
	CameraManager() : CameraManagerBase() {}
};

// これで使用しやすくなる
inline CameraManager* GetCameraManager(void) {
	return CameraManager::Instance();
}