// プレイヤー
// 2017. 1.22	プログラム作成
// Author		SyunMizuno


#include	"Player.h"
#include	"../GameFramework/GameController/GameControllerManager.h"
#include	"../GameFramework/framework/Camera/CameraManager.h"
#include	"../GameFramework/framework/Time/Time.h"
#include	"../Animator/PlayerAnimator.h"
#include	"../Archives/Archives.h"
#include	"../Sword/Sword.h"
#include	"../GameFramework/framework/Texture/TexManager.h"
#include	"../GameFramework/framework/Utility/System/SystemUtility.h"
#include	"../ItemBox/ItemBox.h"
#include	"../GameOver/GameOver.h"
#include	"../GameFramework/framework/Sound/SoundManager.h"


const int ATTACK_MAX = 2;


/*									//
//			コンストラクタ			//
//									*/
Player::Player() :
	m_pInfo(NULL),
	m_pAttack(NULL),
	m_bAttack(false),
	m_bHit(false)
{
	SetName("Player");
	SetTag(Archives::Tag("Player"));
	SetShaderType(_shadertype::SHADER_TOON);

	// Info
	m_pInfo = new PlayerInformation;
	m_pInfo->SetMaxHp(GetStatus().GetMaxLife());
	m_pInfo->SetCurHp(GetStatus().GetLife());

	CreateCollider();
	// Obb
	Obb* obb;
	obb = new Obb;
	obb->r[0] = 1.0f;
	obb->r[1] = 2.5f;
	obb->r[2] = 1.0f;
	obb->p_d.y = 2.0f;
	m_pCollider->SetObb(obb);
	// Tree
	Aabb tree;
	for (int i = 0; i < 3; ++i)
		tree.r[i] = 3.0f;
	tree.p_d.y += 2.5f;
	m_pCollider->SetTree(tree);

	//----- 攻撃
	m_pAttack = new _Attack[ATTACK_MAX];
	for (int i = 0; i < ATTACK_MAX; ++i) {
		// 軌跡
		m_pAttack[i].m_Locus.SetUp(0.7f);
		m_pAttack[i].m_Locus.SetDown(4.5f);
		m_pAttack[i].m_Locus.SetGhostSpeed(0.5f);
		m_pAttack[i].m_Locus.SetRevision(true);
		m_pAttack[i].m_HitSpace.GetCollider()->SetEnable(false);
//		m_pAttack[i].m_HitSpace.SetPower(30);
		// Obb
		obb = new Obb;
		obb->r[0] = 2.0f;
		obb->r[1] = 1.5f;
		obb->r[2] = 1.0f;
		obb->p_d[0] = -1.0f;
		obb->p_d[1] = 2.50f;
		m_pAttack[i].m_HitSpace.GetCollider()->SetObb(obb);
		Aabb colltree;
		for(int i = 0; i < 3; ++ i)
			colltree.r[i] = 5.0f;
		m_pAttack[i].m_HitSpace.GetCollider()->SetTree(colltree);
		m_pAttack[i].m_HitSpace.SetTag(Archives::Tag("Player/Attack"));
	}
	m_pAttack[0].m_Locus.SetTex(GetTexManager()->Get(Archives::Texture("Player/Locus/1")));
	m_pAttack[1].m_Locus.SetTex(GetTexManager()->Get(Archives::Texture("Player/Locus/1")));

	//----- サウンド
	GetSoundManager()->Load(SOUND_SE, Archives::Sound("Swing").c_str(), "Swing");
}


/*									//
//			デストラクタ			//
//									*/
Player::~Player() {
	SAFE_DELETE_ARRAY(m_pAttack);

	GetTexManager()->Delete(Archives::Texture("Player/Locus/1"));

	GetSoundManager()->Delete(SOUND_SE, "Swing");
}


/*									//
//				初期化				//
//									*/
void Player::Init() {
	Character::Init();

	if (!m_pMesh) {
		Debug::LogError("メッシュがありません");
		return;
	}
	for(int i = 0; i < ATTACK_MAX; ++i)
		m_pAttack[i].m_HitSpace.GetTransform()->SetParent(m_pMesh->GetBoneTransform("Armature_Weapon"));

	// Run
	m_pMesh->GetCont().GetOnUpdateAnim("Run")->AddListener([&](double time) {
		Walk(13.0f);
	});
	// Run_Weapon
	m_pMesh->GetCont().GetOnUpdateAnim("Run_Weapon")->AddListener([&](double time) {
		Walk(13.0f);
	});
	// Swing1
	m_pMesh->GetCont().GetOnEnterAnim("Swing1")->AddListener([&]() {
		m_pAttack[0].m_HitSpace.GetCollider()->SetEnable(true);
		m_bAttack = true;
		GetSoundManager()->Play(SOUND_SE, "Swing");
	});
	m_pMesh->GetCont().GetOnUpdateAnim("Swing1")->AddListener([&](double time) {
		Walk(1.0f);
		if(time >= 5.0)
			m_pAttack[0].m_Locus.SetNewPos(GetWeapon()->GetTransform()->GetWorld());
	});
	m_pMesh->GetCont().GetOnExitAnim("Swing1")->AddListener([&]() {
		m_pAttack[0].m_Locus.End();
		m_bAttack = false;
		m_pAttack[0].m_HitSpace.GetCollider()->SetEnable(false);
		m_pAttack[0].m_HitSpace.Reset();
	});
	// Swing2
	m_pMesh->GetCont().GetOnEnterAnim("Swing2")->AddListener([&]() {
		m_pAttack[1].m_HitSpace.GetCollider()->SetEnable(true);
		m_bAttack = true;
		GetSoundManager()->Play(SOUND_SE, "Swing");
	});
	m_pMesh->GetCont().GetOnUpdateAnim("Swing2")->AddListener([&](double time) {
		Walk(1.0f);
		if(time >= 15.0)
			m_pAttack[1].m_Locus.SetNewPos(GetWeapon()->GetTransform()->GetWorld());
	});
	m_pMesh->GetCont().GetOnExitAnim("Swing2")->AddListener([&]() {
		m_pAttack[1].m_Locus.End();
		m_pAttack[1].m_HitSpace.GetCollider()->SetEnable(false);
		m_bAttack = false;
		m_pAttack[1].m_HitSpace.Reset();
	});
	// KnockBack
	m_pMesh->GetCont().GetOnEnterAnim("KnockBack")->AddListener([&]() {
		m_bHit = true;
	});
	m_pMesh->GetCont().GetOnExitAnim("KnockBack")->AddListener([&]() {
		m_bHit = false;
	});
	// Death
	m_pMesh->GetCont().GetOnEnterAnim("Death")->AddListener([&]() {
		new GameOver;
	});
}


