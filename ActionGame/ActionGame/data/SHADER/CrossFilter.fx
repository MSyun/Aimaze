// -------------------------------------------------------------
// クロスフィルタ
// 
// Copyright (c) 2003 IMAGIRE Takashi. All rights reserved.
// -------------------------------------------------------------

// -------------------------------------------------------------
// グローバル変数
// -------------------------------------------------------------


static const int    MAX_SAMPLES = 16;    // 最大サンプリング数
float2 g_avSampleOffsets[MAX_SAMPLES];	// サンプリングの位置
float4 g_avSampleWeights[MAX_SAMPLES];	// サンプリングの重み

float BRIGHT_PASS_THERSHOLD = 1.0f;		// あかりの抽出値（低くすると少しの強さでも抽出


//-----------------------------------------------------------------------------
// サンプラと SetTexture との対応
//-----------------------------------------------------------------------------
sampler s0 : register(s0);
sampler s1 : register(s1);
sampler s2 : register(s2);
sampler s3 : register(s3);
sampler s4 : register(s4);
sampler s5 : register(s5);
sampler s6 : register(s6);
sampler s7 : register(s7);




//-----------------------------------------------------------------------------
// Name: DownScale4x4
// Desc: 1/4の縮小バッファへシーンをコピー
//-----------------------------------------------------------------------------
float4 DownScale4x4 ( in float2 uv : TEXCOORD0 ) : COLOR
{
	float4 sample = 0.0f;

	for( int i=0; i < 16; i++ ) {
		sample += tex2D( s0, uv + g_avSampleOffsets[i] );
	}

	return sample / 16;
}



//-----------------------------------------------------------------------------
// Name: BrightPassFilter
// Desc: 輝度の高い部分だけを抽出する
//-----------------------------------------------------------------------------
float4 BrightPassFilter(in float2 uv : TEXCOORD0) : COLOR
{
	float4 vSample = tex2D( s0, uv );
	
	// 暗い部分をさっぴく
	// 少しの色の強さでも光芒を走らせたいときは、差し引く値を小さくする
	vSample.rgb -= BRIGHT_PASS_THERSHOLD;
	
	// 下限を０にする
	// 光芒の形をはっきりさせたいときには、3.0fを大きくする
	vSample = 3.0f*max(vSample, 0.0f);

	return vSample;
}




//-----------------------------------------------------------------------------
// Name: GaussBlur5x5
// Desc: 中心に近い13個のテクセルをサンプリングする
//      （係数にガウス分布が入っているので、結果的にガウス平均になる）
//-----------------------------------------------------------------------------
float4 GaussBlur5x5 (in float2 uv : TEXCOORD0) : COLOR
{
	float4 sample = 0.0f;

	for( int i=0; i < 13; i++ ) {
		sample += g_avSampleWeights[i]
					 * tex2D( s0, uv + g_avSampleOffsets[i] );
	}

	return sample;
}




//-----------------------------------------------------------------------------
// Name: Star
// Desc: ８サンプリングして、光芒の線を徐々に作成する
//-----------------------------------------------------------------------------
float4 Star ( in float2 uv : TEXCOORD0 ) : COLOR
{
	float4 vColor = 0.0f;

	// 光芒にそった８つの点をサンプリングする
	for(int i = 0; i < 8; i++) {
			vColor += g_avSampleWeights[i]
						 * tex2D(s0, uv + g_avSampleOffsets[i]);
	}

	return vColor;
}




//-----------------------------------------------------------------------------
// Name: MergeTextures_6
// Desc: ６本の光芒を重ね合わせる
//-----------------------------------------------------------------------------
float4 MergeTextures_6(in float2 uv : TEXCOORD0 ) : COLOR
{
	float4 vColor = 0.0f;

	vColor = ( tex2D(s0, uv)
			 + tex2D(s1, uv)
			 + tex2D(s2, uv)
			 + tex2D(s3, uv)
			 + tex2D(s4, uv)
			 + tex2D(s5, uv) )/6.0f;

	return vColor;
}





// -------------------------------------------------------------
// テクニック
// -------------------------------------------------------------



//-----------------------------------------------------------------------------
// Name: DownScale4x4
// Type: Technique                                     
// Desc: 1/4の縮小バッファへシーンをコピー
//-----------------------------------------------------------------------------
technique tDownScale4x4
{
	pass P0
	{
		PixelShader  = compile ps_2_0 DownScale4x4();
		MinFilter[0] = Point;
		AddressU[0] = Clamp;
		AddressV[0] = Clamp;
	}
}




//-----------------------------------------------------------------------------
// Name: BrightPassFilter
// Desc: 輝度の高い部分を抽出する
//-----------------------------------------------------------------------------
technique tBrightPassFilter
{
	pass P0
	{
		PixelShader  = compile ps_2_0 BrightPassFilter();
		MinFilter[0] = Point;
		MagFilter[0] = Point;
	}
}





//-----------------------------------------------------------------------------
// Name: GaussBlur5x5
// Desc: １３テクセルのサンプリングをして、ガウスぼかしをする
//-----------------------------------------------------------------------------
technique tGaussBlur5x5
{
	pass P0
	{
		PixelShader  = compile ps_2_0 GaussBlur5x5();
		MinFilter[0] = Point;
		AddressU[0] = Clamp;
		AddressV[0] = Clamp;
	}
}




//-----------------------------------------------------------------------------
// Name: Star
// Desc: ８サンプリングして、光芒の線を徐々に作成する
//-----------------------------------------------------------------------------
technique tStar
{
	pass P0
	{
		PixelShader  = compile ps_2_0 Star();
		MagFilter[0] = Linear;
		MinFilter[0] = Linear;
	}
}




//-----------------------------------------------------------------------------
// Name: MergeTextures_N
// Desc: ６本の光芒を重ね合わせる
//-----------------------------------------------------------------------------
technique tMergeTextures
{
	pass P0
	{
		PixelShader  = compile ps_2_0 MergeTextures_6();
		MagFilter[0] = Point;
		MinFilter[0] = Point;
		MagFilter[1] = Point;
		MinFilter[1] = Point;
		MagFilter[2] = Point;
		MinFilter[2] = Point;
		MagFilter[3] = Point;
		MinFilter[3] = Point;
		MagFilter[4] = Point;
		MinFilter[4] = Point;
		MagFilter[5] = Point;
		MinFilter[5] = Point;
	}
}
