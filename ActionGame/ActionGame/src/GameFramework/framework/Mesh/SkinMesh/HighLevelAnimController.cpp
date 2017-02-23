// 2015.11.23	プログラム作成
// author		SyunMizuno


#include	"HighLevelAnimController.h"
#include	"../../Utility/System/SystemUtility.h"
#include	"../../Debug/Debug.h"


/*
	MEMO	:
			OneLoopCheck内でスピードをなぜか1.5倍している
			ループ時間関係にバグあり
*/


/*									//
//			コンストラクタ			//
//									*/
HighLevelAnimController::HighLevelAnimController() :
	m_pAnimCont(NULL),
	m_CurAnimID(0),
	m_PreAnimID(0),
	m_onAnyAnimState(NULL)
{
}


/*									//
//			デストラクタ			//
//									*/
HighLevelAnimController::~HighLevelAnimController() {
	Release();
}


/*									//
//				解放				//
//									*/
void HighLevelAnimController::Release() {
	for (vector<HLANIMATION_DESC*>::iterator itr = m_Anim.begin(); itr != m_Anim.end(); ) {
		(*itr)->pAnimSet->Release();
		SAFE_DELETE(*itr);
		itr = m_Anim.erase(itr);
	}
	SAFE_RELEASE(m_pAnimCont);

	SAFE_DELETE(m_onAnyAnimState);
	for (auto it = m_onEnterAnimState.begin(); it != m_onEnterAnimState.end(); ++it) {
		SAFE_DELETE(*it);
	}
	for (auto it = m_onStartAnimState.begin(); it != m_onStartAnimState.end(); ++it) {
		SAFE_DELETE(*it);
	}
	for (auto it = m_onUpdateAnimState.begin(); it != m_onUpdateAnimState.end(); ++it) {
		SAFE_DELETE(*it);
	}
	for (auto it = m_onEndAnimState.begin(); it != m_onEndAnimState.end(); ++it) {
		SAFE_DELETE(*it);
	}
	for (auto it = m_onExitAnimState.begin(); it != m_onExitAnimState.end(); ++it) {
		SAFE_DELETE(*it);
	}
	m_onEnterAnimState.clear();
	m_onStartAnimState.clear();
	m_onUpdateAnimState.clear();
	m_onEndAnimState.clear();
	m_onExitAnimState.clear();
}


/*									//
//アニメーションコントローラーの設定//
//									*/
bool HighLevelAnimController::SetAnimCont( ID3DXAnimationController *pAnimCont ) {
	// 例外処理
	if (pAnimCont == NULL) {
		Debug::LogError("アニメーションコントローラーがありませんでした");
		return false;
	}

	// コントローラーのコピー
	Release();
	pAnimCont->CloneAnimationController(
		pAnimCont->GetMaxNumAnimationOutputs(),
		pAnimCont->GetMaxNumAnimationSets(),
		pAnimCont->GetMaxNumTracks(),
		pAnimCont->GetMaxNumEvents(),
		&m_pAnimCont );

	// アニメーション構造体作成
	for( DWORD i = 0; i < pAnimCont->GetNumAnimationSets(); i++ ) {
		HLANIMATION_DESC* Anim = new HLANIMATION_DESC;
		pAnimCont->GetAnimationSet( i, &(Anim->pAnimSet) );
		Anim->uiAnimID			= i;											// アニメーションID
		Anim->Name				= Anim->pAnimSet->GetName();					// アニメーション名
		Anim->fLoopTime			= 1.0f;											// 1ループする倍速
		Anim->fTrackSpeed		= (float)Anim->pAnimSet->GetPeriod() * Anim->fLoopTime;
		Anim->fShiftTime		= 0.5f;											// 前のアニメーションから完全移行するまでの時間
		Anim->fCurWeightTime	= 1.0f;											// 最大1.0f
		Anim->bLoop				= false;										// ループ確認フラグ
		Anim->dPeriodTime		= Anim->pAnimSet->GetPeriod() * 60.0;
		m_Anim.push_back( Anim );

		m_onEnterAnimState.push_back(new CPPEvent<>);
		m_onStartAnimState.push_back(new CPPEvent<>);
		m_onUpdateAnimState.push_back(new CPPEvent<double>);
		m_onEndAnimState.push_back(new CPPEvent<>);
		m_onExitAnimState.push_back(new CPPEvent<>);
	}
	m_onAnyAnimState = new CPPEvent<>;

	// 新しいアニメーションセットをトラック0に設定
	m_pAnimCont->SetTrackAnimationSet( 0, m_Anim[m_CurAnimID]->pAnimSet );
	// トラック0のスピードの設定
	m_pAnimCont->SetTrackSpeed( 0, m_Anim[m_CurAnimID]->fTrackSpeed );
	return true;
}


