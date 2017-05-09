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
#pragma region variable

	map< string, Camera* >	m_MapCamera;
	bool m_bHelper;

	string m_CurCameraName;

#pragma endregion

public:
#pragma region method

	CameraManagerBase();
	virtual ~CameraManagerBase();

	/* 作成
	// Name	:	生成するカメラの名前
	// return	: 生成したカメラのオブジェクト
	*/
	Camera* Create(const string Name);

	/* 削除
	// Name	:	削除するカメラの名前
	// return	:	S_OK. 成功、E_FAIL. 失敗
	*/
	HRESULT Delete(const string Name);

	/* 取得
	// Name	:	取得するカメラの名前
	// return	:	対応したカメラのインスタンス
	*/
	Camera* Get(const string Name);

	/* セット
	// Name	:	設定するカメラの名前
	// return	:	true. 成功、false. 失敗
	*/
	bool Set(const string Name);

	/* 最後に使用したものを取得
	// return	:	対応したカメラのインスタンス
	*/
	Camera* CurrentCamera() { return Get(m_CurCameraName); }

	/* 管理オブジェクトを自動で削除するか設定 */
	void SetHelper(bool helper) { m_bHelper = helper; }

#pragma endregion
};

class CameraManager : public	CameraManagerBase, public	Singleton<CameraManager> {
public:
	CameraManager() : CameraManagerBase() {}
};

// これで使用しやすくなる
inline CameraManager* GetCameraManager(void) {
	return CameraManager::Instance();
}