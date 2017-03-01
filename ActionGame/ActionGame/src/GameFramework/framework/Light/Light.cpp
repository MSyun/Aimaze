//	ライトクラス
// 2016. 3.27	プログラム作成
// author		SyunMizuno

#include	"Light.h"
#include	<tchar.h>
#include	"../Graphic/Graphics.h"
#include	"../Screen/Screen.h"


/*									//
//			コンストラクタ			//
//									*/
Light::Light() {
	m_fLookLength = 10.0f;
	D3DXMatrixIdentity(&m_mtxView);
	D3DXMatrixIdentity(&m_mtxProj);
	m_fNear		= 1.f;
	m_fFar		= 1000.0f;
	m_fSpeclar	= 100.0f;
	m_vColor	= Color( 1.0f, 1.0f, 1.0f, 1.0f );

	GetTransform()->SetPos(0.0f, 10.0f, 0.0f);
	GetTransform()->LookAt(Point3(0.0f, 0.0f, 3.0f));
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
	Transform* transform = GetTransform();
	D3DXMatrixLookAtLH(
		&m_mtxView,
		&transform->GetPos(),
		&(transform->GetPos() + transform->GetForward() * m_fLookLength),
		&transform->GetUp());
	D3DXMatrixPerspectiveFovLH(
		&m_mtxProj,
		D3DXToRadian(150),
		(float)Screen::GetWidth() / (float)Screen::GetHeight(),
		m_fNear,
		m_fFar);

	// シェーダを使用しない場合固定機能仕様
	if (!_custom) {
		D3DLIGHT9 light;
		ZeroMemory(&light, sizeof(D3DLIGHT9));		// 初期化
		light.Type = D3DLIGHT_DIRECTIONAL;			// 平行光源
		// 拡散光
		light.Diffuse = m_vColor;
		light.Direction = GetDirection3();
		light.Range = 1000.0f;
		GetGraphics()->GetDevice()->SetLight(0, &light);		// ライト設定
		GetGraphics()->GetDevice()->LightEnable(0, TRUE);		// ライト当てる

		GetGraphics()->GetDevice()->SetRenderState(D3DRS_AMBIENT, 0x00444444);	// 数値が高いほど明るい
	} else
		GetGraphics()->GetDevice()->LightEnable(0, FALSE);
}


Vector3 Light::GetDirection3() {
	return GetTransform()->GetForward() * m_fLookLength;
}


Vector4 Light::GetDirection4() {
	Vector3 forward = GetTransform()->GetForward() * m_fLookLength;

	return Vector4(
		forward.x,
		forward.y,
		forward.z,
		1.0f
	);
}