/*									//
// アニメーションコントローラを取得 //
//									*/
bool HighLevelAnimController::GetAnimCont(ID3DXAnimationController **ppAnimCont) {
	HRESULT hr;
	hr = m_pAnimCont->CloneAnimationController(
		m_pAnimCont->GetMaxNumAnimationOutputs(),
		m_pAnimCont->GetMaxNumAnimationSets(),
		m_pAnimCont->GetMaxNumTracks(),
		m_pAnimCont->GetMaxNumEvents(),
		ppAnimCont);

	if (FAILED(hr)) {
		Debug::LogError("アニメーションコントローラーのコピーに失敗しました");
		return false;
	}
	return true;
}


/*									//
//			ループ時間を設定		//
//									*/
bool HighLevelAnimController::SetLoopTime(UINT animID, FLOAT time) {
	// 指定のアニメーションIDの存在をチェック
	if (m_Anim.size() <= animID) {
		Debug::LogError("指定されたアニメーション番号は存在しませんでした : " + animID);
		return false;
	}

	// トラックスピード調節値を算出
	float DefTime = (float)m_Anim[animID]->pAnimSet->GetPeriod();
	m_Anim[animID]->fLoopTime = time;
	m_Anim[animID]->fTrackSpeed = DefTime * time;

	return true;
}
bool HighLevelAnimController::SetLoopTime(string name, FLOAT time) {
	// 指定のアニメーションの存在をチェック
	for (vector<HLANIMATION_DESC*>::iterator itr = m_Anim.begin(); itr != m_Anim.end(); ++itr) {
		if (strcmp((*itr)->Name, name.c_str()) == 0) {
			return SetLoopTime((*itr)->uiAnimID, time);
		}
	}

	Debug::LogError("指定したアニメーション名は存在しません : " + name);
	return false;
}

/*									//
//		動作にかかる時間を設定		//
//									*/
bool HighLevelAnimController::SetShiftTime(UINT animID, FLOAT interval) {
	// 指定のアニメーションIDの存在をチェック
	if (m_Anim.size() <= animID) {
		Debug::LogError("指定されたアニメーション番号は存在しませんでした : " + animID);
		return false;
	}

	// シフト時間を登録
	m_Anim[animID]->fShiftTime = interval;

	return true;
}
bool HighLevelAnimController::SetShiftTime(string name, FLOAT interval) {
	// 指定のアニメーションの存在をチェック
	for (vector<HLANIMATION_DESC*>::iterator itr = m_Anim.begin(); itr != m_Anim.end(); ++itr) {
		if (strcmp((*itr)->Name, name.c_str()) == 0) {
			return SetShiftTime((*itr)->uiAnimID, interval);
		}
	}
	Debug::LogError("指定したアニメーション名は存在しません : " + name);
	return false;
}


/*									//
//		アニメーションを切り替え	//
//									*/
bool HighLevelAnimController::ChangeAnim(UINT animID) {
	// 指定のアニメーションIDの存在をチェック
	if (m_Anim.size() <= animID) {
		Debug::LogError("指定されたアニメーション番号は存在しませんでした : " + animID);
		return false;
	}

	// 異なるアニメーションであるかをチェック
	if (m_CurAnimID == animID)
		return false;

	m_onExitAnimState[m_CurAnimID]->Invoke();

	// 現在のアニメーションセットの設定値を取得
	D3DXTRACK_DESC TD;	// トラックの能力
	m_pAnimCont->GetTrackDesc(0, &TD);

	// 今のアニメーションをトラック1に移行し
	// トラックの設定値も以降
	m_pAnimCont->SetTrackAnimationSet(1, m_Anim[m_CurAnimID]->pAnimSet);
	m_pAnimCont->SetTrackDesc(1, &TD);

	// 新しいアニメーションセットをトラック0に設定
	m_pAnimCont->SetTrackAnimationSet(0, m_Anim[animID]->pAnimSet);

	// トラック0のスピードの設定
	m_pAnimCont->SetTrackSpeed(0, m_Anim[animID]->fTrackSpeed);

	// トラックの合成を許可
	m_pAnimCont->SetTrackEnable(0, true);
	m_pAnimCont->SetTrackEnable(1, true);

	// ウェイト時間を初期化
	m_Anim[animID]->fCurWeightTime = 0.0f;
	// 現在のアニメーション番号を切り替え
	m_PreAnimID = m_CurAnimID;
	m_CurAnimID = animID;

	// グローバル時間をリセット
	m_pAnimCont->ResetTime();

	// ローカル時間リセット
	m_pAnimCont->SetTrackPosition(0, 0.0);
	m_pAnimCont->AdvanceTime(0, NULL);

	// イベントを開始
	m_onEnterAnimState[m_CurAnimID]->Invoke();
	m_onStartAnimState[m_CurAnimID]->Invoke();

	return true;
}
bool HighLevelAnimController::ChangeAnim(string name) {
	// 指定のアニメーションの存在をチェック
	for (vector<HLANIMATION_DESC*>::iterator itr = m_Anim.begin(); itr != m_Anim.end(); ++itr) {
		if (strcmp((*itr)->Name, name.c_str()) == 0) {
			return ChangeAnim((*itr)->uiAnimID);
		}
	}
	Debug::LogError("指定したアニメーション名は存在しません : " + name);
	return false;
}



