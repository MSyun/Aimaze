// �g�V�F�[�_
// r�ɍ�����ۑ�
// g�ɑ��x

// -------------------------------------------------------------
// �O���[�o���ϐ�
// -------------------------------------------------------------

float2	TexOffset;			// 1�e�N�Z�����㉺���E�ʒu�ւ̃I�t�Z�b�g�l
float	SpringPower = 0.5f;	// �΂ˋ��x�B�l��傫������Ɣg�̈ړ����x�������Ȃ�B
float2	AddWavePos;			// �ǉ�����g�̃e�N�Z���ʒu
float	AddWaveHeight;		// �ǉ�����g�̍���

// �g�}�b�v
texture tex;
sampler TexSamp = sampler_state
{
	texture = <tex>;
	MinFilter = LINEAR;
	MagFilter = LINEAR;
	MipFilter = NONE;

	AddressU = Clamp;
	AddressV = Clamp;
};



// -------------------------------------------------------------
// ���_�V�F�[�_����s�N�Z���V�F�[�_�ɓn���f�[�^
// -------------------------------------------------------------
struct VS_OUTPUT {
	float4		Pos		:	POSITION;	// ���W
	float2		Tex		:	TEXCOORD0;	// �e�N�X�`��
};


// -------------------------------------------------------------
// �V�[���̕`��
// -------------------------------------------------------------
VS_OUTPUT VS (
		float4	Pos		:	POSITION,	// ���[�J���ʒu���W
		float4	Color	:	COLOR0,		// �@���x�N�g��
		float2	Tex		:	TEXCOORD0	// �e�N�X�`��
) {
	VS_OUTPUT Out = (VS_OUTPUT)0;		// �o�̓f�[�^

	Out.Pos = Pos;
	Out.Tex = Tex;

	return Out;
}

// -------------------------------------------------------------
// �g�}�b�v�̍X�V����
float4 PS_WaveMap ( VS_OUTPUT	In )	:	COLOR0
{
	// �g�}�b�v���獂���iR����)���A���x�iG�����j���擾
	float4 Wave = tex2D( TexSamp, In.Tex );

	// ���͂̃e�N�Z���̍������擾(�E�㍶��)
	float H1 = tex2D( TexSamp, In.Tex + float2( TexOffset.x,	0.0f		 ) ).r;
	float H2 = tex2D( TexSamp, In.Tex + float2( 0.0f,			TexOffset.y	 ) ).r;
	float H3 = tex2D( TexSamp, In.Tex + float2( -TexOffset.x,	0.0f		 ) ).r;
	float H4 = tex2D( TexSamp, In.Tex + float2( 0.0f,			-TexOffset.y ) ).r;

	// ���x�̍X�V
	float V = ( ( H1 + H2 + H3 + H4 ) * 0.25f - Wave.r ) * SpringPower +	// ���͂̃e�N�Z���̍����Ǝ����̍����̍����v�Z����B���̒l�����x�ƂȂ�B
				Wave.g;														// ���݂̑��x�����Z

	// �����̍X�V
	float H = Wave.r +	// ���݂̍���
				V;		// �v�Z�������x�����Z

	// �V�K�ɔg��ǉ�(�����d����
	if( distance( In.Tex, AddWavePos ) < 0.005f ) {
//		H += 0.5f;
//		V += 0.2f;
		V += AddWaveHeight;
	}

	// �g�̍��������X��0.0f�ʒu�Ɉړ�����悤�ɕ␳����B
	// �V�K�ɔg��ǉ����Ȃ��ꍇ�A���X�ɕ�����ɂȂ�B
//	H = H - H * 0.001f;

	return float4( H, V, 1.0f, 1.0f );
}

// �@���}�b�v�쐬
float4 PS_BumpMap ( VS_OUTPUT	In )	:	COLOR0
{
	float4 Out;

	// ���͂̃e�N�Z���̍������擾(�E�㍶��)
	float H1 = tex2D( TexSamp, In.Tex + float2( TexOffset.x,	0.0f		 ) ).r;
	float H2 = tex2D( TexSamp, In.Tex + float2( 0.0f,			TexOffset.y	 ) ).r;
	float H3 = tex2D( TexSamp, In.Tex + float2( -TexOffset.x,	0.0f		 ) ).r;
	float H4 = tex2D( TexSamp, In.Tex + float2( 0.0f,			-TexOffset.y ) ).r;

	// X�����̍����̕ω��ʂ��v�Z����
	// �g�̍�������-1.0f �` 1.0f�͈̔͂Ŋi�[����Ă���̂�
	float tu = 0.5f * (H3 - H1) + 0.5f;

	// Y�����̍����̕ω��ʂ��v�Z����
	// �g�̍�������-1.0f �` 1.0f�͈̔͂Ŋi�[����Ă���̂�
	float tv = 0.5f * (H4 - H2) + 0.5f;

	// �����}�b�s���O�͍��������g�p����B
	Out = float4( tu, tv, 1.0f, tex2D( TexSamp, In.Tex ).r * 0.5f + 0.5f );

	return Out;
}



// -------------------------------------------------------------
// �e�N�j�b�N
// -------------------------------------------------------------
technique TShader
{
	pass P0	// �g�}�b�v
	{
		VertexShader = compile vs_1_1 VS();
		PixelShader  = compile ps_2_0 PS_WaveMap();
	}

	pass P1	// �@���}�b�v
	{
		VertexShader = compile vs_1_1 VS();
		PixelShader  = compile ps_2_0 PS_BumpMap();
	}
}
