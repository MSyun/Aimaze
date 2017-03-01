// ���C�g�}�l�[�W���[�N���X
// 2016. 3.27	�v���O�����쐬
// 2017. 1.22	�f�o�b�O���O�ǉ�
// author		SyunMizuno


#include	"LightManager.h"
#include	"../Debug/Debug.h"

/*									//
//			�R���X�g���N�^			//
//									*/
LightManagerBase::LightManagerBase() :
	m_bCustom(false),
	m_bHelper(true)
{
	m_MapLight.clear();
}


/*									//
//			�f�X�g���N�^			//
//									*/
LightManagerBase::~LightManagerBase() {
	if (!m_bHelper)		return;

	m_MapLight.clear();
}


/*									//
//			�S���C�g���X�V			//
//									*/
void LightManagerBase::AllSet() {
	map< string, Light* >::iterator p = m_MapLight.begin();

	while (p != m_MapLight.end()) {
		p->second->Set(m_bCustom);
		++p;
	}
}


/*									//
//			���C�g���쐬			//
//									*/
Light* LightManagerBase::Create(const string Name) {
	//----- ���ɓo�^����Ă��邩Check
	if (m_MapLight.end() != m_MapLight.find(Name)) {
		Debug::Log("���C�g��" + Name + "�͂��łɐ�������Ă��܂�");
		return Get(Name);
	}

	//----- �쐬
	Light*	light = new Light;

	// ���X�g�ɓo�^
	m_MapLight.insert(pair<string, Light*>(Name, light));

	return light;
}


/*									//
//			���C�g�̍폜			//
//									*/
// <return> S_OK. ���� : E_FAIL. ���s
HRESULT LightManagerBase::Delete(const string Name) {
	//----- ������
	map< string, Light* >::iterator p = m_MapLight.find(Name);

	// �o�^����Ă��Ȃ��ꍇ
	if (p == m_MapLight.end()) {
		Debug::Log("���C�g��" + Name + "�͓o�^����Ă��Ȃ����ߍ폜�ł��܂���");
		return E_FAIL;
	}

	// �v�f���폜
	delete p->second;
	p->second = NULL;

	m_MapLight.erase(Name);

	return S_OK;
}


/*									//
//			���C�g�̎擾			//
//									*/
Light* LightManagerBase::Get(const string Name) {
	map< string, Light* >::iterator p = m_MapLight.find(Name);

	if (p == m_MapLight.end()) {
		return Create(Name);
	}

	return p->second;
}