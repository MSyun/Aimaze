//	ライトクラス
// 2016. 3.27	プログラム作成
// author		SyunMizuno

#include	"Light.h"
#include	<tchar.h>
#include	"../Graphic/Graphics.h"


/*									//
//			コンストラクタ			//
//									*/
Light::Light() {
	m_vPos		= Vector3( 0.f, 10.f, 0.f );
	m_vLookAt	= Vector3( 0.f, 0.f, 0.f );
	m_vUp		= Vector3( 0.f, 1.f, 0.f );
	D3DXMatrixIdentity( &m_mtxView );
	D3DXMatrixIdentity( &m_mtxProj );
	m_fNear		= 1.f;
	m_fFar		= 20.f;
	m_fSpeclar	= 100.0f;
	m_vColor	= Color( 1.0f, 1.0f, 1.0f, 1.0f );
}


/*									//
//			デストラクタ			//
//									*/
Light::~Light() {
}


/*									//
//				更新				//
//									*/
void Light::Set(bool _custom) {
	D3DXMatrixLookAtLH(&m_mtxView, &m_vPos, &m_vLookAt, &m_vUp);
	D3DXMatrixPerspectiveFovLH(&m_mtxProj, D3DXToRadian(90), 1.f, m_fNear, m_fFar);

	// シェーダを使用しない場合固定機能仕様
	if (!_custom) {
		D3DLIGHT9 light;
		ZeroMemory(&light, sizeof(D3DLIGHT9));		// 初期化
		light.Type = D3DLIGHT_DIRECTIONAL;			// 平行光源
		// 拡散光
		light.Diffuse = m_vColor;
		light.Direction = (Vector3)GetDir();
		light.Range = 1000.0f;
		GetGraphics()->GetDevice()->SetLight(0, &light);		// ライト設定
		GetGraphics()->GetDevice()->LightEnable(0, TRUE);		// ライト当てる

		GetGraphics()->GetDevice()->SetRenderState(D3DRS_AMBIENT, 0x00444444);	// 数値が高いほど明るい
	} else
		GetGraphics()->GetDevice()->LightEnable(0, FALSE);
}