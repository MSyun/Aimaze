// -------------------------------------------------------------
// シャドウマップ
// 
// -------------------------------------------------------------

// -------------------------------------------------------------
// グローバル変数
// -------------------------------------------------------------
float4x4 matWorld[4]	:	WORLD;		// ワールド変換行列配列
float4x4 matView		:	VIEW;		// ビュー変換行列
float4x4 matProj		:	PROJECTION;	// 射影変換行列

int iBlendNum;							// ブレンドする配列の数


// -------------------------------------------------------------
// 頂点シェーダからピクセルシェーダに渡すデータ
// -------------------------------------------------------------
struct VS_OUTPUT
{
	float4 Pos			: POSITION;
	float4 ShadowMapUV	: TEXCOORD0;
};

// -------------------------------------------------------------
// 頂点シェーダプログラム
// -------------------------------------------------------------
VS_OUTPUT VS(
	float4 Pos	: POSITION		// モデルの頂点
) {
	VS_OUTPUT Out = (VS_OUTPUT)0;		// 出力データ

	// 座標変換
	Out.Pos = mul(Pos, matWorld[0]);
	Out.Pos = mul(Out.Pos, matView);
	Out.Pos = mul(Out.Pos, matProj);

	// カメラ座標系での深度をテクスチャに入れる
	Out.ShadowMapUV = Out.Pos;

	return Out;
}


// スキンメッシュ
VS_OUTPUT VS_Skin(
	float4 Pos	: POSITION,		// モデルの頂点
	float4 W	: BLENDWEIGHT
) {
	VS_OUTPUT Out = (VS_OUTPUT)0;		// 出力データ

										//----- 座標変換
	float		Weight[4] = (float[4])W;	// 重みをfloatに分割
	float		LastBlendWeight = 0.0f;		// 最後の行列に掛けられる重み
	float4x4	matCombWorld = 0.0f;		// 合成ワールド変換行列

											// ワールド変換行列をブレンド
	for (int i = 0; i < iBlendNum - 1; i++) {
		LastBlendWeight += Weight[i];				// 最後の重みをここで計算しておく
		matCombWorld += matWorld[i] * Weight[i];
	}

	// 最後の重みを足し算
	matCombWorld += matWorld[iBlendNum - 1] * (1.0f - LastBlendWeight);

	float4 P_after;
	P_after = mul(Pos, matCombWorld);	// ワールド変換
	P_after = mul(P_after, matView);	// ビュー変換
	P_after = mul(P_after, matProj);	// 射影変換
	Out.Pos = P_after;

	// カメラ座標系での深度をテクスチャに入れる
	Out.ShadowMapUV = Out.Pos;

	return Out;
}
// -------------------------------------------------------------
// ピクセルシェーダプログラム
// -------------------------------------------------------------
float4 PS(VS_OUTPUT In) : COLOR0 {
	return In.ShadowMapUV.z / In.ShadowMapUV.w;
}

// -------------------------------------------------------------
// テクニック
// -------------------------------------------------------------
technique TShader {
	// シャドウマップの作成
	pass P0 {
		VertexShader = compile vs_1_1 VS();
		PixelShader = compile ps_2_0 PS();
	}

	// シャドウマップの作成(Ver.スキン
	pass P1 {
		VertexShader = compile vs_1_1 VS();
		PixelShader = compile ps_2_0 PS();
	}
}