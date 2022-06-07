#include "DebugText.h"

DebugText::~DebugText()
{
	// 全てのスプライトデータについて
	for (int i = 0; i < _countof(sprites); i++)
	{
		// スプライトを解放する
		delete sprites[i];
		sprites[i] = nullptr;
	}
}

void DebugText::Initialize(UINT texNumber)
{
	// 全てのスプライトデータについて
	for (int i = 0; i < _countof(sprites); i++)
	{
		// スプライトを生成する
		sprites[i] = new Sprite();
		// スプライトを初期化する
		sprites[i]->Initialize(texNumber);
	}
}

void DebugText::Print(const std::string& text, float x, float y, float scale = 1.0f)
{
	// 全ての文字について
	for (int i = 0; i < text.size(); i++)
	{
		// 最大文字数超過
		if (spriteIndex >= maxCharCount)
		{
			break;
		}

		// １文字取り出す（※ASCIIコードでしか成り立たない
		const unsigned char& character = text[i];

		int fontIndex = character - 32;
		if (character >= 0x7f)
		{
			fontIndex = 0;
		}

		int fontIndexY = fontIndex / fontLineCount;
		int fontIndexX = fontIndex % fontLineCount;

		// 座標計算
		sprites[spriteIndex]->SetPosition(x + fontWidth * scale * i, y);
		sprites[spriteIndex]->SetRect((float)fontIndexX * fontWidth, (float)fontIndexY * fontHeight, (float)fontWidth, (float)fontHeight);
		sprites[spriteIndex]->SetSize(fontWidth * scale, fontHeight * scale);

		// 文字を１つ進める
		spriteIndex++;
	}
}

void DebugText::DrawAll(ID3D12GraphicsCommandList* cmdList)
{
	// 全ての文字のスプライトについて
	for (int i = 0; i < spriteIndex; i++)
	{
		// スプライト描画
		sprites[i]->Draw(cmdList);
	}

	spriteIndex = 0;
}