/*									//
//				更新				//
//									*/
void Player::LateUpdate() {
	Character::LateUpdate();

	for (int i = 0; i < ATTACK_MAX; ++i) {
		m_pAttack[i].m_Locus.Update();
	}
}


/*									//
//				描画				//
//									*/
void Player::LateDraw() {
	Character::LateDraw();

	for (int i = 0; i < ATTACK_MAX; ++i) {
		m_pAttack[i].m_Locus.Draw();
	}
}


#pragma region Input
/*									//
//			移動の入力				//
//									*/
void Player::MoveInput(const string cameraName) {
	m_pAnimator->SetBool("Run", false);

	if (m_bAttack || m_bHit)	return;

	// キーボード
	if (Input::GetKeyPress(GetContManager()->Get()->GetKeyNum(BUTTON_UP))) {
		GetTransform()->FowardBaseRotate(GetCameraManager()->Get(cameraName)->GetTransform(), 180.0f);
		m_pAnimator->SetBool("Run", true);
	} else
	if (Input::GetKeyPress(GetContManager()->Get()->GetKeyNum(BUTTON_RIGHT))) {
		GetTransform()->FowardBaseRotate(GetCameraManager()->Get(cameraName)->GetTransform(), 270.0f);
		m_pAnimator->SetBool("Run", true);
	} else
	if (Input::GetKeyPress(GetContManager()->Get()->GetKeyNum(BUTTON_DOWN))) {
		GetTransform()->FowardBaseRotate(GetCameraManager()->Get(cameraName)->GetTransform(), 0.0f);
		m_pAnimator->SetBool("Run", true);
	} else
	if (Input::GetKeyPress(GetContManager()->Get()->GetKeyNum(BUTTON_LEFT))) {
		GetTransform()->FowardBaseRotate(GetCameraManager()->Get(cameraName)->GetTransform(), 90.0f);
		m_pAnimator->SetBool("Run", true);
	}
	// パッド
	if (Input::GetJoyAxisDegree(0) != -1000) {
		GetTransform()->FowardBaseRotate(GetCameraManager()->Get(cameraName)->GetTransform(), Input::GetJoyAxisDegree(0) + 270.f);
		m_pAnimator->SetBool("Run", true);
	}
}


/*									//
//			攻撃の入力				//
//									*/
void Player::AttackInput() {
	if (Input::GetKeyTrigger(GetContManager()->Get()->GetKeyNum(BUTTON_BATU)) ||
		Input::GetJoyTrigger(0, GetContManager()->Get()->GetJoyNum(BUTTON_BATU))) {
		if(GetWeapon())
			m_pAnimator->SetTrigger("Attack");
	}
	if (Input::GetKeyTrigger(GetContManager()->Get()->GetKeyNum(BUTTON_MARU)) ||
		Input::GetJoyTrigger(0, GetContManager()->Get()->GetJoyNum(BUTTON_MARU))) {
		m_pAnimator->SetBool("Weapon", false);
		SetWeapon(NULL);
	}
	if (Input::GetKeyTrigger(GetContManager()->Get()->GetKeyNum(BUTTON_SANKAKU)) ||
		Input::GetJoyTrigger(0, GetContManager()->Get()->GetJoyNum(BUTTON_SANKAKU))) {
		SetWeapon(new Sword);
		m_pAnimator->SetBool("Weapon", true);
	}
}
#pragma endregion

/*									//
//				移動				//
//									*/
void Player::Walk(float speed) {
	m_pTransform->Translate(WallCheck(Vector3(0.0f, 0.0f, speed)));
}


/*									//
//			当たっている間			//
//									*/
void Player::OnCollisionStay(Obj3D* obj) {
	Character::OnCollisionStay(obj);

	if (obj->GetName() == "ItemBox") {
		if (Input::GetKeyTrigger(GetContManager()->Get()->GetKeyNum(BUTTON_MARU)) ||
			Input::GetJoyTrigger(0, GetContManager()->Get()->GetJoyNum(BUTTON_MARU)))
		{
			ItemBox* box = (ItemBox*)obj;
			SetWeapon((Weapon*)box->Transfer());
			m_pAnimator->SetBool("Weapon", true);
		}
	}
}


/*									//
//			ダメージ処理			//
//									*/
void Player::Damage(int attack) {
	Character::Damage(attack);
	int life = GetStatus().GetLife();
	m_pInfo->SetCurHp(life);
	if (life <= 0) {
		m_pAnimator->SetTrigger("Death");
	} else {
		m_pAnimator->SetTrigger("Hit");
	}
}