// テクスチャ名集約
// 2017. 1.22	プログラム作成
// Author		SyunMizuno



#pragma once


#include	<string>
#include	<map>

using namespace std;


class ArchivesTexture {
private:
	map<string, string>	m_Textures;

public:
	ArchivesTexture();
	virtual ~ArchivesTexture();

	// 検索
	string Find(const string name);

protected:
	// 作成
	virtual void Create();

private:
	// 追加
	virtual void Add(string name, string filename);
};