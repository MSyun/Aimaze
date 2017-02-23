// �h���S��
// 2017. 2. 6	�v���O�����쐬
// Author		SyunMizuno


#include	"Dragon.h"
#include	"../GameFramework/framework/Time/Time.h"
#include	"../GameFramework/framework/Camera/CameraManager.h"
#include	"../GameFramework/Obj/3D/Obj3DManager.h"
#include	"../GameClear/GameClear.h"
#include	"../CameraControll/CameraControll.h"


// �U����̋x�e����
const float INTERVAL_BREATH = 2.0f;
const float INTERVAL_FLAME = 6.0f;
const float INTERVAL_STAMP = 5.0f;
const float INTERVAL_RUSH = 7.0f;


// �U������̋���
const float ATTACK_DIST_FAR		= 80.0f;
const float ATTACK_DIST_MIDDLE	= 50.0f;
const float ATTACK_DIST_NEAR	= 30.0f;


#define		RAND_TEN	(rand() % 10)


/*									//
//			�R���X�g���N�^			//
//									*/
Dragon::Dragon(int max) :
	Enemy(max),
	m_fIntervalCnt(0.0f),
	m_bAttack(false),
	m_bStamp(false),
	m_bBreath(false),
	m_bDeath(false),
	m_bClear(false),
	m_fStampTime(0.0f),
	m_pFlame(NULL),
	m_pBreath(NULL),
	m_pRush(NULL),
	m_pStamp(NULL),
	m_pInfo(NULL)
{
	SetName("Dragon");
	SetShaderType(SHADER_TOON);

	m_ActionDelegate.Set(NULL);
	m_pFlame = new DragonFlame;
	m_pFlame->SetActive(false);
	m_pBreath = new DragonBreath;
	m_pBreath->SetActive(false);
	m_pRush = new DragonRush[2];
	m_pStamp = new DragonStamp;
//	m_pStamp->GetCollider()->SetEnable(false);

	CreateCollider();
	Obb* obb = new Obb;
	obb->r[0] = 5.0f;
	obb->r[1] = 5.0f;
	obb->r[2] = 10.0f;
	obb->p_d[1] = 2.5f;
	m_pCollider->SetObb(obb);

	Aabb aabb;
	aabb.r[0] = 20.0f;
	aabb.r[1] = 20.0f;
	aabb.r[2] = 20.0f;
	m_pCollider->SetTree(aabb);
}


/*									//
//			�f�X�g���N�^			//
//									*/
Dragon::~Dragon() {
	SAFE_DELETE(m_pStamp);
	SAFE_DELETE(m_pFlame);
	SAFE_DELETE(m_pBreath);
	SAFE_DELETE_ARRAY(m_pRush);
}


/*									//
//				������				//
//									*/
void Dragon::Init() {
	Enemy::Init();

	m_pInfo = new BossInformation;
	m_pInfo->SetMaxHp(GetStatus().GetMaxLife());
	m_pInfo->SetCurHp(GetStatus().GetLife());

	m_pFlame->GetTransform()->SetParent(m_pMesh->GetBoneTransform("Armature_Breath"));
	m_pStamp->GetTransform()->SetParent(m_pTransform);
	m_pStamp->GetCollider()->SetEnable(false);

	m_pRush[0].GetTransform()->SetParent(m_pMesh->GetBoneTransform("Armature_Finger2_L"));
	m_pRush[1].GetTransform()->SetParent(m_pMesh->GetBoneTransform("Armature_Finger2_R"));
	m_pRush[0].GetTransform()->SetEularAngles(0.0f, 0.0f, 70.0f);
	m_pRush[1].GetTransform()->SetEularAngles(0.0f, 0.0f, -70.0f);
	for (int i = 0; i < 2; ++i) {
		m_pRush[i].GetTransform()->Translate(0.0f, 3.5f, 0.0f);
		m_pRush[i].End();
		m_pRush[i].GetCollider()->SetEnable(false);
	}

	HighLevelAnimController* cont = &m_pMesh->GetCont();

	// Breath
	cont->GetOnEnterAnim("Breath")->AddListener([&]() {
		m_pTransform->LookAt(m_pPlayer->GetTransform());
		m_bAttack = true;
		m_pBreath->Reset();
	});
	cont->GetOnUpdateAnim("Breath")->AddListener([&](double time) {
		m_pMesh->GetBoneTransform("Armature_Breath")->LookAt(m_pPlayer->GetTransform());
		if (time >= 47.0 && !m_bBreath) {
			m_pBreath->SetActive(true);
			m_bBreath = true;
			m_pBreath->Shot(m_pMesh->GetBoneTransform("Armature_Breath"), 50.0f);
		}
	});
	cont->GetOnExitAnim("Breath")->AddListener([&]() {
		m_fIntervalCnt = INTERVAL_BREATH;
		m_bBreath = false;
	});
	// Flame
	cont->GetOnEnterAnim("Flame")->AddListener([&]() {
		m_pTransform->LookAt(m_pPlayer->GetTransform());
		m_bAttack = true;
	});
	cont->GetOnUpdateAnim("Flame")->AddListener([&](double time) {
		if (time >= 23.0) {
			m_pFlame->SetActive(true);
			m_pTransform->Rotate(0.0f, -20.0f * Time::GetDeltaTime(), 0.0f);
		}
		m_pFlame->Reset();
	});
	cont->GetOnExitAnim("Flame")->AddListener([&]() {
		m_fIntervalCnt = INTERVAL_FLAME;
		m_pFlame->SetActive(false);
	});
	// Rush
	cont->GetOnEnterAnim("Rush")->AddListener([&]() {
		Point3 pos = m_pPlayer->GetTransform()->GetPos();
		m_pTransform->LookAt(Point3(pos.x, m_pTransform->GetPos().y, pos.z));
		m_bAttack = true;
		for (int i = 0; i < 2; ++i) {
			m_pRush[i].GetCollider()->SetEnable(true);
			m_pRush[i].Reset();
		}
	});
	cont->GetOnEndAnim("Rush")->AddListener([&]() {
		m_pTransform->Translate(0.0f, 0.0f, 100.0f * Time::GetDeltaTime(), m_pTransform);
		for(int i = 0; i < 2; ++i)
			m_pRush[i].Record();
	});
	cont->GetOnExitAnim("Rush")->AddListener([&]() {
		m_fIntervalCnt = INTERVAL_RUSH;
		for (int i = 0; i < 2; ++i) {
			m_pRush[i].End();
			m_pRush[i].GetCollider()->SetEnable(false);
		}
	});
	// Glide
	cont->GetOnUpdateAnim("Glide")->AddListener([&](double time) {
		m_pTransform->Translate(0.0f, 0.0f, 30.0f * Time::GetDeltaTime(), m_pTransform);
	});
	// Death
	cont->GetOnEndAnim("Death")->AddListener([&]() {
		if (!m_bClear) {
			new GameClear;
			m_bClear = true;
			Obj3D* obj = GetObj3DManager()->Find("Player");
			if (obj)
				obj->SetOperate(false);
		}
	});
}


