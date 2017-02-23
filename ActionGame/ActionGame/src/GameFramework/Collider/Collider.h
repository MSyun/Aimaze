//	コライダー
// 2017. 1. 2	プログラム作成
// Author		SyunMizuno



#pragma once


#include	<vector>
#include	"../framework/Collision/Collision.h"
#include	"../framework/Convert.h"
#include	"../Obj/Obj.h"


using namespace std;


class Collider {
private:
#pragma region variable

	bool	m_bEnable;

	IObj*	m_pObject;

	int		m_CurrentHit;
	vector<Collider*>	m_aHitObj[2];

	vector<Sphere*>		m_aSphere;
	vector<Capsule*>	m_aCapsule;
	vector<Obb*>		m_aObb;
	vector<Aabb*>		m_aAabb;

	Color	m_CollisionColor;

	Aabb*	m_pTree;
	Color	m_TreeColor;

#pragma endregion

public:
#pragma region method

	Collider(IObj* object);
	virtual ~Collider();

	// 更新
	void Update(Matrix& mat);

	// 描画
	void Draw();

	IObj* GetGameObject() { return m_pObject; }

#pragma region Hit
	// ヒットの通知
	void Hit(Collider* target);
	// ヒットの初期化
	void ResetHit();
	// ヒットの更新
	void HitUpdate();
#pragma endregion

#pragma region Check
	// ヒットのトリガー
	bool CheckCollTrigger(Collider* target);
	// ヒットのステイ
	bool CheckCollStay(Collider* target);
	// ヒットのリリース
	bool CheckCollRelease(Collider* target);
#pragma endregion

#pragma region Get
	// 球取得
	vector<Sphere*> GetSphere() { return m_aSphere; }
	// カプセル取得
	vector<Capsule*> GetCapsule() { return m_aCapsule; }
	// OBB取得
	vector<Obb*> GetObb() { return m_aObb; }
	// AABB取得
	vector<Aabb*> GetAabb() { return m_aAabb; }
	// 8分木取得
	Aabb* GetTreeColl() { return m_pTree; }
#pragma endregion

#pragma region Set
	// 球設定
	void SetSphere(Sphere* sphere);
	// OBB設定
	void SetObb(Obb* obb);
	// カプセル設定
	void SetCapsule(Capsule* capsule);
	// AABB設定
	void SetAabb(Aabb* aabb);
	// 8分木設定
	void SetTree(const Aabb& aabb);

	void SetEnable(bool flg) { m_bEnable = flg; }
#pragma endregion

#pragma region Color
	// あたり判定の色設定
	void SetCollisionColor(Color color);
	void SetCollisionColor(float r, float g, float b, float a);
	// 8分木のあたり判定の色設定
	void SetTreeColor(Color color);
	void SetTreeColor(float r, float g, float b, float a);
#pragma endregion

#pragma endregion

private:
#pragma region conceal method
	// 現フレームのヒット確認
	bool CheckCurrentHit(Collider* target);
	// 前フレームのヒット確認
	bool CheckPrevHit(Collider* target);
	// 8分木の作成
	void CreateTree();

#pragma region Delete
	// 球削除
	void DeleteSphere();
	// OBB削除
	void DeleteObb();
	// カプセル削除
	void DeleteCapsule();
	// AABB削除
	void DeleteAabb();
	// 8分木削除
	void DeleteTree();
#pragma endregion

#pragma endregion
};