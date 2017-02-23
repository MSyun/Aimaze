// 2015.11.23	�v���O�����쐬
// author		SyunMizuno


#include	"HighLevelAnimController.h"
#include	"../../Utility/System/SystemUtility.h"
#include	"../../Debug/Debug.h"


/*
	MEMO	:
			OneLoopCheck���ŃX�s�[�h���Ȃ���1.5�{���Ă���
			���[�v���Ԋ֌W�Ƀo�O����
*/


/*									//
//			�R���X�g���N�^			//
//									*/
HighLevelAnimController::HighLevelAnimController() :
	m_pAnimCont(NULL),
	m_CurAnimID(0),
	m_PreAnimID(0),
	m_onAnyAnimState(NULL)
{
}


/*									//
//			�f�X�g���N�^			//
//									*/
HighLevelAnimController::~HighLevelAnimController() {
	Release();
}


/*									//
//				���				//
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
//�A�j���[�V�����R���g���[���[�̐ݒ�//
//									*/
bool HighLevelAnimController::SetAnimCont( ID3DXAnimationController *pAnimCont ) {
	// ��O����
	if (pAnimCont == NULL) {
		Debug::LogError("�A�j���[�V�����R���g���[���[������܂���ł���");
		return false;
	}

	// �R���g���[���[�̃R�s�[
	Release();
	pAnimCont->CloneAnimationController(
		pAnimCont->GetMaxNumAnimationOutputs(),
		pAnimCont->GetMaxNumAnimationSets(),
		pAnimCont->GetMaxNumTracks(),
		pAnimCont->GetMaxNumEvents(),
		&m_pAnimCont );

	// �A�j���[�V�����\���̍쐬
	for( DWORD i = 0; i < pAnimCont->GetNumAnimationSets(); i++ ) {
		HLANIMATION_DESC* Anim = new HLANIMATION_DESC;
		pAnimCont->GetAnimationSet( i, &(Anim->pAnimSet) );
		Anim->uiAnimID			= i;											// �A�j���[�V����ID
		Anim->Name				= Anim->pAnimSet->GetName();					// �A�j���[�V������
		Anim->fLoopTime			= 1.0f;											// 1���[�v����{��
		Anim->fTrackSpeed		= (float)Anim->pAnimSet->GetPeriod() * Anim->fLoopTime;
		Anim->fShiftTime		= 0.5f;											// �O�̃A�j���[�V�������犮�S�ڍs����܂ł̎���
		Anim->fCurWeightTime	= 1.0f;											// �ő�1.0f
		Anim->bLoop				= false;										// ���[�v�m�F�t���O
		Anim->dPeriodTime		= Anim->pAnimSet->GetPeriod() * 60.0;
		m_Anim.push_back( Anim );

		m_onEnterAnimState.push_back(new CPPEvent<>);
		m_onStartAnimState.push_back(new CPPEvent<>);
		m_onUpdateAnimState.push_back(new CPPEvent<double>);
		m_onEndAnimState.push_back(new CPPEvent<>);
		m_onExitAnimState.push_back(new CPPEvent<>);
	}
	m_onAnyAnimState = new CPPEvent<>;

	// �V�����A�j���[�V�����Z�b�g���g���b�N0�ɐݒ�
	m_pAnimCont->SetTrackAnimationSet( 0, m_Anim[m_CurAnimID]->pAnimSet );
	// �g���b�N0�̃X�s�[�h�̐ݒ�
	m_pAnimCont->SetTrackSpeed( 0, m_Anim[m_CurAnimID]->fTrackSpeed );
	return true;
}


