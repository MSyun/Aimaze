//	遅延型関数クラス
// 2017. 3.22	プログラム作成
// author		SyunMizuno


#include	"DelayFunction.h"
#include	"Collection/DelayFunctionCollection.h"
#include	"../framework/Debug/Debug.h"
#include	"../framework/Time/Time.h"


/*
	memo	:
			後にインスタンスエラーが起こるはずなのでスマートポインタを
			使用する形に変更すること
*/



/*									//
//			コンストラクタ			//
//									*/
DelayFunction::DelayFunction(
	std::function<void(void)> func,
	double time,
	bool timeScale) :
	m_dExeTime(-1.0),
	m_bTimeScale(timeScale)
{
	Regist(func, time);
	DelayFunctionCollection::Add(this);
}


/*									//
//			デストラクタ			//
//									*/
DelayFunction::~DelayFunction() {
}


/*									//
//				計測				//
//									*/
// return : true 実行終了. false 実行中
bool DelayFunction::TimeMate() {
	if (m_bTimeScale) {
		m_dExeTime -= MateScale();
	} else {
		m_dExeTime -= MateNoScale();
	}

	bool end = CheckEnd();
	if (end) {
		m_Function.Invoke();
	}
	return CheckEnd();
}


/*									//
//				登録				//
//									*/
void DelayFunction::Regist(std::function<void(void)> func, double time) {
	if (time < 0.0f) {
		Debug::LogError("DelayFunction::Regist : 設定した時間が虚数になっています");
	}
	m_Function.Set(func);
	m_dExeTime = time;
}


/*									//
//			実行終了確認			//
//									*/
// return : true 実行終了. false 実行中
bool DelayFunction::CheckEnd() {
	return m_dExeTime <= 0.0;
}


/*									//
//		TimeScaleに依存する計測		//
//									*/
double DelayFunction::MateScale() {
	return Time::GetDeltaTime();
}


/*									//
//		TimeScaleに依存しない計測	//
//									*/
double DelayFunction::MateNoScale() {
	return Time::GetUnScaledDeltaTime();
}



void DelayFunc(std::function<void(void)> func, double time) {
	DelayFunction* DF = new DelayFunction(func, time);
}