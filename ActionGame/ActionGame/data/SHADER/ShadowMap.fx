// シャドウマップ
// 2017. 2.25	プログラム作成

#include	"PosCreator.fh"


// -------------------------------------------------------------
// 頂点シェーダからピクセルシェーダに渡すデータ
// -------------------------------------------------------------
struct VS_OUTPUT {
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
	Out.Pos = PosCreator(Pos, matWorld[0]);

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
	float4x4 World = SkinWorldCreator(W);
	Out.Pos = PosCreator(Pos, World);

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
		VertexShader = compile vs_1_1 VS_Skin();
		PixelShader = compile ps_2_0 PS();
	}
}