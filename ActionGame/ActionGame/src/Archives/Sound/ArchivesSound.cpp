// �T�E���h���W��
// 2017. 1.22	�v���O�����쐬
// Author		SyunMizuno



#include	"ArchivesSound.h"
#include	"../../GameFramework/framework/Debug/Debug.h"



/*									//
//			�R���X�g���N�^			//
//									*/
ArchivesSound::ArchivesSound() {
	Create();
}


/*									//
//			�f�X�g���N�^			//
//									*/
ArchivesSound::~ArchivesSound() {
}


/*									//
//				����				//
//									*/
string ArchivesSound::Find(const string name) {
	auto it = m_Sounds.find(name);
	if (it == m_Sounds.end()) {
		Debug::LogError("�T�E���h��" + name + "�͌�����܂���ł���");
		return "";
	}
	return it->second;
}


/*									//
//				�쐬				//
//									*/
void ArchivesSound::Create() {
	Add("Cursor", "data/SOUND/SE/cursor.wav");
	Add("Start", "data/SOUND/SE/start.wav");
	Add("Field", "data/SOUND/BGM/field.wav");
	Add("Swing", "data/SOUND/SE/swing.wav");
	Add("GameOver", "data/SOUND/SE/gameover.wav");
	Add("Hit", "data/SOUND/SE/hit.wav");
}


/*									//
//				�ǉ�				//
//									*/
void ArchivesSound::Add(string name, string filename) {
	m_Sounds.insert(pair<string, string>(name, filename));
}