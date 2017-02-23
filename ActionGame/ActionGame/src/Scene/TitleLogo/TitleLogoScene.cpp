// �^�C�g���V�[��
// 2017. 1. 1	�v���O�����쐬
// author		SyunMizuno


#include	"TitleLogoScene.h"
#include	"../../GameFramework/framework/Debug/Debug.h"
#include	"../../GameFramework/framework/Time/Time.h"
#include	"../../Archives/Archives.h"
#include	"../../GameFramework/framework/Texture/TexManager.h"
#include	"../../GameFramework/framework/Screen/Screen.h"
#include	"../../GameFramework/framework/Utility/System/SystemUtility.h"
#include	"../../GameFramework/framework/Scene/SceneManagerWin.h"
#include	"../../GameFramework/GameController/GameControllerManager.h"
#include	"../../GameFramework/Obj/2D/Obj2DManager.h"
#include	"../../GameFramework/Sprite/FadeColor/FadeColorSprite.h"


/*									//
//			�R���X�g���N�^			//
//									*/
TitleLogoScene::TitleLogoScene() {
}


/*									//
//			�f�X�g���N�^			//
//									*/
TitleLogoScene::~TitleLogoScene() {
}


/*									//
//				������				//
//									*/
bool TitleLogoScene::Init() {
	m_fTime = 0.0f;

	FadeColorSprite* pSprite;
	pSprite = new FadeColorSprite(
		Color(1.0f, 1.0f, 1.0f, 0.0f),
		Color(1.0f, 1.0f, 1.0f, 1.0f),
		1.0f);
	pSprite->SetTexture(GetTexManager()->Get(Archives::Texture("TitleLogo")));
	pSprite->GetRectTransform()->SetPos(Screen::GetWidth() / 2.0f, Screen::GetHeight() / 2.0f, 0.0f);

	m_bChange = false;

	GetSceneManagerWins()->GetFade()->GetOnEndFadeIn()->AddListener([=]() {
		pSprite->Play();
	});
	pSprite->GetOnEndPlay()->AddListener([&]() {
		m_bChange = true;
		Debug::Log("�v�����J�n���܂�");
	});

	return true;
}


/*									//
//				���				//
//									*/
void TitleLogoScene::Release() {
	GetTexManager()->Delete(Archives::Texture("TitleLogo"));
	GetObj2DManager()->AllClear();
}


/*									//
//				�X�V				//
//									*/
void TitleLogoScene::Update() {
	Debug::Print("�^�C�g�����S����\n");

	GetObj2DManager()->Update();
	GetObj2DManager()->LateUpdate();

	if (!m_bChange)	return;

	m_fTime += Time::GetDeltaTime();
	if (m_fTime >= 1.f) {
		GetSceneManagerWins()->ChangeCommand(_T("Title"));
	}
}


/*									//
//				�`��				//
//									*/
void TitleLogoScene::Draw() {
	GetObj2DManager()->Draw();
	GetObj2DManager()->LateDraw();
}


/*									//
//				�ď�����			//
//									*/
bool TitleLogoScene::ReStart() {
	return true;
}