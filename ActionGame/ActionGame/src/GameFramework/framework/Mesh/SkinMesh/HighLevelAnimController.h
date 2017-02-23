// 2015.11.23	プログラム作成
// 2016. 4.19	アニメーション名でも実行できるよう関数追加
// 2016. 8.20	倍速処理追加
// 2016. 8.28	ループフラグ追加
// 2017. 1.17	アニメーション開始・終了イベント追加
// 2017. 1.23	イベント追加
// author		SyunMizuno

#pragma once

#include	<d3dx9.h>
#include	<vector>
#include	<string>
#include	"../../CPPEvent/CPPEvent.h"
using namespace std;



/*
	memo :
			余裕があればイベントではなくクラスに機能を置いた方がいいかも？
*/




// アニメーション上位レベル構造体
struct HLANIMATION_DESC {
	UINT	uiAnimID;				// アニメーションID
	LPCSTR	Name;					// アニメーション名
	ID3DXAnimationSet*	pAnimSet;	// アニメーションセット
	FLOAT	fLoopTime;				// 1ループの倍速
	FLOAT	fTrackSpeed;			// トラックスピード調整値
	FLOAT	fShiftTime;				// シフトするのにかかる時間
	FLOAT	fCurWeightTime;			// 現在のウェイト時間
	bool	bLoop;
	double	dPeriodTime;			// アニメーション終了フレーム
};

// アニメーション上位レベルインターフェイス
interface IHighLevelAnimController {
public:
	// アニメーションコントローラを設定
	bool SetAnimCont(ID3DXAnimationController* pAnimCont);

	// アニメーションコントローラを取得
	bool GetAnimCont(ID3DXAnimationController** ppAnimCont);

	// ループ時間を設定
	bool SetLoopTime(UINT animID, FLOAT time);

	// 動作にかかる時間を設定
	bool SetShiftTime(UINT animID, FLOAT interval);

	// アニメーションを切り替え
	bool ChangeAnim(UINT animID);

	// アニメーションを更新
	bool AdvanceTime(FLOAT time);

	// アニメーションを1回ループ
	bool OneLoopCheck();

	// ループ確認設定
	bool SetLoop(UINT animID, bool loop);

	// 終了フレーム設定
	bool SetPeriod(UINT animID, double period);
};

class HighLevelAnimController : public	IHighLevelAnimController {
private:
#pragma region variable

	vector<HLANIMATION_DESC*>	m_Anim;				// アニメーション構造体配列
	LPD3DXANIMATIONCONTROLLER	m_pAnimCont;		// コントローラー
	UINT						m_CurAnimID;		// 現在のアニメーション
	UINT						m_PreAnimID;		// 前回のアニメーション

	CPPEvent<>*					m_onAnyAnimState;	// どのステートからでも呼べるイベント
	vector<CPPEvent<>*>			m_onEnterAnimState;	// 自身のステートの開始イベント
	vector<CPPEvent<>*>			m_onStartAnimState;	// アニメーション開始イベント
	vector<CPPEvent<double>*>	m_onUpdateAnimState;// アニメーション更新イベント
	vector<CPPEvent<>*>			m_onEndAnimState;	// アニメーション終了イベント
	vector<CPPEvent<>*>			m_onExitAnimState;	// 自身のステートの終了イベント

#pragma endregion

public:
#pragma region method

	HighLevelAnimController();		// コンストラクタ
	~HighLevelAnimController();	// デストラクタ
	void Release();

	// アニメーションコントローラを設定
	bool SetAnimCont(ID3DXAnimationController *pAnimCont);

	// アニメーションコントローラを取得
	bool GetAnimCont(ID3DXAnimationController **ppAnimCont);

	// ループ時間を設定
	bool SetLoopTime(UINT animID, FLOAT time);
	bool SetLoopTime(string name, FLOAT time);

	// 動作にかかる時間を設定
	bool SetShiftTime(UINT animID, FLOAT interval);
	bool SetShiftTime(string name, FLOAT interval);

	// アニメーションを切り替え
	bool ChangeAnim(UINT animID);
	bool ChangeAnim(string name);

	// アニメーションを更新
	bool AdvanceTime(FLOAT time);

	// アニメーションを1回ループ
	bool OneLoopCheck();

	// ループ確認設定
	bool SetLoop(UINT animID, bool loop);
	bool SetLoop(string name, bool loop);

	// 終了フレーム設定
	bool SetPeriod(UINT animID, double period);
	bool SetPeriod(string name, double period);

#pragma region Event
	// どのステートでも呼ばれるイベント
	CPPEvent<>* GetOnAnyState();
	// ステート開始イベント
	CPPEvent<>* GetOnEnterAnim(UINT animID);
	CPPEvent<>* GetOnEnterAnim(string name);
	// アニメーション開始イベント
	CPPEvent<>* GetOnStartAnim(UINT animID);
	CPPEvent<>* GetOnStartAnim(string name);
	// アニメーション更新イベント
	CPPEvent<double>* GetOnUpdateAnim(UINT animID);
	CPPEvent<double>* GetOnUpdateAnim(string name);
	// アニメーション終了イベント
	CPPEvent<>* GetOnEndAnim(UINT animID);
	CPPEvent<>* GetOnEndAnim(string name);
	// ステート終了イベント
	CPPEvent<>* GetOnExitAnim(UINT animID);
	CPPEvent<>* GetOnExitAnim(string name);
#pragma endregion

#pragma endregion
};