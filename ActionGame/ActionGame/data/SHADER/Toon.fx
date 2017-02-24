// トゥーンシェーダ（ペイント＋エッジ
// エッジの色を変えたい場合はテクスチャの真ん中の
// 線の色を変えましょう
// エッジを太くしたい場合テクスチャの真ん中の線の領域を
// 大きくしましょう

// -------------------------------------------------------------
// グローバル変数
// -------------------------------------------------------------

float4x4 matWorld[4]	:	WORLD;		// ワールド変換行列配列
float4x4 matView		:	VIEW;		// ビュー変換行列
float4x4 matProj		:	PROJECTION;	// 射影変換行列

int iBlendNum;							// ブレンドする配列の数

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
};


// -------------------------------------------------------------
// シーンの描画
// -------------------------------------------------------------
VS_OUTPUT VS (
		float3	Pos		:	POSITION,	// ローカル位置座標
		float3	Normal	:	NORMAL,		// 法線ベクトル
		float2	Tex		:	TEXCOORD0	// テクスチャ
) {
	VS_OUTPUT Out = (VS_OUTPUT)0;		// 出力データ

	// 座標変換
	float4 position;
	Out.Pos = mul( float4(Pos, 1.0f), matWorld[0] );
	position = Out.Pos;
	Normal = mul( Normal, (float3x3)matWorld[0] );
	Out.Pos = mul( Out.Pos, matView );
	Out.Pos = mul( Out.Pos, matProj );

	// テクスチャ
	Out.Tex = Tex;

	// 法線
	Out.Normal = normalize(Normal);

	//----- テクスチャのY座標の算出(これをエッジ座標として使用)
	// 頂点座標をワールドビュー変換する
	float3 wv_pos = normalize(mul(position, matView));

	// 法線ベクトルをワールドビュー変換する
	float3 wv_normal = normalize(mul(Normal, matView));

	// 2つのベクトルの内積を0.0～から1.0の値にしてV座標にする
	Out.ToonUV.y = dot(wv_pos, wv_normal) * 0.5 + 0.5;

	return Out;
}

// スキンメッシュバージョン
VS_OUTPUT VS_SKIN (
		float3 Pos		: POSITION,			// ローカル位置座標
		float4 W		: BLENDWEIGHT,		// 重み
		float3 Normal	: NORMAL,			// 法線ベクトル
		float2 Tex		: TEXCOORD0			// テクスチャ
) {
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

	float4 position;
	Out.Pos = mul( float4(Pos, 1.0f), matCombWorld );	// ワールド変換
	position = Out.Pos;
	Normal = mul( Normal, (float3x3)matCombWorld );
	Out.Pos = mul( Out.Pos, matView );	// ビュー変換
	Out.Pos = mul( Out.Pos, matProj );	// 射影変換


	//----- テクスチャ
	Out.Tex = Tex;

	//----- 法線
	Out.Normal = normalize(Normal);

	//----- テクスチャのY座標の算出(これをエッジ座標として使用)
	// 頂点座標をワールドビュー変換する
	float3 wv_pos = normalize(mul(position, matView));

	// 法線ベクトルをワールドビュー変換する
	float3 wv_normal = normalize(mul(Normal, matView));

	// 2つのベクトルの内積を0.0～から1.0の値にしてV座標にする
	Out.ToonUV.y = dot(wv_pos, wv_normal) * 0.5 + 0.5;

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
