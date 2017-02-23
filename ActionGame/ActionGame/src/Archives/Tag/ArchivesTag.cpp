// �^�O���W��
// 2017. 1.22	�v���O�����쐬
// Author		SyunMizuno



#include	"ArchivesTag.h"
#include	"../../GameFramework/framework/Debug/Debug.h"



/*									//
//			�R���X�g���N�^			//
//									*/
ArchivesTag::ArchivesTag() {
	Create();
}


/*									//
//			�f�X�g���N�^			//
//									*/
ArchivesTag::~ArchivesTag() {
}


/*									//
//				����				//
//									*/
unsigned char ArchivesTag::Find(const string name) {
	auto it = m_Tags.find(name);
	if (it == m_Tags.end()) {
		Debug::LogError("�^�O" + name + "�͌�����܂���ł���");
		return 0;
	}
	return it->second;
}


/*									//
//				�쐬				//
//									*/
void ArchivesTag::Create() {
	Add("Default", 0);
	Add("ItemBox", 1);
	Add("Player", 2);
	Add("Enemy", 3);
	Add("Stage", 4);
	Add("Player/Attack", 5);
	Add("Enemy/Attack", 6);
	Add("Allow", 7);
}


/*									//
//				�ǉ�				//
//									*/
void ArchivesTag::Add(string name, unsigned char num) {
	m_Tags.insert(pair<string, unsigned char>(name, num));
}