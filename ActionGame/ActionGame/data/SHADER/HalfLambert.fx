// ハーフランバート
// 

#include	"PosCreator.fh"
#include	"Shadow.fh"


// -------------------------------------------------------------
// グローバル変数
// -------------------------------------------------------------
float4 vLightDir;	// ライトの方向
float4 vColor;		// ライト＊メッシュの色

// テクスチャ
texture tex;
sampler TexSamp = sampler_state {
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
struct VS_OUTPUT {
	float4	Pos			: POSITION;
	float2	Tex			: TEXCOORD0;
	float3	Normal		: TEXCOORD1;

	float4	ShadowMapUV	: TEXCOORD2;
	float4	Depth		: TEXCOORD3;
};
// -------------------------------------------------------------
// シーンの描画
// -------------------------------------------------------------
VS_OUTPUT VS(
		float4 Pos		: POSITION,			// ローカル位置座標
		float3 Normal	: NORMAL,			// 法線ベクトル
		float2 Tex		: TEXCOORD0			// テクスチャ
){
	VS_OUTPUT Out = (VS_OUTPUT)0;		// 出力データ
	
	// 座標変換
	float4x4 World = matWorld[0];
	Out.Pos = PosCreator(Pos, World);

	// テクスチャ
	Out.Tex = Tex;

	// 法線
	Out.Normal = mul(Normal, (float3x3)World);


	float4x4 WLP = CreateLightWVP(World);
	float4x4 WLPB = WLP * matScaleBias;

	// シャドウマップ
	Out.ShadowMapUV = mul(Pos, WLPB);
	Out.Depth = mul(Pos, WLP);

	return Out;
}

// スキンメッシュバージョン
VS_OUTPUT VS_SKIN(
		float4 Pos		: POSITION,			// ローカル位置座標
		float3 Normal	: NORMAL,			// 法線ベクトル
		float2 Tex		: TEXCOORD0,		// テクスチャ
		float4 W		: BLENDWEIGHT		// 重み
){
	VS_OUTPUT Out = (VS_OUTPUT)0;		// 出力データ

	//----- 座標変換
	float4x4 World = SkinWorldCreator(W);
	Out.Pos = PosCreator(Pos, World);

	//----- テクスチャ
	Out.Tex = Tex;

	// 法線
	Out.Normal = mul(Normal, (float3x3)World);


	float4x4 WLP = CreateLightWVP(World);
	float4x4 WLPB = WLP * matScaleBias;

	// シャドウマップ
	Out.ShadowMapUV = mul(Pos, WLPB);
	Out.Depth = mul(Pos, WLP);

	return Out;
}

// -------------------------------------------------------------
float4 PS_pass0(VS_OUTPUT In) : COLOR
{
	// 頂点色
	float3 L = -vLightDir.xyz;
	float3 N = normalize(In.Normal);

	float3 p = dot(N, L);
	p = (p + 1.0f) * 0.5f;
	p *= p;
	// テクスチャがある場合マテリアルカラーはいらないかも
	float4 Col;
	Col.xyz = vColor.xyz * p;

	Col.w = 1.0f;


	float  shadow = tex2Dproj(ShadowMapSamp, In.ShadowMapUV).x;
//	return (float4)shadow;
//	Col = Col + ((shadow * In.Depth.w < In.Depth.z - fBias) ? 0 : Col * 0.3f);

	return Col * tex2D(TexSamp, In.Tex);
	//return (float4)In.Depth.z;
//	return (float4)(In.Depth.z / In.Depth.w);
//	return In.ShadowMapUV;
	return In.Depth;
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

	float  shadow = tex2Dproj(ShadowMapSamp, In.ShadowMapUV).x;
//	return (float4)shadow;
//	Col = Col + ((shadow * In.Depth.w < In.Depth.z - fBias) ? 0 : Col * 0.3f);

	return Col;
	//return (float4)In.Depth.z;
//	return (float4)(In.Depth.z / In.Depth.w);
//	return In.ShadowMapUV;
	return In.Depth;
}

// -------------------------------------------------------------
// テクニック
// -------------------------------------------------------------
technique TShader
{
	pass P0	// テクスチャあり
	{
		VertexShader = compile vs_1_1 VS();
		PixelShader = compile ps_2_0 PS_pass0();
	}

	pass P1	// テクスチャなし
	{
		VertexShader = compile vs_1_1 VS();
		PixelShader = compile ps_2_0 PS_pass1();
	}

	// スキンメッシュ
	pass P2	// テクスチャあり
	{
		VertexShader = compile vs_1_1 VS_SKIN();
		PixelShader = compile ps_2_0 PS_pass0();
	}

	pass P3	// テクスチャなし
	{
		VertexShader = compile vs_1_1 VS_SKIN();
		PixelShader = compile ps_2_0 PS_pass1();
	}
}
