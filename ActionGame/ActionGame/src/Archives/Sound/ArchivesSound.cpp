// サウンド名集約
// 2017. 1.22	プログラム作成
// Author		SyunMizuno



#include	"ArchivesSound.h"
#include	"../../GameFramework/framework/Debug/Debug.h"



/*									//
//			コンストラクタ			//
//									*/
ArchivesSound::ArchivesSound() {
	Create();
}


/*									//
//			デストラクタ			//
//									*/
ArchivesSound::~ArchivesSound() {
}


/*									//
//				検索				//
//									*/
string ArchivesSound::Find(const string name) {
	auto it = m_Sounds.find(name);
	if (it == m_Sounds.end()) {
		Debug::LogError("サウンド名" + name + "は見つかりませんでした");
		return "";
	}
	return it->second;
}


/*									//
//				作成				//
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
//				追加				//
//									*/
void ArchivesSound::Add(string name, string filename) {
	m_Sounds.insert(pair<string, string>(name, filename));
}