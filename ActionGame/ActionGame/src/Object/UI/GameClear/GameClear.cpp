// ゲームクリア―クラス
// 2017. 2. 13	プログラム作成
// author		SyunMizuno


#include	"GameClear.h"
#include	"../../../GameFramework/GameController/GameControllerManager.h"
#include	"../../../GameFramework/framework/Screen/Screen.h"
#include	"../../../GameFramework/framework/Texture/TexManager.h"
#include	"../../../Archives/Archives.h"
#include	"../../../GameFramework/Obj/3D/Obj3DManager.h"
#include	"../../../GameFramework/framework/Scene/SceneManagerWin.h"
#include	"../../../GameFramework/framework/Sound/SoundManager.h"



/*									//
//			コンストラクタ			//
//									*/
GameClear::GameClear() {
	Vector2 Size((float)Screen::GetWidth(), (float)Screen::GetHeight());

	// 文字
	SetName("GameClearBer");
	Init(
		GetTexManager()->Get(Archives::Texture("GameClear/Ber")),
		Size,
		1,
		50,
		0);
	this->SetSizeDefault();
	this->SetAnimationSpeed(0.01f);
	GetRectTransform()->SetScale(1.0f, 0.05f, 1.0f);
	GetRectTransform()->SetPos(Size.x * 0.5f, Size.y * 0.6f, 0.0f);
	SetActive(false);

	// font
	m_pFont = new MovingSprite(
		Point3(Size.x * 1.5f, Size.y * 0.45f, 0.0f),
		Point3(Size.x * 0.5f, Size.y * 0.45f, 0.0f),
		0.5f,
		false,
		true);
	m_pFont->SetTexture(GetTexManager()->Get(Archives::Texture("GameClear")));
	m_pFont->GetOnEndPlay()->AddListener(bind(&GameClear::SetActive, this, true));
	m_pFont->GetOnEndPlay()->AddListener(bind(&GameClear::SetOperate, this, true));
}


/*									//
//			デストラクタ			//
//									*/
GameClear::~GameClear() {
	SAFE_DELETE(m_pFont);

	GetTexManager()->Delete(Archives::Texture("GameClear/Ber"));
	GetTexManager()->Delete(Archives::Texture("GameClear"));
}


/*									//
//				入力				//
//									*/
void GameClear::Input() {
	for (int i = 0; i < BUTTON_MAX; ++i) {
		// 十字
		if (i == BUTTON_DOWN ||
			i == BUTTON_UP ||
			i == BUTTON_RIGHT ||
			i == BUTTON_LEFT) {
			continue;
		}

		// 入力
		if (Input::GetKeyTrigger(GetContManager()->Get()->GetKeyNum(i)) ||
			Input::GetJoyTrigger(0, GetContManager()->Get()->GetJoyNum(i))) {
			GetSceneManagerWins()->ChangeCommand("Title");
		}
	}
}