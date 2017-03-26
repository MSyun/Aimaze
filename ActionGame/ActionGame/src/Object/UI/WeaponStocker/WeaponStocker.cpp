// 所持武器の表示用Sprite
// 2017. 3.25	プログラム作成
// author		SyunMizuno


#include	"WeaponStocker.h"
#include	"../../../GameFramework/framework/Screen/Screen.h"
#include	"../../../GameFramework/framework/Texture/TexManager.h"
#include	"../../../Archives/Archives.h"
#include	"../../../GameFramework/framework/Debug/Debug.h"
#include	"../../../GameFramework/framework/Graphic/Graphics.h"


/*									//
//			コンストラクタ			//
//									*/
WeaponStocker::WeaponStocker() {
	SetName("WeaponStocker");

	SetTexture(GetTexManager()->Get(Archives::Texture("Cartridge")));

	Vector2 Size((float)Screen::GetWidth(), (float)Screen::GetHeight());
	Vector2 HalfTexSize(GetRectTransform()->GetSize() / 2.0f);
	SetBeginPos(Size.x + HalfTexSize.x, Size.y + HalfTexSize.y, 0.0f);
	SetEndPos(Size.x - HalfTexSize.x, Size.y - HalfTexSize.y, 0.0f);
	SetChangeTime(0.2f);
	ChangeEase(EaseType::EaseOutBack);
	Reset(false);

	CreateSwordTip();
	CreateStickTip();
	for (int i = 0; i < _eWeaponType::WEAPON_MAX; ++i) {
		ReSetTipPos(i);
	}
}


/*									//
//			デストラクタ			//
//									*/
WeaponStocker::~WeaponStocker() {
	GetTexManager()->Delete(Archives::Texture("Cartridge"));
	GetTexManager()->Delete(Archives::Texture("SwordTip"));

	for (auto it = m_WeaponTip.begin(); it != m_WeaponTip.end(); ++it) {
		SAFE_DELETE(*it);
	}
	m_WeaponTip.clear();
}


/*									//
//			武器の種類を設定		//
//									*/
void WeaponStocker::SetWeaponType(_eWeaponType type) {
	m_nCurrentWeapon = type;

	for (int i = 0; i < _eWeaponType::WEAPON_MAX; ++i) {
		m_WeaponTip[i]->SetActive(i == m_nCurrentWeapon);
	}
}


/*									//
//				更新				//
//									*/
void WeaponStocker::Update() {
	MovingSprite::Update();

	ReSetTipPos(m_nCurrentWeapon);
}


/*									//
//				描画				//
//									*/
void WeaponStocker::Draw() {
	GetGraphics()->SetWrapMode(WRAPMODE_CLAMP);
	MovingSprite::Draw();
}


/*									//
//			剣チップ作成			//
//									*/
void WeaponStocker::CreateSwordTip() {
	Sprite* SwordTip = new Sprite;
	SwordTip->SetTexture(GetTexManager()->Get(Archives::Texture("SwordTip")));
	SwordTip->GetRectTransform()->SetScale(0.7f, 0.7f, 0.7f);
	m_WeaponTip.push_back(SwordTip);
}


/*									//
//			杖チップ作成			//
//									*/
void WeaponStocker::CreateStickTip() {
	Sprite* StickTip = new Sprite;
	StickTip->SetTexture(GetTexManager()->Get(Archives::Texture("StickTip")));
	StickTip->GetRectTransform()->SetScale(0.7f, 0.7f, 0.7f);
	m_WeaponTip.push_back(StickTip);
}


/*									//
//		チップの座標を再設定		//
//									*/
void WeaponStocker::ReSetTipPos(int WeaponType) {
	Vector2 Pos(GetRectTransform()->GetPos());
	Vector2 TipSize(m_WeaponTip[WeaponType]->GetRectTransform()->GetSize());
	Vector2 Scale(m_WeaponTip[WeaponType]->GetRectTransform()->GetScale());
	m_WeaponTip[WeaponType]->GetRectTransform()->SetPos(
		Pos.x + TipSize.x * Scale.x / 5.0f,
		Pos.y + TipSize.y * Scale.y / 5.0f,
		0.0f);
}