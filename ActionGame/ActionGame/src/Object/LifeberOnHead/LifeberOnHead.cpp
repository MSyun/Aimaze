// ����̃��C�t������
// 2017. 1.31	�v���O�����쐬
// Author		SyunMizuno


#include	"LifeberOnHead.h"
#include	"../../GameFramework/framework/Texture/TexManager.h"
#include	"../../Archives/Archives.h"
#include	"../../GameFramework/framework/Utility/System/SystemUtility.h"




/*									//
//			�R���X�g���N�^			//
//									*/
LifeberOnHead::LifeberOnHead(int max) {
	Quad* pQuad;

	pQuad = new Quad;
	pQuad->SetTex(GetTexManager()->Get(Archives::Texture("Lifeber/Back")));
	pQuad->SetUp(3.0f, 0.3f);
	pQuad->SetBillFlg(true);
	pQuad->GetTransform()->SetParent(m_pTransform);

	m_pGauge = new Gauge3(max);
	m_pGauge->SetTex(GetTexManager()->Get(Archives::Texture("Lifeber/In")));
	m_pGauge->Init(2.85f, 0.18f);
	m_pGauge->SetBillFlg(true);
	m_pGauge->GetTransform()->SetParent(m_pTransform);

	pQuad = new Quad;
	pQuad->SetTex(GetTexManager()->Get(Archives::Texture("Lifeber/Out")));
	pQuad->SetUp(3.0f, 0.3f);
	pQuad->SetBillFlg(true);
	pQuad->GetTransform()->SetParent(m_pTransform);

	pQuad = new Quad;
	pQuad->SetTex(GetTexManager()->Get(Archives::Texture("Lifeber/Frame")));
	pQuad->SetUp(3.05f, 0.35f);
	pQuad->SetBillFlg(true);
	pQuad->GetTransform()->SetParent(m_pTransform);
}


/*									//
//			�f�X�g���N�^			//
//									*/
LifeberOnHead::~LifeberOnHead() {

}