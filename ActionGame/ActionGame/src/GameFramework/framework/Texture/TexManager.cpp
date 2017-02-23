// �e�N�X�`���}�l�[�W���[�N���X
// 2015.11.14	�v���W�F�N�g�쐬
// 2016. 1.13	Singleton�֕ύX
// 2016. 1.30	Flyweight�p�^�[���֕ύX
// �����		SyunMizuno
// �Q�ƃT�C�g	http://www.c3.club.kyutech.ac.jp/gamewiki/index.php?%A5%B2%A1%BC%A5%E0%C5%AA%A5%AF%A5%E9%A5%B9%C0%DF%B7%D7

#include	"TexManager.h"
#include	<tchar.h>
#include	"../Utility/System/SystemUtility.h"
#include	"../Debug/Debug.h"
#include	"../Graphic/Graphics.h"


/*									//
//			�R���X�g���N�^			//
//									*/
TexManagerBase::TexManagerBase() :
	m_bHelper(true)
{
	m_MapTexture.clear();
}


/*									//
//			�f�X�g���N�^			//
//									*/
TexManagerBase::~TexManagerBase() {
	if (!m_bHelper)		return;

	map< string, LPDIRECT3DTEXTURE9 >::iterator p = m_MapTexture.begin();

	//----- �S�v�f�폜
	while (p != m_MapTexture.end()) {
		SAFE_RELEASE(p->second);
		++p;
	}

	m_MapTexture.clear();
}


/*									//
//			�e�N�X�`�����쐬		//
//									*/
LPDIRECT3DTEXTURE9 TexManagerBase::Create(const string FileName) {
	if (FileName.empty()) {
		Debug::LogError("�t�@�C����" + FileName + "������܂���");
		return NULL;
	}

	//----- ���ɓo�^����Ă��邩Check
	if (m_MapTexture.end() != m_MapTexture.find(FileName)) {
		Debug::Log("�e�N�X�`��" + FileName + "���ɓo�^����Ă��܂�");
		return Get(FileName);
	}

	//----- �ϐ��錾
	LPDIRECT3DTEXTURE9	texture;

	//----- �e�N�X�`�����쐬
	HRESULT hr;
	hr = D3DXCreateTextureFromFile(GetGraphics()->GetDevice(), FileName.c_str(), &texture);
	if (hr != S_OK) {
		MessageBox(NULL, _T("�e�N�X�`���̓ǂݍ��݂Ɏ��s���܂���"), FileName.c_str(), MB_OK);
		return NULL;
	}
	// ���X�g�ɓo�^
	m_MapTexture.insert(pair<string, LPDIRECT3DTEXTURE9>(FileName, texture));

	return texture;
}


/*									//
//			�e�N�X�`�����폜		//
//									*/
HRESULT TexManagerBase::Delete(const string Name) {
	//----- ������
	map< string, LPDIRECT3DTEXTURE9 >::iterator p = m_MapTexture.find(Name);

	// �o�^����Ă��Ȃ��ꍇ
	if (p == m_MapTexture.end()) {
		Debug::Log("�e�N�X�`��" + Name + "�͓o�^����Ă��Ȃ����ߍ폜�ł��܂���");
		return E_FAIL;
	}

	// �v�f���폜
	p->second->Release();
	p->second = NULL;

	// ���X�g������
	m_MapTexture.erase(Name);

	return S_OK;
}


/*									//
//			�e�N�X�`�����擾		//
//									*/
LPDIRECT3DTEXTURE9 TexManagerBase::Get(const string Name) {
	// �C�e���[�^�̍쐬
	map< string, LPDIRECT3DTEXTURE9 >::iterator p = m_MapTexture.find(Name);

	// �o�^����ĂȂ��ꍇ
	if (p == m_MapTexture.end()) {
		return Create(Name);
	}

	return p->second;
}