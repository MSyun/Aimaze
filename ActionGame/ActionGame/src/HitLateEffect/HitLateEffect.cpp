// 攻撃ヒット時の遅延効果
// 2017. 3. 30	プログラム作成
// author		SyunMizuno


#include	"HitLateEffect.h"
#include	"../GameFramework/DelayFunction/DelayFunction.h"
#include	"../GameFramework/framework/Time/Time.h"
#include	"../GameFramework/framework/Debug/Debug.h"



/*									//
//			コンストラクタ			//
//									*/
HitLateEffect::HitLateEffect() {

}


/*									//
//			デストラクタ			//
//									*/
HitLateEffect::~HitLateEffect() {

}


/*									//
//			敵のヒット時			//
//									*/
void HitLateEffect::EnemyHit() {
//	Time::SetTimeScale(0.20f);
//	DelayFunc([]() {
//		Time::SetTimeScale(1.0f);
//	}, 0.1, false);
//	Debug::Log("攻撃が当たったから動きを遅くするよ");
}