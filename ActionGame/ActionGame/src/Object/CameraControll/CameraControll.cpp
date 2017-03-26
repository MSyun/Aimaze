// カメラ制御クラス
// 2017. 1.30	プログラム作成
// Author		SyunMizuno


#include	"CameraControll.h"
#include	"../../GameFramework/GameController/GameControllerManager.h"
#include	"../../GameFramework/Obj/3D/Obj3DManager.h"
#include	"../../GameFramework/framework/Time/Time.h"


const float RotateSpeed = 180.0f;
const float DistSpeed = 5.0f;



/*									//
//			コンストラクタ			//
//									*/
CameraControll::CameraControll() :
	m_Player(NULL),
	m_Camera(NULL),
	m_vShake(0.0f, 0.0f, 0.0f),
	m_bShake(false),
	m_fCounter(0.0f),
	m_fDeviation(0.0f),
	m_fCyCle(0.0f)
{
	SetName("CameraControll");
}


/*									//
//				初期化				//
//									*/
void CameraControll::Init() {
	Obj3D::Init();

	// Player
	Obj3D* obj = GetObj3DManager()->Find("Player");
	if (!obj) {
		Debug::LogError("Playerが見つかりませんでした");
		return;
	}
	m_Player = obj->GetTransform();

	// Camera
	Transform* trans = GetTransform()->Find("MainCamera");
	if (!trans) {
		Debug::LogError("Cameraが子階層に見つかりませんでした");
		return;
	}
	m_Camera = (Camera*)trans->GetGameObject();

	GetTransform()->SetPos(m_Player->GetPos());
	m_Camera->GetTransform()->LookAt(m_Player->GetPos() + Vector3(0.0f, 1.0f, 0.0f));

}


/*									//
//				更新				//
//									*/
void CameraControll::LateUpdate() {
	Obj3D::LateUpdate();
	ShakeCount();

	GetTransform()->SetPos(m_Player->GetPos());
	GetTransform()->Translate(m_vShake, m_pTransform);

	Debug::Print("\nカメラ座標");
	Debug::Print(m_Camera->GetTransform()->GetPos());
}


/*									//
//				入力				//
//									*/
void CameraControll::Input() {
	// 回転
	if (Input::GetKeyPress(GetContManager()->Get()->GetKeyNum(BUTTON_R1)) ||
		Input::GetJoyPress(0, GetContManager()->Get()->GetJoyNum(BUTTON_R1))) {
		GetTransform()->Rotate(0.0f, RotateSpeed * Time::GetDeltaTime(), 0.0f);
	} else
	if (Input::GetKeyPress(GetContManager()->Get()->GetKeyNum(BUTTON_L1)) ||
		Input::GetJoyPress(0, GetContManager()->Get()->GetJoyNum(BUTTON_L1))) {
		GetTransform()->Rotate(0.0f, -RotateSpeed * Time::GetDeltaTime(), 0.0f);
	}
	// カメラの距離
	//if (Input::GetKeyPress(GetContManager()->Get()->GetKeyNum(BUTTON_R2)) ||
	//	Input::GetJoyPress(0, GetContManager()->Get()->GetJoyNum(BUTTON_R2))) {
	//	Transform* trans = m_Camera->GetTransform();
	//	trans->Translate(0.0f, 0.0f, DistSpeed * Time::GetDeltaTime(), trans);
	//} else
	//if (Input::GetKeyPress(GetContManager()->Get()->GetKeyNum(BUTTON_L2)) ||
	//	Input::GetJoyPress(0, GetContManager()->Get()->GetJoyNum(BUTTON_L2))) {
	//	Transform* trans = m_Camera->GetTransform();
	//	trans->Translate(0.0f, 0.0f, -DistSpeed * Time::GetDeltaTime(), trans);
	//}
}


/*									//
//				揺れ				//
//									*/
void CameraControll::Shake(float time, float deviat) {
	m_bShake = true;
	m_fCounter = 0.0f;
	m_fDeviation = deviat;
	m_fCyCle = time;
}


/*									//
//				揺れ計測			//
//									*/
void CameraControll::ShakeCount() {
	if (!m_bShake)	return;

	float rad = D3DX_PI * 2.0f / m_fCyCle * m_fCounter;
	float vec = sinf(rad) * m_fDeviation;

	m_vShake.y = vec;

	m_fDeviation -= Time::GetDeltaTime();
	if (m_fDeviation <= 0.0f) {
		m_bShake = false;
		m_vShake.y = 0.0f;
	}
	m_fCounter += Time::GetDeltaTime();
}