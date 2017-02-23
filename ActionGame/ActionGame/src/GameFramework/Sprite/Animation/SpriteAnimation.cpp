// author		SyunMizuno
// 2016. 2. 4	プログラム作成


#include	"SpriteAnimation.h"
#include	"../../framework/Time/Time.h"


/*									//
//			コンストラクタ			//
//									*/
SpriteAnimation::SpriteAnimation() :
	m_fCounter(0.0f),
	m_nPattern(0),
	m_nDivideX(1),
	m_nDivideY(1),
	m_fAnimationSpeed(0.1f)
{
	SetName("SpriteAnimation");
}


/*									//
//			デストラクタ			//
//									*/
SpriteAnimation::~SpriteAnimation() {
}


/*									//
//				初期化				//
//									*/
HRESULT SpriteAnimation::Init(
	IDirect3DTexture9* _pTex,
	Vector2 _size,
	int		_divX,
	int		_divY,
	float	_count)
{
	Sprite::SetTexture(_pTex);
	GetRectTransform()->SetSize(_size);
	m_fCounter = _count;
	m_nDivideX = _divX;
	m_nDivideY = _divY;

	return S_OK;
}


/*									//
//				更新				//
//									*/
void SpriteAnimation::Update() {
	SetVtx();
	SetColor();
	Animation();
}


/*									//
//		テクスチャ座標の設定		//
//									*/
void SpriteAnimation::SetTex() {
	m_aVtx[0].tex.x = (float)(m_nPattern % m_nDivideX *(1.0f / m_nDivideX));
	m_aVtx[0].tex.y = (float)(m_nPattern / m_nDivideX *(1.0f / m_nDivideY));
	m_aVtx[1].tex.x = m_aVtx[0].tex.x + 1.0f / m_nDivideX;
	m_aVtx[1].tex.y = m_aVtx[0].tex.y;
	m_aVtx[2].tex.x = m_aVtx[0].tex.x;
	m_aVtx[2].tex.y = m_aVtx[0].tex.y + 1.0f / m_nDivideY;
	m_aVtx[3].tex.x = m_aVtx[1].tex.x;
	m_aVtx[3].tex.y = m_aVtx[2].tex.y;
}


/*									//
//		アニメーション番号の設定	//
//									*/
void SpriteAnimation::Animation() {
	m_fCounter += Time::GetDeltaTime();
	if(m_fCounter >= m_fAnimationSpeed) {
		m_fCounter = 0.0f;
		// アニメパターンの切り替え
		++m_nPattern;
		SetTex();
	}
}


/*									//
//		アニメーション速度の設定	//
//									*/
void SpriteAnimation::SetAnimationSpeed(float speed) {
	m_fAnimationSpeed = speed;
}