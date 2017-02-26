// シェーダ名集約
// 2017. 1.22	プログラム作成
// Author		SyunMizuno



#include	"ArchivesShader.h"
#include	"../../GameFramework/framework/Debug/Debug.h"



/*									//
//			コンストラクタ			//
//									*/
ArchivesShader::ArchivesShader() {
	Create();
}


/*									//
//			デストラクタ			//
//									*/
ArchivesShader::~ArchivesShader() {
}


/*									//
//				検索				//
//									*/
string ArchivesShader::Find(const string name) {
	auto it = m_Shaders.find(name);
	if (it == m_Shaders.end()) {
		Debug::LogError("シェーダ名" + name + "は見つかりませんでした");
		return "";
	}
	return it->second;
}


/*									//
//				作成				//
//									*/
void ArchivesShader::Create() {
	Add("CrossFilter", "data/SHADER/CrossFilter.fx");
	Add("Gaussian", "data/SHADER/Gaussian.fx");
	Add("HalfLambert", "data/SHADER/HalfLambert.fx");
	Add("Lambert", "data/SHADER/Lambert.fx");
	Add("ParallaxMapping", "data/SHADER/ParallaxMapping.fx");
	Add("Phong", "data/SHADER/Phong.fx");
	Add("Toon", "data/SHADER/Toon.fx");
	Add("Wave", "data/SHADER/Wave.fx");
	Add("ShadowMap", "data/SHADER/ShadowMap.fx");
}


/*									//
//				追加				//
//									*/
void ArchivesShader::Add(string name, string filename) {
	m_Shaders.insert(pair<string, string>(name, filename));
}