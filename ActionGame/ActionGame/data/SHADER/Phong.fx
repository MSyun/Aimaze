// 鏡面反射光
// 
// スキンメッシュ部分にバグあり

#include	"PosCreator.fh"

// -------------------------------------------------------------
// グローバル変数
// -------------------------------------------------------------
float4	vLightDir;					// ライトの方向
float4	vColor;						// ライト＊メッシュの色
float3	vEyePos;					// カメラの位置（ローカル座標系）
float	Specular = 100.f;			// スペキュラーの範囲
float	SpecularPower = 1.0f;		// スペキュラーの強さ(0にするとランバートになる)
float	LightBloomPower = 1.0f;		// ライトブルームの強さ

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
	float3	N			: TEXCOORD1;
	float3	Eye			: TEXCOORD2;
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

//	float4 position;
//	position = mul( float4(Pos, 1.0f), matWorld[0] );
//	Normal = mul( Normal, (float3x3)matWorld[0] );

	// テクスチャ
	Out.Tex = Tex;
	
	// 頂点色
	float3 L = -vLightDir.xyz;
	float amb = vLightDir.w;
	Out.Color = vColor * max(amb, dot(L, Normal));
//	Out.Color.w = 1.0f;

	// 鏡面反射用のベクトル
	Out.N	= Normal.xyz;
//	Out.Eye	= vEyePos - position.xyz;
	Out.Eye	= vEyePos - Pos;

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
	float4x4 matWorld = SkinWorldCreator(W);
	Out.Pos = PosCreator(Pos, matWorld);

	float4 position;
	position = mul(Pos, matWorld);
	Normal = mul(Normal, (float3x3)matWorld);

	//----- テクスチャ
	Out.Tex = Tex;

	//----- 頂点色
	float3 L = -vLightDir.xyz;
	float amb = vLightDir.w;
	Out.Color = vColor * max(amb, dot(L, Normal));
//	Out.Color.w = 1.0f;

	// 鏡面反射用のベクトル
	Out.N	= Normal.xyz;
	Out.Eye	= vEyePos - position.xyz;
	
	return Out;
}

// -------------------------------------------------------------
struct PS_OUTPUT {
	float4 Col		:	COLOR0;
	float4 Luminous	:	COLOR1;
};

PS_OUTPUT PS_pass0(VS_OUTPUT In)
{
	PS_OUTPUT Out = (PS_OUTPUT)0;

	// ライトベクトル
	float3 L = -vLightDir.xyz;
	// ハーフベクトル
	float3 H = normalize(L + normalize(In.Eye));
	// 法線を正規化（VSからPSの間で値が変わるため）
	float3 N = normalize(In.N);
	// 鏡面反射光
	float S = pow(max(0,dot(N,H)), Specular) * SpecularPower;

	Out.Col = In.Color * tex2D( TexSamp, In.Tex )	// 拡散光＋環境光
			+ S;

	// 輝度情報をセット
	float p = S * LightBloomPower;
	Out.Luminous = float4(p,p,p,1.0f);

	return Out;
}

PS_OUTPUT PS_pass1(VS_OUTPUT In)
{
	PS_OUTPUT Out = (PS_OUTPUT)0;

	float3 L = -vLightDir.xyz;
	float3 H = normalize(L + normalize(In.Eye));	// ハーフベクトル
	float3 N = normalize(In.N);
	float S = pow(max(0,dot(N,H)), Specular) * SpecularPower;

	Out.Col = In.Color								// 拡散光＋環境光
			+ S;

	// 輝度情報をセット
	float p = S * LightBloomPower;
	Out.Luminous = float4(p,p,p,1.0f);

	return Out;
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
