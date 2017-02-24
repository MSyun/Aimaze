// �����}�b�s���O�V�F�[�_

// -------------------------------------------------------------
// �O���[�o���ϐ�
// -------------------------------------------------------------

float4x4 matWorld[4]	:	WORLD;		// ���[���h�ϊ��s��z��
float4x4 matView		:	VIEW;		// �r���[�ϊ��s��
float4x4 matProj		:	PROJECTION;	// �ˉe�ϊ��s��

int iBlendNum;							// �u�����h����z��̐�

float4	vLightDir;					// ���C�g�̕���
float4	vColor;						// ���C�g�����b�V���̐F
float3	vEyePos;					// �J�����̈ʒu�i���[�J�����W�n�j
float	Specular = 0.0f;			// �X�y�L�����[�͈̔�
float	SpecularPower = 0.0f;		// �X�y�L�����[�̋���(0�ɂ���ƃ����o�[�g�ɂȂ�)
float	fHLimit = 0.016;			// �����̍ő�l�i�傫������قǍ����Ȃ�

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

texture Bumptex;
sampler BumpSamp = sampler_state
{
	texture = <Bumptex>;
	MinFilter = LINEAR;
	MagFilter = LINEAR;
	MipFilter = NONE;

	AddressU = Clamp;
	AddressV = Clamp;
};


// -------------------------------------------------------------
// ���_�V�F�[�_����s�N�Z���V�F�[�_�ɓn���f�[�^
// -------------------------------------------------------------
struct VS_OUTPUT
{
	float4	Pos			: POSITION;
	float2	Tex			: TEXCOORD0;
	float3	Light		: TEXCOORD1;
	float3	Eye			: TEXCOORD2;
};
// -------------------------------------------------------------
// �V�[���̕`��
// -------------------------------------------------------------
VS_OUTPUT VS(
		float3 Pos		: POSITION,			// ���[�J���ʒu���W
		float3 Tangent	: TANGENT0,			// �ڐ�x�N�g��
		float3 Binormal	: BINORMAL0,		// �]�@���x�N�g��
		float3 Normal	: NORMAL,			// �@���x�N�g��
		float2 Tex		: TEXCOORD0			// �e�N�X�`��
){
	VS_OUTPUT Out = (VS_OUTPUT)0;			// �o�̓f�[�^
	
	// ���W�ϊ�
	float4 position;
	Out.Pos = mul( float4(Pos, 1.0f), matWorld[0] );
	position = Out.Pos;
	Out.Pos = mul( Out.Pos, matView );
	Out.Pos = mul( Out.Pos, matProj );

	// �e�N�X�`��
	Out.Tex = Tex;
	
	// �����x�N�g��
	float3 Eye = normalize( vEyePos - position.xyz );
	Out.Eye.x = dot( Eye, Tangent );
	Out.Eye.y = dot( Eye, Binormal );
	Out.Eye.z = dot( Eye, Normal );
	Out.Eye = normalize( Out.Eye );

	// ���C�g�x�N�g��
	float3 Light = -vLightDir.xyz;
	Out.Light.x = dot( Light, Tangent );
	Out.Light.y = dot( Light, Binormal );
	Out.Light.z = dot( Light, Normal );
	Out.Light = normalize( Out.Light );

	return Out;
}
// �X�L�����b�V���o�[�W����
VS_OUTPUT VS_SKIN(
		float3 Pos		: POSITION,			// ���[�J���ʒu���W
		float4 W		: BLENDWEIGHT,		// �d��
		float3 Tangent	: TANGENT0,			// �ڐ�x�N�g��
		float3 Binormal	: BINORMAL0,		// �]�@���x�N�g��
		float3 Normal	: NORMAL,			// �@���x�N�g��
		float2 Tex		: TEXCOORD0			// �e�N�X�`��
){
	VS_OUTPUT Out = (VS_OUTPUT)0;		// �o�̓f�[�^

	//----- ���W�ϊ�
	float		Weight[4] = (float[4])W;	// �d�݂�float�ɕ���
	float		LastBlendWeight = 0.0f;		// �Ō�̍s��Ɋ|������d��
	float4x4	matCombWorld = 0.0f;		// �������[���h�ϊ��s��

	// ���[���h�ϊ��s����u�����h
	for( int i = 0; i < iBlendNum-1; i++ ) {
		LastBlendWeight += Weight[i];				// �Ō�̏d�݂������Ōv�Z���Ă���
		matCombWorld += matWorld[i] * Weight[i];
	}

	// �Ō�̏d�݂𑫂��Z
	matCombWorld += matWorld[iBlendNum-1] * (1.0f-LastBlendWeight);

	float4 position;
	Out.Pos = mul( Pos, matCombWorld );	// ���[���h�ϊ�
	position = Out.Pos;
	Out.Pos = mul( Out.Pos, matView );	// �r���[�ϊ�
	Out.Pos = mul( Out.Pos, matProj );	// �ˉe�ϊ�

	//----- �e�N�X�`��
	Out.Tex = Tex;

	// �����x�N�g��
	float3 Eye = normalize( vEyePos - position.xyz );
	Out.Eye.x = dot( Eye, Tangent );
	Out.Eye.y = dot( Eye, Binormal );
	Out.Eye.z = dot( Eye, Normal );
	Out.Eye = normalize( Out.Eye );

	// ���C�g�x�N�g��
	float3 Light = -vLightDir.xyz;
	Out.Light.x = dot( Light, Tangent );
	Out.Light.y = dot( Light, Binormal );
	Out.Light.z = dot( Light, Normal );
	Out.Light = normalize( Out.Light );

	return Out;
}

