// ウェーブスプライト
// 2017. 2. 4	プログラム作成
// author		SyunMizuno


#include	"LoadingSprite.h"
#include	"../Archives/Archives.h"
#include	"../GameFramework/framework/Texture/TexManager.h"
#include	"../GameFramework/framework/Screen/Screen.h"


const float GAP_WIDTH = 33.0f;
const float MOVE_HEIGHT = 10.0f;
const float CHANGE_TIME = 0.15f;


/*									//
//			コンストラクタ			//
//									*/
LoadingSprite::LoadingSprite() {
	MovingSprite* sprite;

	Vector2 size((float)Screen::GetWidth(), (float)Screen::GetHeight());

	for (int i = 0; i < 10; ++i) {
		sprite = new MovingSprite;
		Vector2 texSize = sprite->GetRectTransform()->GetSize();
		Vector3 pos(
			size.x * 0.5f + (GAP_WIDTH * i - GAP_WIDTH * 5.0f),
			size.y * 0.5f,
			0.0f);
		sprite->GetRectTransform()->SetPos(pos);
		sprite->SetActive(false);
		sprite->DontDestroyOnLoad();
		sprite->SetName("MovingSprite/Loading...");
		sprite->SetBeginPos(pos);
		sprite->SetChangeTime(CHANGE_TIME);
		sprite->SetEndPos(pos.x, pos.y - MOVE_HEIGHT, pos.z);
		sprite->GetOnEndPlay()->AddListener([=]() { sprite->Play(true); });
		if (i != 0)
			m_aSprite[i - 1]->GetOnEndPlay()->AddListener([=]() { sprite->Play(false); });
		m_aSprite.push_back(sprite);
	}
	m_aSprite[0]->SetTexture(GetTexManager()->Get(Archives::Texture("Loading/L")));
	m_aSprite[1]->SetTexture(GetTexManager()->Get(Archives::Texture("Loading/o")));
	m_aSprite[2]->SetTexture(GetTexManager()->Get(Archives::Texture("Loading/a")));
	m_aSprite[3]->SetTexture(GetTexManager()->Get(Archives::Texture("Loading/d")));
	m_aSprite[4]->SetTexture(GetTexManager()->Get(Archives::Texture("Loading/i")));
	m_aSprite[5]->SetTexture(GetTexManager()->Get(Archives::Texture("Loading/n")));
	m_aSprite[6]->SetTexture(GetTexManager()->Get(Archives::Texture("Loading/g")));
	m_aSprite[7]->SetTexture(GetTexManager()->Get(Archives::Texture("Loading/dot")));
	m_aSprite[8]->SetTexture(GetTexManager()->Get(Archives::Texture("Loading/dot")));
	m_aSprite[9]->SetTexture(GetTexManager()->Get(Archives::Texture("Loading/dot")));

	m_aSprite[0]->Play(false);
	m_aSprite[9]->GetOnEndPlay()->AddListener([=]() { m_aSprite[0]->Play(false); });
}


/*									//
//				解放				//
//									*/
LoadingSprite::~LoadingSprite() {
	GetTexManager()->Delete(Archives::Texture("Loading/L"));
	GetTexManager()->Delete(Archives::Texture("Loading/o"));
	GetTexManager()->Delete(Archives::Texture("Loading/a"));
	GetTexManager()->Delete(Archives::Texture("Loading/d"));
	GetTexManager()->Delete(Archives::Texture("Loading/i"));
	GetTexManager()->Delete(Archives::Texture("Loading/n"));
	GetTexManager()->Delete(Archives::Texture("Loading/g"));
	GetTexManager()->Delete(Archives::Texture("Loading/dot"));

	for (auto it = m_aSprite.begin(); it != m_aSprite.end(); ++ it)
		SAFE_DELETE(*it);
}


/*									//
//				更新				//
//									*/
void LoadingSprite::Update() {
	for (auto it = m_aSprite.begin(); it != m_aSprite.end(); ++ it)
		(*it)->UpdateAll();
	for (auto it = m_aSprite.begin(); it != m_aSprite.end(); ++ it)
		(*it)->LateUpdateAll();
}


/*									//
//				描画				//
//									*/
void LoadingSprite::Draw() {
	for (auto it = m_aSprite.begin(); it != m_aSprite.end(); ++ it)
		(*it)->DrawAll();
	for (auto it = m_aSprite.begin(); it != m_aSprite.end(); ++ it)
		(*it)->LateDrawAll();
}