// ���C�g�}�l�[�W���[�N���X
// 2016. 3.27	�v���O�����쐬
// 2016. 9. 3	����m�F�p�w���p�[�ϐ��ǉ�
// 2017. 2.23	�����o����f�o�C�X��r��
// author		SyunMizuno

#pragma once

#include	<map>
#include	<string>
#include	"Light.h"
#include	"../Singleton/Singleton.h"

using namespace std;

class LightManagerBase {
protected:
	map< string, Light* >	m_MapLight;

	// �Œ�@�\�g�p�t���O
	bool m_bCustom;

	bool m_bHelper;

public:
	LightManagerBase();
	virtual ~LightManagerBase();

	// �S�X�V
	void AllSet();

	// �쐬
	Light* Create(const string Name);

	// �폜
	HRESULT Delete(const string Name);

	// �擾
	Light* Get(const string Name);

	void SetCustom(bool _custom) { m_bCustom = _custom; }

	void SetHelper(bool helper) { m_bHelper = helper; }
};

class LightManager : public	LightManagerBase, public	Singleton<LightManager> {
public:
	LightManager() : LightManagerBase() {}
};

// ����Ŏg�p���₷���Ȃ�
inline LightManager* GetLightManager(void) {
	return LightManager::Instance();
}