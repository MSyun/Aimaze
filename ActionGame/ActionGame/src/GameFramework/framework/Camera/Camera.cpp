//////////////////////////////////////////////////////////////
// カメラクラス
// 2016. 7. 3	プログラム作成
// Author		SyunMizuno
//////////////////////////////////////////////////////////////


#include	"Camera.h"
#include	"../Screen/Screen.h"
#include	"../Utility/System/SystemUtility.h"
#include	"../Graphic/Graphics.h"

/*										//
//			コンストラクタ				//
//										*/
Camera::Camera() :
	m_fFovy(60.0f),
	m_fAspect((float)Screen::GetWidth() / (float)Screen::GetHeight()),
	m_fNear(1.0f),
	m_fFar(1000.0f)
{
	GetTransform()->SetPos(0.0f, 5.0f, -10.0f);
	m_fLookLength = 5.0f;
	CreateSky();
	SetName("Camera");
}


/*										//
//			デストラクタ				//
//										*/
Camera::~Camera() {
	DeleteSky();
}


/*										//
//				更新					//
//										*/
void Camera::Update() {
	Obj3D::Update();

	// スカイボックス更新
	if (m_pSky) {
		m_pSky->GetTransform()->SetPos(GetTransform()->GetPos());
	}
}


/*										//
//				セット					//
//										*/
void Camera::Set() {
	Transform*	transform = GetTransform();
	// ビューマトリクスの設定
	D3DXMatrixLookAtLH(
		&m_matView,
		&transform->GetPos(),
		&(transform->GetPos() + transform->GetForward() * m_fLookLength),
		&transform->GetUp());
	GetGraphics()->SetView(m_matView);

	// プロジェクションマトリクスの設定
	D3DXMatrixPerspectiveFovLH(
		&m_matProj,
		D3DXToRadian(m_fFovy),
		m_fAspect,
		m_fNear,
		m_fFar);
	GetGraphics()->SetProjection(m_matProj);
}


/*										//
//			スカイボックス作成			//
//										*/
void Camera::CreateSky() {
	if (!m_pSky) {
		m_pSky = new Skydome;
	}
}


/*										//
//			スカイボックス削除			//
//										*/
void Camera::DeleteSky() {
	SAFE_DELETE(m_pSky);
}