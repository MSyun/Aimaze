//	遅延型関数管理クラス
// 2017. 3.22	プログラム作成
// author		SyunMizuno


#include	"DelayFunctionCollection.h"
#include	"../../framework/Debug/Debug.h"
#include	"../../framework/Utility/System/SystemUtility.h"


std::vector<DelayFunction*> DelayFunctionCollection::m_aCollection;


/*									//
//			コンストラクタ			//
//									*/
DelayFunctionCollection::DelayFunctionCollection() {

}


/*									//
//			デストラクタ			//
//									*/
DelayFunctionCollection::~DelayFunctionCollection() {
	AllDelete();
}


/*									//
//				追加				//
//									*/
void DelayFunctionCollection::Add(DelayFunction* func) {
	if (!func) {
		Debug::LogError("DelayFunctionCollection::Add : Null Instances");
	}
	m_aCollection.push_back(func);
}


/*									//
//				更新				//
//									*/
void DelayFunctionCollection::Update() {
	for (auto it = m_aCollection.begin(); it != m_aCollection.end(); ) {
		if ((*it)->TimeMate()) {
			SAFE_DELETE(*it);
			it = m_aCollection.erase(it);
			continue;
		}
		++it;
	}
}


/*									//
//			全要素削除				//
//									*/
void DelayFunctionCollection::AllDelete() {
	for (auto it = m_aCollection.begin(); it != m_aCollection.end(); ++it) {
		SAFE_DELETE(*it);
	}
	m_aCollection.clear();
}