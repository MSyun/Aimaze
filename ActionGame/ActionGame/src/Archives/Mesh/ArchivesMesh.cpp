// ���b�V�����W��
// 2017. 1.22	�v���O�����쐬
// Author		SyunMizuno



#include	"ArchivesMesh.h"
#include	"../../GameFramework/framework/Debug/Debug.h"



/*									//
//			�R���X�g���N�^			//
//									*/
ArchivesMesh::ArchivesMesh() {
	Create();
}


/*									//
//			�f�X�g���N�^			//
//									*/
ArchivesMesh::~ArchivesMesh() {
}


/*									//
//				����				//
//									*/
string ArchivesMesh::Find(const string name) {
	auto it = m_Meshs.find(name);
	if (it == m_Meshs.end()) {
		Debug::LogError("���b�V����" + name + "�͌�����܂���ł���");
		return "";
	}
	return it->second;
}


/*									//
//				�쐬				//
//									*/
void ArchivesMesh::Create() {
	Add("Player", "data/MESH/ruby/ruby.x");
	Add("Skydome", "data/MESH/skydome/sky.x");
	Add("ItemBox", "data/MESH/ItemBox.x");
	Add("Sword", "data/MESH/sword/sword.x");
	Add("Dragon", "data/MESH/dragon/dragon.x");
	Add("Wizard", "data/MESH/wizard/wizard.x");
	Add("Wolf", "data/MESH/wolf/wolf.x");
	Add("Stick", "data/MESH/stick/stick.x");
	Add("Stage", "data/MESH/stage/stage.x");
	Add("Land", "data/MESH/Land/Land.x");
	Add("Allow", "data/MESH/Allow.x");
}


/*									//
//				�ǉ�				//
//									*/
void ArchivesMesh::Add(string name, string filename) {
	m_Meshs.insert(pair<string, string>(name, filename));
}