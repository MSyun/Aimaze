// オオカミ
// 2017. 1.26	プログラム作成
// Author		SyunMizuno


#include	"Wolf.h"
#include	"../Archives/Archives.h"
#include	"../GameFramework/framework/Mesh/SkinMesh/SkinMeshManager.h"
#include	"../Animator/WolfAnimator.h"
#include	"../GameFramework/framework/Time/Time.h"
#include	"../GameFramework/GameController/GameControllerManager.h"
#include	"../GameFramework/framework/Camera/CameraManager.h"



const float SCALE = 0.2f;
const float ATTACK_DIST = 10.0f;
const float MOVE_DIST = 20.0f;
const float INTERVAL_TIME = 2.0f;



/*									//
//			コンストラクタ			//
//									*/
Wolf::Wolf(int max) :
	Enemy(max),
	m_bAttack(false),
	m_bInterval(false),
	m_fIntervalCnt(0.0f)
{
	SetName("Wolf");
	SetShaderType(SHADER_TOON);

	SetModel(GetSkinMeshManager()->Get(Archives::Mesh("Wolf")), new WolfAnimator);
	GetTransform()->SetScale(SCALE, SCALE, SCALE);

	CreateCollider();
	Collider* collider = GetCollider();
	// Obb
	Obb* obb = new Obb;
	obb->r[0] = 3.5f * SCALE;
	obb->r[1] = 7.5f * SCALE;
	obb->r[2] = 14.0f * SCALE;
	obb->p_d = Point3(0.0f, 10.0f, 0.0f) * SCALE;
	collider->SetObb(obb);
	// Tree
	Aabb aabb;
	aabb.r[0] = 4.0f;
	aabb.r[1] = 2.0f;
	aabb.r[2] = 4.0f;
	aabb.p_d[1] = 1.0f;
	collider->SetTree(aabb);

	m_HitSpace.GetTransform()->SetParent(m_pTransform);
	m_HitSpace.SetActive(false);
	obb = new Obb;
	obb->r[0] = 3.5f * SCALE;
	obb->r[1] = 7.5f * SCALE;
	obb->r[2] = 14.0f * SCALE;
	obb->p_d = Point3(0.0f, 10.0f, 0.0f) * SCALE;
	m_HitSpace.GetCollider()->SetObb(obb);
	m_HitSpace.SetTag(Archives::Tag("Enemy/Attack"));
}


/*									//
//			デストラクタ			//
//									*/
Wolf::~Wolf() {

}


/*									//
//				初期化				//
//									*/
void Wolf::Init() {
	Enemy::Init();

	CreateHeadLife();
	CreateDeathEffect();

	m_pLifeber->GetTransform()->SetLocalPos(0.0f, 20.0f, 0.0f);

	GetMesh()->GetCont().GetOnUpdateAnim("Walk")->AddListener([&](double time) {
		m_pTransform->Translate(WallCheck(Vector3(0.0f, 0.0f, 5.0f)));
		m_pTransform->LookAt(m_pPlayer->GetTransform());
	});

	GetMesh()->GetCont().GetOnEnterAnim("Threat")->AddListener([&]() {
		m_bAttack = true;
	});
	GetMesh()->GetCont().GetOnEnterAnim("Attack")->AddListener([&]() {
		m_HitSpace.SetActive(true);
		m_HitSpace.Reset();
	});
	GetMesh()->GetCont().GetOnUpdateAnim("Attack")->AddListener([&](double time) {
		m_pTransform->Translate(WallCheck(Vector3(0.0f, 0.0f, 30.0f)));
	});
	GetMesh()->GetCont().GetOnExitAnim("Attack")->AddListener([&]() {
		m_bInterval = true;
		m_HitSpace.SetActive(false);
	});
}


/*									//
//				更新				//
//									*/
void Wolf::Update() {
	Enemy::Update();

	Action();

	if (m_bInterval) {
		m_fIntervalCnt += Time::GetDeltaTime();
		if (INTERVAL_TIME <= m_fIntervalCnt) {
			m_fIntervalCnt = 0.0f;
			m_bInterval = false;
			m_bAttack = false;
		}
	}
}


/*									//
//				行動				//
//									*/
void Wolf::Action() {
	if (m_bAttack || m_bInterval)	return;
	float dist = D3DXVec3Length(&(m_pPlayer->GetTransform()->GetPos() - m_pTransform->GetPos()));
	if (dist <= ATTACK_DIST) {
		m_pAnimator->SetBool("Walk", false);
		m_pAnimator->SetTrigger("Attack");
		m_pTransform->LookAt(m_pPlayer->GetTransform());
	} else
	if (dist <= MOVE_DIST) {
		m_pAnimator->SetBool("Walk", true);
	} else {
		m_pAnimator->SetBool("Walk", false);
	}
}


#pragma region Input
/*									//
//			移動の入力				//
//									*/
void Wolf::MoveInput(const string cameraName) {
	m_pAnimator->SetBool("Walk", false);
	// キーボード
	if (Input::GetKeyPress(GetContManager()->Get()->GetKeyNum(BUTTON_UP))) {
		GetTransform()->FowardBaseRotate(GetCameraManager()->Get(cameraName)->GetTransform(), 0.0f);
		m_pAnimator->SetBool("Walk", true);
	} else
	if (Input::GetKeyPress(GetContManager()->Get()->GetKeyNum(BUTTON_RIGHT))) {
		GetTransform()->FowardBaseRotate(GetCameraManager()->Get(cameraName)->GetTransform(), 90.0f);
		m_pAnimator->SetBool("Walk", true);
	} else
	if (Input::GetKeyPress(GetContManager()->Get()->GetKeyNum(BUTTON_DOWN))) {
		GetTransform()->FowardBaseRotate(GetCameraManager()->Get(cameraName)->GetTransform(), 180.0f);
		m_pAnimator->SetBool("Walk", true);
	} else
	if (Input::GetKeyPress(GetContManager()->Get()->GetKeyNum(BUTTON_LEFT))) {
		GetTransform()->FowardBaseRotate(GetCameraManager()->Get(cameraName)->GetTransform(), 270.0f);
		m_pAnimator->SetBool("Walk", true);
	}
	// パッド
	if (Input::GetJoyAxisDegree(0) != -1000) {
		GetTransform()->FowardBaseRotate(GetCameraManager()->Get(cameraName)->GetTransform(), Input::GetJoyAxisDegree(0) + 90.f);
		m_pAnimator->SetBool("Walk", true);
	}
}


/*									//
//			攻撃の入力				//
//									*/
void Wolf::AttackInput() {
	if (Input::GetKeyTrigger(GetContManager()->Get()->GetKeyNum(BUTTON_BATU)) ||
		Input::GetJoyTrigger(0, GetContManager()->Get()->GetJoyNum(BUTTON_BATU))) {
		m_pAnimator->SetTrigger("Attack");
	}
}
#pragma endregion