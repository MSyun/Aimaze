// -------------------------------------------------------------
// ハーフランバート
// 
// Copyright (c) 2003 IMAGIRE Takashi. All rights reserved.
// -------------------------------------------------------------

// -------------------------------------------------------------
// グローバル変数
// -------------------------------------------------------------

float4x4 matWorld[4]	:	WORLD;		// ワールド変換行列配列
float4x4 matView		:	VIEW;		// ビュー変換行列
float4x4 matProj		:	PROJECTION;	// 射影変換行列

int iBlendNum;							// ブレンドする配列の数

float4 vLightDir;	// ライトの方向
float4 vColor;		// ライト＊メッシュの色


texture tex;
sampler TexSamp = sampler_state
{
	texture = <tex>;
	MinFilter = LINEAR;
	MagFilter = LINEAR;
	MipFilter = NONE;

	AddressU = Clamp;
	AddressV = Clamp;
};


// -------------------------------------------------------------
// 頂点シェーダからピクセルシェーダに渡すデータ
// -------------------------------------------------------------
struct VS_OUTPUT
{
	float4	Pos			: POSITION;
	float2	Tex			: TEXCOORD0;
	float3	Normal		: TEXCOORD1;
};
// -------------------------------------------------------------
// シーンの描画
// -------------------------------------------------------------
VS_OUTPUT VS(
		float3 Pos		: POSITION,			// ローカル位置座標
		float3 Normal	: NORMAL,			// 法線ベクトル
		float2 Tex		: TEXCOORD0			// テクスチャ
){
	VS_OUTPUT Out = (VS_OUTPUT)0;		// 出力データ
	
	// 座標変換
	Out.Pos = mul( float4(Pos, 1.0f), matWorld[0] );
	Normal = mul( Normal, (float3x3)matWorld[0] );
	Out.Pos = mul( Out.Pos, matView );
	Out.Pos = mul( Out.Pos, matProj );

	// テクスチャ
	Out.Tex = Tex;

	// 法線
	Out.Normal = Normal;

	return Out;
}

// スキンメッシュバージョン
VS_OUTPUT VS_SKIN(
		float3 Pos		: POSITION,			// ローカル位置座標
		float3 Normal	: NORMAL,			// 法線ベクトル
		float2 Tex		: TEXCOORD0,		// テクスチャ
		float4 W		: BLENDWEIGHT		// 重み
){
	VS_OUTPUT Out = (VS_OUTPUT)0;		// 出力データ

	//----- 座標変換
	float		Weight[4] = (float[4])W;	// 重みをfloatに分割
	float		LastBlendWeight = 0.0f;		// 最後の行列に掛けられる重み
	float4x4	matCombWorld = 0.0f;		// 合成ワールド変換行列

	// ワールド変換行列をブレンド
	for( int i = 0; i < iBlendNum-1; i++ ) {
		LastBlendWeight += Weight[i];				// 最後の重みをここで計算しておく
		matCombWorld += matWorld[i] * Weight[i];
	}

	// 最後の重みを足し算
	matCombWorld += matWorld[iBlendNum-1] * (1.0f-LastBlendWeight);

	Out.Pos = mul( float4(Pos, 1.0f), matCombWorld );	// ワールド変換
	Normal = mul( Normal, (float3x3)matCombWorld );
	Out.Pos = mul( Out.Pos, matView );	// ビュー変換
	Out.Pos = mul( Out.Pos, matProj );	// 射影変換

	//----- テクスチャ
	Out.Tex = Tex;

	// 法線
	Out.Normal = Normal;
	
	return Out;
}

// -------------------------------------------------------------
float4 PS_pass0(VS_OUTPUT In) : COLOR
{
	// 頂点色
	float3 L = -vLightDir.xyz;
//	float amb = vLightDir.w;
	float3 N = normalize(In.Normal);

	float3 p = dot(N, L);
	p = (p + 1.0f) * 0.5f;
	p *= p;
	// テクスチャがある場合マテリアルカラーはいらないかも
	float4 Col;
//	float4 Col = p;
	Col.xyz = vColor.xyz * p;

	Col.w = 1.0f;

	return Col * tex2D( TexSamp, In.Tex );
}

float4 PS_pass1(VS_OUTPUT In) : COLOR
{
	// 頂点色
	float3 L = -vLightDir.xyz;
	float amb = vLightDir.w;
	float3 N = normalize(In.Normal);

	float3 p = dot(N, L);
	p = p * 0.5f + 0.5f;
	p *= p;
	float4 Col;
	Col.xyz = vColor.xyz * p;

	Col.w = 1.0f;

	return Col;
}

// -------------------------------------------------------------
// テクニック
// -------------------------------------------------------------
technique TShader
{
	pass P0	// テクスチャあり
	{
		VertexShader = compile vs_1_1 VS();
		PixelShader  = compile ps_2_0 PS_pass0();
	}

	pass P1	// テクスチャなし
	{
		VertexShader = compile vs_1_1 VS();
		PixelShader  = compile ps_2_0 PS_pass1();
	}

	// スキンメッシュ
	pass P2	// テクスチャあり
	{
		VertexShader = compile vs_1_1 VS_SKIN();
		PixelShader  = compile ps_2_0 PS_pass0();
	}

	pass P3	// テクスチャなし
	{
		VertexShader = compile vs_1_1 VS_SKIN();
		PixelShader  = compile ps_2_0 PS_pass1();
	}
}
