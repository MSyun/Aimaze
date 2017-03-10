// �����蔻��͈̓N���X
// 2017. 1.31	�v���O�����쐬
// Author		SyunMizuno


#include	"AttackCollisionSpace.h"
#include	"../Character/Enemy/Enemy.h"
#include	"../../GameFramework/framework/Sound/SoundManager.h"
#include	"../../Archives/Archives.h"



/*									//
//			�R���X�g���N�^			//
//									*/
AttackCollisionSpace::AttackCollisionSpace() :
	m_nPower(1),
	m_pEffect(NULL)
{
	SetName("AttackCollisionSpace");

	CreateCollider();
	m_pCollider->SetCollisionColor(1.0f, 0.0f, 0.0f, 1.0f);

	m_pEffect = new SlashEffect;

	// �T�E���h
	GetSoundManager()->Load(SOUND_SE, Archives::Sound("Hit").c_str(), "Hit");
}


/*									//
//			�f�X�g���N�^			//
//									*/
AttackCollisionSpace::~AttackCollisionSpace() {
	SAFE_DELETE(m_pEffect);
}


void AttackCollisionSpace::Init() {
	Obj3D::Init();
}


/*									//
//				���Z�b�g			//
//									*/
void AttackCollisionSpace::Reset() {
	m_aHit.clear();
}


/*									//
//		�����蔻��i�������Ă��	//
//									*/
void AttackCollisionSpace::OnCollisionStay(Obj3D* obj) {
	for (auto it = m_aHit.begin(); it != m_aHit.end(); ++it) {
		// ���ɓ����������m�F
		if ((*it) == obj)	return;
	}

	m_aHit.push_back(obj);

	Character* character = (Character*)obj;
	character->Damage(GetPower());
	Point3 pos = character->GetTransform()->GetPos();
	m_pEffect->GetTransform()->SetPos(pos.x, pos.y + 2.0f, pos.z);
	// �����������̃G�t�F�N�g
	m_pEffect->RePlay();

	GetSoundManager()->Play(SOUND_SE, "Hit");
}