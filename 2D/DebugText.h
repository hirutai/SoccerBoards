#pragma once
#include <Windows.h>
#include <d3d12.h>
#include <d3dx12.h>
#include <string>

#include "Sprite.h"

/// <summary>
/// デバッグテキストクラス
/// </summary>
class DebugText
{
#pragma region 定数
public:
	static const int maxCharCount = 256; // 最大文字数
	static const int fontWidth = 9; // フォント画像内１文字分の横幅
	static const int fontHeight = 18; // フォント画像内１文字分の縦幅
	static const int fontLineCount = 14; // フォント画像内１行分の文字数
#pragma endregion

#pragma region メンバ関数
public:
	/// <summary>
	/// デスストラクタ
	/// </summary>
	~DebugText();

	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="texNumber">テクスチャ番号</param>
	void Initialize(UINT texNumber);

	/// <summary>
	/// プリント
	/// </summary>
	/// <param name="text">テキスト</param>
	/// <param name="x">x座標</param>
	/// <param name="y">y座標</param>
	/// <param name="scale">スケール</param>
	void Print(const std::string& text, float x, float y, float scale);

	/// <summary>
	/// 描画
	/// </summary>
	/// <param name="cmdList">コマンドリスト</param>
	void DrawAll(ID3D12GraphicsCommandList* cmdList);
#pragma endregion

#pragma region メンバ変数
private:
	Sprite* sprites[maxCharCount]; // スプライトデータ配列
	int spriteIndex = 0; // スプライトデータ配列の番号
#pragma endregion
};