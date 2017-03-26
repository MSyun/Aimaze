// �Q�[���X�^�[�g�X�v���C�g
// 2017. 2. 7	�v���O�����쐬
// author		SyunMizuno


#include	"GameStart.h"
#include	"../../../GameFramework/framework/Screen/Screen.h"
#include	"../../../GameFramework/framework/Time/Time.h"
#include	"../../../GameFramework/framework/Graphic/Graphics.h"


/*									//
//			�R���X�g���N�^			//
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
//			�f�X�g���N�^			//
//									*/
GameStart::~GameStart() {
}


/*									//
//				�X�V				//
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
//				�`��				//
//									*/
void GameStart::Draw() {
	GetGraphics()->SetWrapMode(_eWrapMode::WRAPMODE_CLAMP);
	MovingSprite::Draw();
}