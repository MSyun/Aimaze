// タグ名集約
// 2017. 1.22	プログラム作成
// Author		SyunMizuno



#include	"ArchivesTag.h"
#include	"../../GameFramework/framework/Debug/Debug.h"



/*									//
//			コンストラクタ			//
//									*/
ArchivesTag::ArchivesTag() {
	Create();
}


/*									//
//			デストラクタ			//
//									*/
ArchivesTag::~ArchivesTag() {
}


/*									//
//				検索				//
//									*/
unsigned char ArchivesTag::Find(const string name) {
	auto it = m_Tags.find(name);
	if (it == m_Tags.end()) {
		Debug::LogError("タグ" + name + "は見つかりませんでした");
		return 0;
	}
	return it->second;
}


/*									//
//				作成				//
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
//				追加				//
//									*/
void ArchivesTag::Add(string name, unsigned char num) {
	m_Tags.insert(pair<string, unsigned char>(name, num));
}