// -------------------------------------------------------------

float4 PS_pass0(VS_OUTPUT In)	:	COLOR
{
	// �����}�b�v���Q�Ƃ��A�������擾����
	// �������̓������Ɋi�[����Ă���
	float h = tex2D( BumpSamp, In.Tex ).a;

	// �e�N�Z���𒸓_���W�n�ł̎����x�N�g�������ɏd�݂�t���Ă��炷�B
	float2 Tex = In.Tex + fHLimit * h * In.Eye.xy;

	// �@���}�b�v���Q�Ƃ��A�@�����擾����
	// �@���}�b�v��-1.0f �` 1.0f�̒l��0.0f �` 1.0f�͈̔͂ɕۑ����Ă���̂ŕϊ�����
	float3 Normal = 2.0f * tex2D( BumpSamp, Tex ).xyz - 1.0f;

	// �t�H���V�F�[�f�B���O
	float3 H = normalize( In.Light + In.Eye );

	// �X�y�L�����J���[���v�Z
	float S = pow( max( 0.0f, dot(Normal, H) ), Specular ) * SpecularPower;

	// �}�e���A���F�����肷��
	return tex2D( TexSamp, Tex ) * max(vLightDir.w, dot( Normal, In.Light ) ) + S;
}

float4 PS_pass1(VS_OUTPUT In)	:	COLOR
{
	// �����}�b�v���Q�Ƃ��A�������擾����
	// �������̓������Ɋi�[����Ă���
	float h = tex2D( BumpSamp, In.Tex ).a;

	// �e�N�Z���𒸓_���W�n�ł̎����x�N�g�������ɏd�݂�t���Ă��炷�B
	float2 Tex = In.Tex + fHLimit * h * In.Eye.xy;

	// �@���}�b�v���Q�Ƃ��A�@�����擾����
	// �@���}�b�v��-1.0f �` 1.0f�̒l��0.0f �` 1.0f�͈̔͂ɕۑ����Ă���̂ŕϊ�����
	float3 Normal = 2.0f * tex2D( BumpSamp, Tex ).xyz - 1.0f;

	// �t�H���V�F�[�f�B���O
	float3 H = normalize( In.Light + In.Eye );

	// �X�y�L�����J���[���v�Z
	float S = pow( max( 0.0f, dot(Normal, H) ), Specular ) * SpecularPower;

	// �}�e���A���F�����肷��
	return max(vLightDir.w, dot( Normal, In.Light ) ) + S;
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
