// グラフィック
// 2017. 2. 23	プログラム作成
// author		SyunMizuno


#pragma once

#include	<d3dx9.h>
#include	"IGraphics/IGraphics.h"
#include	"../Singleton/Singleton.h"

/*
	Memo	:
			DirectX9限定になっているので要修正
*/


class Graphics	:	public	IGraphics	,	public	Singleton<Graphics> {
private:
	LPDIRECT3DDEVICE9	m_pDevice;

public:
	Graphics();
	virtual ~Graphics();

	LPDIRECT3DDEVICE9 GetDevice();
	void SetDevice(LPDIRECT3DDEVICE9 _dev);

#pragma region View
	// ビュー行列の設定
	virtual void SetView(const Matrix& view);
	// ビュー行列の取得
	virtual Matrix GetView();
#pragma endregion

#pragma region Projection
	// プロジェクション行列の設定
	virtual void SetProjection(const Matrix& proj);
	// プロジェクション行列の取得
	virtual Matrix GetProjection();
#pragma endregion

#pragma region Alpha Blend
	// αブレンドを使用するか設定
	virtual void SetAlphaBlendEnable(bool flg);
	// αブレンドの設定を取得
	// true. 使用中 : false. 未使用
	virtual bool GetAlphaBlendEnable();
#pragma endregion

#pragma region Z Enable
	// Zバッファを使用するか設定
	virtual void SetZEnable(bool flg);
	// Zバッファの設定を取得
	// true. 使用中 : false. 未使用
	virtual bool GetZEnable();
#pragma endregion

#pragma region Wrap Mode
	// Wrapの設定
	virtual void SetWrapMode(_eWrapMode mode, _eWrapVec vec = WRAPVEC_UV);
	// Wrapの取得
	virtual _eWrapMode GetWrapMode(_eWrapVec vec);
#pragma endregion

#pragma region Cull Mode
	// Cullingの設定
	virtual void SetCullMode(_eCullMode mode);
	// Cullingの取得
	virtual _eCullMode GetCullMode();
#pragma endregion


private:
	// Wrapから数字へ変更
	DWORD WrapModeToNum(_eWrapMode mode);
	// 数字からWrapへ変更
	_eWrapMode NumToWrapMode(DWORD num);
};


inline Graphics* GetGraphics() {
	return Graphics::Instance();
}