// ゲーム用DirectX9
// 2016.12.29	プログラム作成
// author		SyunMizuno


#pragma once


#include	"../framework/DirectX/DX9Base.h"
#include	"../framework/Scene/SceneManagerWin.h"
#include	"../framework/Texture/TexManager.h"
#include	"../framework/Mesh/SkinMesh/SkinMeshManager.h"
#include	"../framework/Light/LightManager.h"
#include	"../framework/Camera/CameraManager.h"
#include	"../framework/Sound/SoundManager.h"
#include	"../framework/Shader/ShaderManager.h"
#include	"../Obj/3D/Obj3DManager.h"
#include	"../Obj/2D/Obj2DManager.h"
#include	"../GameController/GameControllerManager.h"


class	GameDirectX9Base	:	public	DX9Base {
protected:
#pragma region variable

	SceneBase*				m_pCurrentScene;
	SceneManagerWin*		m_pSceneManager;
	TexManager*				m_pTexManager;
	SkinMeshManager*		m_pMeshManager;
	LightManager*			m_pLightManager;
	CameraManager*			m_pCameraManager;
	SoundManager*			m_pSoundManager;
	ShaderManager*			m_pShaderManager;
	Obj3DManager*			m_pObj3DManager;
	Obj2DManager*			m_pObj2DManager;
	GameControllerManager*	m_pControllerManager;

#pragma endregion

public:
#pragma region method

	GameDirectX9Base(Application* app);
	virtual ~GameDirectX9Base();

	virtual HRESULT Init();
	virtual HRESULT Step();
	virtual HRESULT Release();

#pragma endregion

protected:
#pragma region conceal method

	virtual HRESULT InvalidateDeviceObjects();
	virtual HRESULT InvalidateDeviceObjectsGame() { return S_OK; }
	virtual HRESULT RestoreDeviceObjects();
	virtual HRESULT RestoreDeviceObjectsGame() { return S_OK; }
	virtual HRESULT InitGame() { return S_OK; }
	virtual HRESULT InitApp();
	virtual HRESULT ReleaseGame() { return S_OK; }
	virtual HRESULT ReleaseApp();
	virtual void Update();
	virtual void Draw();

#pragma endregion
};