/*									//
//				�X�V				//
//									*/
void Dragon::Update() {
	Enemy::Update();

	if (m_bDeath) {
		Obj3D* obj = GetCameraManager()->CurrentCamera();
		if (!obj)	return;
		obj->GetTransform()->LookAt(m_pMesh->GetBoneTransform("Armature_Breath"));
		return;
	}

	Action();

	// �o�^���̃A�N�V���������s
	m_ActionDelegate.Invoke();

	if (m_fIntervalCnt > 0.0f) {
		Debug::Print("�C���^�[�o����\n");
		m_ActionDelegate.Set(NULL);
		m_fIntervalCnt -= Time::GetDeltaTime();
		if (0.0f >= m_fIntervalCnt) {
			m_fIntervalCnt = 0.0f;
			m_bAttack = false;
		}
	}
}


/*									//
//				�s��				//
//									*/
void Dragon::Action() {
	if (CheckAttack())
		return;


	float dist = D3DXVec3Length(&(m_pPlayer->GetTransform()->GetPos() - m_pTransform->GetPos()));
	// �ߋ���
	if (dist <= ATTACK_DIST_NEAR) {
		NearAnalyze();
	} else
	// ������
	if (dist <= ATTACK_DIST_MIDDLE) {
		MiddleAnalyze();
	} else
	// ������
	/*if (dist <= ATTACK_DIST_FAR)*/ {
		FarAnalyze();
	}
}


/*									//
//				�U�����m�F			//
//									*/
bool Dragon::CheckAttack() {
	if (m_bAttack ||
		m_fIntervalCnt > 0.0f ||
		m_bStamp)
		return true;

	return false;
}


#pragma region Analyze
/*									//
//			�ߋ����U���I��			//
//									*/
void Dragon::NearAnalyze() {
	int rate = RAND_TEN;

	// ���݂Ԃ�
	if (rate < 3) {
		AddListenAttack(ATTACK_STAMP);
	} else
	// �K��(�ꎞ�I
	if (rate < 7) {
		AddListenAttack(ATTACK_STAMP);
	} else
	// �ːi
	{
		AddListenAttack(ATTACK_RUSH);
	}
}
/*									//
//			�������U���I��			//
//									*/
void Dragon::MiddleAnalyze() {
	int rate = RAND_TEN;

	// �Ή�����
	if (rate < 3) {
		AddListenAttack(ATTACK_FLAME);
	} else
	// �΋�
	{
		AddListenAttack(ATTACK_BREATH);
	}
}
/*									//
//			�������U���I��			//
//									*/
void Dragon::FarAnalyze() {
	int rate = RAND_TEN;

	// �ːi
	if (rate < 3) {
		AddListenAttack(ATTACK_RUSH);
	} else
	// �΋�
	{
		AddListenAttack(ATTACK_BREATH);
	}
}
#pragma endregion


