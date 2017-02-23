// タグ名集約
// 2017. 1.22	プログラム作成
// Author		SyunMizuno



#pragma once


#include	<string>
#include	<map>

using namespace std;


class ArchivesTag {
private:
	map<string, unsigned char>	m_Tags;

public:
	ArchivesTag();
	virtual ~ArchivesTag();

	// 検索
	unsigned char Find(const string name);

protected:
	// 作成
	virtual void Create();

private:
	// 追加
	virtual void Add(string name, unsigned char num);
};