//	�V�F�[�_�}�l�[�W���N���X
// 2016. 7. 5	�v���O�����쐬
// author		SyunMizuno

#pragma once


#include	"HlslBase.h"
#include	"../Singleton/Singleton.h"
#include	<map>

#include	"Lambert/Lambert.h"
#include	"Phong/Phong.h"
#include	"HalfLambert/HalfLambert.h"
#include	"Gaussian/Gaussian.h"
#include	"Toon/Toon.h"
#include	"ParallaxMapping/ParallaxMapping.h"
#include	"CrossFilter/CrossFilter.h"


using namespace std;

/*
	MEMO	:
			�����o�[�g�̓t�H���Řd����̂ł���Ȃ�����
			�V���h�E�̓V�F�[�_���Ńt�H���E�n�[�t�����o�[�g���̔�r�����邽��
			�����񋓂������邩��
			���X�g����X�t�B���^�[�ƕ`��n�ŕ������邩��
*/

typedef	enum {
	SHADER_NONE = 0,
	SHADER_LAMBERT,		// �����o�[�g
	SHADER_PHONG,		// �t�H��
	SHADER_PARALLAX,	// ����
	SHADER_HALFLAMBERT,	// �n�[�t�����o�[�g
	SHADER_TOON,		// �g�D�[��
	SHADER_GAUSS,		// �K�E�X�t�B���^
	SHADER_CROSS,		// �N���X�t�B���^

	SHADER_MAX
} _shadertype;



class	ShaderManagerBase {
protected:
	// �V�F�[�_�i�[�p�}�b�v
	map< _shadertype, IHlslBase* >	m_MapShader;

public:
	ShaderManagerBase();
	virtual ~ShaderManagerBase();

	IHlslBase* Create(_shadertype type);
	IHlslBase* Get(_shadertype type);
	void Delete(_shadertype type);

	HRESULT Invalidate();
	HRESULT Restore();
};


class	ShaderManager : public	Singleton<ShaderManager>, public	ShaderManagerBase {
public:
	ShaderManager() : ShaderManagerBase() {}
};

inline	ShaderManager*	GetShaderManager() {
	return ShaderManager::Instance();
}