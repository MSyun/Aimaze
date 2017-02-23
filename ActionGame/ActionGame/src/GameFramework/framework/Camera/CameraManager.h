// �J�����}�l�[�W���[�N���X
// 2016. 4.14	�v���O�����쐬
// 2016. 9. 3	����m�F�p�w���p�[�ϐ��ǉ�
// author		SyunMizuno

#pragma once


#include	<map>
#include	<string>
#include	"Camera.h"
#include	"../Singleton/Singleton.h"

using namespace std;


class CameraManagerBase {
protected:
	map< string, Camera* >	m_MapCamera;
	bool m_bHelper;

	string m_CurCameraName;

public:
	CameraManagerBase();
	virtual ~CameraManagerBase();

	// �쐬
	Camera* Create(const string Name);

	// �폜
	HRESULT Delete(const string Name);

	// �擾
	Camera* Get(const string Name);

	// �Z�b�g
	bool Set(const string Name);

	// �Ō�Ɏg�p�������̂��擾
	Camera* CurrentCamera() { return Get(m_CurCameraName); }

	void SetHelper(bool helper) { m_bHelper = helper; }
};

class CameraManager : public	CameraManagerBase, public	Singleton<CameraManager> {
public:
	CameraManager() : CameraManagerBase() {}
};

// ����Ŏg�p���₷���Ȃ�
inline CameraManager* GetCameraManager(void) {
	return CameraManager::Instance();
}