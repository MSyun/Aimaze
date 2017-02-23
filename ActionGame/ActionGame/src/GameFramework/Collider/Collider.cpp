//	コライダー
// 2017. 1. 2	プログラム作成
// Author		SyunMizuno


#include	"Collider.h"
#include	"../framework/Utility/System/SystemUtility.h"
#include	"../framework/Debug/Debug.h"



/*									//
//			コンストラクタ			//
//									*/
Collider::Collider(IObj* object) :
	m_bEnable(true),
	m_pObject(object),
	m_pTree(NULL),
	m_CollisionColor(0, 255, 255, 255),
	m_TreeColor(255, 255, 0, 255),
	m_CurrentHit(0)
{
	if (!object) {
		Debug::Log("Collider : オブジェクトが存在しません");
	}
}


/*									//
//			デストラクタ			//
//									*/
Collider::~Collider() {
	DeleteSphere();
	DeleteCapsule();
	DeleteObb();
	DeleteAabb();
	DeleteTree();
	for (int i = 0; i < 2; ++i)
		m_aHitObj[i].clear();
}


/*									//
//				更新				//
//									*/
void Collider::Update(Matrix& mat) {
	HitUpdate();

	for (auto it = m_aSphere.begin(); it != m_aSphere.end(); ++it)
		UpdateCollision(**it, mat);
	for (auto it = m_aCapsule.begin(); it != m_aCapsule.end(); ++it)
		UpdateCollision(**it, mat);
	for (auto it = m_aObb.begin(); it != m_aObb.end(); ++it)
		UpdateCollision(**it, mat);
	for (auto it = m_aAabb.begin(); it != m_aAabb.end(); ++it)
		UpdateCollision(**it, mat);
	if (m_pTree)
		UpdateCollision(*m_pTree, mat);
}


/*									//
//				描画				//
//									*/
void Collider::Draw() {
	if (!m_bEnable)	return;

	for (auto it = m_aSphere.begin(); it != m_aSphere.end(); ++it)
		DrawCollision(**it, m_CollisionColor);
	for (auto it = m_aCapsule.begin(); it != m_aCapsule.end(); ++it)
		DrawCollision(**it, m_CollisionColor);
	for (auto it = m_aObb.begin(); it != m_aObb.end(); ++it)
		DrawCollision(**it, m_CollisionColor);
	for (auto it = m_aAabb.begin(); it != m_aAabb.end(); ++it)
		DrawCollision(**it, m_CollisionColor);
	// Tree
	if (m_pTree)
		DrawCollision(*m_pTree, m_TreeColor);
}

#pragma region Hit
/*									//
//			ヒットの通知			//
//									*/
void Collider::Hit(Collider* target) {
	if (!m_bEnable)	return;
	m_aHitObj[m_CurrentHit].push_back(target);
}


/*									//
//			ヒットの初期化			//
//									*/
void Collider::ResetHit() {
	for (int i = 0; i < 2; ++i)
		m_aHitObj[i].clear();
}


/*									//
//			ヒットの更新			//
//									*/
void Collider::HitUpdate() {
	m_CurrentHit = (m_CurrentHit + 1) % 2;
	m_aHitObj[m_CurrentHit].clear();
}
#pragma endregion

#pragma region Check
/*									//
//			ヒットのトリガー		//
//									*/
bool Collider::CheckCollTrigger(Collider* target) {
	if (!m_bEnable)	return false;

	if (CheckCurrentHit(target) &&
		!CheckPrevHit(target)) {
		return true;
	}

	return false;
}


/*									//
//			ヒットのステイ			//
//									*/
bool Collider::CheckCollStay(Collider* target) {
	if (!m_bEnable)	return false;

	if (CheckCurrentHit(target) &&
		CheckPrevHit(target)) {
		return true;
	}

	return false;
}


/*									//
//			ヒットのリリース		//
//									*/
bool Collider::CheckCollRelease(Collider* target) {
	if (!m_bEnable)	return false;

	if (!CheckCurrentHit(target) &&
		CheckPrevHit(target)) {
		return true;
	}

	return false;
}
#pragma endregion

#pragma region Set
/*									//
//				球設定				//
//									*/
void Collider::SetSphere(Sphere* sphere) {
	if (!sphere)	return;

	m_aSphere.push_back(sphere);
	CreateTree();
}


