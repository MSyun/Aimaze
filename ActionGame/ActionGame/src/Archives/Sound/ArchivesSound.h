// サウンド名集約
// 2017. 1.22	プログラム作成
// Author		SyunMizuno



#pragma once


#include	<string>
#include	<map>

using namespace std;


class ArchivesSound {
private:
	map<string, string>	m_Sounds;

public:
	ArchivesSound();
	virtual ~ArchivesSound();

	// 検索
	string Find(const string name);

protected:
	// 作成
	virtual void Create();

private:
	// 追加
	virtual void Add(string name, string filename);
};