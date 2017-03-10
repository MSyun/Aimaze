// �v���C���[���
// 2017. 2. 9	�v���O�����쐬
// Author		SyunMizuno



#include	"PlayerInformation.h"
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

// �T�C�Y
const float LIFE_SIZE_WIDTH = 400.0f;
const float LIFE_SIZE_HEIGHT = 7.5f;
const float SUB_SIZE_WIDTH = 200.0f;
const float SUB_SIZE_HEIGHT = 5.f;

// ���W
const float LIFE_POS_X = 100.f + LIFE_SIZE_WIDTH / 2;
#define	LIFE_POS_Y				(Screen::GetHeight() - Screen::GetHeight() / 10.f)
const float BER_INTERVAL = 0.7f;




/*									//
//			�R���X�g���N�^			//
//									*/
PlayerInformation::PlayerInformation() :
	m_nMaxHp(10),
	m_nCurHp(10),
	m_pGauge(NULL)
{
	for (int i = 0; i < 2; ++i) {
		m_aHitPoint.push_back(new SpriteAnimation);

	}
	m_pGauge = new Gauge2;

	SetName("PlayerInformation");
	m_aHitPoint[HP_FRAME]->SetName("Player/HpFrame");
	m_aHitPoint[HP_FRAME_SPACE]->SetName("Player/HpFrameSpace");
	m_pGauge->SetName("PlayerInformation/Gauge");
}


/*									//
//			�f�X�g���N�^			//
//									*/
PlayerInformation::~PlayerInformation() {
	for (auto it = m_aHitPoint.begin(); it != m_aHitPoint.end(); ++it)
		SAFE_DELETE(*it);
	SAFE_DELETE(m_pGauge);
}


/*									//
//			�̗͍ő�l�ݒ�			//
//									*/
void PlayerInformation::SetMaxHp(int max) {
	m_nMaxHp = max;
	m_nCurHp = Clamp(m_nCurHp, 0, m_nMaxHp);
	m_pGauge->SetMaxNum((float)m_nMaxHp);
	m_pGauge->SetCurrentNum((float)m_nCurHp);
}


/*									//
//			�̗͌��ݒl�ݒ�			//
//									*/
void PlayerInformation::SetCurHp(int hp) {
	m_nCurHp = Clamp(hp, 0, m_nMaxHp);
	m_pGauge->SetCurrentNum((float)m_nCurHp);
}


/*									//
//				������				//
//									*/
void PlayerInformation::Init() {
	Vector2	Size((float)Screen::GetWidth(), (float)Screen::GetHeight());

	// HP�t���[���g
	m_aHitPoint[HP_FRAME]->Init(GetTexManager()->Get(Archives::Texture("Player/Frame")), Vector2(LIFE_SIZE_WIDTH + 15.f, LIFE_SIZE_HEIGHT + 15.f), 1, 1, 0);
	m_aHitPoint[HP_FRAME]->GetRectTransform()->SetPos(100.f + LIFE_SIZE_WIDTH / 2, Size.y - Size.y / 10.f, 0.0f);

	// �t���[���Ԋu
	m_aHitPoint[HP_FRAME_SPACE]->Init(GetTexManager()->Get(Archives::Texture("Player/Frame")), Vector2(LIFE_SIZE_WIDTH + 10.f, LIFE_SIZE_HEIGHT + 10.f), 1, 1, 0);
	m_aHitPoint[HP_FRAME_SPACE]->GetRectTransform()->SetPos(100.f + LIFE_SIZE_WIDTH / 2, Size.y - Size.y / 10.f, 0.0f);
	m_aHitPoint[HP_FRAME_SPACE]->SetColor(Color(0, 0, 0, 255));

	// �̗̓o�[
	m_pGauge->SetTexture(GetTexManager()->Get(Archives::Texture("Player/MaxLifeber")));
	m_pGauge->SetMaxNum((float)m_nMaxHp);
	m_pGauge->SetCurrentNum((float)m_nCurHp);
	m_pGauge->GetRectTransform()->SetPos(LIFE_POS_X, LIFE_POS_Y, 0.0f);
	m_pGauge->GetRectTransform()->SetSize(LIFE_SIZE_WIDTH, LIFE_SIZE_HEIGHT);

	SetActive(false);
}