/*									//
//			カプセル設定			//
//									*/
void Collider::SetCapsule(Capsule* capsule) {
	if (!capsule)	return;

	m_aCapsule.push_back(capsule);
	CreateTree();
}


/*									//
//				OBB設定				//
//									*/
void Collider::SetObb(Obb* obb) {
	if (!obb)	return;

	m_aObb.push_back(obb);
	CreateTree();
}


/*									//
//				AABB設定			//
//									*/
void Collider::SetAabb(Aabb* aabb) {
	if (!aabb)	return;

	m_aAabb.push_back(aabb);
	CreateTree();
}


/*									//
//				8分木設定			//
//									*/
void Collider::SetTree(const Aabb& aabb) {
	CreateTree();
	m_pTree->p_d = aabb.p_d;
	for (int i = 0; i < 3; i ++)
		m_pTree->r[i] = aabb.r[i];
}
#pragma endregion

#pragma region Color
/*									//
//			あたり判定の色設定		//
//									*/
void Collider::SetCollisionColor(Color color) {
	m_CollisionColor = color;
}


/*									//
//			あたり判定の色設定		//
//									*/
void Collider::SetCollisionColor(float r, float g, float b, float a) {
	m_CollisionColor.r = r;
	m_CollisionColor.g = g;
	m_CollisionColor.b = b;
	m_CollisionColor.a = a;
}


/*									//
//		8分木のあたり判定の色設定	//
//									*/
void Collider::SetTreeColor(Color color) {
	m_TreeColor = color;
}


/*									//
//		8分木のあたり判定の色設定	//
//									*/
void Collider::SetTreeColor(float r, float g, float b, float a) {
	m_TreeColor.r = r;
	m_TreeColor.g = g;
	m_TreeColor.b = b;
	m_TreeColor.a = a;
}
#pragma endregion

#pragma region conceal method
/*									//
//		現フレームのヒット確認		//
//									*/
bool Collider::CheckCurrentHit(Collider* target) {
	if (!m_bEnable)	return false;

	for (auto it = m_aHitObj[m_CurrentHit].begin(); it != m_aHitObj[m_CurrentHit].end(); ++it)
		if ((*it) == target)
			return true;

	return false;
}

/*									//
//		前フレームのヒット確認		//
//									*/
bool Collider::CheckPrevHit(Collider* target) {
	if (!m_bEnable)	return false;

	int Num = (m_CurrentHit + 1) % 2;
	for (auto it = m_aHitObj[Num].begin(); it != m_aHitObj[Num].end(); ++it)
		if ((*it) == target)
			return true;

	return false;
}

/*									//
//				8分木の作成			//
//									*/
void Collider::CreateTree() {
	if (m_pTree)	return;
	m_pTree = new Aabb;

	//m_pTreeColl->SetMat(&GetWorld());

	// 初期半径設定
	for (int i = 0; i < 3; i ++)
		m_pTree->r[i] = 3.0f;
}

#pragma region Delete
/*									//
//				球削除				//
//									*/
void Collider::DeleteSphere() {
	for (auto it = m_aSphere.begin(); it != m_aSphere.end(); ++it) {
		SAFE_DELETE(*it);
	}
	m_aSphere.clear();
}


/*									//
//			カプセル削除			//
//									*/
void Collider::DeleteCapsule() {
	for (auto it = m_aCapsule.begin(); it != m_aCapsule.end(); ++it) {
		SAFE_DELETE(*it);
	}
	m_aCapsule.clear();
}


/*									//
//				OBB削除				//
//									*/
void Collider::DeleteObb() {
	for (auto it = m_aObb.begin(); it != m_aObb.end(); ++it) {
		SAFE_DELETE(*it);
	}
	m_aObb.clear();
}


/*									//
//				AABB削除			//
//									*/
void Collider::DeleteAabb() {
	for (auto it = m_aAabb.begin(); it != m_aAabb.end(); ++it) {
		SAFE_DELETE(*it);
	}
	m_aAabb.clear();
}


/*									//
//				8分木削除			//
//									*/
void Collider::DeleteTree() {
	SAFE_DELETE(m_pTree);
}
#pragma endregion

#pragma endregion