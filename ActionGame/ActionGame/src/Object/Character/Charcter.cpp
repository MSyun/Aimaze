// �L�����N�^�[�N���X
// 2017. 1. 3	�v���O�����쐬
// Author		SyunMizuno



#include	"Charcter.h"
#include	"../../GameFramework/GameController/GameControllerManager.h"
#include	"../../GameFramework/framework/Camera/CameraManager.h"
#include	"../../GameFramework/framework/Time/Time.h"
#include	"../../GameFramework/Obj/3D/Obj3DManager.h"
#include	"../../Archives/Archives.h"
#include	"../../GameFramework/framework/Utility/Math/MathUtility.h"


/*									//
//			�R���X�g���N�^			//
//									*/
Character::Character() :
	m_pWeapon(NULL),
	m_pStageMesh(NULL)
{
	CreateMesh();
	SetName("Character");
}


/*									//
//			�f�X�g���N�^			//
//									*/
Character::~Character() {
}


/*									//
//				������				//
//									*/
void Character::Init() {
	Obj3D::Init();

	Obj3D* Field = GetObj3DManager()->FindWithTag("Stage");
	if (!Field) {
		Debug::LogError("Stage�^�O��������܂���");
		return;
	}
	m_pStageMesh = Field->GetMesh();
	if (!m_pStageMesh) {
		Debug::LogError("Stage��mesh������܂���");
		return;
	}

}


/*									//
//			����̐ݒ�				//
//									*/
void Character::SetWeapon(Weapon* weapon) {
	if (m_pWeapon) {
		m_pWeapon->GetTransform()->RemoveParent();
		m_pWeapon->SetOwner(NULL);
	}
	m_pWeapon = weapon;

	if (m_pWeapon) {
		Transform* trans = m_pWeapon->GetTransform();
		trans->SetLocalPos(0.0f, 0.0f, 0.0f);
		trans->SetEularAngles(0.0f, -90.0f, 0.0f);
		trans->SetParent(GetMesh()->GetBoneTransform("Armature_Weapon"));
	}
}


/*									//
//				����				//
//									*/
void Character::Input() {
	MoveInput("Main");
	AttackInput();
}


/*									//
//			�ړ��̓���				//
//									*/
void Character::MoveInput(const string cameraName) {
#define	DefaultSpeed	(-0.2f)
	// �L�[�{�[�h
	if (Input::GetKeyPress(GetContManager()->Get()->GetKeyNum(BUTTON_UP))) {
		GetTransform()->FowardBaseRotate(GetCameraManager()->Get(cameraName)->GetTransform(), 0.0f);
		m_pTransform->Translate(0.0f, 0.0f, DefaultSpeed * Time::GetDeltaTime(), m_pTransform);
	} else
	if (Input::GetKeyPress(GetContManager()->Get()->GetKeyNum(BUTTON_RIGHT))) {
		GetTransform()->FowardBaseRotate(GetCameraManager()->Get(cameraName)->GetTransform(), 90.0f);
		m_pTransform->Translate(0.0f, 0.0f, DefaultSpeed * Time::GetDeltaTime(), m_pTransform);
	} else
	if (Input::GetKeyPress(GetContManager()->Get()->GetKeyNum(BUTTON_DOWN))) {
		GetTransform()->FowardBaseRotate(GetCameraManager()->Get(cameraName)->GetTransform(), 180.0f);
		m_pTransform->Translate(0.0f, 0.0f, DefaultSpeed * Time::GetDeltaTime(), m_pTransform);
	} else
	if (Input::GetKeyPress(GetContManager()->Get()->GetKeyNum(BUTTON_LEFT))) {
		GetTransform()->FowardBaseRotate(GetCameraManager()->Get(cameraName)->GetTransform(), 270.0f);
		m_pTransform->Translate(0.0f, 0.0f, DefaultSpeed * Time::GetDeltaTime(), m_pTransform);
	}
	// �p�b�h
	if (Input::GetJoyAxisDegree(0) != -1000) {
		GetTransform()->FowardBaseRotate(GetCameraManager()->Get(cameraName)->GetTransform(), Input::GetJoyAxisDegree(0) + 90.f);
		m_pTransform->Translate(0.0f, 0.0f, DefaultSpeed * Time::GetDeltaTime(), m_pTransform);
	}
}


