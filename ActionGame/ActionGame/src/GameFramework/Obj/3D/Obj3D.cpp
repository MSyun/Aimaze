//////////////////////////////////////////////////////////////
// 3Dオブジェクトクラス
// 2016. 5.19	プログラム作成
// Author		SyunMizuno
//////////////////////////////////////////////////////////////


#include	"Obj3D.h"
#include	"../../framework/Time/Time.h"

#include	"Obj3DManager.h"
#include	"../../framework/Utility/System/SystemUtility.h"



/*									//
//			コンストラクタ			//
//									*/
Obj3D::Obj3D() :
	m_pMesh(NULL),
	m_pAnimator(NULL),
	m_ShaderType(_shadertype::SHADER_HALFLAMBERT),
	m_pTransform(NULL),
	m_pCollider(NULL)
{
	m_pTransform = new Transform(this);
	GetObj3DManager()->Add(this);
}


/*									//
//			デストラクタ			//
//									*/
Obj3D::~Obj3D() {
	//----- 削除
	DeleteMesh();		// メッシュ
	DeleteCollider();
	// マネージャーにお知らせ
	GetObj3DManager()->Delete(this);
	SAFE_DELETE(m_pTransform);
}


/*									//
//			メッシュの更新			//
//									*/
void Obj3D::UpdateMesh() {
	if (!m_pMesh)	return;

	// アニメーション
	float framerate = (float)Time::GetSettingFrameRate();
	m_pMesh->GetCont().AdvanceTime(60.0f * (60.0f / framerate) / 4800.0f * Time::GetDeltaTime() * framerate);

	// アニメーター
	if (m_pAnimator)
		m_pAnimator->Update();

	// 更新
	m_pMesh->Update(m_pTransform->GetWorld());
}


/*									//
//				全更新				//
//									*/
void Obj3D::UpdateAll() {
	if (!GetActive())	return;

	IObj::UpdateAll();
	if (GetOperate())	Input();
	UpdateMesh();
	Update();

	vector<Transform*>* childs = m_pTransform->GetChilds();
	for (auto it = childs->begin(); it != childs->end(); ++it) {
		(*it)->GetGameObject()->UpdateAll();
	}
}
void Obj3D::LateUpdateAll() {
	if (!GetActive())	return;

	LateUpdate();
	if (m_pMesh)	m_pMesh->LateUpdate();

	vector<Transform*>* childs = m_pTransform->GetChilds();
	for (auto it = childs->begin(); it != childs->end(); ++it) {
		(*it)->GetGameObject()->LateUpdateAll();
	}

	if(m_pCollider)	m_pCollider->Update(m_pTransform->GetWorld());
}


/*									//
//				全描画				//
//									*/
void Obj3D::DrawAll() {
	if (!GetActive())	return;

	Draw();
	if (m_pMesh)		m_pMesh->Draw(m_ShaderType);
	if (m_pCollider)	m_pCollider->Draw();

	vector<Transform*>* childs = m_pTransform->GetChilds();
	for (auto it = childs->begin(); it != childs->end(); ++it) {
		(*it)->GetGameObject()->DrawAll();
	}
}
void Obj3D::LateDrawAll() {
	if (!GetActive())	return;

	LateDraw();
	if (m_pMesh)	m_pMesh->LateDraw(m_ShaderType);

	vector<Transform*>* childs = m_pTransform->GetChilds();
	for (auto it = childs->begin(); it != childs->end(); ++it) {
		(*it)->GetGameObject()->LateDrawAll();
	}
}
void Obj3D::DrawShadow() {
	if (!GetActive() || !GetShadow())	return;

	// 影の描画
	if (m_pMesh) {
		m_pMesh->DrawShadow();
	} else {
		// 丸影
	}

	vector<Transform*>* childs = m_pTransform->GetChilds();
	for (auto it = childs->begin(); it != childs->end(); ++it) {
		(*it)->GetGameObject()->DrawShadow();
	}
}

#pragma region Collider
/*									//
//			あたり判定作成			//
//									*/
void Obj3D::CreateCollider() {
	if (!m_pCollider)
		m_pCollider = new Collider(this);
}


/*									//
//			あたり判定削除			//
//									*/
void Obj3D::DeleteCollider() {
	SAFE_DELETE(m_pCollider);
}
#pragma endregion

#pragma region Mesh
/*									//
//			メッシュ作成			//
//									*/
void Obj3D::CreateMesh() {
	if (!m_pMesh)
		m_pMesh = new SkinMesh;
}


/*									//
//			メッシュ削除			//
//									*/
void Obj3D::DeleteMesh() {
	SAFE_DELETE(m_pMesh);
	SAFE_DELETE(m_pAnimator);
}


/*									//
//			メッシュ設定			//
//									*/
void Obj3D::SetModel(_tSkin* pSkin, IAnimator* pAnimator) {
	CreateMesh();
	m_pMesh->ManagerGet(pSkin);
	m_pAnimator = pAnimator;
}
#pragma endregion


/*									//
//				削除通知			//
//									*/
void Obj3D::Destroy() {
	IObj::Destroy();
	m_pTransform->Destroy();
}


/*									//
//			アクティブの設定		//
//									*/
void Obj3D::SetActive(bool active) {
	IObj::SetActive(active);
	if (!active) {
		if (m_pCollider)
			m_pCollider->ResetHit();
	}
}