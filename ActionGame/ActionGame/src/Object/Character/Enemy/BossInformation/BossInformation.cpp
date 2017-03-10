// ボス情報
// 2017. 2. 10	プログラム作成
// Author		SyunMizuno



#include	"BossInformation.h"
#include	"../../../../GameFramework/framework/Utility/Math/MathUtility.h"
#include	"../../../../Archives/Archives.h"
#include	"../../../../GameFramework/framework/Texture/TexManager.h"
#include	"../../../../GameFramework/framework/Screen/Screen.h"


enum {
	HP_FRAME = 0,
	HP_FRAME_SPACE,
	HP,
	MAX
};

// サイズ
const float LIFE_SIZE_WIDTH = 700.0f;
const float LIFE_SIZE_HEIGHT = 7.5f;

// 座標
const float LIFE_POS_X = 100.f + LIFE_SIZE_WIDTH / 2;
#define	LIFE_POS_Y				(Screen::GetHeight() / 10.f)
const float BER_INTERVAL = 0.7f;




/*									//
//			コンストラクタ			//
//									*/
BossInformation::BossInformation() :
	m_nMaxHp(10),
	m_nCurHp(10),
	m_pGauge(NULL)
{
	for (int i = 0; i < 2; ++i) {
		m_aHitPoint.push_back(new SpriteAnimation);

	}
	m_pGauge = new Gauge2;

	SetName("BossInformation");
	m_aHitPoint[HP_FRAME]->SetName("Boss/HpFrame");
	m_aHitPoint[HP_FRAME_SPACE]->SetName("Boss/HpFrameSpace");
	m_pGauge->SetName("BossInformation/Gauge");
}


/*									//
//			デストラクタ			//
//									*/
BossInformation::~BossInformation() {
	for (auto it = m_aHitPoint.begin(); it != m_aHitPoint.end(); ++it)
		SAFE_DELETE(*it);
	SAFE_DELETE(m_pGauge);
}


/*									//
//			体力最大値設定			//
//									*/
void BossInformation::SetMaxHp(int max) {
	m_nMaxHp = max;
	m_nCurHp = Clamp(m_nCurHp, 0, m_nMaxHp);
	m_pGauge->SetMaxNum((float)m_nMaxHp);
	m_pGauge->SetCurrentNum((float)m_nCurHp);
}


/*									//
//			体力現在値設定			//
//									*/
void BossInformation::SetCurHp(int hp) {
	m_nCurHp = Clamp(hp, 0, m_nMaxHp);
	m_pGauge->SetCurrentNum((float)m_nCurHp);
}


/*									//
//				初期化				//
//									*/
void BossInformation::Init() {
	Vector2	Size((float)Screen::GetWidth(), (float)Screen::GetHeight());

	// HPフレーム枠
	m_aHitPoint[HP_FRAME]->Init(
		GetTexManager()->Get(Archives::Texture("Boss/Frame")),
		Vector2(LIFE_SIZE_WIDTH + 15.f, LIFE_SIZE_HEIGHT + 15.f),
		1,
		1,
		0);
	m_aHitPoint[HP_FRAME]->GetRectTransform()->SetPos(LIFE_POS_X, LIFE_POS_Y, 0.0f);

	// フレーム間隔
	m_aHitPoint[HP_FRAME_SPACE]->Init(GetTexManager()->Get(Archives::Texture("Boss/Frame")), Vector2(LIFE_SIZE_WIDTH + 10.f, LIFE_SIZE_HEIGHT + 10.f), 1, 1, 0);
	m_aHitPoint[HP_FRAME_SPACE]->GetRectTransform()->SetPos(LIFE_POS_X, LIFE_POS_Y, 0.0f);
	m_aHitPoint[HP_FRAME_SPACE]->SetColor(Color(0.0f, 0.0f, 0.0f, 255.0f));

	// 体力バー
	m_pGauge->SetTexture(GetTexManager()->Get(Archives::Texture("Boss/MaxLifeber")));
	m_pGauge->SetMaxNum((float)m_nMaxHp);
	m_pGauge->SetCurrentNum((float)m_nCurHp);
	m_pGauge->GetRectTransform()->SetPos(LIFE_POS_X, LIFE_POS_Y, 0.0f);
	m_pGauge->GetRectTransform()->SetSize(LIFE_SIZE_WIDTH, LIFE_SIZE_HEIGHT);

	SetActive(false);
}