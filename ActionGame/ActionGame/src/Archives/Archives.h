// �t�@�C���W��
// 2017. 1. 3	�v���O�����쐬
// 2017. 1.22	�N���X�̒ǉ�
// Author		SyunMizuno


#pragma once


#include	"Texture/ArchivesTexture.h"
#include	"Mesh/ArchivesMesh.h"
#include	"Tag/ArchivesTag.h"
#include	"Sound/ArchivesSound.h"
#include	"Shader/ArchivesShader.h"


// �A�[�J�C�u
class Archives {
private:
#pragma region variable

	static ArchivesMesh		MeshName;
	static ArchivesTexture	TexName;
	static ArchivesSound	SoundName;
	static ArchivesTag		TagName;
	static ArchivesShader	ShaderName;

#pragma endregion

public:
#pragma region method

	static string Mesh(const string Name) {
		return MeshName.Find(Name);
	}
	static string Texture(const string Name) {
		return TexName.Find(Name);
	}
	static string Sound(const string Name) {
		return SoundName.Find(Name);
	}
	static unsigned char Tag(const string Name) {
		return TagName.Find(Name);
	}
	static string Shader(const string Name) {
		return ShaderName.Find(Name);
	}

#pragma endregion
};