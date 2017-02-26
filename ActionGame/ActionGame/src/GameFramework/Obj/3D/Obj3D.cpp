//////////////////////////////////////////////////////////////
// 3D�I�u�W�F�N�g�N���X
// 2016. 5.19	�v���O�����쐬
// Author		SyunMizuno
//////////////////////////////////////////////////////////////


#include	"Obj3D.h"
#include	"../../framework/Time/Time.h"

#include	"Obj3DManager.h"
#include	"../../framework/Utility/System/SystemUtility.h"



/*									//
//			�R���X�g���N�^			//
//									*/
Obj3D::Obj3D() :
	m_pMesh(NULL),
	m_pAnimator(NULL),
	m_ShaderType(_shadertype::SHADER_HALFLAMBERT),
	m_pTransform(NULL),
	m_pCollider(NULL)
{
	m_pTransform = new Transform(this);
	GetObj3DManager()->Add(this);
}


/*									//
//			�f�X�g���N�^			//
//									*/
Obj3D::~Obj3D() {
	//----- �폜
	DeleteMesh();		// ���b�V��
	DeleteCollider();
	// �}�l�[�W���[�ɂ��m�点
	GetObj3DManager()->Delete(this);
	SAFE_DELETE(m_pTransform);
}


/*									//
//			���b�V���̍X�V			//
//									*/
void Obj3D::UpdateMesh() {
	if (!m_pMesh)	return;

	// �A�j���[�V����
	float framerate = (float)Time::GetSettingFrameRate();
	m_pMesh->GetCont().AdvanceTime(60.0f * (60.0f / framerate) / 4800.0f * Time::GetDeltaTime() * framerate);

	// �A�j���[�^�[
	if (m_pAnimator)
		m_pAnimator->Update();

	// �X�V
	m_pMesh->Update(m_pTransform->GetWorld());
}


/*									//
//				�S�X�V				//
//									*/
void Obj3D::UpdateAll() {
	if (!GetActive())	return;

	IObj::UpdateAll();
	if (GetOperate())	Input();
	UpdateMesh();
	Update();

	vector<Transform*>* childs = m_pTransform->GetChilds();
	for (auto it = childs->begin(); it != childs->end(); ++it) {
		(*it)->GetGameObject()->UpdateAll();
	}
}
void Obj3D::LateUpdateAll() {
	if (!GetActive())	return;

	LateUpdate();
	if (m_pMesh)	m_pMesh->LateUpdate();

	vector<Transform*>* childs = m_pTransform->GetChilds();
	for (auto it = childs->begin(); it != childs->end(); ++it) {
		(*it)->GetGameObject()->LateUpdateAll();
	}

	if(m_pCollider)	m_pCollider->Update(m_pTransform->GetWorld());
}


/*									//
//				�S�`��				//
//									*/
void Obj3D::DrawAll() {
	if (!GetActive())	return;

	Draw();
	if (m_pMesh)		m_pMesh->Draw(m_ShaderType);
	if (m_pCollider)	m_pCollider->Draw();

	vector<Transform*>* childs = m_pTransform->GetChilds();
	for (auto it = childs->begin(); it != childs->end(); ++it) {
		(*it)->GetGameObject()->DrawAll();
	}
}
void Obj3D::LateDrawAll() {
	if (!GetActive())	return;

	LateDraw();
	if (m_pMesh)	m_pMesh->LateDraw(m_ShaderType);

	vector<Transform*>* childs = m_pTransform->GetChilds();
	for (auto it = childs->begin(); it != childs->end(); ++it) {
		(*it)->GetGameObject()->LateDrawAll();
	}
}
void Obj3D::DrawShadow() {
	if (!GetActive() || !GetShadow())	return;

	// �e�̕`��
	if (m_pMesh) {
		m_pMesh->DrawShadow();
	} else {
		// �ۉe
	}

	vector<Transform*>* childs = m_pTransform->GetChilds();
	for (auto it = childs->begin(); it != childs->end(); ++it) {
		(*it)->GetGameObject()->DrawShadow();
	}
}

#pragma region Collider
/*									//
//			�����蔻��쐬			//
//									*/
void Obj3D::CreateCollider() {
	if (!m_pCollider)
		m_pCollider = new Collider(this);
}


/*									//
//			�����蔻��폜			//
//									*/
void Obj3D::DeleteCollider() {
	SAFE_DELETE(m_pCollider);
}
#pragma endregion

#pragma region Mesh
/*									//
//			���b�V���쐬			//
//									*/
void Obj3D::CreateMesh() {
	if (!m_pMesh)
		m_pMesh = new SkinMesh;
}


/*									//
//			���b�V���폜			//
//									*/
void Obj3D::DeleteMesh() {
	SAFE_DELETE(m_pMesh);
	SAFE_DELETE(m_pAnimator);
}


/*									//
//			���b�V���ݒ�			//
//									*/
void Obj3D::SetModel(_tSkin* pSkin, IAnimator* pAnimator) {
	CreateMesh();
	m_pMesh->ManagerGet(pSkin);
	m_pAnimator = pAnimator;
}
#pragma endregion


/*									//
//				�폜�ʒm			//
//									*/
void Obj3D::Destroy() {
	IObj::Destroy();
	m_pTransform->Destroy();
}


/*									//
//			�A�N�e�B�u�̐ݒ�		//
//									*/
void Obj3D::SetActive(bool active) {
	IObj::SetActive(active);
	if (!active) {
		if (m_pCollider)
			m_pCollider->ResetHit();
	}
}