/*									//
//			�ǔ���m�F				//
//									*/
Vector3 Character::WallCheck(Vector3& speed) {
	if (!m_pStageMesh)	return speed;

	Transform* trans = GetTransform();
	Point3 vP = trans->GetPos();
	Vector3 vX = trans->GetRight();
	Vector3 vY = trans->GetUp();
	Vector3 vZ = trans->GetForward();
	bool bHit = false;

	// �ړ���
	Vector3 move =
		vX * (speed.x * Time::GetDeltaTime()) +
		vY * (speed.y * Time::GetDeltaTime()) +
		vZ * (speed.z * Time::GetDeltaTime());

	// �ړ���̏ꏊ��ݒ�
	Point3 pos = vP + move;

	//----- �n�`�Ƃ̃��C����
	Point3 Cross;
	Vector3 Normal;

	// �ǔ���1���
	Obb* obb = GetCollider()->GetObb()[0];
	obb->p += move;

	// ���C�̓_
	Point3 LDBRay = AnalyzeRayPos(pos, *obb, false, false, false);
	Point3 RDBRay = AnalyzeRayPos(pos, *obb, true, false, false);
	Point3 RDFRay = AnalyzeRayPos(pos, *obb, true, false, true);
	Point3 LDFRay = AnalyzeRayPos(pos, *obb, false, false, true);
	// OBB�̍��E�̂����蔻������s
	bHit = m_pStageMesh->Intersect(LDBRay, RDFRay, false, &Cross, &Normal);
	if (!bHit)	bHit = m_pStageMesh->Intersect(RDBRay, LDFRay, false, &Cross, &Normal);

	//----- �߂荞�ݔ���
	float Len = 0.0f;	// �߂荞�񂾒���
	if (bHit) {
		bHit = OBBvsPlane(*obb, Normal, Cross, &Len);
	}

	//----- �߂荞��ł�
	if (bHit) {
		// �ђʋ�����߂�
		D3DXVec3Normalize(&Normal, &Normal);
		Vector3 RetLen = Normal * Len;
		move += RetLen;
		//----- �ǔ���2���
		LDBRay += RetLen;
		RDBRay += RetLen;
		LDFRay += RetLen;
		RDFRay += RetLen;
		// �Ίp���Ŕ���
		bHit = m_pStageMesh->Intersect(LDBRay, LDFRay, false, &Cross, &Normal);
		if (!bHit)	bHit = m_pStageMesh->Intersect(RDBRay, RDFRay, false, &Cross, &Normal);

		//----- �܂��߂荞��ł�
		if (bHit) {
			obb->p += (Normal * Len);
			bHit = OBBvsPlane(*obb, Normal, Cross, &Len);
			// �ђʋ�����߂�
			if (bHit) {
				D3DXVec3Normalize(&Normal, &Normal);
				move += (Normal * Len);
			}
		}
	}

	return move;
}


/*									//
//		OBB��背�C�p���W���		//
//									*/
Point3 Character::AnalyzeRayPos(
	const Point3& pos,
	const Obb& obb,
	bool x,
	bool y,
	bool z)
{
	Point3 NewPosition(0.0f, 0.0f, 0.0f);

	if (x)	NewPosition += (obb.axis[0] * obb.r[0]);
	else	NewPosition -= (obb.axis[0] * obb.r[0]);
	if (y)	NewPosition += (obb.axis[1] * obb.r[1]);
	else	NewPosition -= (obb.axis[1] * obb.r[1]);
	if (z)	NewPosition += (obb.axis[2] * obb.r[2]);
	else	NewPosition -= (obb.axis[2] * obb.r[2]);

	return pos + NewPosition;
}