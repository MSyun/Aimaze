//	アニメーションカーブ
// 2017. 2.20	プログラム作成
// Author		SyunMizuno


#pragma once

#include	"../Convert.h"
#include	<vector>
using namespace std;


enum EaseType {
	// Quad
	EaseInQuad,
	EaseOutQuad,
	EaseInOutQuad,
	// Cubic
	EaseInCubic,
	EaseOutCubic,
	EaseInOutCubic,
	// Quart
	EaseInQuart,
	EaseOutQuart,
	EaseInOutQuart,
	// Quint
	EaseInQuint,
	EaseOutQuint,
	EaseInOutQuint,
	// Expo
	EaseInExpo,
	EaseOutExpo,
	EaseInOutExpo,
	// Circ
	EaseInCirc,
	EaseOutCirc,
	EaseInOutCirc,
	// Sine
	EaseInSine,
	EaseOutSine,
	EaseInOutSine,
	// liner
	EaseLiner,
	// Bounce
	EaseInBounce,
	EaseOutBounce,
	EaseInOutBounce,
	// Back
	EaseInBack,
	EaseOutBack,
	EaseInOutBack,
	// Elastic
	EaseInElastic,
	EaseOutElastic,
	EaseInOutElastic,
};

struct Keyframe {
	Vector2	fInTangent;		// 前のキーフレームから、このキーフレームに近づくときの接線
	Vector2	fOutTangent;	// 次のキーフレームに向かって、キーフレームを離れる際の接線
	float	fTime;			// キーフレームの時間
	float	fValue;			// キーフレームでのカーブの値

	Keyframe(float time, float value) :
		fInTangent(time, value),
		fOutTangent(time, value),
		fTime(time),
		fValue(value) {}
	Keyframe(float time, float value, Vector2 inTangent, Vector2 outTangent) :
		fInTangent(inTangent),
		fOutTangent(outTangent),
		fTime(time),
		fValue(value) {}
	Keyframe(Keyframe& key) :
		fInTangent(key.fInTangent),
		fOutTangent(key.fOutTangent),
		fTime(key.fTime),
		fValue(key.fValue) {}
};


class AnimationCurve {
private:
#pragma region variable

	vector<Keyframe*>	m_Keys;			// 登録済みのキーフレーム
	EaseType			m_CurrentEase;	// 現在の補完の種類

#pragma endregion

public:
	AnimationCurve();
	virtual ~AnimationCurve();

	// 補完設定
	void SetEase(EaseType type);
	// 補完取得
	EaseType GetEase();

	// キーの数を取得
	int Length();

	/* キーの追加
	// time		:	キーを追加する時間
	// value	:	キーの値
	// return	:	追加されたキーのインデックス
	//				既にキーがその時間に存在する場合は-1
	*/
	int AddKey(float time, float value);

	/* キーの追加
	// key		:	追加するキーフレーム
	// return	:	追加されたキーのインデックス
	//				既にキーがその時間に存在する場合は-1
	*/
	int AddKey(Keyframe& key);

	/* キーの削除
	// index	:	削除するキーのインデックス
	*/
	void RemoveKey(int index);

	/* timeのカーブを評価
	// time		:	評価したいカーブ内の時間
	// return	:	指定された時間のカーブの値
	*/
	float Evaluate(float time);

#pragma region EaseCreate
	// Liner
	void CreateLiner();
	// Quad
	void CreateInQuad();
	void CreateOutQuad();
	void CreateInOutQuad();
	// Cubic
	void CreateInCubic();
	void CreateOutCubic();
	void CreateInOutCubic();
	// Quart
	void CreateInQuart();
	void CreateOutQuart();
	void CreateInOutQuart();
	// Quint
	void CreateInQuint();
	void CreateOutQuint();
	void CreateInOutQuint();
	// Expo
	void CreateInExpo();
	void CreateOutExpo();
	void CreateInOutExpo();
	// Circ
	void CreateInCirc();
	void CreateOutCirc();
	void CreateInOutCirc();
	// Sine
	void CreateInSine();
	void CreateOutSine();
	void CreateInOutSine();
	// Bounce
	void CreateInBounce();
	void CreateOutBounce();
	void CreateInOutBounce();
	// Back
	void CreateInBack();
	void CreateOutBack();
	void CreateInOutBack();
	// Elastic
	void CreateInElastic();
	void CreateOutElastic();
	void CreateInOutElastic();
#pragma endregion

protected:
	// 補完の変更
	void ChangeEase(EaseType type);
};