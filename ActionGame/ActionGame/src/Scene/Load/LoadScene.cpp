// ���[�f�B���O�V�[���N���X
// 2017. 2. 4	�v���O�����쐬
// Author		SyunMizuno


#include	"LoadScene.h"
#include	"../../GameFramework/Obj/2D/Obj2DManager.h"
#include	"../../GameFramework/Obj/3D/Obj3DManager.h"
#include	"../../Archives/Archives.h"
#include	"../../GameFramework/framework/Texture/TexManager.h"
#include	"../../GameFramework/framework/Convert.h"
#include	"../../GameFramework/framework/Screen/Screen.h"
#include	"../../GameFramework/framework/Scene/SceneManagerWin.h"
#include	"../../GameFramework/GameController/GameControllerManager.h"
#include	"../../GameFramework/framework/Time/Time.h"
#include	"../../GameFramework/framework/Graphic/Graphics.h"

/*									//
//			�R���X�g���N�^			//
//									*/
LoadScene::LoadScene() :
	m_pGauge(NULL),
	m_pLoadSprite(NULL)
{
}


/*									//
//			�f�X�g���N�^			//
//									*/
LoadScene::~LoadScene() {

}


/*									//
//				������				//
//									*/
bool LoadScene::Init() {
	Vector3 size = Vector3((float)Screen::GetWidth(), (float)Screen::GetHeight(), 0.0f);
	Vector3 pos(size.x / 2, size.y * 0.1f * 8.f, 0.0f);

	Sprite* sprite;

	// NowLoading...
	m_pLoadSprite = new LoadingSprite;

	// �g
	sprite = new Sprite;
	sprite->SetTexture(GetTexManager()->Get(Archives::Texture("Loading/GaugeFrame")));
	sprite->GetRectTransform()->SetPos(pos);
	sprite->SetName("LoadingGaugeFrame");
	m_aSprite.push_back(sprite);

	// �Q�[�W�̐F
	m_pGauge = new Gauge2;
	m_pGauge->SetTexture(GetTexManager()->Get(Archives::Texture("Loading/GaugeIn")));
	m_pGauge->GetRectTransform()->SetPos(pos);
	m_pGauge->SetName("LoadingGaugeIn");
	m_pGauge->SetCurrentNum(0);
	m_aSprite.push_back(m_pGauge);

	for (auto it = m_aSprite.begin(); it != m_aSprite.end(); it ++) {
		(*it)->SetActive(false);
		(*it)->DontDestroyOnLoad();
	}

	return true;
}


/*									//
//				���				//
//									*/
void LoadScene::Release() {
	for (auto it = m_aSprite.begin(); it != m_aSprite.end(); it ++) {
		SAFE_DELETE(*it);
	}
	m_aSprite.clear();

	SAFE_DELETE(m_pLoadSprite);

	GetTexManager()->Delete(Archives::Texture("Loading/GaugeIn"));
	GetTexManager()->Delete(Archives::Texture("Loading/GaugeFrame"));
}


/*									//
//				�X�V				//
//									*/
void LoadScene::Update() {
	Debug::Print("Scene : LoadScene\n");
	int Rate = (int)GetSceneManagerWins()->GetLoadRate();
	m_pGauge->SetNum((float)Rate);

	m_pLoadSprite->Update();
	for (auto it = m_aSprite.begin(); it != m_aSprite.end(); ++it)
		(*it)->UpdateAll();
	for (auto it = m_aSprite.begin(); it != m_aSprite.end(); ++it)
		(*it)->LateUpdateAll();

	// �ǂݍ��݃V�[���I��
	if(m_pGauge->CheckTarget())
		GetSceneManagerWins()->LoadingEnd();
}


/*									//
//				�`��				//
//									*/
void LoadScene::Draw() {
	for (auto it = m_aSprite.begin(); it != m_aSprite.end(); ++it)
		(*it)->DrawAll();
	m_pLoadSprite->Draw();
	for (auto it = m_aSprite.begin(); it != m_aSprite.end(); ++it)
		(*it)->LateDrawAll();
}


/*									//
//				�ď�����			//
//									*/
bool LoadScene::ReStart() {
	m_pGauge->SetCurrentNum(0.0f);
	return true;
}