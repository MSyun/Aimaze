// ランバート
// 

#include	"PosCreator.fh"

// -------------------------------------------------------------
// グローバル変数
// -------------------------------------------------------------
float4 vLightDir;	// ライトの方向
float4 vColor;		// ライト＊メッシュの色


texture tex;
sampler TexSamp = sampler_state {
	texture = <tex>;
	MinFilter = LINEAR;
	MagFilter = LINEAR;
	MipFilter = NONE;

	AddressU = Wrap;
	AddressV = Wrap;
};


// -------------------------------------------------------------
// 頂点シェーダからピクセルシェーダに渡すデータ
// -------------------------------------------------------------
struct VS_OUTPUT {
	float4	Pos			: POSITION;
	float4	Color		: COLOR0;
	float2	Tex			: TEXCOORD0;
	float3	Normal		: TEXCOORD1;
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
	Out.Pos = PosCreator(Pos, matWorld[0]);

	// テクスチャ
	Out.Tex = Tex;

	// 法線
	Out.Normal = mul(Normal, (float3x3)matWorld[0]);;

	return Out;
}

// スキンメッシュバージョン
VS_OUTPUT VS_SKIN(
		float4 Pos		: POSITION,			// ローカル位置座標
		float4 W		: BLENDWEIGHT,		// 重み
		float3 Normal	: NORMAL,			// 法線ベクトル
		float2 Tex		: TEXCOORD0			// テクスチャ
){
	VS_OUTPUT Out = (VS_OUTPUT)0;		// 出力データ

	// 座標変換
	float4x4 matWorld = SkinWorldCreator(W);
	Out.Pos = PosCreator(Pos, matWorld);

	// テクスチャ
	Out.Tex = Tex;

	// 法線
	Out.Normal = mul(Normal, (float3x3)matWorld);
	
	return Out;
}

// -------------------------------------------------------------
float4 PS_pass0(VS_OUTPUT In) : COLOR
{
	// 頂点色
	float3 L = -vLightDir.xyz;
	float amb = vLightDir.w;
	float3 N = normalize(In.Normal);

	In.Color = vColor * max(amb, dot(N, L));
	In.Color.w = 1.0f;


	return In.Color * tex2D( TexSamp, In.Tex );
}

float4 PS_pass1(VS_OUTPUT In) : COLOR
{
	// 頂点色
	float3 L = -vLightDir.xyz;
	float amb = vLightDir.w;
	float3 N = normalize(In.Normal);

	In.Color = vColor * max(amb, dot(N, L));
	In.Color.w = 1.0f;

	return In.Color;
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