/*									//
//		アニメーションを更新		//
//									*/
bool HighLevelAnimController::AdvanceTime(FLOAT time) {
	// アニメーションの確認
	if (m_pAnimCont == NULL)
		return false;

	// 合成中か否かを判定
	m_Anim[m_CurAnimID]->fCurWeightTime += time;
	if (m_Anim[m_CurAnimID]->fCurWeightTime <= m_Anim[m_CurAnimID]->fShiftTime) {
		// 合成中。ウェイトを算出
		float Weight = m_Anim[m_CurAnimID]->fCurWeightTime / m_Anim[m_CurAnimID]->fShiftTime;
		m_pAnimCont->SetTrackWeight(0, Weight);		// 現在のアニメーション
		m_pAnimCont->SetTrackWeight(1, 1 - Weight);	// 前のアニメーション
	} else {
		// 合成終了中。通常アニメーションをするTrack0のウェイトを最大値に
		m_pAnimCont->SetTrackWeight(0, 1.0f);		// 現在のアニメーション
		m_pAnimCont->SetTrackEnable(1, false);	// 前のアニメーションを無効にする
	}

	m_onUpdateAnimState[m_CurAnimID]->Invoke(m_pAnimCont->GetTime() * m_Anim[m_CurAnimID]->fLoopTime * 60.0);
	m_onAnyAnimState->Invoke();

	// ループ確認
	if (!m_Anim[m_CurAnimID]->bLoop) {
		if (OneLoopCheck()) {
			time = 0.0;
			m_onEndAnimState[m_CurAnimID]->Invoke();
		}
	}

	// 時間を更新
	m_pAnimCont->AdvanceTime(time, NULL);

	return true;
}


/*									//
//		アニメーションを1回ループ	//
//									*/
bool HighLevelAnimController::OneLoopCheck() {
	double localtime = 0;
	double baisoku = 0;
	baisoku = m_Anim[m_CurAnimID]->fLoopTime;
	localtime = m_pAnimCont->GetTime() * baisoku * 60.0;

	if (localtime >= m_Anim[m_CurAnimID]->dPeriodTime - DBL_EPSILON) {
		return true;
	}

	return false;
}


/*									//
//			ループ確認の設定		//
//									*/
bool HighLevelAnimController::SetLoop(UINT animID, bool loop) {
	// 指定のアニメーションIDの存在をチェック
	if (m_Anim.size() <= animID) {
		Debug::LogError("指定されたアニメーション番号は存在しませんでした : " + animID);
		return false;
	}

	// シフト時間を登録
	m_Anim[animID]->bLoop = loop;

	return true;
}
bool HighLevelAnimController::SetLoop(string name, bool loop) {
	// 指定のアニメーションの存在をチェック
	for (auto itr = m_Anim.begin(); itr != m_Anim.end(); ++itr) {
		if (strcmp((*itr)->Name, name.c_str()) == 0) {
			return SetLoop((*itr)->uiAnimID, loop);
		}
	}
	Debug::LogError("指定したアニメーション名は存在しません : " + name);
	return false;
}


/*									//
//			終了フレーム設定		//
//									*/
bool HighLevelAnimController::SetPeriod(UINT animID, double period) {
	// 指定のアニメーションIDの存在をチェック
	if (m_Anim.size() <= animID) {
		Debug::LogError("指定されたアニメーション番号は存在しませんでした : " + animID);
		return false;
	}

	// シフト時間を登録
	m_Anim[animID]->dPeriodTime = period;

	return true;
}
bool HighLevelAnimController::SetPeriod(string name, double period) {
	// 指定のアニメーションの存在をチェック
	for (auto itr = m_Anim.begin(); itr != m_Anim.end(); ++itr) {
		if (strcmp((*itr)->Name, name.c_str()) == 0) {
			return SetPeriod((*itr)->uiAnimID, period);
		}
	}
	Debug::LogError("指定したアニメーション名は存在しません : " + name);
	return false;
}