/*									//
// �A�j���[�V�����R���g���[�����擾 //
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
		Debug::LogError("�A�j���[�V�����R���g���[���[�̃R�s�[�Ɏ��s���܂���");
		return false;
	}
	return true;
}


/*									//
//			���[�v���Ԃ�ݒ�		//
//									*/
bool HighLevelAnimController::SetLoopTime(UINT animID, FLOAT time) {
	// �w��̃A�j���[�V����ID�̑��݂��`�F�b�N
	if (m_Anim.size() <= animID) {
		Debug::LogError("�w�肳�ꂽ�A�j���[�V�����ԍ��͑��݂��܂���ł��� : " + animID);
		return false;
	}

	// �g���b�N�X�s�[�h���ߒl���Z�o
	float DefTime = (float)m_Anim[animID]->pAnimSet->GetPeriod();
	m_Anim[animID]->fLoopTime = time;
	m_Anim[animID]->fTrackSpeed = DefTime * time;

	return true;
}
bool HighLevelAnimController::SetLoopTime(string name, FLOAT time) {
	// �w��̃A�j���[�V�����̑��݂��`�F�b�N
	for (vector<HLANIMATION_DESC*>::iterator itr = m_Anim.begin(); itr != m_Anim.end(); ++itr) {
		if (strcmp((*itr)->Name, name.c_str()) == 0) {
			return SetLoopTime((*itr)->uiAnimID, time);
		}
	}

	Debug::LogError("�w�肵���A�j���[�V�������͑��݂��܂��� : " + name);
	return false;
}

/*									//
//		����ɂ����鎞�Ԃ�ݒ�		//
//									*/
bool HighLevelAnimController::SetShiftTime(UINT animID, FLOAT interval) {
	// �w��̃A�j���[�V����ID�̑��݂��`�F�b�N
	if (m_Anim.size() <= animID) {
		Debug::LogError("�w�肳�ꂽ�A�j���[�V�����ԍ��͑��݂��܂���ł��� : " + animID);
		return false;
	}

	// �V�t�g���Ԃ�o�^
	m_Anim[animID]->fShiftTime = interval;

	return true;
}
bool HighLevelAnimController::SetShiftTime(string name, FLOAT interval) {
	// �w��̃A�j���[�V�����̑��݂��`�F�b�N
	for (vector<HLANIMATION_DESC*>::iterator itr = m_Anim.begin(); itr != m_Anim.end(); ++itr) {
		if (strcmp((*itr)->Name, name.c_str()) == 0) {
			return SetShiftTime((*itr)->uiAnimID, interval);
		}
	}
	Debug::LogError("�w�肵���A�j���[�V�������͑��݂��܂��� : " + name);
	return false;
}


/*									//
//		�A�j���[�V������؂�ւ�	//
//									*/
bool HighLevelAnimController::ChangeAnim(UINT animID) {
	// �w��̃A�j���[�V����ID�̑��݂��`�F�b�N
	if (m_Anim.size() <= animID) {
		Debug::LogError("�w�肳�ꂽ�A�j���[�V�����ԍ��͑��݂��܂���ł��� : " + animID);
		return false;
	}

	// �قȂ�A�j���[�V�����ł��邩���`�F�b�N
	if (m_CurAnimID == animID)
		return false;

	m_onExitAnimState[m_CurAnimID]->Invoke();

	// ���݂̃A�j���[�V�����Z�b�g�̐ݒ�l���擾
	D3DXTRACK_DESC TD;	// �g���b�N�̔\��
	m_pAnimCont->GetTrackDesc(0, &TD);

	// ���̃A�j���[�V�������g���b�N1�Ɉڍs��
	// �g���b�N�̐ݒ�l���ȍ~
	m_pAnimCont->SetTrackAnimationSet(1, m_Anim[m_CurAnimID]->pAnimSet);
	m_pAnimCont->SetTrackDesc(1, &TD);

	// �V�����A�j���[�V�����Z�b�g���g���b�N0�ɐݒ�
	m_pAnimCont->SetTrackAnimationSet(0, m_Anim[animID]->pAnimSet);

	// �g���b�N0�̃X�s�[�h�̐ݒ�
	m_pAnimCont->SetTrackSpeed(0, m_Anim[animID]->fTrackSpeed);

	// �g���b�N�̍���������
	m_pAnimCont->SetTrackEnable(0, true);
	m_pAnimCont->SetTrackEnable(1, true);

	// �E�F�C�g���Ԃ�������
	m_Anim[animID]->fCurWeightTime = 0.0f;
	// ���݂̃A�j���[�V�����ԍ���؂�ւ�
	m_PreAnimID = m_CurAnimID;
	m_CurAnimID = animID;

	// �O���[�o�����Ԃ����Z�b�g
	m_pAnimCont->ResetTime();

	// ���[�J�����ԃ��Z�b�g
	m_pAnimCont->SetTrackPosition(0, 0.0);
	m_pAnimCont->AdvanceTime(0, NULL);

	// �C�x���g���J�n
	m_onEnterAnimState[m_CurAnimID]->Invoke();
	m_onStartAnimState[m_CurAnimID]->Invoke();

	return true;
}
bool HighLevelAnimController::ChangeAnim(string name) {
	// �w��̃A�j���[�V�����̑��݂��`�F�b�N
	for (vector<HLANIMATION_DESC*>::iterator itr = m_Anim.begin(); itr != m_Anim.end(); ++itr) {
		if (strcmp((*itr)->Name, name.c_str()) == 0) {
			return ChangeAnim((*itr)->uiAnimID);
		}
	}
	Debug::LogError("�w�肵���A�j���[�V�������͑��݂��܂��� : " + name);
	return false;
}



