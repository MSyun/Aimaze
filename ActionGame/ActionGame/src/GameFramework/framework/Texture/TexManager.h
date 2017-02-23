// �e�N�X�`���}�l�[�W���[�N���X
// 2015.11.14	�v���W�F�N�g�쐬
// 2016. 1.13	Singleton�֕ύX
// 2016. 9. 3	����m�F�p�w���p�[�ϐ��ǉ�
// author		SyunMizuno
// �Q�ƃT�C�g	http://www.c3.club.kyutech.ac.jp/gamewiki/index.php?%A5%B2%A1%BC%A5%E0%C5%AA%A5%AF%A5%E9%A5%B9%C0%DF%B7%D7

#pragma once

#include	<map>
#include	<d3dx9.h>
#include	<string>
#include	"../Singleton/Singleton.h"

#pragma comment ( lib, "d3d9.lib" )		// �`�揈���ɕK�v

using namespace std;


class TexManagerBase {
protected:
	map< string, LPDIRECT3DTEXTURE9 >	m_MapTexture;
	bool m_bHelper;

public:
	TexManagerBase();
	virtual ~TexManagerBase();

	// �쐬
	LPDIRECT3DTEXTURE9 Create(const string FileName);

	// �폜
	HRESULT Delete(const string Name);

	// �擾
	LPDIRECT3DTEXTURE9 Get(const string Name);

	void SetHelper(bool helper) { m_bHelper = helper; }
};

class TexManager : public	TexManagerBase, public	Singleton<TexManager> {
public:
	TexManager() : TexManagerBase() {}
};

// ����Ŏg�p���₷���Ȃ�
inline TexManager* GetTexManager(void) {
	return TexManager::Instance();
}