///////////////////////////////////////////////////////////////
// スプライトクラス
// author		SyunMizuno
// 2016. 1. 3	プログラム作成
///////////////////////////////////////////////////////////////

#include	"Sprite.h"
#include	"Obj2DManager.h"
#include	"../../framework/Debug/Debug.h"
#include	"../../framework/Utility/System/SystemUtility.h"
#include	"../../framework/Graphic/Graphics.h"

/*									//
//			コンストラクタ			//
//									*/
Sprite::Sprite() :
	m_pTex(NULL),
	m_Color(255, 255, 255, 255),
	m_vTexPos(0.0f, 0.0f),
	m_vTexSize(1.0f, 1.0f)
{
	GetObj2DManager()->Add(this);

	m_pRectTransform = new RectTransform(this);

	MakeVtx();
}


/*									//
//			デストラクタ			//
//									*/
Sprite::~Sprite() {
	GetObj2DManager()->Delete(this);
	SAFE_DELETE(m_pRectTransform);
}


/*									//
//				更新				//
//									*/
void Sprite::Update() {
	SetVtx();
	SetTex();
	SetColor();
}


/*									//
//				描画				//
//									*/
void Sprite::Draw() {
	// ライティングを無効にする
	DWORD lightflg;
	GetGraphics()->GetDevice()->GetRenderState(D3DRS_LIGHTING, &lightflg);
	GetGraphics()->GetDevice()->SetRenderState(D3DRS_LIGHTING, FALSE);

	// 頂点フォーマットの設定
	GetGraphics()->GetDevice()->SetFVF(FVF_VERTEX_2D);

	// テクスチャのセット
	GetGraphics()->GetDevice()->SetTexture(0, m_pTex);

	// ポリゴンの描画
	GetGraphics()->GetDevice()->DrawPrimitiveUP(
		D3DPT_TRIANGLESTRIP,	// 1:プリミティブの種類
		NUM_POLYGON,			// 2:ポリゴン数
		m_aVtx,					// 3:配列の先頭アドレス
		sizeof(VERTEX_2D));	// 4:頂点のデータの大きさ

	// ラインティングを戻す
	GetGraphics()->GetDevice()->SetRenderState(D3DRS_LIGHTING, lightflg);
}


/*									//
//			頂点初期化				//
//									*/
void Sprite::MakeVtx() {
	// rhwの設定( 1.0固定 )
	m_aVtx[0].rhw = 
	m_aVtx[1].rhw = 
	m_aVtx[2].rhw = 
	m_aVtx[3].rhw = 1.0f;

	// カラーの設定( 0 ～ 255 )
	Sprite::SetColor();

	// 座標
	Sprite::SetVtx();

	// テクスチャ
	Sprite::SetTex();
}


/*									//
//			頂点座標の設定			//
//									*/
void Sprite::SetVtx() {
	Vector3* Vtx = m_pRectTransform->GetVertex();

	for (int i = 0; i < 4; ++ i) {
		m_aVtx[i].pos = Vtx[i];
	}
}


/*									//
//		テクスチャ座標の初期化		//
//									*/
void Sprite::SetTex() {
	// テクスチャ座標の設定
	m_aVtx[0].tex = m_vTexPos;
	m_aVtx[1].tex = Vector2(m_vTexPos.x + m_vTexSize.x, m_vTexPos.y);
	m_aVtx[2].tex = Vector2(m_vTexPos.x, m_vTexPos.y + m_vTexSize.y);
	m_aVtx[3].tex = Vector2(m_vTexPos.x + m_vTexSize.x, m_vTexPos.y + m_vTexSize.y);
}


/*									//
//			頂点カラーの設定		//
//									*/
void Sprite::SetColor() {
	// ( 0 ～ 255 )
	m_aVtx[0].col = m_Color;
	m_aVtx[1].col = m_Color;
	m_aVtx[2].col = m_Color;
	m_aVtx[3].col = m_Color;
}


/*									//
//		初期テクスチャサイズの取得	//
// 2のべき乗でないと自動で補正される*/
void Sprite::SetSizeDefault() {
	if (!m_pTex) {
		Debug::Log("テクスチャ未設定のため情報の取得はできません");
		return;
	}

	// テクスチャサイズの取得
	D3DSURFACE_DESC desc;

	if (FAILED(m_pTex->GetLevelDesc(0, &desc))) {
		Debug::LogError("テクスチャ情報の取得に失敗しました");
		return;
	}

	m_pRectTransform->SetSize((float)desc.Width, (float)desc.Height);
}