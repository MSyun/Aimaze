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
	// ビューの設定
	virtual void SetView(Matrix& view);
	// ビューの取得
	virtual Matrix GetView();
#pragma endregion

#pragma region Proj
	// プロジェクションの設定
	virtual void SetProjection(Matrix& proj);
	// プロジェクションの取得
	virtual Matrix GetProjection();
#pragma endregion
};


inline Graphics* GetGraphics() {
	return Graphics::Instance();
}