//////////////////////////////////////////////////////////////
// 3Dオブジェクトクラス
// 2016. 5.19	プログラム作成
// 2016. 9.14	オクツリー用コリジョン追加
// 2017. 1.23	アニメーター追加
// Author		SyunMizuno
//////////////////////////////////////////////////////////////

#pragma once


#include	"../Obj.h"
#include	"../../framework/Convert.h"
#include	"../../framework/Mesh/SkinMesh/SkinMesh.h"
#include	"../../Transform/Transform.h"
#include	"../../Collider/Collider.h"
#include	"../../Animator/IAnimator.h"


using namespace std;

/*
	memo	:
			HLSLのタイプを設定すること
			アニメーターはメッシュの中に隠蔽したほうがよい
*/


class	Obj3D : public	IObj {
private:
#pragma region conceal variable

	_shadertype m_ShaderType;	// シェーダタイプ

#pragma endregion

protected:
#pragma region variable

	Transform*	m_pTransform;	// 行列類
	Collider*	m_pCollider;	// あたり判定
	SkinMesh*	m_pMesh;		// メッシュ
	IAnimator*	m_pAnimator;	// アニメーター

#pragma endregion

public:
#pragma region method

	Obj3D();
	virtual ~Obj3D();

	virtual void UpdateAll();
	virtual void LateUpdateAll();
	virtual void DrawAll();
	virtual void LateDrawAll();
	virtual void DrawShadow();

	Transform* GetTransform() { return m_pTransform; }
	Collider* GetCollider() { return m_pCollider; }
	SkinMesh* GetMesh() { return m_pMesh; }

	virtual void OnCollisionTrigger(Obj3D* obj) {}
	virtual void OnCollisionStay(Obj3D* obj) {}
	virtual void OnCollisionRelease(Obj3D* obj) {}


#pragma region Collider
	void CreateCollider();
	void DeleteCollider();
#pragma endregion

#pragma region Mesh
	void CreateMesh();				// 作成
	void DeleteMesh();				// 削除
	void SetModel(_tSkin* pSkin, IAnimator* pAnimator = NULL);	// モデルの設定
#pragma endregion

#pragma region Shader Type
	_shadertype GetShaderType() { return m_ShaderType; }			// シェーダタイプ取得
	void SetShaderType(_shadertype type) { m_ShaderType = type; }	// シェーダタイプ設定
#pragma endregion

	virtual void Destroy();

	virtual void SetActive(bool active);

#pragma endregion

protected:
#pragma region conceal method

	virtual void Init() {
		if(m_pAnimator)
			m_pAnimator->AnimatorSetting(&m_pMesh->GetCont());
	};
	virtual void Update() {}
	virtual void LateUpdate() {}
	virtual void Draw() {}
	virtual void LateDraw() {}

	// メッシュの更新
	void UpdateMesh();

#pragma endregion
};