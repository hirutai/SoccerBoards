#pragma once

#include <xaudio2.h>
#include <map>
#include <wrl.h>

/// <summary>
/// サウンドマネージャ
/// </summary>
class SoundManager
{
#pragma region エイリアス
private:
	// Microsoft::WRL::を省略
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
	// std::mapを省略
	template <class T, class T2> using map = std::map<T, T2>;
#pragma endregion

#pragma region サブクラス
public:
	// チャンクヘッダ
	struct  Chunk
	{
		char id[4]; // チャンク毎のID
		int size; // チャンクサイズ
	};

	// RIFFヘッダチャンク
	struct RiffHeader
	{
		Chunk chunk; // "RIFF"
		char type[4]; //WAVE"
	};

	// FMTチャンク
	struct FormatChunk
	{
		Chunk chunk; // "fmt"
		WAVEFORMAT fmt; // 波形フォーマット
	};

	// 音声データ
	struct SoundData
	{
		WAVEFORMATEX wfex{}; // 波形フォーマット
		char* pBuffer; // 波形データ
		unsigned int dataSize; // 波形データのサイズ
	};
#pragma endregion

#pragma region メンバ関数
public:
	/// <summary>
	/// デスストラクタ
	/// </summary>
	~SoundManager();

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize();

	/// <summary>
	/// サウンドファイルの読み込み
	/// </summary>
	/// <param name="number">サウンドデータ番号</param>
	/// <param name="filename">ファイル名</param>
	void LoadWave(int number, const char* filename);

	/// <summary>
	/// サウンドファイルの読み込みと再生
	/// </summary>
	/// <param name="number">サウンドデータ番号</param>
	void PlayWave(int number);
#pragma endregion

#pragma region メンバ変数
private:
	ComPtr<IXAudio2> xAudio2; // XAudio2用
	IXAudio2MasteringVoice* masterVoice = nullptr; // マスターボイス
	map<int, SoundData> soundDatas;	// 波形データの連想配列
#pragma endregion
};