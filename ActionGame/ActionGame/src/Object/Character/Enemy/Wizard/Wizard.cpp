// 魔法使い
// 2017. 1.26	プログラム作成
// Author		SyunMizuno




#include	"Wizard.h"
#include	"../../../../Archives/Archives.h"
#include	"../../../../GameFramework/framework/Texture/TexManager.h"
#include	"../../../../GameFramework/framework/Mesh/SkinMesh/SkinMeshManager.h"
#include	"../../../../GameFramework/GameController/GameControllerManager.h"
#include	"../../../../GameFramework/framework/Time/Time.h"
#include	"../../../../Factory/Factory.h"


const float SCALE = 0.35f;
const float ATTACK_DIST = 20.0f;
const float INTERVAL_TIME = 3.0f;


/*									//
//			コンストラクタ			//
//									*/
Wizard::Wizard(int max) :
	Enemy(max),
	m_bAttack(false),
	m_bInterval(false),
	m_fIntervalCnt(0.0f),
	m_pShot(NULL)
{
	SetName("Wizard");
	SetShaderType(SHADER_TOON);

	SetModel(GetSkinMeshManager()->Get(Archives::Mesh("Wizard")), Factory::Create(GAME_ANIMATOR_WIZARD));
	GetTransform()->SetScale(SCALE, SCALE, SCALE);

	CreateCollider();
	Collider* collider = GetCollider();
	// Obb
	Obb* obb = new Obb;
	obb->r[0] = 2.f * SCALE;
	obb->r[1] = 9.f * SCALE;
	obb->r[2] = 2.0f * SCALE;
	obb->p_d = Point3(0.0f, 8.0f, 0.0f) * SCALE;
	collider->SetObb(obb);
	// Tree
	Aabb tree;
	for (int i = 0; i < 3; ++i)
		tree.r[i] = 3.0f;
	tree.p_d.y += 3.0f;
	m_pCollider->SetTree(tree);

	m_pShot = new AttackMagic;
	m_pShot->SetSpeed(20.0f);
	m_pShot->SetDefaultTime(3.0f);
	m_pShot->GetTransform()->SetParent(m_pTransform);

	m_pRing = new Ring;
	m_pRing->GetTransform()->SetParent(m_pTransform);
	m_pRing->GetTransform()->SetLocalPos(0.0f, 10.0f, 0.0f);
	m_pRing->SetRadius(8.0f);
	m_pRing->SetWeight(3.0f);
	m_pRing->SetActive(false);
	m_pRing->SetTex(GetTexManager()->Get(Archives::Texture("Ring")));
}


/*									//
//			デストラクタ			//
//									*/
Wizard::~Wizard() {

}


/*									//
//				初期化				//
//									*/
void Wizard::Init() {
	Enemy::Init();

	CreateHeadLife();
	CreateDeathEffect();

	m_pLifeber->GetTransform()->SetLocalPos(0.0f, 20.0f, 0.0f);

	SetWeapon((Weapon*)Factory::Create(GAME_OBJ_STICK));

	GetMesh()->GetCont().GetOnEnterAnim("Threat")->AddListener([&]() {
		m_bAttack = true;
		m_pShot->Reset();
		m_pRing->SetActive(true);
		m_pRing->Reset();
	});
	GetMesh()->GetCont().GetOnUpdateAnim("Threat")->AddListener([&](double time) {
		m_pTransform->LookAt(m_pPlayer->GetTransform());
		m_pRing->GetTransform()->Rotate(0.0f, 60.0f * Time::GetDeltaTime(), 0.0f);
	});
	GetMesh()->GetCont().GetOnExitAnim("Threat")->AddListener([&]() {
		m_pRing->SetActive(false);
	});
	GetMesh()->GetCont().GetOnEndAnim("Attack")->AddListener([&]() {
		m_pShot->GetTransform()->SetPos(GetWeapon()->GetTransform()->GetPos());
		m_pShot->Shot();
	});
	GetMesh()->GetCont().GetOnExitAnim("Attack")->AddListener([&]() {
		m_bInterval = true;
	});
}


/*									//
//				更新				//
//									*/
void Wizard::Update() {
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
void Wizard::Action() {
	if (m_bAttack || m_bInterval)	return;

	float dist = D3DXVec3Length(&(m_pPlayer->GetTransform()->GetPos() - m_pTransform->GetPos()));
	if (dist <= ATTACK_DIST) {
		m_pAnimator->SetTrigger("Attack");
	}
}


/*									//
//				攻撃入力			//
//									*/
void Wizard::AttackInput() {
	if (Input::GetKeyTrigger(GetContManager()->Get()->GetKeyNum(BUTTON_BATU)) ||
		Input::GetJoyTrigger(0, GetContManager()->Get()->GetJoyNum(BUTTON_BATU))) {
		m_pAnimator->SetTrigger("Attack");
	}
}