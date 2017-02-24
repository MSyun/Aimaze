// タイトルシーン
// 2017. 1. 1	プログラム作成
// author		SyunMizuno


#include	"TitleScene.h"
#include	"../../GameFramework/framework/Debug/Debug.h"
#include	"../../Archives/Archives.h"
#include	"../../GameFramework/framework/Texture/TexManager.h"
#include	"../../GameFramework/framework/Screen/Screen.h"
#include	"../../GameFramework/framework/Application/Application.h"
#include	"../../GameFramework/framework/Utility/System/SystemUtility.h"
#include	"../../GameFramework/framework/Scene/SceneManagerWin.h"
#include	"../../GameFramework/GameController/GameControllerManager.h"
#include	"../../GameFramework/Obj/2D/Obj2DManager.h"
#include	"../../GameFramework/Obj/3D/Obj3DManager.h"
#include	"../../GameFramework/Sprite/FadeColor/FadeColorSprite.h"
#include	"../../GameFramework/framework/Sound/SoundManager.h"
#include	"../../GameFramework/framework/Graphic/Graphics.h"
#include	"../../GameFramework/DrawManager/DrawManager.h"



typedef enum {
	MODE_GAME = 0,
	MODE_OPTION,
	MODE_END,
	MODE_MAX
} _mode;


const float MODE_FONT_GAP = 0.08f;

const Point3 MODE_FONT_POS = Point3(0.5f, 0.72f, 0.0f);


/*									//
//			コンストラクタ			//
//									*/
TitleScene::TitleScene() :
	m_nSelectMode(MODE_MAX)
{

}


/*									//
//			デストラクタ			//
//									*/
TitleScene::~TitleScene() {

}


/*									//
//				初期化				//
//									*/
bool TitleScene::Init() {
	Vector2 Size = Vector2((float)Screen::GetWidth(), (float)Screen::GetHeight());

	// モード
	m_nSelectMode = MODE_MAX;

	// 背景
	Sprite* pSprite;
	pSprite = new Sprite;
	pSprite->SetTexture(GetTexManager()->Get(Archives::Texture("Title/Back")));
	pSprite->GetRectTransform()->SetPos(Size.x * 0.5f, Size.y * 0.5f, 0.0f);
	pSprite->GetRectTransform()->SetSize(1280.0f, 720.0f);

	// タイトル名
	pSprite = new Sprite;
	pSprite->SetTexture(GetTexManager()->Get(Archives::Texture("Title/Name")));
	pSprite->GetRectTransform()->SetScale(1.3f, 1.3f, 1.3f);
	pSprite->GetRectTransform()->SetPos(Size.x * 0.5f, Size.y * 0.2f, 0.0f);

	// TouchStart
	FadeColorSprite* pPush;
	pPush = new FadeColorSprite(
		Color(1.0f, 1.0f, 1.0f, 0.0f),
		Color(1.0f, 1.0f, 1.0f, 1.0f),
		1.0f,
		false,
		true);
	pPush->SetName("TouchStart");
	pPush->SetTexture(GetTexManager()->Get(Archives::Texture("PressButton")));
	pPush->GetRectTransform()->SetPos(Size.x * 0.5f, Size.y * 0.8f, 0.0f);
	pPush->SetActive(false);
	pPush->GetOnEndPlayReverse()->AddListener([=]() { pPush->Play(false); });
	pPush->GetOnEndPlay()->AddListener([=]() { pPush->Play(true); });
	m_pPress = pPush;

	// 選択バー
	pSprite = new Sprite;
	pSprite->SetTexture(GetTexManager()->Get(Archives::Texture("Title/Selector")));
	m_pSelecter = pSprite->GetRectTransform();
	m_pSelecter->SetPos(Size.x * MODE_FONT_POS.x, Size.y * MODE_FONT_POS.y, MODE_FONT_POS.z );
	m_pSelecter->SetScale(0.5f, 0.3f, 0.5f);
	pSprite->SetActive(false);

	// モード
	for (int i = 0; i < MODE_MAX; ++i) {
		pSprite = new Sprite;
		pSprite->GetRectTransform()->SetPos(Size.x * MODE_FONT_POS.x, Size.y * (MODE_FONT_POS.y + MODE_FONT_GAP * i), MODE_FONT_POS.z);
		pSprite->GetRectTransform()->SetScale(0.5f, 0.5f, 0.5f);
		pSprite->SetActive(false);
		m_aMode.push_back(pSprite);
	}
	m_aMode[MODE_GAME]->SetTexture(GetTexManager()->Get(Archives::Texture("Title/Begin")));
	m_aMode[MODE_OPTION]->SetTexture(GetTexManager()->Get(Archives::Texture("Title/Option")));
	m_aMode[MODE_END]->SetTexture(GetTexManager()->Get(Archives::Texture("Title/End")));

	GetSceneManagerWins()->GetFade()->GetOnEndFadeIn()->AddListener([=]() { pPush->SetActive(true); });

	m_pPress->GetOnDestroy()->AddListener([&]() {
		for (auto it = m_aMode.begin(); it != m_aMode.end(); ++it)
			(*it)->SetActive(true);
		m_pSelecter->GetGameObject()->SetActive(true);
	});

	// 入力
	m_bInput = false;
	GetSceneManagerWins()->GetFade()->GetOnEndFadeIn()->AddListener([&]() {
		m_bInput = true;
		Debug::Log("入力OK");
	});

	//----- サウンド
	GetSoundManager()->Load(SOUND_SE, Archives::Sound("Cursor").c_str(), "Cursor");
	GetSoundManager()->Load(SOUND_SE, Archives::Sound("Start").c_str(), "Start");

	return true;
}


