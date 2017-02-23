// ミッションクラス
// 2017. 2. 7	プログラム作成
// author		SyunMizuno



#include	"Mission.h"
#include	"../GameFramework/GameController/GameControllerManager.h"
#include	"../GameFramework/framework/Screen/Screen.h"
#include	"../GameFramework/framework/Texture/TexManager.h"
#include	"../Archives/Archives.h"


/*									//
//			コンストラクタ			//
//									*/
Mission::Mission(
	Vector3 begin,
	Vector3 end,
	float time,
	bool reverse,
	bool awake) :
	ScalingSprite(begin, end, time, reverse, awake),
	m_pButton(NULL)
{
	Vector2 Size((float)Screen::GetWidth(), (float)Screen::GetHeight());

	// Maru
	m_pButton = new FadeColorSprite(
		Color(1.0f, 1.0f, 1.0f, 0.0f),
		Color(1.0f, 1.0f, 1.0f, 1.0f),
		1.0f);
	m_pButton->Reset(false);
	m_pButton->SetTexture(GetTexManager()->Get(Archives::Texture("Button/MARU")));
	m_pButton->GetOnEndPlayReverse()->AddListener([&]() {
		m_pButton->Play(false);
	});
	m_pButton->GetOnEndPlay()->AddListener([&]() {
		m_pButton->Play(true);
	});
	GetOnEndPlay()->AddListener([&]() {
		m_pButton->Play(false);
		SetOperate(true);
	});
	GetOnEndPlayReverse()->AddListener([&]() {
		Destroy();
	});
	m_pButton->GetRectTransform()->SetPos(Size.x * 0.66f, Size.y * 0.6f, 0.0f);
	m_pButton->GetRectTransform()->SetScale(0.1f, 0.1f, 0.1f);

	ChangeEase(EaseOutBack);
}


/*									//
//			デストラクタ			//
//									*/
Mission::~Mission() {

}


/*									//
//				入力				//
//									*/
void Mission::Input() {
	GameController* cont = GetContManager()->Get();
	if (Input::GetKeyTrigger(cont->GetKeyNum(BUTTON_MARU)) ||
		Input::GetJoyTrigger(0, cont->GetJoyNum(BUTTON_MARU)))
	{
		SetOperate(false);
		this->Play(true);
		m_pButton->Destroy();
	}
}