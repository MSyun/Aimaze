// �e�N�X�`�����W��
// 2017. 1.22	�v���O�����쐬
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

	// ����
	string Find(const string name);

protected:
	// �쐬
	virtual void Create();

private:
	// �ǉ�
	virtual void Add(string name, string filename);
};