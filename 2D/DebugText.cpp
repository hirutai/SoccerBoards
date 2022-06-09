#include "DebugText.h"

DebugText::~DebugText()
{
	// �S�ẴX�v���C�g�f�[�^�ɂ���
	for (int i = 0; i < _countof(sprites); i++)
	{
		// �X�v���C�g���������
		delete sprites[i];
		sprites[i] = nullptr;
	}
}

void DebugText::Initialize(UINT texNumber)
{
	// �S�ẴX�v���C�g�f�[�^�ɂ���
	for (int i = 0; i < _countof(sprites); i++)
	{
		// �X�v���C�g�𐶐�����
		sprites[i] = new Sprite();
		// �X�v���C�g������������
		sprites[i]->Initialize(texNumber);
	}
}

void DebugText::Print(const std::string& text, float x, float y, float scale = 1.0f)
{
	// �S�Ă̕����ɂ���
	for (int i = 0; i < text.size(); i++)
	{
		// �ő啶��������
		if (spriteIndex >= maxCharCount)
		{
			break;
		}

		// �P�������o���i��ASCII�R�[�h�ł������藧���Ȃ�
		const unsigned char& character = text[i];

		int fontIndex = character - 32;
		if (character >= 0x7f)
		{
			fontIndex = 0;
		}

		int fontIndexY = fontIndex / fontLineCount;
		int fontIndexX = fontIndex % fontLineCount;

		// ���W�v�Z
		sprites[spriteIndex]->SetPosition(x + fontWidth * scale * i, y);
		sprites[spriteIndex]->SetRect((float)fontIndexX * fontWidth, (float)fontIndexY * fontHeight, (float)fontWidth, (float)fontHeight);
		sprites[spriteIndex]->SetSize(fontWidth * scale, fontHeight * scale);

		// �������P�i�߂�
		spriteIndex++;
	}
}

void DebugText::DrawAll()
{
	// �S�Ă̕����̃X�v���C�g�ɂ���
	for (int i = 0; i < spriteIndex; i++)
	{
		// �X�v���C�g�`��
		sprites[i]->Draw();
	}

	spriteIndex = 0;
}