#pragma once

#include <xaudio2.h>
#include <map>
#include <wrl.h>

/// <summary>
/// �T�E���h�}�l�[�W��
/// </summary>
class SoundManager
{
#pragma region �G�C���A�X
private:
	// Microsoft::WRL::���ȗ�
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
	// std::map���ȗ�
	template <class T, class T2> using map = std::map<T, T2>;
#pragma endregion

#pragma region �T�u�N���X
public:
	// �`�����N�w�b�_
	struct  Chunk
	{
		char id[4]; // �`�����N����ID
		int size; // �`�����N�T�C�Y
	};

	// RIFF�w�b�_�`�����N
	struct RiffHeader
	{
		Chunk chunk; // "RIFF"
		char type[4]; //WAVE"
	};

	// FMT�`�����N
	struct FormatChunk
	{
		Chunk chunk; // "fmt"
		WAVEFORMAT fmt; // �g�`�t�H�[�}�b�g
	};

	// �����f�[�^
	struct SoundData
	{
		WAVEFORMATEX wfex{}; // �g�`�t�H�[�}�b�g
		char* pBuffer; // �g�`�f�[�^
		unsigned int dataSize; // �g�`�f�[�^�̃T�C�Y
	};
#pragma endregion

#pragma region �����o�֐�
public:
	/// <summary>
	/// �f�X�X�g���N�^
	/// </summary>
	~SoundManager();

	/// <summary>
	/// ������
	/// </summary>
	void Initialize();

	/// <summary>
	/// �T�E���h�t�@�C���̓ǂݍ���
	/// </summary>
	/// <param name="number">�T�E���h�f�[�^�ԍ�</param>
	/// <param name="filename">�t�@�C����</param>
	void LoadWave(int number, const char* filename);

	/// <summary>
	/// �T�E���h�t�@�C���̓ǂݍ��݂ƍĐ�
	/// </summary>
	/// <param name="number">�T�E���h�f�[�^�ԍ�</param>
	void PlayWave(int number);
#pragma endregion

#pragma region �����o�ϐ�
private:
	ComPtr<IXAudio2> xAudio2; // XAudio2�p
	IXAudio2MasteringVoice* masterVoice = nullptr; // �}�X�^�[�{�C�X
	map<int, SoundData> soundDatas;	// �g�`�f�[�^�̘A�z�z��
#pragma endregion
};