// ゲームスタートスプライト
// 2017. 2. 7	プログラム作成
// author		SyunMizuno


#include	"GameStart.h"
#include	"../../../GameFramework/framework/Screen/Screen.h"
#include	"../../../GameFramework/framework/Time/Time.h"
#include	"../../../GameFramework/framework/Graphic/Graphics.h"


/*									//
//			コンストラクタ			//
//									*/
GameStart::GameStart() :
	m_fTime(-1.0f)
{
	Vector2 size((float)Screen::GetWidth(), (float)Screen::GetHeight());

	SetBeginPos(size.x * 1.5f, size.y * 0.5f, 0.0f);
	SetEndPos(size.x * 0.5f, size.y * 0.5f, 0.0f);
	GetOnEndPlay()->AddListener([=]() {
		m_fTime = 1.0f;
		SetBeginPos(-size.x * 0.5f, size.y * 0.5f, 0.0f);
	});
	Reset(false);
	ChangeEase(EaseOutCirc);

	SetChangeTime(1.0f);
}


/*									//
//			デストラクタ			//
//									*/
GameStart::~GameStart() {
}


/*									//
//				更新				//
//									*/
void GameStart::Update() {
	MovingSprite::Update();

	if (m_fTime > 0.0f) {
		m_fTime -= Time::GetDeltaTime();
		if (m_fTime <= 0.0f) {
			Play(true);
			GetOnEndPlayReverse()->AddListener([&]() { Destroy(); });
		}
	}
}


/*									//
//				描画				//
//									*/
void GameStart::Draw() {
	GetGraphics()->SetWrapMode(_eWrapMode::WRAPMODE_CLAMP);
	MovingSprite::Draw();
}