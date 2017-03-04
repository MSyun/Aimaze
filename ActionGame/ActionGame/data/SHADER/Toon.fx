// トゥーンシェーダ（ペイント＋エッジ
// エッジの色を変えたい場合はテクスチャの真ん中の
// 線の色を変えましょう
// エッジを太くしたい場合テクスチャの真ん中の線の領域を
// 大きくしましょう

#include	"PosCreator.fh"
#include	"Shadow.fh"

// -------------------------------------------------------------
// グローバル変数
// -------------------------------------------------------------
float4 vLightDir;	// ライトの方向
float4 vColor;		// ライト＊メッシュの色

// メッシュ
texture tex;
sampler TexSamp = sampler_state
{
	texture = <tex>;
	MinFilter = LINEAR;
	MagFilter = LINEAR;
	MipFilter = NONE;

	AddressU = Wrap;
	AddressV = Wrap;
};

// トゥーン
texture toontex;
sampler ToonSmp = sampler_state
{
	texture = <toontex>;
	MinFilter = LINEAR;
	MagFilter = LINEAR;
	MipFilter = LINEAR;

	AddressU = Clamp;
	AddressV = Wrap;
};




// -------------------------------------------------------------
// 頂点シェーダからピクセルシェーダに渡すデータ
// -------------------------------------------------------------
struct VS_OUTPUT {
	float4		Pos		:	POSITION;	// 座標
	float2		Tex		:	TEXCOORD0;	// テクスチャ
	float3		Normal	:	TEXCOORD1;	// 法線
	float2		ToonUV	:	TEXCOORD2;	// トゥーンのエッジ

	float4	ShadowMapUV	: TEXCOORD3;
	float4	Depth		: TEXCOORD4;
};


// -------------------------------------------------------------
// シーンの描画
// -------------------------------------------------------------
VS_OUTPUT VS (
		float4	Pos		:	POSITION,	// ローカル位置座標
		float3	Normal	:	NORMAL,		// 法線ベクトル
		float2	Tex		:	TEXCOORD0	// テクスチャ
) {
	VS_OUTPUT Out = (VS_OUTPUT)0;		// 出力データ

	// 座標変換
	float4x4 World = matWorld[0];
	Out.Pos = PosCreator(Pos, World);

	float4 position;
	position = mul(Pos, World);
	Normal = mul(Normal, (float3x3)World);

	// テクスチャ
	Out.Tex = Tex;

	// 法線
	Out.Normal = normalize(Normal);

	//----- テクスチャのY座標の算出(これをエッジ座標として使用)
	float3 wv_pos = normalize(mul(position, matCameraView));
	float3 wv_normal = normalize(mul(Normal, matCameraView));

	// 2つのベクトルの内積を0.0～から1.0の値にしてV座標にする
	Out.ToonUV.y = dot(wv_pos, wv_normal) * 0.5 + 0.5;


	//float4x4 WLP = CreateLightWVP(World);
	//float4x4 WLPB = WLP * matScaleBias;

	//// シャドウマップ
	//Out.ShadowMapUV = mul(Pos, WLPB);
	//Out.Depth = mul(Pos, WLP);

	return Out;
}

// スキンメッシュバージョン
VS_OUTPUT VS_SKIN (
		float4 Pos		: POSITION,			// ローカル位置座標
		float3 Normal	: NORMAL,			// 法線ベクトル
		float2 Tex		: TEXCOORD0,		// テクスチャ
		float4 W		: BLENDWEIGHT		// 重み
) {
	VS_OUTPUT Out = (VS_OUTPUT)0;		// 出力データ

	//----- 座標変換
	float4x4 World = SkinWorldCreator(W);
	Out.Pos = PosCreator(Pos, World);

	float4 position;
	position = mul(Pos, World);
	Normal = mul(Normal, (float3x3)World);

	//----- テクスチャ
	Out.Tex = Tex;

	//----- 法線
	Out.Normal = normalize(Normal);

	//----- テクスチャのY座標の算出(これをエッジ座標として使用)
	float3 wv_pos = normalize(mul(position, matCameraView));
	float3 wv_normal = normalize(mul(Normal, matCameraView));

	// 2つのベクトルの内積を0.0～から1.0の値にしてV座標にする
	Out.ToonUV.y = dot(wv_pos, wv_normal) * 0.5 + 0.5;


	//float4x4 WLP = CreateLightWVP(World);
	//float4x4 WLPB = WLP * matScaleBias;

	//// シャドウマップ
	//Out.ShadowMapUV = mul(Pos, WLPB);
	//Out.Depth = mul(Pos, WLP);

	return Out;
}

// -------------------------------------------------------------
float4 PS_pass0 ( VS_OUTPUT	In )	:	COLOR
{
	// トゥーンテクスチャ座標
	//----- 頂点色
	float3 L = -vLightDir.xyz;
	float3 N = normalize(In.Normal);
	float amb = vLightDir.w;

	//----- テクスチャのX座標の算出
	// ライトの計算	（ハーフランバート版
	float4 p = dot(N, L);
	p = p * 0.5f + 0.5f;
	p *= p;

	// ライトを利用しテクスチャ座標を計算
	float4 Col = tex2D(ToonSmp, float2(p.x, In.ToonUV.y));


	//float  shadow = tex2Dproj(ShadowMapSamp, In.ShadowMapUV).x;
//	Col = Col + ((shadow * In.Depth.w < In.Depth.z - fBias) ? 0 : Col * 0.3f);

	return tex2D(TexSamp, In.Tex) * Col * vColor;
}

float4 PS_pass1(VS_OUTPUT In)	:	COLOR
{
	// トゥーンテクスチャ座標
	//----- 頂点色
	float3 L = -vLightDir.xyz;
	float3 N = normalize(In.Normal);
	float amb = vLightDir.w;

	//----- テクスチャのX座標の算出
	// ライトの計算	（ハーフランバート版
	float4 p = dot(N, L);
	p = p * 0.5f + 0.5f;
	p *= p;

	// ライトを利用しテクスチャ座標を計算
	float4 Col = tex2D(ToonSmp, float2(p.x, In.ToonUV.y));


	//float  shadow = tex2Dproj(ShadowMapSamp, In.ShadowMapUV).x;
//	Col = Col + ((shadow * In.Depth.w < In.Depth.z - fBias) ? 0 : Col * 0.3f);

	return Col * vColor;
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
