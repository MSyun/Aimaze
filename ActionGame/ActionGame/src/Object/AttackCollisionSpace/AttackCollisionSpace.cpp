// あたり判定範囲クラス
// 2017. 1.31	プログラム作成
// Author		SyunMizuno


#include	"AttackCollisionSpace.h"
#include	"../Character/Enemy/Enemy.h"
#include	"../../GameFramework/framework/Sound/SoundManager.h"
#include	"../../Archives/Archives.h"



/*									//
//			コンストラクタ			//
//									*/
AttackCollisionSpace::AttackCollisionSpace() :
	m_nPower(1),
	m_pEffect(NULL)
{
	SetName("AttackCollisionSpace");

	CreateCollider();
	m_pCollider->SetCollisionColor(1.0f, 0.0f, 0.0f, 1.0f);

	m_pEffect = new SlashEffect;

	// サウンド
	GetSoundManager()->Load(SOUND_SE, Archives::Sound("Hit").c_str(), "Hit");
}


/*									//
//			デストラクタ			//
//									*/
AttackCollisionSpace::~AttackCollisionSpace() {
	SAFE_DELETE(m_pEffect);
}


void AttackCollisionSpace::Init() {
	Obj3D::Init();
}


/*									//
//				リセット			//
//									*/
void AttackCollisionSpace::Reset() {
	m_aHit.clear();
}


/*									//
//		あたり判定（当たってる間	//
//									*/
void AttackCollisionSpace::OnCollisionStay(Obj3D* obj) {
	for (auto it = m_aHit.begin(); it != m_aHit.end(); ++it) {
		// 既に当たったか確認
		if ((*it) == obj)	return;
	}

	m_aHit.push_back(obj);

	Character* character = (Character*)obj;
	character->Damage(GetPower());
	Point3 pos = character->GetTransform()->GetPos();
	m_pEffect->GetTransform()->SetPos(pos.x, pos.y + 2.0f, pos.z);
	// 当たった時のエフェクト
	m_pEffect->RePlay();

	GetSoundManager()->Play(SOUND_SE, "Hit");
}