// -------------------------------------------------------------
// �N���X�t�B���^
// 
// Copyright (c) 2003 IMAGIRE Takashi. All rights reserved.
// -------------------------------------------------------------

// -------------------------------------------------------------
// �O���[�o���ϐ�
// -------------------------------------------------------------


static const int    MAX_SAMPLES = 16;    // �ő�T���v�����O��
float2 g_avSampleOffsets[MAX_SAMPLES];	// �T���v�����O�̈ʒu
float4 g_avSampleWeights[MAX_SAMPLES];	// �T���v�����O�̏d��

float BRIGHT_PASS_THERSHOLD = 1.0f;		// ������̒��o�l�i�Ⴍ����Ə����̋����ł����o


//-----------------------------------------------------------------------------
// �T���v���� SetTexture �Ƃ̑Ή�
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
// Desc: 1/4�̏k���o�b�t�@�փV�[�����R�s�[
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
// Desc: �P�x�̍������������𒊏o����
//-----------------------------------------------------------------------------
float4 BrightPassFilter(in float2 uv : TEXCOORD0) : COLOR
{
	float4 vSample = tex2D( s0, uv );
	
	// �Â������������҂�
	// �����̐F�̋����ł���䊂𑖂点�����Ƃ��́A���������l������������
	vSample.rgb -= BRIGHT_PASS_THERSHOLD;
	
	// �������O�ɂ���
	// ��䊂̌`���͂����肳�������Ƃ��ɂ́A3.0f��傫������
	vSample = 3.0f*max(vSample, 0.0f);

	return vSample;
}




//-----------------------------------------------------------------------------
// Name: GaussBlur5x5
// Desc: ���S�ɋ߂�13�̃e�N�Z�����T���v�����O����
//      �i�W���ɃK�E�X���z�������Ă���̂ŁA���ʓI�ɃK�E�X���ςɂȂ�j
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
// Desc: �W�T���v�����O���āA��䊂̐������X�ɍ쐬����
//-----------------------------------------------------------------------------
float4 Star ( in float2 uv : TEXCOORD0 ) : COLOR
{
	float4 vColor = 0.0f;

	// ��䊂ɂ������W�̓_���T���v�����O����
	for(int i = 0; i < 8; i++) {
			vColor += g_avSampleWeights[i]
						 * tex2D(s0, uv + g_avSampleOffsets[i]);
	}

	return vColor;
}




//-----------------------------------------------------------------------------
// Name: MergeTextures_6
// Desc: �U�{�̌�䊂��d�ˍ��킹��
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
// �e�N�j�b�N
// -------------------------------------------------------------



//-----------------------------------------------------------------------------
// Name: DownScale4x4
// Type: Technique                                     
// Desc: 1/4�̏k���o�b�t�@�փV�[�����R�s�[
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
// Desc: �P�x�̍��������𒊏o����
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
// Desc: �P�R�e�N�Z���̃T���v�����O�����āA�K�E�X�ڂ���������
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
// Desc: �W�T���v�����O���āA��䊂̐������X�ɍ쐬����
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
// Desc: �U�{�̌�䊂��d�ˍ��킹��
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