/*									//
//		�A�j���[�V�������X�V		//
//									*/
bool HighLevelAnimController::AdvanceTime(FLOAT time) {
	// �A�j���[�V�����̊m�F
	if (m_pAnimCont == NULL)
		return false;

	// ���������ۂ��𔻒�
	m_Anim[m_CurAnimID]->fCurWeightTime += time;
	if (m_Anim[m_CurAnimID]->fCurWeightTime <= m_Anim[m_CurAnimID]->fShiftTime) {
		// �������B�E�F�C�g���Z�o
		float Weight = m_Anim[m_CurAnimID]->fCurWeightTime / m_Anim[m_CurAnimID]->fShiftTime;
		m_pAnimCont->SetTrackWeight(0, Weight);		// ���݂̃A�j���[�V����
		m_pAnimCont->SetTrackWeight(1, 1 - Weight);	// �O�̃A�j���[�V����
	} else {
		// �����I�����B�ʏ�A�j���[�V����������Track0�̃E�F�C�g���ő�l��
		m_pAnimCont->SetTrackWeight(0, 1.0f);		// ���݂̃A�j���[�V����
		m_pAnimCont->SetTrackEnable(1, false);	// �O�̃A�j���[�V�����𖳌��ɂ���
	}

	m_onUpdateAnimState[m_CurAnimID]->Invoke(m_pAnimCont->GetTime() * m_Anim[m_CurAnimID]->fLoopTime * 60.0);
	m_onAnyAnimState->Invoke();

	// ���[�v�m�F
	if (!m_Anim[m_CurAnimID]->bLoop) {
		if (OneLoopCheck()) {
			time = 0.0;
			m_onEndAnimState[m_CurAnimID]->Invoke();
		}
	}

	// ���Ԃ��X�V
	m_pAnimCont->AdvanceTime(time, NULL);

	return true;
}


/*									//
//		�A�j���[�V������1�񃋁[�v	//
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
//			���[�v�m�F�̐ݒ�		//
//									*/
bool HighLevelAnimController::SetLoop(UINT animID, bool loop) {
	// �w��̃A�j���[�V����ID�̑��݂��`�F�b�N
	if (m_Anim.size() <= animID) {
		Debug::LogError("�w�肳�ꂽ�A�j���[�V�����ԍ��͑��݂��܂���ł��� : " + animID);
		return false;
	}

	// �V�t�g���Ԃ�o�^
	m_Anim[animID]->bLoop = loop;

	return true;
}
bool HighLevelAnimController::SetLoop(string name, bool loop) {
	// �w��̃A�j���[�V�����̑��݂��`�F�b�N
	for (auto itr = m_Anim.begin(); itr != m_Anim.end(); ++itr) {
		if (strcmp((*itr)->Name, name.c_str()) == 0) {
			return SetLoop((*itr)->uiAnimID, loop);
		}
	}
	Debug::LogError("�w�肵���A�j���[�V�������͑��݂��܂��� : " + name);
	return false;
}


