// �T�E���h���W��
// 2017. 1.22	�v���O�����쐬
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

	// ����
	string Find(const string name);

protected:
	// �쐬
	virtual void Create();

private:
	// �ǉ�
	virtual void Add(string name, string filename);
};