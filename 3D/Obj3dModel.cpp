#include "Obj3dModel.h"
#include <cassert>
#include <fstream>
#include <sstream>

using namespace std;
using namespace DirectX;

ID3D12Device* Obj3dModel::device = nullptr;
TextureManager* Obj3dModel::textureManager = nullptr;

void Obj3dModel::StaticInitialize(ID3D12Device* device, TextureManager* textureManager)
{
	Obj3dModel::device = device;
	Obj3dModel::textureManager = textureManager;
}

void Obj3dModel::Initialize(const std::string& modelName, UINT texNumber)
{
	this->texNumber = texNumber;

	// ファイルストリーム
	ifstream file;
	// .objファイルを開く
	const string filename = modelName + ".obj";
	const string directoryPath = "Resources/3DObject/" + modelName + "/";
	file.open(directoryPath + filename);

	// ファイルオープン失敗をチェック
	assert(!file.fail());

	vector<XMFLOAT3> positions; // 頂点座標
	vector<XMFLOAT3> normals; // 法線ベクトル
	vector<XMFLOAT2> texcoords; // テクスチャUV

	// 1行ずつ読み込む
	string line;
	while (getline(file, line))
	{
		// １行分の文字列をストリームに変換して解析しやすくする
		istringstream line_stream(line);

		// 半角スペース区切りで行の先頭文字列を取得
		string key;
		getline(line_stream, key, ' ');

		// 先頭文字列がvなら頂点座標
		if (key == "v")
		{
			// X,Y,Z座標読み込み
			XMFLOAT3 position{};
			line_stream >> position.x;
			line_stream >> position.y;
			line_stream >> position.z;
			// 座標データに追加
			positions.emplace_back(position);
		}

		// 先頭文字列がvtならテクスチャ
		if (key == "vt")
		{
			// U,V成分読み込み
			XMFLOAT2 texcoord{};
			line_stream >> texcoord.x;
			line_stream >> texcoord.y;
			// V方向反転
			texcoord.y = 1.0f - texcoord.y;
			// テクスチャ座標データに追加
			texcoords.emplace_back(texcoord);
		}

		// 先頭文字列がvnなら法線ベクトル
		if (key == "vn")
		{
			// X,Y,Z成分読み込み
			XMFLOAT3 normal{};
			line_stream >> normal.x;
			line_stream >> normal.y;
			line_stream >> normal.z;
			// 法線ベクトルデータに追加
			normals.emplace_back(normal);
		}

		// 先頭文字列がmtllibならマテリアル
		if (key == "mtllib")
		{
			// マテリアルのファイル名読み込み
			string filename;
			line_stream >> filename;
			// マテリアル読み込み
			LoadMaterial(directoryPath, filename);
		}

		// 先頭文字列がfならポリゴン（三角形）
		if (key == "f")
		{
			// 半角スペース区切りで行の続きを読み込む
			string index_string;
			while (getline(line_stream, index_string, ' '))
			{
				// 頂点インデックス１個分の文字列をストリームに変換して解析しやすくする
				istringstream index_stream(index_string);
				unsigned short indexPosition, indexNormal, indexTexcoord;
				index_stream >> indexPosition;
				index_stream.seekg(1, ios_base::cur); // スラッシュを飛ばす
				index_stream >> indexTexcoord;
				index_stream.seekg(1, ios_base::cur); // スラッシュを飛ばす
				index_stream >> indexNormal;

				// 頂点データの追加
				Vertex vertex{};
				vertex.pos = positions[indexPosition - 1];
				vertex.normal = normals[indexNormal - 1];
				vertex.uv = texcoords[indexTexcoord - 1];
				vertices.emplace_back(vertex);
				// インデックスデータの追加
				indices.emplace_back((unsigned short)indices.size());
			}
		}
	}

	HRESULT result = S_FALSE;

	UINT sizeVB = static_cast<UINT>(sizeof(Vertex) * vertices.size());

	// 頂点バッファ生成
	result = device->
		CreateCommittedResource(
			&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD),
			D3D12_HEAP_FLAG_NONE,
			&CD3DX12_RESOURCE_DESC::Buffer(sizeVB),
			D3D12_RESOURCE_STATE_GENERIC_READ,
			nullptr,
			IID_PPV_ARGS(&vertBuff)
		);

	assert(SUCCEEDED(result)); // 警告

	UINT sizeIB = static_cast<UINT>(sizeof(unsigned short) * indices.size());

	// インデックスバッファ生成
	result = device->
		CreateCommittedResource(
			&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD),
			D3D12_HEAP_FLAG_NONE,
			&CD3DX12_RESOURCE_DESC::Buffer(sizeIB),
			D3D12_RESOURCE_STATE_GENERIC_READ,
			nullptr,
			IID_PPV_ARGS(&indexBuff)
		);

	assert(SUCCEEDED(result)); // 警告

	// 頂点バッファへのデータ転送
	Vertex* vertMap = nullptr;
	result = vertBuff->Map(0, nullptr, (void**)&vertMap);
	if (SUCCEEDED(result))
	{
		copy(vertices.begin(), vertices.end(), vertMap);
		vertBuff->Unmap(0, nullptr);
	}

	// インデックスバッファへのデータ転送
	unsigned short* indexMap = nullptr;
	result = indexBuff->Map(0, nullptr, (void**)&indexMap);
	if (SUCCEEDED(result))
	{
		copy(indices.begin(), indices.end(), indexMap);
		indexBuff->Unmap(0, nullptr);
	}

	// 頂点バッファビューの作成
	vbView.BufferLocation = vertBuff->GetGPUVirtualAddress();
	vbView.SizeInBytes = sizeVB;
	vbView.StrideInBytes = sizeof(vertices[0]);

	// インデックスバッファビューの作成
	ibView.BufferLocation = indexBuff->GetGPUVirtualAddress();
	ibView.Format = DXGI_FORMAT_R16_UINT;
	ibView.SizeInBytes = sizeIB;
}