/*									//
//			�I���t���[���ݒ�		//
//									*/
bool HighLevelAnimController::SetPeriod(UINT animID, double period) {
	// �w��̃A�j���[�V����ID�̑��݂��`�F�b�N
	if (m_Anim.size() <= animID) {
		Debug::LogError("�w�肳�ꂽ�A�j���[�V�����ԍ��͑��݂��܂���ł��� : " + animID);
		return false;
	}

	// �V�t�g���Ԃ�o�^
	m_Anim[animID]->dPeriodTime = period;

	return true;
}
bool HighLevelAnimController::SetPeriod(string name, double period) {
	// �w��̃A�j���[�V�����̑��݂��`�F�b�N
	for (auto itr = m_Anim.begin(); itr != m_Anim.end(); ++itr) {
		if (strcmp((*itr)->Name, name.c_str()) == 0) {
			return SetPeriod((*itr)->uiAnimID, period);
		}
	}
	Debug::LogError("�w�肵���A�j���[�V�������͑��݂��܂��� : " + name);
	return false;
}

#pragma region Event
/*									//
//	�ǂ̃X�e�[�g�ł��Ă΂��C�x���g//
//									*/
CPPEvent<>* HighLevelAnimController::GetOnAnyState() {
	return m_onAnyAnimState;
}
/*									//
//		�X�e�[�g�J�n�C�x���g		//
//									*/
CPPEvent<>* HighLevelAnimController::GetOnEnterAnim(UINT animID) {
	if (animID >= m_onEnterAnimState.size()) {
		Debug::LogError("�w�肳�ꂽ�A�j���[�V�����ԍ��͑��݂��܂���ł��� : " + animID);
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
	Debug::LogError("�w�肵���A�j���[�V�������͑��݂��܂��� : " + name);
	return NULL;
}
/*									//
//		�A�j���[�V�����J�n�C�x���g	//
//									*/
CPPEvent<>* HighLevelAnimController::GetOnStartAnim(UINT animID) {
	if (animID >= m_onStartAnimState.size()) {
		Debug::LogError("�w�肳�ꂽ�A�j���[�V�����ԍ��͑��݂��܂���ł��� : " + animID);
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
	Debug::LogError("�w�肵���A�j���[�V�������͑��݂��܂��� : " + name);
	return NULL;
}
/*									//
//		�A�j���[�V�����X�V�C�x���g	//
//									*/
CPPEvent<double>* HighLevelAnimController::GetOnUpdateAnim(UINT animID) {
	if (animID >= m_onUpdateAnimState.size()) {
		Debug::LogError("�w�肳�ꂽ�A�j���[�V�����ԍ��͑��݂��܂���ł��� : " + animID);
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
	Debug::LogError("�w�肵���A�j���[�V�������͑��݂��܂��� : " + name);
	return NULL;
}
/*									//
//		�A�j���[�V�����I���C�x���g	//
//									*/
CPPEvent<>* HighLevelAnimController::GetOnEndAnim(UINT animID) {
	if (animID >= m_onEndAnimState.size()) {
		Debug::LogError("�w�肳�ꂽ�A�j���[�V�����ԍ��͑��݂��܂���ł��� : " + animID);
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
	Debug::LogError("�w�肵���A�j���[�V�������͑��݂��܂��� : " + name);
	return NULL;
}
/*									//
//		�X�e�[�g�I���C�x���g		//
//									*/
CPPEvent<>* HighLevelAnimController::GetOnExitAnim(UINT animID) {
	if (animID >= m_onExitAnimState.size()) {
		Debug::LogError("�w�肳�ꂽ�A�j���[�V�����ԍ��͑��݂��܂���ł��� : " + animID);
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
	Debug::LogError("�w�肵���A�j���[�V�������͑��݂��܂��� : " + name);
	return NULL;
}
#pragma endregion