#pragma region Event
/*									//
//	どのステートでも呼ばれるイベント//
//									*/
CPPEvent<>* HighLevelAnimController::GetOnAnyState() {
	return m_onAnyAnimState;
}
/*									//
//		ステート開始イベント		//
//									*/
CPPEvent<>* HighLevelAnimController::GetOnEnterAnim(UINT animID) {
	if (animID >= m_onEnterAnimState.size()) {
		Debug::LogError("指定されたアニメーション番号は存在しませんでした : " + animID);
		return NULL;
	}

	return m_onEnterAnimState[animID];
}
CPPEvent<>* HighLevelAnimController::GetOnEnterAnim(string name) {
	for (auto it = m_Anim.begin(); it != m_Anim.end(); ++it) {
		if (strcmp((*it)->Name, name.c_str()) == 0) {
			return GetOnEnterAnim((*it)->uiAnimID);
		}
	}
	Debug::LogError("指定したアニメーション名は存在しません : " + name);
	return NULL;
}
/*									//
//		アニメーション開始イベント	//
//									*/
CPPEvent<>* HighLevelAnimController::GetOnStartAnim(UINT animID) {
	if (animID >= m_onStartAnimState.size()) {
		Debug::LogError("指定されたアニメーション番号は存在しませんでした : " + animID);
		return NULL;
	}

	return m_onStartAnimState[animID];
}
CPPEvent<>* HighLevelAnimController::GetOnStartAnim(string name) {
	for (auto it = m_Anim.begin(); it != m_Anim.end(); ++it) {
		if (strcmp((*it)->Name, name.c_str()) == 0) {
			return GetOnStartAnim((*it)->uiAnimID);
		}
	}
	Debug::LogError("指定したアニメーション名は存在しません : " + name);
	return NULL;
}
/*									//
//		アニメーション更新イベント	//
//									*/
CPPEvent<double>* HighLevelAnimController::GetOnUpdateAnim(UINT animID) {
	if (animID >= m_onUpdateAnimState.size()) {
		Debug::LogError("指定されたアニメーション番号は存在しませんでした : " + animID);
		return NULL;
	}

	return m_onUpdateAnimState[animID];
}
CPPEvent<double>* HighLevelAnimController::GetOnUpdateAnim(string name) {
	for (auto it = m_Anim.begin(); it != m_Anim.end(); ++it) {
		if (strcmp((*it)->Name, name.c_str()) == 0) {
			return GetOnUpdateAnim((*it)->uiAnimID);
		}
	}
	Debug::LogError("指定したアニメーション名は存在しません : " + name);
	return NULL;
}
/*									//
//		アニメーション終了イベント	//
//									*/
CPPEvent<>* HighLevelAnimController::GetOnEndAnim(UINT animID) {
	if (animID >= m_onEndAnimState.size()) {
		Debug::LogError("指定されたアニメーション番号は存在しませんでした : " + animID);
		return NULL;
	}

	return m_onEndAnimState[animID];
}
CPPEvent<>* HighLevelAnimController::GetOnEndAnim(string name) {
	for (auto it = m_Anim.begin(); it != m_Anim.end(); ++it) {
		if (strcmp((*it)->Name, name.c_str()) == 0) {
			return GetOnEndAnim((*it)->uiAnimID);
		}
	}
	Debug::LogError("指定したアニメーション名は存在しません : " + name);
	return NULL;
}
/*									//
//		ステート終了イベント		//
//									*/
CPPEvent<>* HighLevelAnimController::GetOnExitAnim(UINT animID) {
	if (animID >= m_onExitAnimState.size()) {
		Debug::LogError("指定されたアニメーション番号は存在しませんでした : " + animID);
		return NULL;
	}

	return m_onExitAnimState[animID];
}
CPPEvent<>* HighLevelAnimController::GetOnExitAnim(string name) {
	for (auto it = m_Anim.begin(); it != m_Anim.end(); ++it) {
		if (strcmp((*it)->Name, name.c_str()) == 0) {
			return GetOnExitAnim((*it)->uiAnimID);
		}
	}
	Debug::LogError("指定したアニメーション名は存在しません : " + name);
	return NULL;
}
#pragma endregion