void Obj3dModel::Draw(ID3D12GraphicsCommandList* cmdList)
{
	// 頂点バッファの設定
	cmdList->IASetVertexBuffers(0, 1, &vbView);
	// インデックスバッファの設定
	cmdList->IASetIndexBuffer(&ibView);

	// デスクリプタヒープ配列
	textureManager->SetDescriptorHeaps(cmdList);

	// シェーダリソースビューのセット
	textureManager->SetShaderResourcesView(cmdList, 2, texNumber);

	// 描画コマンドの設定
	cmdList->DrawIndexedInstanced((UINT)indices.size(), 1, 0, 0, 0);
}

void Obj3dModel::AddVertex(const Vertex& vertex)
{
	vertices.emplace_back(vertex);
}

void Obj3dModel::AddIndex(unsigned short index)
{
	indices.emplace_back(index);
}

void Obj3dModel::LoadMaterial(const std::string& directoryPath, const std::string& filename)
{
	// ファイルストリーム
	std::ifstream file;
	// マテリアルファイルを開く
	file.open(directoryPath + filename);
	// ファイルオープン失敗をチェック
	assert(!file.fail());

	// １行ずつ読み込む
	string line;
	while (getline(file, line))
	{
		// １行分の文字列をストリームに変換
		std::istringstream line_stream(line);
		// 半角スペース区切りで行の先頭文字列を取得
		string key;
		getline(line_stream, key, ' ');

		// 先頭のタブ文字は無視する
		if (key[0] == '\t')
		{
			key.erase(key.begin()); // 先頭の文字を削除
		}

		// 先頭文字列がnewmtlならマテリアル名
		if (key == "newmtl")
		{
			// マテリアル名読み込み
			line_stream >> material.name;
		}
		// 先頭文字列がKaならアンビエント色
		if (key == "Ka")
		{
			line_stream >> material.ambient.x;
			line_stream >> material.ambient.y;
			line_stream >> material.ambient.z;
		}
		// 先頭文字列がKdならディフューズ色
		if (key == "Kd")
		{
			line_stream >> material.diffuse.x;
			line_stream >> material.diffuse.y;
			line_stream >> material.diffuse.z;
		}
		// 先頭文字列がKsならスペキュラー色
		if (key == "Ks")
		{
			line_stream >> material.specular.x;
			line_stream >> material.specular.y;
			line_stream >> material.specular.z;
		}
	}
}
