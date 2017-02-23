//	シェーダマネージャクラス
// 2016. 7. 5	プログラム作成
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
			ランバートはフォンで賄えるのでいらないかも
			シャドウはシェーダ内でフォン・ハーフランバート等の比較がいるため
			数分列挙が増えるかも
			リストを後々フィルターと描画系で分離するかも
*/

typedef	enum {
	SHADER_NONE = 0,
	SHADER_LAMBERT,		// ランバート
	SHADER_PHONG,		// フォン
	SHADER_PARALLAX,	// 視差
	SHADER_HALFLAMBERT,	// ハーフランバート
	SHADER_TOON,		// トゥーン
	SHADER_GAUSS,		// ガウスフィルタ
	SHADER_CROSS,		// クロスフィルタ

	SHADER_MAX
} _shadertype;



class	ShaderManagerBase {
protected:
	// シェーダ格納用マップ
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