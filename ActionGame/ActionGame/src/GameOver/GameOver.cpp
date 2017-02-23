// ゲームオーバークラス
// 2017. 2. 9	プログラム作成
// author		SyunMizuno


#include	"GameOver.h"
#include	"../GameFramework/GameController/GameControllerManager.h"
#include	"../GameFramework/framework/Screen/Screen.h"
#include	"../GameFramework/framework/Texture/TexManager.h"
#include	"../Archives/Archives.h"
#include	"../GameFramework/Obj/3D/Obj3DManager.h"
#include	"../GameFramework/framework/Scene/SceneManagerWin.h"
#include	"../GameFramework/framework/Sound/SoundManager.h"



/*									//
//			コンストラクタ			//
//									*/
GameOver::GameOver() :
	m_pBack(NULL)
{
	Vector2 Size((float)Screen::GetWidth(), (float)Screen::GetHeight());

	// 文字
	SetName("GameOver");
	SetTexture(GetTexManager()->Get(Archives::Texture("GameOver")));
	GetRectTransform()->SetPos(Size.x, Size.y, 0.0f);
	SetBeginPos(Size.x * 0.5f, -Size.y * 0.5f, 0.0f);
	SetEndPos(Size.x * 0.5f, Size.y * 0.5f, 0.20f);
	SetChangeTime(3.0f);
	Reset(false);
	Play(false);
	GetOnEndPlay()->AddListener(bind(&GameOver::ChangeOperate, this));

	// 背景
	m_pBack = new FadeColorSprite(
		Color(0.3f, 0.3f, 0.3f, 0.0f),
		Color(0.3f, 0.3f, 0.3f, 0.5f),
		2.0f);
	m_pBack->SetName("GameOver/Back");
	m_pBack->Reset(false);
	m_pBack->GetRectTransform()->SetPos(Size.x * 0.5f, Size.y * 0.5f, 0.0f);
	m_pBack->GetRectTransform()->SetSize(Size);
	m_pBack->Reset(false);
	m_pBack->Play(false);

	Obj3D* obj;
	obj = GetObj3DManager()->Find("Player");
	if (obj)
		obj->ChangeOperate();
	obj = GetObj3DManager()->Find("CameraControll");
	if (obj)
		obj->ChangeOperate();

	GetSoundManager()->Load(SOUND_SE, Archives::Sound("GameOver").c_str(), "Gameover");
	GetSoundManager()->Play(SOUND_SE, "Gameover");
	GetSoundManager()->Stop(SOUND_BGM, "Field");
}


/*									//
//			デストラクタ			//
//									*/
GameOver::~GameOver() {
	SAFE_DELETE(m_pBack);
	GetSoundManager()->Delete(SOUND_SE, "Gameover");
}


/*									//
//				入力				//
//									*/
void GameOver::Input() {
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