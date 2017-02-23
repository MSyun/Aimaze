// 描画マネージャー
// 2017. 2. 23	プログラム作成
// author		SyunMizuno


#pragma once




class DrawManagerBase {
public:
	DrawManagerBase();
	virtual ~DrawManagerBase();

	// 描画
	void Draw();
	void LateDraw();

	// 抽出
	void Extract();

protected:
	// 影描画
	void DrawShadow();

	// 視錐台
	void CheckFrustum();

	// フィルタ描画
	void FilterDraw();
};