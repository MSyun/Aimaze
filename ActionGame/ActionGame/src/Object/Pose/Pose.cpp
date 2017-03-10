// ポーズ
// 2017. 2. 10	プログラム作成
// author		SyunMizuno


#include	"Pose.h"
#include	"../../GameFramework/framework/Screen/Screen.h"
#include	"../../GameFramework/framework/Texture/TexManager.h"
#include	"../../Archives/Archives.h"
#include	"../../GameFramework/framework/Sound/SoundManager.h"
#include	"../../GameFramework/framework/Time/Time.h"
#include	"../../GameFramework/GameController/GameControllerManager.h"
#include	"../../GameFramework/framework/Scene/SceneManagerWin.h"
#include	"../../GameFramework/framework/Collision/Collision.h"
#include	"../../GameFramework/Obj/3D/Obj3DManager.h"


typedef enum {
	SELECT_BACK = 0,
	SELECT_OPTION,
	SELECT_DEBUG,
	SELECT_END,
	SELECT_MAX
} _select;


/*									//
//			コンストラクタ			//
//									*/
Pose::Pose() :
	m_bOpen(false),
	m_pSelector(NULL),
	m_nSelect(SELECT_BACK),
	m_pPlayer(NULL)
{
	Vector2 size((float)Screen::GetWidth(), (float)Screen::GetHeight());

	// 枠
	Sprite* sprite;
	sprite = new Sprite;
	sprite->SetTexture(GetTexManager()->Get(Archives::Texture("Menu/Menu")));
	sprite->GetRectTransform()->SetPos(size.x * 0.5f, size.y * 0.5f, 0.0f);
	m_aSprite.push_back(sprite);

	// 選択
	m_pSelector = new FadeColorSprite(
		Color(1.0f, 1.0f, 1.0f, 1.0f),
		Color(1.0f, 1.0f, 1.0f, 0.2f),
		1.0f
	);
	m_pSelector->DependTimeScale(false);
	m_pSelector->GetOnEndPlayReverse()->AddListener([=]() { m_pSelector->Play(false); });
	m_pSelector->GetOnEndPlay()->AddListener([=]() { m_pSelector->Play(true); });
	m_pSelector->SetTexture(GetTexManager()->Get(Archives::Texture("Menu/Selector")));
	m_pSelector->GetRectTransform()->SetPos(size.x * 0.5f, size.y * 0.4f, 0.0f);
	m_pSelector->Play(false);
	// 戻る
	sprite = new Sprite;
	sprite->SetTexture(GetTexManager()->Get(Archives::Texture("Menu/Back")));
	sprite->GetRectTransform()->SetPos(size.x * 0.5f, size.y * 0.4f, 0.0f);
	m_aMode.push_back(sprite);
	// オプション
	sprite = new Sprite;
	sprite->SetTexture(GetTexManager()->Get(Archives::Texture("Menu/Option")));
	sprite->GetRectTransform()->SetPos(size.x * 0.5f, size.y * 0.5f, 0.0f);
	m_aMode.push_back(sprite);
	// デバッグ
	sprite = new Sprite;
	sprite->SetTexture(GetTexManager()->Get(Archives::Texture("Menu/Debug")));
	sprite->GetRectTransform()->SetPos(size.x * 0.5f, size.y * 0.6f, 0.0f);
	m_aMode.push_back(sprite);
	// 諦める
	sprite = new Sprite;
	sprite->SetTexture(GetTexManager()->Get(Archives::Texture("Menu/End")));
	sprite->GetRectTransform()->SetPos(size.x * 0.5f, size.y * 0.7f, 0.0f);
	m_aMode.push_back(sprite);

	////----- サウンド
	//GetSoundManager()->Load(SOUND_SE, "data/SOUND/SE/pose.wav", "pose");
	//GetSoundManager()->Load(SOUND_SE, "data/SOUND/SE/cursor.wav", "cursor");

	Close();
}


/*									//
//				初期化				//
//									*/
void Pose::Init() {
	Sprite::Init();

	GetRectTransform()->SetScale(0.0f, 0.0f, 0.0f);

	m_pPlayer = GetObj3DManager()->Find("Player");
}


