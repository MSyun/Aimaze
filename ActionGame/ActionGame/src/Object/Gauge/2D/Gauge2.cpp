// 2Dゲージクラス
// 2017. 2. 4	プログラム作成
// Author		SyunMizuno


#include	"Gauge2.h"
#include	"../../../GameFramework/framework/Utility/Math/MathUtility.h"
#include	"../../../GameFramework/framework/Time/Time.h"


/*									//
//			コンストラクタ			//
//									*/
Gauge2::Gauge2(int _max) :
	m_fMax((float)_max),
	m_fCur((float)_max),
	m_fTarget((float)_max)
{
	SetName("Gauge2");
}


/*									//
//			デストラクタ			//
//									*/
Gauge2::~Gauge2() {

}


/*									//
//				更新				//
//									*/
void Gauge2::Update() {
	Sprite::Update();

	if (m_fTarget > m_fCur) {
		m_fCur += (Time::GetDeltaTime() * m_fTarget);
		if (m_fTarget < m_fCur)
			m_fCur = m_fTarget;
	} else
	if (m_fTarget < m_fCur) {
		m_fCur -= (Time::GetDeltaTime() * m_fTarget);
		if (m_fTarget > m_fCur)
			m_fCur = m_fTarget;
	}
}


/*									//
//			最大の数値を設定		//
//									*/
void Gauge2::SetMaxNum(float _max) {
	m_fMax = _max;

	if (m_fMax < m_fCur) {
		m_fCur = m_fTarget = m_fMax;
	}
}


/*									//
//			現在の数値を設定		//
//									*/
void Gauge2::SetCurrentNum(float _num) {
	m_fTarget = m_fCur = Clamp(_num, 0.0f, m_fMax);
}


/*									//
//			目的の値を設定			//
//									*/
void Gauge2::SetNum(float _num) {
	m_fTarget = _num;
}


/*									//
//			頂点座標の設定			//
//									*/
void Gauge2::SetVtx() {
	RectTransform* trans = GetRectTransform();
	Vector3* Vtx = trans->GetVertex();
	float sizeX = trans->GetSize().x / m_fMax * m_fCur;
	float Angle = trans->GetAngle();
	float fSin = sinf(D3DXToRadian(Angle));
	float fCos = cosf(D3DXToRadian(Angle));
	Vector3 scale = trans->GetScale();
	Vector2 size = trans->GetSize();
	Vector3 pos = trans->GetPos();

	m_aVtx[0].pos = Vtx[0];

	m_aVtx[1].pos = Vector3(
		m_aVtx[0].pos.x + (sizeX * fCos * scale.x) - (-sizeX * fSin * scale.x),
		pos.y - (size.y * 0.5f * fSin * scale.y) + (-size.y * 0.5f * fCos * scale.y),
		pos.z);

	m_aVtx[2].pos = Vtx[2];

	m_aVtx[3].pos = Vector3(
		m_aVtx[2].pos.x + (sizeX * fCos * scale.x) - (sizeX * fSin * scale.x),
		pos.y - (size.y * 0.5f * fSin * scale.y) + (size.y * 0.5f * fCos * scale.y),
		0.0f);
}


/*									//
//		テクスチャ座標の設定		//
//									*/
void Gauge2::SetTex() {
	// テクスチャ座標
	float texX = 1.0f / m_fMax * m_fCur;

	// テクスチャ座標の設定
	m_aVtx[0].tex = Vector2(0.0f, 0.0f);
	m_aVtx[1].tex = Vector2(texX, 0.0f);
	m_aVtx[2].tex = Vector2(0.0f, 1.0f);
	m_aVtx[3].tex = Vector2(texX, 1.0f);
}