// �J�����}�l�[�W���[�N���X
// 2016. 3.27	�v���O�����쐬
// 2017. 1. 4	�f�[�^�폜��ObjManager�ֈ�C
// �����		SyunMizuno


#include	"CameraManager.h"
#include	"../Debug/Debug.h"

/*									//
//			�R���X�g���N�^			//
//									*/
CameraManagerBase::CameraManagerBase() :
	m_bHelper(true),
	m_CurCameraName("")
{
	m_MapCamera.clear();
}


/*									//
//			�f�X�g���N�^			//
//									*/
CameraManagerBase::~CameraManagerBase() {
	if (!m_bHelper)		return;

	// �v�f���
	m_MapCamera.clear();
}


/*									//
//			�J�������쐬			//
//									*/
Camera* CameraManagerBase::Create(const string Name) {
	if (m_MapCamera.end() != m_MapCamera.find(Name)) {
		Debug::Log("�J������" + Name + "�͂��łɐ�������Ă��܂�");
		return Get(Name);
	}

	Camera* camera = new Camera;
	camera->SetName(Name + "Camera");
	m_MapCamera.insert(pair<string, Camera*>(Name, camera));

	return camera;
}


/*									//
//			�J�����̍폜			//
//									*/
HRESULT CameraManagerBase::Delete(const string Name) {
	//----- ������
	map< string, Camera* >::iterator p = m_MapCamera.find(Name);

	if (p == m_MapCamera.end()) {
		Debug::Log("�J������" + Name + "�͓o�^����Ă��Ȃ����ߍ폜�ł��܂���");
		return E_FAIL;
	}

	delete p->second;
	p->second = NULL;

	m_MapCamera.erase(Name);

	return S_OK;
}


/*									//
//			�J�����̎擾			//
//									*/
Camera* CameraManagerBase::Get(const string Name) {
	map< string, Camera* >::iterator p = m_MapCamera.find(Name);

	if (p == m_MapCamera.end()) {
		return Create(Name);
	}

	return p->second;
}


/*									//
//			�J�����̐ݒ�			//
//									*/
bool CameraManagerBase::Set(const string Name) {
	if (Name.empty()) {
		Debug::LogError("�J�����̖��O������܂���");
		return false;
	}

	Camera* camera = Get(Name);
	if (!camera) {
		Debug::LogError("�J������" + Name + "�͑��݂��܂���ł���");
		return false;
	}

	camera->Set();
	m_CurCameraName = Name;
	return true;
}