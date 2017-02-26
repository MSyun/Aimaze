// 描画マネージャー
// 2017. 2. 23	プログラム作成
// author		SyunMizuno


#include	"DrawManager.h"
#include	"../framework/Shader/ShaderManager.h"


/*									//
//			コンストラクタ			//
//									*/
DrawManagerBase::DrawManagerBase() {

}


/*									//
//			デストラクタ			//
//									*/
DrawManagerBase::~DrawManagerBase() {

}


/*									//
//			リスト取得				//
//									*/
void DrawManagerBase::SetList(Obj3DManagerBase* mana) {
	m_pObjList = mana->GetList();
}


/*									//
//				描画				//
//									*/
void DrawManagerBase::Draw() {
	for (auto it = m_pObjList->begin(); it != m_pObjList->end(); ++it) {
		// 親の確認
		if ((*it)->pObj->GetTransform()->CheckParent())
			continue;

		(*it)->pObj->DrawAll();
	}
}


/*									//
//				後描画				//
//									*/
void DrawManagerBase::LateDraw() {
	for (auto it = m_pObjList->begin(); it != m_pObjList->end(); ++it) {
		// 親の確認
		if ((*it)->pObj->GetTransform()->CheckParent())
			continue;

		(*it)->pObj->LateDrawAll();
	}
}


/*									//
//				抽出				//
//									*/
void DrawManagerBase::Extract() {
	CheckFrustum();
	CheckOclusion();
}


/*									//
//				影作成				//
//									*/
void DrawManagerBase::CreateShadow() {
	IHlslBase* shadow = GetShaderManager()->Get(SHADER_SHADOW);
	shadow->Begin();

	for (auto it = m_pObjList->begin(); it != m_pObjList->end(); ++it) {
		if ((*it)->pObj->GetTransform()->CheckParent())
			continue;

		(*it)->pObj->DrawShadow();
	}

	shadow->End();
}


/*									//
//				視錐台				//
//									*/
void DrawManagerBase::CheckFrustum() {

}


/*									//
//			オクルージョン			//
//									*/
void DrawManagerBase::CheckOclusion() {

}


/*									//
//			フィルタ描画			//
//									*/
void DrawManagerBase::FilterDraw() {

}