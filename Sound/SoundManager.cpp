#include "SoundManager.h"
#include <cassert>
#include <fstream>

#pragma comment(lib,"xaudio2.lib")

SoundManager::~SoundManager()
{
	// 読み込みサウンドの波形データを削除
	for (auto& pair : soundDatas)
	{
		delete pair.second.pBuffer;
	}
}

void SoundManager::Initialize()
{
	HRESULT result;

	// XAudioエンジンのインスタンスを生成
	result = XAudio2Create(&xAudio2, 0, XAUDIO2_DEFAULT_PROCESSOR);
	
	assert(SUCCEEDED(result)); // 警告
	
	// マスターボイスを生成
	result = xAudio2->CreateMasteringVoice(&masterVoice);

	assert(SUCCEEDED(result)); // 警告
}

void SoundManager::LoadWave(int number, const char* filename)
{
	SoundData soundData{};

	// ファイルストリーム
	std::ifstream file;
	// Waveファイルを開く
	file.open(filename, std::ios_base::binary);

	assert(SUCCEEDED(file.fill())); // 警告

	// RIFFヘッダーの読み込み
	RiffHeader riff;
	file.read((char*)&riff, sizeof(riff));
	// ファイルがRIFFかチェック
	if (strncmp(riff.chunk.id, "RIFF", 4) != 0)
	{
		assert(0);
	}

	// Formatチャンクの読み込み
	FormatChunk format;
	file.read((char*)&format, sizeof(format));

	// Dataチャンクの読み込み
	Chunk data;
	file.read((char*)&data, sizeof(data));

	// Dataチャンクのデータ部（波形データ）の読み込み
	soundData.pBuffer = new char[data.size];
	file.read(soundData.pBuffer, data.size);

	// 波形データのサイズを記録
	soundData.dataSize = data.size;

	// Waveファイルを閉じる
	file.close();

	WAVEFORMATEX wfex{};
	// 波形フォーマットの設定
	memcpy(&soundData.wfex, &format.fmt, sizeof(format.fmt));
	soundData.wfex.wBitsPerSample = format.fmt.nBlockAlign * 8 / format.fmt.nChannels;

	// 連想配列に要素を追加
	soundDatas.insert(std::make_pair(number, soundData));
}

void SoundManager::PlayWave(int number)
{
	SoundData& soundData = soundDatas[number];

	HRESULT result;

	// 波形フォーマットを元にSourceVoiceの生成
	IXAudio2SourceVoice* pSourceVoice = nullptr;
	result = xAudio2->CreateSourceVoice(&pSourceVoice, &soundData.wfex, 0, 2.0f, nullptr);

	assert(SUCCEEDED(result)); // 警告

	// 再生する波形データの設定
	XAUDIO2_BUFFER buf{};
	buf.pAudioData = (BYTE*)soundData.pBuffer;
	buf.pContext = soundData.pBuffer;
	buf.Flags = XAUDIO2_END_OF_STREAM;
	buf.AudioBytes = soundData.dataSize;

	// 波形データの再生
	result = pSourceVoice->SubmitSourceBuffer(&buf);
	result = pSourceVoice->Start();
}