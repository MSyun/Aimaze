// �X�L�����b�V���}�l�[�W���[�N���X
// 2015.11.28	�v���W�F�N�g�쐬
// 2016. 1.13	Singleton�֕ύX
// 2016. 1.30	Flyweight�p�^�[���֕ύX
// 2016. 2. 6	����
// author		SyunMizuno

#pragma warning( disable : 4996 )

#include	"SkinMeshManager.h"
#include	<tchar.h>
#include	"MyAllocHierarchy.h"
#include	"HighLevelAnimController.h"
#include	"../../Debug/Debug.h"
#include	"../../Graphic/Graphics.h"


/*									//
//			�R���X�g���N�^			//
//									*/
SkinMeshManagerBase::SkinMeshManagerBase() :
	m_bHelper(true)
{
	m_MapSkin.clear();
}


/*									//
//			�f�X�g���N�^			//
//									*/
SkinMeshManagerBase::~SkinMeshManagerBase() {
	if (!m_bHelper)	return;

	map< string, _tSkin* >::iterator p = m_MapSkin.begin();

	//----- �S�v�f�폜
	while (p != m_MapSkin.end()) {
		Release(p->second);
		++p;
	}
	m_MapSkin.clear();
}


/*									//
//			���b�V�����쐬			//
//									*/
_tSkin* SkinMeshManagerBase::Create(const string FileName) {
	if (m_MapSkin.end() != m_MapSkin.find(FileName)) {
		Debug::Log("���b�V���t�@�C����" + FileName + "�͂��łɐ�������Ă��܂�");
		return Get(FileName);
	}

	//----- ���b�V�����쐬
	// �ϐ��錾
	_tSkin *SkinMesh = new _tSkin;
	MyAllocHierarchy	Hierarchy;

	// �f�B���N�g���𒊏o
	TCHAR szDir[_MAX_PATH];
	TCHAR szDirWk[_MAX_DIR];
	_tsplitpath(FileName.c_str(), szDir, szDirWk, NULL, NULL);
	lstrcat(szDir, szDirWk);
	Hierarchy.SetDirectory(szDir);

	// ���b�V���ǂݍ���
	if (FAILED(D3DXLoadMeshHierarchyFromX(
		FileName.c_str(),
		D3DXMESH_MANAGED,
		GetGraphics()->GetDevice(),
		&Hierarchy,
		NULL,
		&SkinMesh->pFrameRoot,
		&SkinMesh->pController)))
	{
		MessageBox(NULL, _T("���b�V���̓ǂݍ��݂Ɏ��s���܂���"), FileName.c_str(), MB_OK);
		delete SkinMesh;
		return NULL;
	}

	// �t���[�����̐���
	Hierarchy.FindSkin(SkinMesh->pFrameRoot, SkinMesh->pFrameRoot);

	// ���X�g�ɓo�^
	m_MapSkin.insert(pair<string, _tSkin*>(FileName, SkinMesh));

	return SkinMesh;
}


/*									//
//			���b�V���̍폜			//
//									*/
HRESULT SkinMeshManagerBase::Delete(const string Name) {
	//----- ������
	map< string, _tSkin* >::iterator p = m_MapSkin.find(Name);

	if (p == m_MapSkin.end()) {
		Debug::Log("���b�V���t�@�C����" + Name + "�͓o�^����Ă��Ȃ����ߍ폜�ł��܂���");
		return E_FAIL;
	}

	Release(p->second);

	m_MapSkin.erase(Name);

	return S_OK;
}


/*									//
//			���b�V���̎擾			//
//									*/
_tSkin* SkinMeshManagerBase::Get(const string Name) {
	map< string, _tSkin* >::iterator p = m_MapSkin.find(Name);

	if (p == m_MapSkin.end()) {
		return Create(Name);
	}

	return p->second;
}


/*									//
//			�w�胁�b�V���̍폜		//
//									*/
void SkinMeshManagerBase::Release(_tSkin* skin) {
	MyAllocHierarchy	Hierarchy;

	if (skin->pFrameRoot != NULL)
		Hierarchy.DestroyFrame(skin->pFrameRoot);

	if (skin->pController != NULL) {
		skin->pController->Release();
		skin->pController = NULL;
	}

	delete skin;
	skin = NULL;
}