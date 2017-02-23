// �X�L�����b�V���}�l�[�W���[�N���X
// 2015.11.28	�v���W�F�N�g�쐬
// 2016. 1.13	Singleton�֕ύX
// 2016. 9. 3	����m�F�p�w���p�[�ϐ��ǉ�
// author		SyunMizuno

#pragma once

#include	<map>
#include	<string>
#include	"../../Singleton/Singleton.h"
#include	"SkinMesh.h"

using namespace std;

/*
	memo:
		�{�[�������s��		�R�s�[
		�}�e���A��			�R�s�[
		�R���g���[���[		�R�s�[
		�t���[��			�Q��
		�R���e�i			�R�s�[
		���_�i�|���S���j	�Q��
		�X�L��				�Q��
		�e�N�X�`��			�Q��
*/



class SkinMeshManagerBase {
protected:
	map< string, _tSkin* >	m_MapSkin;
	bool m_bHelper;

public:
	SkinMeshManagerBase();
	virtual ~SkinMeshManagerBase();

	// �쐬
	_tSkin* Create(const string FileName);

	// �폜
	HRESULT Delete(const string Name);

	// �擾
	_tSkin* Get(const string Name);

	void SetHelper(bool helper) { m_bHelper = helper; }

protected:
	// �w�胁�b�V���̍폜
	void Release(_tSkin* skin);
};

class SkinMeshManager : public	SkinMeshManagerBase, public	Singleton<SkinMeshManager> {
public:
	SkinMeshManager() : SkinMeshManagerBase() {}
};

// ����Ŏg�p���₷���Ȃ�
inline SkinMeshManager* GetSkinMeshManager(void) {
	return SkinMeshManager::Instance();
}