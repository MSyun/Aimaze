// �V�F�[�_���W��
// 2017. 1.22	�v���O�����쐬
// Author		SyunMizuno



#include	"ArchivesShader.h"
#include	"../../GameFramework/framework/Debug/Debug.h"



/*									//
//			�R���X�g���N�^			//
//									*/
ArchivesShader::ArchivesShader() {
	Create();
}


/*									//
//			�f�X�g���N�^			//
//									*/
ArchivesShader::~ArchivesShader() {
}


/*									//
//				����				//
//									*/
string ArchivesShader::Find(const string name) {
	auto it = m_Shaders.find(name);
	if (it == m_Shaders.end()) {
		Debug::LogError("�V�F�[�_��" + name + "�͌�����܂���ł���");
		return "";
	}
	return it->second;
}


/*									//
//				�쐬				//
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
//				�ǉ�				//
//									*/
void ArchivesShader::Add(string name, string filename) {
	m_Shaders.insert(pair<string, string>(name, filename));
}