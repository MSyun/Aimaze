// �^�O���W��
// 2017. 1.22	�v���O�����쐬
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

	// ����
	unsigned char Find(const string name);

protected:
	// �쐬
	virtual void Create();

private:
	// �ǉ�
	virtual void Add(string name, unsigned char num);
};