/*									//
//			デストラクタ			//
//									*/
Pose::~Pose() {
	for (auto it = m_aSprite.begin(); it != m_aSprite.end(); ++it)
		SAFE_DELETE(*it);
	for (auto it = m_aMode.begin(); it != m_aMode.end(); ++it)
		SAFE_DELETE(*it);
	SAFE_DELETE(m_pSelector);

	GetTexManager()->Delete(Archives::Texture("Menu/End"));
	GetTexManager()->Delete(Archives::Texture("Menu/Debug"));
	GetTexManager()->Delete(Archives::Texture("Menu/Option"));
	GetTexManager()->Delete(Archives::Texture("Menu/Back"));
	GetTexManager()->Delete(Archives::Texture("Menu/Selector"));
	GetTexManager()->Delete(Archives::Texture("Menu/Menu"));
}


/*									//
//				開く				//
//									*/
void Pose::Open() {
//	GetSoundManager()->Play(SOUND_SE, Archives::Sound("").c_str());
	for (auto it = m_aSprite.begin(); it != m_aSprite.end(); ++it)
		(*it)->SetActive(true);
	for (auto it = m_aMode.begin(); it != m_aMode.end(); ++it)
		(*it)->SetActive(true);
	m_pSelector->SetActive(true);
	Time::SetTimeScale(0.0f);
	m_bOpen = true;
	if (m_pPlayer)
		m_pPlayer->SetOperate(false);
	m_pSelector->GetRectTransform()->SetPos(m_aMode[m_nSelect]->GetRectTransform()->GetPos());
}


/*									//
//				閉じる				//
//									*/
void Pose::Close() {
	m_nSelect = SELECT_BACK;
//	GetSoundManager()->Play(SOUND_SE, Archives::Sound("").c_str());
	for (auto it = m_aSprite.begin(); it != m_aSprite.end(); ++it)
		(*it)->SetActive(false);
	for (auto it = m_aMode.begin(); it != m_aMode.end(); ++it)
		(*it)->SetActive(false);
	m_pSelector->SetActive(false);
	Time::SetTimeScale(1.0f);
	m_bOpen = false;
	if (m_pPlayer)
		m_pPlayer->SetOperate(true);
}


/*									//
//				更新				//
//									*/
void Pose::Update() {
	Sprite::Update();

	//----- スタートボタン
	if (Input::GetKeyTrigger(GetContManager()->Get()->GetKeyNum(BUTTON_START)) ||
		Input::GetJoyTrigger(0, GetContManager()->Get()->GetJoyNum(BUTTON_START)))
	{
		if (m_bOpen)
			Close();
		else
			Open();
	}

	if (!m_bOpen)	return;

	if (!ModeSelect())	return;

	Selection();
}


/*									//
//			モードの選択			//
//									*/
bool Pose::ModeSelect() {
	// 上
	if (Input::GetKeyTrigger(GetContManager()->Get()->GetKeyNum(BUTTON_UP)) ||
		Input::GetJoyAxisTrigger(0, JOY_Y) == GetContManager()->Get()->GetJoyNum(BUTTON_UP)) {
		m_nSelect = m_nSelect - 1 < 0 ? SELECT_MAX - 1 : m_nSelect - 1;
		//		GetSoundManager()->Play(SOUND_SE, Archives::Sound("").c_str());
		m_pSelector->GetRectTransform()->SetPos(m_aMode[m_nSelect]->GetRectTransform()->GetPos());
	} else
	// 下
	if (Input::GetKeyTrigger(GetContManager()->Get()->GetKeyNum(BUTTON_DOWN)) ||
		Input::GetJoyAxisTrigger(0, JOY_Y) == GetContManager()->Get()->GetJoyNum(BUTTON_DOWN)) {
		m_nSelect = (m_nSelect + 1) % SELECT_MAX;
		//		GetSoundManager()->Play(SOUND_SE, Archives::Sound("").c_str());
		m_pSelector->GetRectTransform()->SetPos(m_aMode[m_nSelect]->GetRectTransform()->GetPos());
	} else
	// 決定
	if (Input::GetKeyTrigger(GetContManager()->Get()->GetKeyNum(BUTTON_MARU)) ||
		Input::GetJoyTrigger(0, GetContManager()->Get()->GetJoyNum(BUTTON_MARU)))
	{
		return true;
	}

	return false;
}


/*									//
//				選択				//
//									*/
void Pose::Selection() {
	switch (m_nSelect) {
	case SELECT_BACK:
		Close();
		break;

	case SELECT_OPTION:
//		GetSceneManagerWins()->PushCommand("Config");
		break;

	case SELECT_DEBUG:
		ChangeDrawCollider();
		Debug::ChangeDraw();
		break;

	case SELECT_END:
		GetSceneManagerWins()->ChangeCommand("Title");
		Close();
		break;
	}
}