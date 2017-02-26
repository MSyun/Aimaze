//	�V�F�[�_�}�l�[�W���N���X
// 2016. 7. 5	�v���O�����쐬
// author		SyunMizuno


#include	"ShaderManager.h"
#include	"../Debug/Debug.h"


/*									//
//			�R���X�g���N�^			//
//									*/
ShaderManagerBase::ShaderManagerBase() {
	m_MapShader.clear();
}


/*									//
//			�f�X�g���N�^			//
//									*/
ShaderManagerBase::~ShaderManagerBase() {
	map< _shadertype, IHlslBase* >::iterator it = m_MapShader.begin();

	while (it != m_MapShader.end()) {
		if (it->second) {
			delete it->second;
			it->second = NULL;
		}
		++it;
	}
	m_MapShader.clear();
}


/*									//
//			�V�F�[�_�̍쐬			//
//									*/
IHlslBase* ShaderManagerBase::Create(_shadertype type) {
	//----- �ϐ��錾
	IHlslBase* hlsl;
	map< _shadertype, IHlslBase* >::iterator p = m_MapShader.find(type);
	// �o�^�ς�
	if (p != m_MapShader.end())
		return Get(type);

	switch(type) {
	case SHADER_LAMBERT:
		hlsl = new Lambert;
		break;

	case SHADER_PHONG:
		hlsl = new Phong;
		break;

	case SHADER_PARALLAX:
		hlsl = new ParallaxMapping;
		break;

	case SHADER_SHADOW:
		hlsl = new ShadowMap;
		break;

	case SHADER_HALFLAMBERT:
		hlsl = new HalfLambert;
		break;

	case SHADER_TOON:
		hlsl = new Toon;
		break;

	case SHADER_GAUSS:
		hlsl = new Gaussian;
		break;

	case SHADER_CROSS:
		hlsl = new CrossFilter;
		break;

	default:
		Debug::Log("���݂��Ȃ��V�F�[�_�ł�");
		return NULL;
	};

	hlsl->Load();
	m_MapShader.insert(pair<_shadertype, IHlslBase*>(type, hlsl));

	return hlsl;
}


/*									//
//			�V�F�[�_�̎擾			//
//									*/
// <return> �V�F�[�_ : NULL. ����
IHlslBase* ShaderManagerBase::Get(_shadertype type) {
	map< _shadertype, IHlslBase* >::iterator p = m_MapShader.find(type);

	if (p == m_MapShader.end()) {
		return Create(type);
	}

	return p->second;
}


/*									//
//			�V�F�[�_�̍폜			//
//									*/
void ShaderManagerBase::Delete(_shadertype type) {
	map< _shadertype, IHlslBase* >::iterator p = m_MapShader.find(type);

	if (p == m_MapShader.end()) {
		Debug::Log("�V�F�[�_�͑��݂��Ȃ����ߍ폜�ł��܂���");
		return;
	}

	if (p->second) {
		delete p->second;
		p->second = NULL;
	}

	m_MapShader.erase(type);

	return;
}


/*									//
//			Invalidate				//
//									*/
// <return> S_OK. ���� : E_FAIL. ���s
HRESULT ShaderManagerBase::Invalidate() {
	map< _shadertype, IHlslBase* >::iterator it = m_MapShader.begin();
	HRESULT hr = S_OK;

	while (it != m_MapShader.end()) {
		hr = (*it).second->Invalidate();
		if (FAILED(hr)) {
			Debug::LogError("�V�F�[�_��Invaridate�Ɏ��s���܂���");
			return hr;
		}
		++it;
	}
	return hr;
}


/*									//
//				Restore				//
//									*/
// <return> S_OK. ���� : E_FAIL. ���s
HRESULT ShaderManagerBase::Restore() {
	map< _shadertype, IHlslBase* >::iterator it = m_MapShader.begin();
	HRESULT hr = S_OK;

	while (it != m_MapShader.end()) {
		hr = (*it).second->Restore();
		if (FAILED(hr)) {
			Debug::LogError("�V�F�[�_��Restore�Ɏ��s���܂���");
			return hr;
		}
		++it;
	}
	return hr;
}