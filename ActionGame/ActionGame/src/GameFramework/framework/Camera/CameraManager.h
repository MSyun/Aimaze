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
#pragma region variable

	map< string, Camera* >	m_MapCamera;
	bool m_bHelper;

	string m_CurCameraName;

#pragma endregion

public:
#pragma region method

	CameraManagerBase();
	virtual ~CameraManagerBase();

	/* �쐬
	// Name	:	��������J�����̖��O
	// return	: ���������J�����̃I�u�W�F�N�g
	*/
	Camera* Create(const string Name);

	/* �폜
	// Name	:	�폜����J�����̖��O
	// return	:	S_OK. �����AE_FAIL. ���s
	*/
	HRESULT Delete(const string Name);

	/* �擾
	// Name	:	�擾����J�����̖��O
	// return	:	�Ή������J�����̃C���X�^���X
	*/
	Camera* Get(const string Name);

	/* �Z�b�g
	// Name	:	�ݒ肷��J�����̖��O
	// return	:	true. �����Afalse. ���s
	*/
	bool Set(const string Name);

	/* �Ō�Ɏg�p�������̂��擾
	// return	:	�Ή������J�����̃C���X�^���X
	*/
	Camera* CurrentCamera() { return Get(m_CurCameraName); }

	/* �Ǘ��I�u�W�F�N�g�������ō폜���邩�ݒ� */
	void SetHelper(bool helper) { m_bHelper = helper; }

#pragma endregion
};

class CameraManager : public	CameraManagerBase, public	Singleton<CameraManager> {
public:
	CameraManager() : CameraManagerBase() {}
};

// ����Ŏg�p���₷���Ȃ�
inline CameraManager* GetCameraManager(void) {
	return CameraManager::Instance();
}