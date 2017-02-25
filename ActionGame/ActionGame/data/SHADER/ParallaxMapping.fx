// 視差マッピングシェーダ

#include	"PosCreator.fh"

// -------------------------------------------------------------
// グローバル変数
// -------------------------------------------------------------
float4	vLightDir;					// ライトの方向
float4	vColor;						// ライト＊メッシュの色
float3	vEyePos;					// カメラの位置（ローカル座標系）
float	Specular = 0.0f;			// スペキュラーの範囲
float	SpecularPower = 0.0f;		// スペキュラーの強さ(0にするとランバートになる)
float	fHLimit = 0.016;			// 高さの最大値（大きくするほど高くなる

texture tex;
sampler TexSamp = sampler_state {
	texture = <tex>;
	MinFilter = LINEAR;
	MagFilter = LINEAR;
	MipFilter = NONE;

	AddressU = Wrap;
	AddressV = Wrap;
};

texture Bumptex;
sampler BumpSamp = sampler_state {
	texture = <Bumptex>;
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
	float3	Light		: TEXCOORD1;
	float3	Eye			: TEXCOORD2;
};
// -------------------------------------------------------------
// シーンの描画
// -------------------------------------------------------------
VS_OUTPUT VS(
		float4 Pos		: POSITION,			// ローカル位置座標
		float3 Tangent	: TANGENT0,			// 接戦ベクトル
		float3 Binormal	: BINORMAL0,		// 従法線ベクトル
		float3 Normal	: NORMAL,			// 法線ベクトル
		float2 Tex		: TEXCOORD0			// テクスチャ
){
	VS_OUTPUT Out = (VS_OUTPUT)0;			// 出力データ
	
	// 座標変換
	Out.Pos = PosCreator(Pos, matWorld[0]);

	float4 position;
	position = mul(Pos, matWorld[0]);

	// テクスチャ
	Out.Tex = Tex;
	
	// 視線ベクトル
	float3 Eye = normalize(vEyePos - position.xyz);
	Out.Eye.x = dot(Eye, Tangent);
	Out.Eye.y = dot(Eye, Binormal);
	Out.Eye.z = dot(Eye, Normal);
	Out.Eye = normalize(Out.Eye);

	// ライトベクトル
	float3 Light = -vLightDir.xyz;
	Out.Light.x = dot(Light, Tangent);
	Out.Light.y = dot(Light, Binormal);
	Out.Light.z = dot(Light, Normal);
	Out.Light = normalize(Out.Light);

	return Out;
}
// スキンメッシュバージョン
VS_OUTPUT VS_SKIN(
		float4 Pos		: POSITION,			// ローカル位置座標
		float4 W		: BLENDWEIGHT,		// 重み
		float3 Tangent	: TANGENT0,			// 接戦ベクトル
		float3 Binormal	: BINORMAL0,		// 従法線ベクトル
		float3 Normal	: NORMAL,			// 法線ベクトル
		float2 Tex		: TEXCOORD0			// テクスチャ
){
	VS_OUTPUT Out = (VS_OUTPUT)0;		// 出力データ

	//----- 座標変換
	float4x4 matWorld = SkinWorldCreator(W);
	Out.Pos = PosCreator(Pos, matWorld);

	float4 position;
	position = mul(Pos, matWorld);

	//----- テクスチャ
	Out.Tex = Tex;

	// 視線ベクトル
	float3 Eye = normalize(vEyePos - position.xyz);
	Out.Eye.x = dot(Eye, Tangent);
	Out.Eye.y = dot(Eye, Binormal);
	Out.Eye.z = dot(Eye, Normal);
	Out.Eye = normalize(Out.Eye);

	// ライトベクトル
	float3 Light = -vLightDir.xyz;
	Out.Light.x = dot(Light, Tangent);
	Out.Light.y = dot(Light, Binormal);
	Out.Light.z = dot(Light, Normal);
	Out.Light = normalize(Out.Light);

	return Out;
}

// -------------------------------------------------------------

float4 PS_pass0(VS_OUTPUT In)	:	COLOR
{
	// 高さマップを参照し、高さを取得する
	// 高さ情報はα成分に格納されている
	float h = tex2D( BumpSamp, In.Tex ).a;

	// テクセルを頂点座標系での視線ベクトル方向に重みを付けてずらす。
	float2 Tex = In.Tex + fHLimit * h * In.Eye.xy;

	// 法線マップを参照し、法線を取得する
	// 法線マップは-1.0f ～ 1.0fの値を0.0f ～ 1.0fの範囲に保存してあるので変換する
	float3 Normal = 2.0f * tex2D( BumpSamp, Tex ).xyz - 1.0f;

	// フォンシェーディング
	float3 H = normalize( In.Light + In.Eye );

	// スペキュラカラーを計算
	float S = pow( max( 0.0f, dot(Normal, H) ), Specular ) * SpecularPower;

	// マテリアル色を決定する
	return tex2D( TexSamp, Tex ) * max(vLightDir.w, dot( Normal, In.Light ) ) + S;
}

float4 PS_pass1(VS_OUTPUT In)	:	COLOR
{
	// 高さマップを参照し、高さを取得する
	// 高さ情報はα成分に格納されている
	float h = tex2D( BumpSamp, In.Tex ).a;

	// テクセルを頂点座標系での視線ベクトル方向に重みを付けてずらす。
	float2 Tex = In.Tex + fHLimit * h * In.Eye.xy;

	// 法線マップを参照し、法線を取得する
	// 法線マップは-1.0f ～ 1.0fの値を0.0f ～ 1.0fの範囲に保存してあるので変換する
	float3 Normal = 2.0f * tex2D( BumpSamp, Tex ).xyz - 1.0f;

	// フォンシェーディング
	float3 H = normalize( In.Light + In.Eye );

	// スペキュラカラーを計算
	float S = pow( max( 0.0f, dot(Normal, H) ), Specular ) * SpecularPower;

	// マテリアル色を決定する
	return max(vLightDir.w, dot( Normal, In.Light ) ) + S;
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
