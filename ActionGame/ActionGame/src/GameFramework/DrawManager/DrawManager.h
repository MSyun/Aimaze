// �`��}�l�[�W���[
// 2017. 2. 23	�v���O�����쐬
// author		SyunMizuno


#pragma once




class DrawManagerBase {
public:
	DrawManagerBase();
	virtual ~DrawManagerBase();

	// �`��
	void Draw();
	void LateDraw();

	// ���o
	void Extract();

protected:
	// �e�`��
	void DrawShadow();

	// ������
	void CheckFrustum();

	// �t�B���^�`��
	void FilterDraw();
};