/*									//
//				解放				//
//									*/
void TitleScene::Release() {
	//----- サウンド
	GetSoundManager()->Delete(SOUND_SE, "Cursor");
	GetSoundManager()->Delete(SOUND_SE, "Start");

	GetTexManager()->Delete(Archives::Texture("Title/End"));
	GetTexManager()->Delete(Archives::Texture("Title/Option"));
	GetTexManager()->Delete(Archives::Texture("Title/Begin"));
	GetTexManager()->Delete(Archives::Texture("Title/Selector"));
	GetTexManager()->Delete(Archives::Texture("PressButton"));
	GetTexManager()->Delete(Archives::Texture("Title/Name"));
	GetTexManager()->Delete(Archives::Texture("Title/Back"));
	for (auto it = m_aMode.begin(); it != m_aMode.end(); ++it)
		SAFE_DELETE(*it);
	m_aMode.clear();
	m_pSelecter = NULL;
	GetObj2DManager()->AllClear();
	GetObj3DManager()->AllClear();
}


/*									//
//				更新				//
//									*/
void TitleScene::Update() {
	Debug::Print("Scene : Title\n");

	GetObj3DManager()->Update();
	GetObj2DManager()->Update();
	GetObj3DManager()->LateUpdate();
	GetObj2DManager()->LateUpdate();
	GetObj3DManager()->CheckCollider();

	GetObj3DManager()->CheckDestroy();
	GetObj2DManager()->CheckDestroy();


	if (!m_bInput)	return;

	// モードの選択
	if (!ModeSelect())	return;

	// シーン決定
	ChangeScene();
}


/*									//
//			モード選択の入力		//
//									*/
bool TitleScene::ModeSelect() {
	if (m_nSelectMode != MODE_MAX) {
		// 上
		if (Input::GetKeyTrigger(GetContManager()->Get()->GetKeyNum(BUTTON_UP)) ||
			Input::GetJoyAxisTrigger(0, JOY_Y) == GetContManager()->Get()->GetJoyNum(BUTTON_UP))
		{
			m_nSelectMode = m_nSelectMode - 1 < 0 ? MODE_MAX - 1 : m_nSelectMode - 1;
			m_pSelecter->SetPos(m_aMode[m_nSelectMode]->GetRectTransform()->GetPos());
			GetSoundManager()->Play(SOUND_SE, "Cursor");
		} else
		// 下
		if (Input::GetKeyTrigger(GetContManager()->Get()->GetKeyNum(BUTTON_DOWN)) ||
			Input::GetJoyAxisTrigger(0, JOY_Y) == GetContManager()->Get()->GetJoyNum(BUTTON_DOWN))
		{
			m_nSelectMode = (m_nSelectMode + 1) % MODE_MAX;
			m_pSelecter->SetPos(m_aMode[m_nSelectMode]->GetRectTransform()->GetPos());
			GetSoundManager()->Play(SOUND_SE, "Cursor");
		} else
		// 決定
		if (Input::GetKeyTrigger(GetContManager()->Get()->GetKeyNum(BUTTON_MARU)) ||
			Input::GetJoyTrigger(0, GetContManager()->Get()->GetJoyNum(BUTTON_MARU)))
		{
			GetSoundManager()->Play(SOUND_SE, "Start");
			return true;
		}
	} else {
	// Push Any Button
		for (int i = 0; i < BUTTON_MAX; ++i) {
			// 十字
			if (i == BUTTON_DOWN ||
				i == BUTTON_UP ||
				i == BUTTON_RIGHT ||
				i == BUTTON_LEFT)
			{
				continue;
			}

			// 入力
			if (Input::GetKeyTrigger(GetContManager()->Get()->GetKeyNum(i)) ||
				Input::GetJoyTrigger(0, GetContManager()->Get()->GetJoyNum(i)))
			{
				GetSoundManager()->Play(SOUND_SE, "Cursor");
				return true;
			}
		}
	}

	return false;
}


/*									//
//			シーンの変更			//
//									*/
void TitleScene::ChangeScene() {
	switch (m_nSelectMode) {
	case MODE_GAME:
		GetSceneManagerWins()->ChangeLoadCommand("Game");
		break;

	case MODE_OPTION:
		GetSceneManagerWins()->PushCommand("Config");
		break;

	case MODE_END:
		Application::End();
		break;

	case MODE_MAX:
		m_nSelectMode = MODE_GAME;
		m_pPress->Destroy();
		break;
	}
}


/*									//
//				描画				//
//									*/
void TitleScene::Draw() {

	GetGraphics()->GetDevice()->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
	GetDrawManager()->Draw();

	GetGraphics()->GetDevice()->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	GetDrawManager()->LateDraw();

	GetObj2DManager()->Draw();
	GetObj2DManager()->LateDraw();
}