/*									//
//			�U���p�֐��o�^			//
//									*/
void Dragon::AddListenAttack(_attack anim) {
	m_ActionDelegate.Set(NULL);

	switch (anim) {
	case ATTACK_BREATH:
		Debug::Log("�΋��J�n");
		m_ActionDelegate.Set(bind(&Dragon::AttackBreath, this));
		m_pAnimator->SetInt("Attack", 2);
		break;

	case ATTACK_FLAME:
		Debug::Log("�Ή����ˊJ�n");
		m_ActionDelegate.Set(bind(&Dragon::AttackFlame, this));
		m_pAnimator->SetInt("Attack", 3);
		break;

	case ATTACK_STAMP:
		Debug::Log("���݂Ԃ������J�n");
		m_ActionDelegate.Set(bind(&Dragon::AttackStamp, this));
		m_pAnimator->SetBool("Wing", true);
		m_bStamp = true;
		m_fStampTime = 3.0f;
		m_nStampState = 0;
		break;

	case ATTACK_RUSH:
		Debug::Log("�˂����ݏ����J�n");
		m_ActionDelegate.Set(bind(&Dragon::AttackRush, this));
		m_pAnimator->SetInt("Attack", 1);
		break;

	default:
		Debug::Print("�����ɓ����ĂȂ���\n");
		break;
	}
}


/*									//
//			���݂Ԃ�����			//
//									*/
void Dragon::AttackStamp() {
	float delta = Time::GetDeltaTime();
	Point3 pos = m_pPlayer->GetTransform()->GetPos();

	switch (m_nStampState) {
	case 0:	// ��s
		m_pTransform->Translate(0.0f, 12.0f * delta, 0.0f, m_pTransform);
		m_fStampTime -= delta;
		if (m_fStampTime <= 0.0f) {
			m_nStampState++;
			m_fStampTime = 2.0f;
		}
		break;

	case 1:	// ���@�̌����ֈړ�
		m_pTransform->LookAt(Point3(pos.x, m_pTransform->GetPos().y, pos.z));
		m_pTransform->Translate(0.0f, 0.0f, 18.0f * delta, m_pTransform);
		m_fStampTime -= delta;
		if (m_fStampTime <= 0.0f) {
			m_fStampTime = 1.0f;
			m_nStampState++;
		}
		break;

	case 2:	// �ꎞ��~
		m_fStampTime -= delta;
		if (m_fStampTime <= 0.0f) {
			m_fStampTime = 0.5f;
			m_nStampState++;
		}
		break;

	case 3:	// �~���
		if (m_fStampSpeed <= 0.0f) {
			m_fStampSpeed = m_pTransform->GetPos().y / m_fStampTime;
			m_pStamp->GetCollider()->SetEnable(true);
		}
		m_fStampTime -= delta;
		m_pTransform->Translate(0.0f, -m_fStampSpeed * delta, 0.0f, m_pTransform);
		if (m_fStampTime <= 0.0f) {
			m_fStampTime = 0.0f;
			m_nStampState = 0;
			m_pAnimator->SetBool("Wing", false);
			m_bStamp = false;
			m_fIntervalCnt = INTERVAL_STAMP;
			m_fStampSpeed = 0.0f;
			m_pTransform->SetPos(m_pTransform->GetPos().x, 0.0f, m_pTransform->GetPos().z);
			m_nStampState ++;
			m_pStamp->Stamp();
			m_pStamp->GetCollider()->SetEnable(false);

			CameraControll* obj = (CameraControll*)GetObj3DManager()->Find("CameraControll");
			if (obj) {
				obj->Shake(0.1f, 0.8f);
			} else {
				Debug::Log("CameraControll��������Ȃ��̂ŗh����N�����܂���");
			}
		}
		break;

	default:
		Debug::Log("���݂Ԃ��I��");
		break;
	}

}


/*									//
//				�΋�����			//
//									*/
void Dragon::AttackBreath() {
	Debug::Print("�΋�����\n");

}


/*									//
//			�Ή����ˏ���			//
//									*/
void Dragon::AttackFlame() {
	Debug::Print("�Ή����ˏ���\n");

}


/*									//
//				�˂����ݏ���		//
//									*/
void Dragon::AttackRush() {
	Debug::Print("�˂����ݏ���\n");

}


/*									//
//			�_���[�W����			//
//									*/
void Dragon::Damage(int attack) {
	Character::Damage(attack);
	int life = GetStatus().GetLife();
	m_pInfo->SetCurHp(life);
	if (life <= 0) {
		Death();
	} else {
//		m_pAnimator->SetTrigger("Hit");
	}
}


/*									//
//			���S���̐ݒ�			//
//									*/
void Dragon::Death() {
	if (m_bDeath)	return;

	m_pAnimator->SetTrigger("Death");
	m_bDeath = true;

	Obj3D* obj = GetCameraManager()->CurrentCamera();
	if (!obj)	return;

	Point3 pos = m_pTransform->GetPos();
	pos += (m_pTransform->GetRight() * 10.0f);
	pos += (m_pTransform->GetUp() * 10.0f);
	pos += (m_pTransform->GetForward() * 30.0f);
	obj->GetTransform()->SetPos(pos);
	obj = GetObj3DManager()->Find("CameraControll");
	obj->GetTransform()->RemoveParent();
	if (!obj)	return;
	obj->SetActive(false);
}