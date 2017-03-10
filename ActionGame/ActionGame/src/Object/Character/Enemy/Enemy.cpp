// 敵
// 2017. 1.31	プログラム作成
// Author		SyunMizuno


#include	"Enemy.h"
#include	"../../../GameFramework/Obj/3D/Obj3DManager.h"
#include	"../../../Archives/Archives.h"
#include	"../../../GameFramework/framework/Graphic/Graphics.h"


/*									//
//			コンストラクタ			//
//									*/
Enemy::Enemy(int max) :
	m_pPlayer(NULL),
	m_pLifeber(NULL),
	m_pDeathEffect(NULL)
{
	GetStatus().SetMaxLife(max);
	SetTag(Archives::Tag("Enemy"));
}


/*									//
//			デストラクタ			//
//									*/
Enemy::~Enemy() {
	SAFE_DELETE(m_pDeathEffect);
}


/*									//
//				初期化				//
//									*/
void Enemy::Init() {
	Character::Init();

	m_pPlayer = GetObj3DManager()->FindWithTag("Player");
	if (!m_pPlayer) {
		Debug::LogError("プレイヤーが見つかりません");
		return;
	}
}


/*									//
//			ダメージ処理			//
//									*/
void Enemy::Damage(int attack) {
	Character::Damage(attack);
	if(m_pLifeber)
		m_pLifeber->SetCurLife(GetStatus().GetLife());

	if (GetStatus().GetLife() <= 0.0f) {
		SetWeapon(NULL);
		SetActive(false);

		if (m_pDeathEffect) {
			m_pDeathEffect->SetActive(true);
			m_pDeathEffect->GetTransform()->SetPos(m_pTransform->GetPos());
			m_pDeathEffect->GetTransform()->Translate(0.0f, 2.0f, 0.0f);
			m_pDeathEffect->Play();
		}
	}
}


/*									//
//			頭上のライフ作成		//
//									*/
void Enemy::CreateHeadLife() {
	if (m_pLifeber)	return;

	m_pLifeber = new LifeberOnHead(GetStatus().GetMaxLife());
	m_pLifeber->GetTransform()->SetParent(m_pTransform);
}


/*									//
//			死亡エフェクト作成		//
//									*/
void Enemy::CreateDeathEffect() {
	if (m_pDeathEffect)	return;

	m_pDeathEffect = new DeathEffect;
	m_pDeathEffect->SetUp(2.0f, 2.0f);
	m_pDeathEffect->SetActive(false);
}