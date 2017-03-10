// キャラクタークラス
// 2017. 1. 3	プログラム作成
// Author		SyunMizuno



#include	"Charcter.h"
#include	"../../GameFramework/GameController/GameControllerManager.h"
#include	"../../GameFramework/framework/Camera/CameraManager.h"
#include	"../../GameFramework/framework/Time/Time.h"
#include	"../../GameFramework/Obj/3D/Obj3DManager.h"
#include	"../../Archives/Archives.h"
#include	"../../GameFramework/framework/Utility/Math/MathUtility.h"


/*									//
//			コンストラクタ			//
//									*/
Character::Character() :
	m_pWeapon(NULL),
	m_pStageMesh(NULL)
{
	CreateMesh();
	SetName("Character");
}


/*									//
//			デストラクタ			//
//									*/
Character::~Character() {
}


/*									//
//				初期化				//
//									*/
void Character::Init() {
	Obj3D::Init();

	Obj3D* Field = GetObj3DManager()->FindWithTag("Stage");
	if (!Field) {
		Debug::LogError("Stageタグが見つかりません");
		return;
	}
	m_pStageMesh = Field->GetMesh();
	if (!m_pStageMesh) {
		Debug::LogError("Stageのmeshがありません");
		return;
	}

}


/*									//
//			武器の設定				//
//									*/
void Character::SetWeapon(Weapon* weapon) {
	if (m_pWeapon) {
		m_pWeapon->GetTransform()->RemoveParent();
		m_pWeapon->SetOwner(NULL);
	}
	m_pWeapon = weapon;

	if (m_pWeapon) {
		Transform* trans = m_pWeapon->GetTransform();
		trans->SetLocalPos(0.0f, 0.0f, 0.0f);
		trans->SetEularAngles(0.0f, -90.0f, 0.0f);
		trans->SetParent(GetMesh()->GetBoneTransform("Armature_Weapon"));
	}
}


/*									//
//				入力				//
//									*/
void Character::Input() {
	MoveInput("Main");
	AttackInput();
}


/*									//
//			移動の入力				//
//									*/
void Character::MoveInput(const string cameraName) {
#define	DefaultSpeed	(-0.2f)
	// キーボード
	if (Input::GetKeyPress(GetContManager()->Get()->GetKeyNum(BUTTON_UP))) {
		GetTransform()->FowardBaseRotate(GetCameraManager()->Get(cameraName)->GetTransform(), 0.0f);
		m_pTransform->Translate(0.0f, 0.0f, DefaultSpeed * Time::GetDeltaTime(), m_pTransform);
	} else
	if (Input::GetKeyPress(GetContManager()->Get()->GetKeyNum(BUTTON_RIGHT))) {
		GetTransform()->FowardBaseRotate(GetCameraManager()->Get(cameraName)->GetTransform(), 90.0f);
		m_pTransform->Translate(0.0f, 0.0f, DefaultSpeed * Time::GetDeltaTime(), m_pTransform);
	} else
	if (Input::GetKeyPress(GetContManager()->Get()->GetKeyNum(BUTTON_DOWN))) {
		GetTransform()->FowardBaseRotate(GetCameraManager()->Get(cameraName)->GetTransform(), 180.0f);
		m_pTransform->Translate(0.0f, 0.0f, DefaultSpeed * Time::GetDeltaTime(), m_pTransform);
	} else
	if (Input::GetKeyPress(GetContManager()->Get()->GetKeyNum(BUTTON_LEFT))) {
		GetTransform()->FowardBaseRotate(GetCameraManager()->Get(cameraName)->GetTransform(), 270.0f);
		m_pTransform->Translate(0.0f, 0.0f, DefaultSpeed * Time::GetDeltaTime(), m_pTransform);
	}
	// パッド
	if (Input::GetJoyAxisDegree(0) != -1000) {
		GetTransform()->FowardBaseRotate(GetCameraManager()->Get(cameraName)->GetTransform(), Input::GetJoyAxisDegree(0) + 90.f);
		m_pTransform->Translate(0.0f, 0.0f, DefaultSpeed * Time::GetDeltaTime(), m_pTransform);
	}
}


/*									//
//			壁判定確認				//
//									*/
Vector3 Character::WallCheck(Vector3& speed) {
	if (!m_pStageMesh)	return speed;

	Transform* trans = GetTransform();
	Point3 vP = trans->GetPos();
	Vector3 vX = trans->GetRight();
	Vector3 vY = trans->GetUp();
	Vector3 vZ = trans->GetForward();
	bool bHit = false;

	// 移動量
	Vector3 move =
		vX * (speed.x * Time::GetDeltaTime()) +
		vY * (speed.y * Time::GetDeltaTime()) +
		vZ * (speed.z * Time::GetDeltaTime());

	// 移動後の場所を設定
	Point3 pos = vP + move;

	//----- 地形とのレイ判定
	Point3 Cross;
	Vector3 Normal;

	// 壁判定1回目
	Obb* obb = GetCollider()->GetObb()[0];
	obb->p += move;

	// レイの点
	Point3 LDBRay = AnalyzeRayPos(pos, *obb, false, false, false);
	Point3 RDBRay = AnalyzeRayPos(pos, *obb, true, false, false);
	Point3 RDFRay = AnalyzeRayPos(pos, *obb, true, false, true);
	Point3 LDFRay = AnalyzeRayPos(pos, *obb, false, false, true);
	// OBBの左右のあたり判定を実行
	bHit = m_pStageMesh->Intersect(LDBRay, RDFRay, false, &Cross, &Normal);
	if (!bHit)	bHit = m_pStageMesh->Intersect(RDBRay, LDFRay, false, &Cross, &Normal);

	//----- めり込み判定
	float Len = 0.0f;	// めり込んだ長さ
	if (bHit) {
		bHit = OBBvsPlane(*obb, Normal, Cross, &Len);
	}

	//----- めり込んでた
	if (bHit) {
		// 貫通距離を戻す
		D3DXVec3Normalize(&Normal, &Normal);
		Vector3 RetLen = Normal * Len;
		move += RetLen;
		//----- 壁判定2回目
		LDBRay += RetLen;
		RDBRay += RetLen;
		LDFRay += RetLen;
		RDFRay += RetLen;
		// 対角線で判定
		bHit = m_pStageMesh->Intersect(LDBRay, LDFRay, false, &Cross, &Normal);
		if (!bHit)	bHit = m_pStageMesh->Intersect(RDBRay, RDFRay, false, &Cross, &Normal);

		//----- まためり込んでる
		if (bHit) {
			obb->p += (Normal * Len);
			bHit = OBBvsPlane(*obb, Normal, Cross, &Len);
			// 貫通距離を戻す
			if (bHit) {
				D3DXVec3Normalize(&Normal, &Normal);
				move += (Normal * Len);
			}
		}
	}

	return move;
}


/*									//
//		OBBよりレイ用座標解析		//
//									*/
Point3 Character::AnalyzeRayPos(
	const Point3& pos,
	const Obb& obb,
	bool x,
	bool y,
	bool z)
{
	Point3 NewPosition(0.0f, 0.0f, 0.0f);

	if (x)	NewPosition += (obb.axis[0] * obb.r[0]);
	else	NewPosition -= (obb.axis[0] * obb.r[0]);
	if (y)	NewPosition += (obb.axis[1] * obb.r[1]);
	else	NewPosition -= (obb.axis[1] * obb.r[1]);
	if (z)	NewPosition += (obb.axis[2] * obb.r[2]);
	else	NewPosition -= (obb.axis[2] * obb.r[2]);

	return pos + NewPosition;
}