// �g�D�[���V�F�[�_�i�y�C���g�{�G�b�W
// �G�b�W�̐F��ς������ꍇ�̓e�N�X�`���̐^�񒆂�
// ���̐F��ς��܂��傤
// �G�b�W�𑾂��������ꍇ�e�N�X�`���̐^�񒆂̐��̗̈��
// �傫�����܂��傤

#include	"PosCreator.fh"
#include	"Shadow.fh"

// -------------------------------------------------------------
// �O���[�o���ϐ�
// -------------------------------------------------------------
float4 vLightDir;	// ���C�g�̕���
float4 vColor;		// ���C�g�����b�V���̐F

// ���b�V��
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

// �g�D�[��
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
// ���_�V�F�[�_����s�N�Z���V�F�[�_�ɓn���f�[�^
// -------------------------------------------------------------
struct VS_OUTPUT {
	float4		Pos		:	POSITION;	// ���W
	float2		Tex		:	TEXCOORD0;	// �e�N�X�`��
	float3		Normal	:	TEXCOORD1;	// �@��
	float2		ToonUV	:	TEXCOORD2;	// �g�D�[���̃G�b�W

	float4	ShadowMapUV	: TEXCOORD3;
	float4	Depth		: TEXCOORD4;
};


// -------------------------------------------------------------
// �V�[���̕`��
// -------------------------------------------------------------
VS_OUTPUT VS (
		float4	Pos		:	POSITION,	// ���[�J���ʒu���W
		float3	Normal	:	NORMAL,		// �@���x�N�g��
		float2	Tex		:	TEXCOORD0	// �e�N�X�`��
) {
	VS_OUTPUT Out = (VS_OUTPUT)0;		// �o�̓f�[�^

	// ���W�ϊ�
	float4x4 World = matWorld[0];
	Out.Pos = PosCreator(Pos, World);

	float4 position;
	position = mul(Pos, World);
	Normal = mul(Normal, (float3x3)World);

	// �e�N�X�`��
	Out.Tex = Tex;

	// �@��
	Out.Normal = normalize(Normal);

	//----- �e�N�X�`����Y���W�̎Z�o(������G�b�W���W�Ƃ��Ďg�p)
	float3 wv_pos = normalize(mul(position, matCameraView));
	float3 wv_normal = normalize(mul(Normal, matCameraView));

	// 2�̃x�N�g���̓��ς�0.0�`����1.0�̒l�ɂ���V���W�ɂ���
	Out.ToonUV.y = dot(wv_pos, wv_normal) * 0.5 + 0.5;


	//float4x4 WLP = CreateLightWVP(World);
	//float4x4 WLPB = WLP * matScaleBias;

	//// �V���h�E�}�b�v
	//Out.ShadowMapUV = mul(Pos, WLPB);
	//Out.Depth = mul(Pos, WLP);

	return Out;
}

// �X�L�����b�V���o�[�W����
VS_OUTPUT VS_SKIN (
		float4 Pos		: POSITION,			// ���[�J���ʒu���W
		float3 Normal	: NORMAL,			// �@���x�N�g��
		float2 Tex		: TEXCOORD0,		// �e�N�X�`��
		float4 W		: BLENDWEIGHT		// �d��
) {
	VS_OUTPUT Out = (VS_OUTPUT)0;		// �o�̓f�[�^

	//----- ���W�ϊ�
	float4x4 World = SkinWorldCreator(W);
	Out.Pos = PosCreator(Pos, World);

	float4 position;
	position = mul(Pos, World);
	Normal = mul(Normal, (float3x3)World);

	//----- �e�N�X�`��
	Out.Tex = Tex;

	//----- �@��
	Out.Normal = normalize(Normal);

	//----- �e�N�X�`����Y���W�̎Z�o(������G�b�W���W�Ƃ��Ďg�p)
	float3 wv_pos = normalize(mul(position, matCameraView));
	float3 wv_normal = normalize(mul(Normal, matCameraView));

	// 2�̃x�N�g���̓��ς�0.0�`����1.0�̒l�ɂ���V���W�ɂ���
	Out.ToonUV.y = dot(wv_pos, wv_normal) * 0.5 + 0.5;


	//float4x4 WLP = CreateLightWVP(World);
	//float4x4 WLPB = WLP * matScaleBias;

	//// �V���h�E�}�b�v
	//Out.ShadowMapUV = mul(Pos, WLPB);
	//Out.Depth = mul(Pos, WLP);

	return Out;
}

// -------------------------------------------------------------
float4 PS_pass0 ( VS_OUTPUT	In )	:	COLOR
{
	// �g�D�[���e�N�X�`�����W
	//----- ���_�F
	float3 L = -vLightDir.xyz;
	float3 N = normalize(In.Normal);
	float amb = vLightDir.w;

	//----- �e�N�X�`����X���W�̎Z�o
	// ���C�g�̌v�Z	�i�n�[�t�����o�[�g��
	float4 p = dot(N, L);
	p = p * 0.5f + 0.5f;
	p *= p;

	// ���C�g�𗘗p���e�N�X�`�����W���v�Z
	float4 Col = tex2D(ToonSmp, float2(p.x, In.ToonUV.y));


	//float  shadow = tex2Dproj(ShadowMapSamp, In.ShadowMapUV).x;
//	Col = Col + ((shadow * In.Depth.w < In.Depth.z - fBias) ? 0 : Col * 0.3f);

	return tex2D(TexSamp, In.Tex) * Col * vColor;
}

float4 PS_pass1(VS_OUTPUT In)	:	COLOR
{
	// �g�D�[���e�N�X�`�����W
	//----- ���_�F
	float3 L = -vLightDir.xyz;
	float3 N = normalize(In.Normal);
	float amb = vLightDir.w;

	//----- �e�N�X�`����X���W�̎Z�o
	// ���C�g�̌v�Z	�i�n�[�t�����o�[�g��
	float4 p = dot(N, L);
	p = p * 0.5f + 0.5f;
	p *= p;

	// ���C�g�𗘗p���e�N�X�`�����W���v�Z
	float4 Col = tex2D(ToonSmp, float2(p.x, In.ToonUV.y));


	//float  shadow = tex2Dproj(ShadowMapSamp, In.ShadowMapUV).x;
//	Col = Col + ((shadow * In.Depth.w < In.Depth.z - fBias) ? 0 : Col * 0.3f);

	return Col * vColor;
}

// -------------------------------------------------------------
// �e�N�j�b�N
// -------------------------------------------------------------
technique TShader
{
	pass P0	// �e�N�X�`������
	{
		VertexShader = compile vs_1_1 VS();
		PixelShader  = compile ps_2_0 PS_pass0();
	}

	pass P1	// �e�N�X�`���Ȃ�
	{
		VertexShader = compile vs_1_1 VS();
		PixelShader  = compile ps_2_0 PS_pass1();
	}

	// �X�L�����b�V��
	pass P2	// �e�N�X�`������
	{
		VertexShader = compile vs_1_1 VS_SKIN();
		PixelShader  = compile ps_2_0 PS_pass0();
	}

	pass P3	// �e�N�X�`���Ȃ�
	{
		VertexShader = compile vs_1_1 VS_SKIN();
		PixelShader  = compile ps_2_0 PS_pass1();
	}
}
