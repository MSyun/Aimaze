//	シェーダマネージャクラス
// 2016. 7. 5	プログラム作成
// author		SyunMizuno


#include	"ShaderManager.h"
#include	"../Debug/Debug.h"


/*									//
//			コンストラクタ			//
//									*/
ShaderManagerBase::ShaderManagerBase() {
	m_MapShader.clear();
}


/*									//
//			デストラクタ			//
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
//			シェーダの作成			//
//									*/
IHlslBase* ShaderManagerBase::Create(_shadertype type) {
	//----- 変数宣言
	IHlslBase* hlsl;
	map< _shadertype, IHlslBase* >::iterator p = m_MapShader.find(type);
	// 登録済み
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
		Debug::Log("存在しないシェーダです");
		return NULL;
	};

	hlsl->Load();
	m_MapShader.insert(pair<_shadertype, IHlslBase*>(type, hlsl));

	return hlsl;
}


/*									//
//			シェーダの取得			//
//									*/
// <return> シェーダ : NULL. 無し
IHlslBase* ShaderManagerBase::Get(_shadertype type) {
	map< _shadertype, IHlslBase* >::iterator p = m_MapShader.find(type);

	if (p == m_MapShader.end()) {
		return Create(type);
	}

	return p->second;
}


/*									//
//			シェーダの削除			//
//									*/
void ShaderManagerBase::Delete(_shadertype type) {
	map< _shadertype, IHlslBase* >::iterator p = m_MapShader.find(type);

	if (p == m_MapShader.end()) {
		Debug::Log("シェーダは存在しないため削除できません");
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
// <return> S_OK. 成功 : E_FAIL. 失敗
HRESULT ShaderManagerBase::Invalidate() {
	map< _shadertype, IHlslBase* >::iterator it = m_MapShader.begin();
	HRESULT hr = S_OK;

	while (it != m_MapShader.end()) {
		hr = (*it).second->Invalidate();
		if (FAILED(hr)) {
			Debug::LogError("シェーダのInvaridateに失敗しました");
			return hr;
		}
		++it;
	}
	return hr;
}


/*									//
//				Restore				//
//									*/
// <return> S_OK. 成功 : E_FAIL. 失敗
HRESULT ShaderManagerBase::Restore() {
	map< _shadertype, IHlslBase* >::iterator it = m_MapShader.begin();
	HRESULT hr = S_OK;

	while (it != m_MapShader.end()) {
		hr = (*it).second->Restore();
		if (FAILED(hr)) {
			Debug::LogError("シェーダのRestoreに失敗しました");
			return hr;
		}
		++it;
	}
	return hr;
}