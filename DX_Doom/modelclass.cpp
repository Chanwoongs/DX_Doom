////////////////////////////////////////////////////////////////////////////////
// Filename: modelclass.cpp
////////////////////////////////////////////////////////////////////////////////
#include "modelclass.h"


ModelClass::ModelClass(XMFLOAT3* m_positions, int m_instanceCount, bool needVertices)
{
	m_vertexBuffer = 0;
	m_indexBuffer = 0;
	m_instanceBuffer = 0;
	m_Texture = 0;
	m_TextureArray = 0;
	m_model = 0;

	m_textureCount = 0;
	m_normalCount = 0;
	m_faceCount = 0;

	this->m_instanceCount = m_instanceCount;
	m_instancePosition = new XMFLOAT3[m_instanceCount];

	for (int i = 0; i < m_instanceCount; i++)
	{
		m_instancePosition[i] = m_positions[i];
	}
}


ModelClass::ModelClass(const ModelClass& other)
{
}


ModelClass::~ModelClass()
{
}


bool ModelClass::Initialize(ID3D11Device* device, const WCHAR* modelFilename, const WCHAR* textureFilename, const WCHAR* multiTextureFilename, 
	const WCHAR* bumpMapTextureFilename)
{
	bool result;

	// Load in the model data,
	result = LoadModel(modelFilename);
	if (!result)
	{
		return false;
	}

	// 모델의 법선, 접선 및 이항 벡터를 계산합니다.
	CalculateModelVectors();

	// Initialize the vertex and index buffers.
	result = InitializeBuffers(device);
	if(!result)
	{
		return false;
	}

	// Load the texture for this model.
	result = LoadTextures(device, textureFilename, multiTextureFilename, bumpMapTextureFilename);
	if(!result)
	{
		return false;
	}

	return true;
}


void ModelClass::Shutdown()
{
	// Release the model texture.
	ReleaseTexture();

	// Release the model texture.
	ReleaseTextures();

	// Shutdown the vertex and index buffers.
	ShutdownBuffers();

	// Release the model data.
	ReleaseModel();

	return;
}


void ModelClass::Render(ID3D11DeviceContext* deviceContext)
{
	// Put the vertex and index buffers on the graphics pipeline to prepare them for drawing.
	RenderBuffers(deviceContext);

	return;
}

ID3D11ShaderResourceView** ModelClass::GetTextureArray()
{
	return m_TextureArray->GetTextureArray();
}


int ModelClass::GetVertexCount()
{
	return m_vertexCount;
}

int ModelClass::GetIndexCount()
{
	return m_indexCount;
}

int ModelClass::GetInstanceCount()
{
	return m_instanceCount;
}


ID3D11ShaderResourceView* ModelClass::GetTexture()
{
	return m_Texture->GetTexture();
}


bool ModelClass::InitializeBuffers(ID3D11Device* device)
{
	VertexType* vertices;
	InstanceType* instances;
	unsigned long* indices;
	D3D11_BUFFER_DESC vertexBufferDesc, indexBufferDesc, instanceBufferDesc;
    D3D11_SUBRESOURCE_DATA vertexData, indexData;
	HRESULT result;
	int i;

	// Create the vertex array.
	vertices = new VertexType[m_vertexCount];
	if(!vertices)
	{
		return false;
	}

	// Create the index array.
	indices = new unsigned long[m_indexCount];
	if(!indices)
	{
		return false;
	}

	// Load the vertex array and index array with data.
	for (i = 0; i < m_vertexCount; i++)
	{
		vertices[i].position = XMFLOAT3(m_model[i].x, m_model[i].y, m_model[i].z);
		vertices[i].texture = XMFLOAT2(m_model[i].tu, m_model[i].tv);
		vertices[i].normal = XMFLOAT3(m_model[i].nx, m_model[i].ny, m_model[i].nz);
		vertices[i].tangent = XMFLOAT3(m_model[i].tx, m_model[i].ty, m_model[i].tz);
		vertices[i].binormal = XMFLOAT3(m_model[i].bx, m_model[i].by, m_model[i].bz);

		indices[i] = i;
	}

	// Set up the description of the static vertex buffer.
    vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
    vertexBufferDesc.ByteWidth = sizeof(VertexType) * m_vertexCount;
    vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
    vertexBufferDesc.CPUAccessFlags = 0;
    vertexBufferDesc.MiscFlags = 0;
	vertexBufferDesc.StructureByteStride = 0;

	// Give the subresource structure a pointer to the vertex data.
    vertexData.pSysMem = vertices;
	vertexData.SysMemPitch = 0;
	vertexData.SysMemSlicePitch = 0;

	// Now create the vertex buffer.
    result = device->CreateBuffer(&vertexBufferDesc, &vertexData, &m_vertexBuffer);
	if(FAILED(result))
	{
		return false;
	}

	instances = new InstanceType[m_instanceCount];
	if (!instances)
	{
		return false;
	}

	// 데이터로 인스턴스 배열을 로드합니다.
	for (int i = 0; i < m_instanceCount; i++)
	{
		instances[i].position = m_instancePosition[i];
	}

	// 인스턴스 버퍼의 설명을 설정합니다.
	instanceBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	instanceBufferDesc.ByteWidth = sizeof(InstanceType) * m_instanceCount;
	instanceBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	instanceBufferDesc.CPUAccessFlags = 0;
	instanceBufferDesc.MiscFlags = 0;
	instanceBufferDesc.StructureByteStride = 0;

	// 하위 리소스 구조에 인스턴스 데이터에 대한 포인터를 제공합니다.
	D3D11_SUBRESOURCE_DATA instanceData;
	instanceData.pSysMem = instances;
	instanceData.SysMemPitch = 0;
	instanceData.SysMemSlicePitch = 0;

	// 인스턴스 버퍼를 만듭니다.
	if (FAILED(device->CreateBuffer(&instanceBufferDesc, &instanceData, &m_instanceBuffer)))
	{
		return false;
	}

	// Set up the description of the static index buffer.
    indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
    indexBufferDesc.ByteWidth = sizeof(unsigned long) * m_indexCount;
    indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
    indexBufferDesc.CPUAccessFlags = 0;
    indexBufferDesc.MiscFlags = 0;
	indexBufferDesc.StructureByteStride = 0;

	// Give the subresource structure a pointer to the index data.
    indexData.pSysMem = indices;
	indexData.SysMemPitch = 0;
	indexData.SysMemSlicePitch = 0;

	// Create the index buffer.
	result = device->CreateBuffer(&indexBufferDesc, &indexData, &m_indexBuffer);
	if(FAILED(result))
	{
		return false;
	}

	// Release the arrays now that the vertex and index buffers have been created and loaded.
	delete [] vertices;
	vertices = 0;

	delete [] indices;
	indices = 0;

	delete [] instances;
	instances = 0;

	return true;
}


void ModelClass::ShutdownBuffers()
{
	if (m_instancePosition)
	{
		delete[] m_instancePosition;
		m_instancePosition = 0;
	}
	// Release the index buffer.
	if (m_instanceBuffer)
	{
		m_instanceBuffer->Release();
		m_instanceBuffer = 0;
	}

	// Release the index buffer.
	if(m_indexBuffer)
	{
		m_indexBuffer->Release();
		m_indexBuffer = 0;
	}

	// Release the vertex buffer.
	if(m_vertexBuffer)
	{
		m_vertexBuffer->Release();
		m_vertexBuffer = 0;
	}

	return;
}


void ModelClass::RenderBuffers(ID3D11DeviceContext* deviceContext)
{
	// 버퍼 오프셋과 스트라이드를 설정합니다.
	unsigned int strides[2] = { sizeof(VertexType), sizeof(InstanceType) };
	unsigned int offsets[2] = { 0, 0 };
    
	// 포인터의 배열을 정점 버퍼와 인스턴스 버퍼로 설정합니다.
	ID3D11Buffer* bufferPointers[2] = { m_vertexBuffer, m_instanceBuffer };
	 
	// Set the vertex buffer to active in the input assembler so it can be rendered.
	deviceContext->IASetVertexBuffers(0, 2, bufferPointers, strides, offsets);

    // Set the index buffer to active in the input assembler so it can be rendered.
	deviceContext->IASetIndexBuffer(m_indexBuffer, DXGI_FORMAT_R32_UINT, 0);

    // Set the type of primitive that should be rendered from this vertex buffer, in this case triangles.
	deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	return;
}


bool ModelClass::LoadTexture(ID3D11Device* device, const WCHAR* filename)
{
	bool result;


	// Create the texture object.
	m_Texture = new TextureClass;
	if(!m_Texture)
	{
		return false;
	}

	// Initialize the texture object.
	result = m_Texture->Initialize(device, filename);
	if(!result)
	{
		return false;
	}

	return true;
}

bool ModelClass::LoadTextures(ID3D11Device* device, const WCHAR* filename1, const WCHAR* filename2, const WCHAR* filename3)
{
	bool result;
	// Create the texture array object.
	m_TextureArray = new TextureArrayClass;
	if (!m_TextureArray)
	{
		return false;
	}
	// Initialize the texture array object.
	result = m_TextureArray->Initialize(device, filename1, filename2, filename3);
	if (!result)
	{
		return false;
	}
	return true;
}


void ModelClass::ReleaseTexture()
{
	// Release the texture object.
	if(m_Texture)
	{
		m_Texture->Shutdown();
		delete m_Texture;
		m_Texture = 0;
	}

	return;
}

void ModelClass::ReleaseTextures()
{
	// Release the texture array object.
	if (m_TextureArray)
	{
		m_TextureArray->Shutdown();
		delete m_TextureArray;
		m_TextureArray = 0;
	}
	return;
}

bool ModelClass::LoadModel(const WCHAR* filename)
{
	ReadFileCounts(filename);

	return true;
}

void ModelClass::ReleaseModel()
{
	if (m_model)
	{
		delete[] m_model;
		m_model = 0;
	}

	return;
}

bool ModelClass::ReadFileCounts(const WCHAR* filename)
{
	ifstream fin;
	char input;
	// Initialize the counts.
	int vertexCount = 0;
	int textureCount = 0;
	int normalCount = 0;
	int faceCount = 0;
	// Open the file.
	fin.open(filename);
	// Check if it was successful in opening the file.
	if (fin.fail() == true)
	{
		return false;
	}
	// Read from the file and continue to read until the end of the file is reached.
	fin.get(input);
	while (!fin.eof())
	{
		// If the line starts with 'v' then count either the vertex, the texture coordinates, or the normal vector.
		if (input == 'v')
		{
			fin.get(input);
			if (input == ' ') { vertexCount++; }
			if (input == 't') { textureCount++; }
			if (input == 'n') { normalCount++; }
		}

		// If the line starts with 'f' then increment the face count.
		if (input == 'f')
		{
			fin.get(input);
			if (input == ' ') { faceCount++; }
		}

		// Otherwise read in the remainder of the line.
		while (input != '\n')
		{
			fin.get(input);
			if (fin.eof())
				break;
		}

		// Start reading the beginning of the next line.
		fin.get(input);
	}
	// Close the file.
	fin.close();

	LoadDataStructures(filename, vertexCount, textureCount, normalCount, faceCount);

	return true;
}

bool ModelClass::LoadDataStructures(const WCHAR* filename, int vertexCount, int textureCount, int normalCount, int faceCount)
{
	XMFLOAT3* vertices, * texcoords, * normals;
	FaceType* faces;
	ifstream fin;
	int vertexIndex, texcoordIndex, normalIndex, faceIndex, vIndex, tIndex, nIndex;
	char input, input2;
	ofstream fout;

	// Initialize the four data structures.
	vertices = new XMFLOAT3[vertexCount];
	if (!vertices)
	{
		return false;
	}

	texcoords = new XMFLOAT3[textureCount];
	if (!texcoords)
	{
		return false;
	}

	normals = new XMFLOAT3[normalCount];
	if (!normals)
	{
		return false;
	}

	faces = new FaceType[faceCount];
	if (!faces)
	{
		return false;
	}

	// Initialize the indexes.
	vertexIndex = 0;
	texcoordIndex = 0;
	normalIndex = 0;
	faceIndex = 0;

	// Open the file.
	fin.open(filename);

	// Check if it was successful in opening the file.
	if (fin.fail() == true)
	{
		return false;
	}

	// Read in the vertices, texture coordinates, and normals into the data structures.
	// Important: Also convert to left hand coordinate system since Maya uses right hand coordinate system.
	fin.get(input);
	while (!fin.eof())
	{
		if (input == 'v')
		{
			fin.get(input);

			// Read in the vertices.
			if (input == ' ')
			{
				fin >> vertices[vertexIndex].x >> vertices[vertexIndex].y >>
					vertices[vertexIndex].z;

				// Invert the Z vertex to change to left hand system.
				vertices[vertexIndex].z = vertices[vertexIndex].z * -1.0f;
				vertexIndex++;
			}

			// Read in the texture uv coordinates.
			if (input == 't')
			{
				fin >> texcoords[texcoordIndex].x >> texcoords[texcoordIndex].y;
				// Invert the V texture coordinates to left hand system.
				texcoords[texcoordIndex].y = 1.0f - texcoords[texcoordIndex].y;
				texcoordIndex++;
			}

			// Read in the normals.
			if (input == 'n')
			{
				fin >> normals[normalIndex].x >> normals[normalIndex].y >>
					normals[normalIndex].z;

				// Invert the Z normal to change to left hand system.
				normals[normalIndex].z = normals[normalIndex].z * -1.0f;
				normalIndex++;
			}
		}

		// Read in the faces.
		if (input == 'f')
		{
			fin.get(input);
			if (input == ' ')
			{
				// Read the face data in backwards to convert it to a left hand system from right hand system.
				fin >> faces[faceIndex].vIndex3 >> input2 >> faces[faceIndex].tIndex3 >>
					input2 >> faces[faceIndex].nIndex3 >> faces[faceIndex].vIndex2 >> input2 >>
					faces[faceIndex].tIndex2 >> input2 >> faces[faceIndex].nIndex2 >>
					faces[faceIndex].vIndex1 >> input2 >> faces[faceIndex].tIndex1 >> input2 >>
					faces[faceIndex].nIndex1;
				faceIndex++;
			}
		}

		// Read in the remainder of the line.
		while (input != '\n')
		{
			fin.get(input);
			if (fin.eof())
				break;
		}

		// Start reading the beginning of the next line.
		fin.get(input);
	}

	//// Close the file.
	//fin.close();
	//// Open the output file.
	//fout.open("model.txt");
	//// Write out the file header that our model format uses.
	//fout << "Vertex Count: " << (faceCount * 3) << endl;
	//fout << endl;
	//fout << "Data:" << endl;
	//fout << endl;

	m_vertexCount = faceCount * 3;

	// Set the number of indices to be the same as the vertex count.
	m_indexCount = m_vertexCount;

	// Create the model using the vertex count that was read in.
	m_model = new ModelType[m_vertexCount];
	if (!m_model)
	{
		return false;
	}

	// Now loop through all the faces and output the three vertices for each face.
	for (int i = 0; i < faceIndex; i++)
	{
		vIndex = faces[i].vIndex1 - 1;
		tIndex = faces[i].tIndex1 - 1;
		nIndex = faces[i].nIndex1 - 1;
		//fout << vertices[vIndex].x << ' ' << vertices[vIndex].y << ' ' << vertices[vIndex].z << ' '
		//	<< texcoords[tIndex].x << ' ' << texcoords[tIndex].y << ' '
		//	<< normals[nIndex].x << ' ' << normals[nIndex].y << ' ' << normals[nIndex].z << endl;

		m_model[i * 3].x = vertices[vIndex].x;
		m_model[i * 3].y = vertices[vIndex].y;
		m_model[i * 3].z = vertices[vIndex].z;

		m_model[i * 3].tu = texcoords[tIndex].x;
		m_model[i * 3].tv = texcoords[tIndex].y;

		m_model[i * 3].nx = normals[nIndex].x;
		m_model[i * 3].ny = normals[nIndex].y;
		m_model[i * 3].nz = normals[nIndex].z;

		vIndex = faces[i].vIndex2 - 1;
		tIndex = faces[i].tIndex2 - 1;
		nIndex = faces[i].nIndex2 - 1;
		//fout << vertices[vIndex].x << ' ' << vertices[vIndex].y << ' ' << vertices[vIndex].z << ' '
		//	<< texcoords[tIndex].x << ' ' << texcoords[tIndex].y << ' '
		//	<< normals[nIndex].x << ' ' << normals[nIndex].y << ' ' << normals[nIndex].z << endl;

		m_model[i * 3 + 1].x = vertices[vIndex].x;
		m_model[i * 3 + 1].y = vertices[vIndex].y;
		m_model[i * 3 + 1].z = vertices[vIndex].z;

		m_model[i * 3 + 1].tu = texcoords[tIndex].x;
		m_model[i * 3 + 1].tv = texcoords[tIndex].y;

		m_model[i * 3 + 1].nx = normals[nIndex].x;
		m_model[i * 3 + 1].ny = normals[nIndex].y;
		m_model[i * 3 + 1].nz = normals[nIndex].z;

		vIndex = faces[i].vIndex3 - 1;
		tIndex = faces[i].tIndex3 - 1;
		nIndex = faces[i].nIndex3 - 1;
		//fout << vertices[vIndex].x << ' ' << vertices[vIndex].y << ' ' << vertices[vIndex].z << ' '
		//	<< texcoords[tIndex].x << ' ' << texcoords[tIndex].y << ' '
		//	<< normals[nIndex].x << ' ' << normals[nIndex].y << ' ' << normals[nIndex].z << endl;

		m_model[i * 3 + 2].x = vertices[vIndex].x;
		m_model[i * 3 + 2].y = vertices[vIndex].y;
		m_model[i * 3 + 2].z = vertices[vIndex].z;

		m_model[i * 3 + 2].tu = texcoords[tIndex].x;
		m_model[i * 3 + 2].tv = texcoords[tIndex].y;

		m_model[i * 3 + 2].nx = normals[nIndex].x;
		m_model[i * 3 + 2].ny = normals[nIndex].y;
		m_model[i * 3 + 2].nz = normals[nIndex].z;
	}

	//// Close the output file.
	//fout.close();

	// Release the four data structures.
	if (vertices)
	{
		delete[] vertices;
		vertices = 0;
	}

	if (texcoords)
	{
		delete[] texcoords;
		texcoords = 0;
	}

	if (normals)
	{
		delete[] normals;
		normals = 0;
	}

	if (faces)
	{
		delete[] faces;
		faces = 0;
	}

	return true;
}

void ModelClass::CalculateModelVectors()
{
	TempVertexType vertex1, vertex2, vertex3;
	VectorType tangent, binormal, normal;


	// 모델의 면 수를 계산합니다.
	int faceCount = m_vertexCount / 3;

	// 모델 데이터에 대한 인덱스를 초기화합니다.
	int index = 0;

	// 모든면을 살펴보고 접선, 비공식 및 법선 벡터를 계산합니다.
	for (int i = 0; i < faceCount; i++)
	{
		// 모델에서이면에 대한 세 개의 정점을 가져옵니다.
		vertex1.x = m_model[index].x;
		vertex1.y = m_model[index].y;
		vertex1.z = m_model[index].z;
		vertex1.tu = m_model[index].tu;
		vertex1.tv = m_model[index].tv;
		vertex1.nx = m_model[index].nx;
		vertex1.ny = m_model[index].ny;
		vertex1.nz = m_model[index].nz;
		index++;

		vertex2.x = m_model[index].x;
		vertex2.y = m_model[index].y;
		vertex2.z = m_model[index].z;
		vertex2.tu = m_model[index].tu;
		vertex2.tv = m_model[index].tv;
		vertex2.nx = m_model[index].nx;
		vertex2.ny = m_model[index].ny;
		vertex2.nz = m_model[index].nz;
		index++;

		vertex3.x = m_model[index].x;
		vertex3.y = m_model[index].y;
		vertex3.z = m_model[index].z;
		vertex3.tu = m_model[index].tu;
		vertex3.tv = m_model[index].tv;
		vertex3.nx = m_model[index].nx;
		vertex3.ny = m_model[index].ny;
		vertex3.nz = m_model[index].nz;
		index++;

		// 표면의 탄젠트와 바이 노멀을 계산합니다.
		CalculateTangentBinormal(vertex1, vertex2, vertex3, tangent, binormal);

		// 접선과 binormal을 사용하여 새 법선을 계산합니다.
		CalculateNormal(tangent, binormal, normal);

		// 모델 구조에서 면의 법선, 접선 및 바이 노멀을 저장합니다.
		m_model[index - 1].nx = normal.x;
		m_model[index - 1].ny = normal.y;
		m_model[index - 1].nz = normal.z;
		m_model[index - 1].tx = tangent.x;
		m_model[index - 1].ty = tangent.y;
		m_model[index - 1].tz = tangent.z;
		m_model[index - 1].bx = binormal.x;
		m_model[index - 1].by = binormal.y;
		m_model[index - 1].bz = binormal.z;

		m_model[index - 2].nx = normal.x;
		m_model[index - 2].ny = normal.y;
		m_model[index - 2].nz = normal.z;
		m_model[index - 2].tx = tangent.x;
		m_model[index - 2].ty = tangent.y;
		m_model[index - 2].tz = tangent.z;
		m_model[index - 2].bx = binormal.x;
		m_model[index - 2].by = binormal.y;
		m_model[index - 2].bz = binormal.z;

		m_model[index - 3].nx = normal.x;
		m_model[index - 3].ny = normal.y;
		m_model[index - 3].nz = normal.z;
		m_model[index - 3].tx = tangent.x;
		m_model[index - 3].ty = tangent.y;
		m_model[index - 3].tz = tangent.z;
		m_model[index - 3].bx = binormal.x;
		m_model[index - 3].by = binormal.y;
		m_model[index - 3].bz = binormal.z;
	}
}


void ModelClass::CalculateTangentBinormal(TempVertexType vertex1, TempVertexType vertex2, TempVertexType vertex3,
	VectorType& tangent, VectorType& binormal)
{
	float vector1[3], vector2[3];
	float tuVector[2], tvVector[2];


	// 현재 표면의 두 벡터를 계산합니다.
	vector1[0] = vertex2.x - vertex1.x;
	vector1[1] = vertex2.y - vertex1.y;
	vector1[2] = vertex2.z - vertex1.z;

	vector2[0] = vertex3.x - vertex1.x;
	vector2[1] = vertex3.y - vertex1.y;
	vector2[2] = vertex3.z - vertex1.z;

	// tu 및 tv 텍스처 공간 벡터를 계산합니다.
	tuVector[0] = vertex2.tu - vertex1.tu;
	tvVector[0] = vertex2.tv - vertex1.tv;

	tuVector[1] = vertex3.tu - vertex1.tu;
	tvVector[1] = vertex3.tv - vertex1.tv;

	// 탄젠트 / 바이 노멀 방정식의 분모를 계산합니다.
	float den = 1.0f / (tuVector[0] * tvVector[1] - tuVector[1] * tvVector[0]);

	// 교차 곱을 계산하고 계수로 곱하여 접선과 비 구식을 얻습니다.
	tangent.x = (tvVector[1] * vector1[0] - tvVector[0] * vector2[0]) * den;
	tangent.y = (tvVector[1] * vector1[1] - tvVector[0] * vector2[1]) * den;
	tangent.z = (tvVector[1] * vector1[2] - tvVector[0] * vector2[2]) * den;

	binormal.x = (tuVector[0] * vector2[0] - tuVector[1] * vector1[0]) * den;
	binormal.y = (tuVector[0] * vector2[1] - tuVector[1] * vector1[1]) * den;
	binormal.z = (tuVector[0] * vector2[2] - tuVector[1] * vector1[2]) * den;

	// 이 법선의 길이를 계산합니다.
	float length = sqrt((tangent.x * tangent.x) + (tangent.y * tangent.y) + (tangent.z * tangent.z));

	// 법선을 표준화 한 다음 저장합니다.
	tangent.x = tangent.x / length;
	tangent.y = tangent.y / length;
	tangent.z = tangent.z / length;

	// 이 법선의 길이를 계산합니다.
	length = sqrt((binormal.x * binormal.x) + (binormal.y * binormal.y) + (binormal.z * binormal.z));

	// 법선을 표준화 한 다음 저장합니다.
	binormal.x = binormal.x / length;
	binormal.y = binormal.y / length;
	binormal.z = binormal.z / length;
}


void ModelClass::CalculateNormal(VectorType tangent, VectorType binormal, VectorType& normal)
{
	// 법선 벡터를 줄 수있는 접선과 binormal의 외적을 계산합니다.
	normal.x = (tangent.y * binormal.z) - (tangent.z * binormal.y);
	normal.y = (tangent.z * binormal.x) - (tangent.x * binormal.z);
	normal.z = (tangent.x * binormal.y) - (tangent.y * binormal.x);

	// 법선의 길이를 계산합니다.
	float length = sqrt((normal.x * normal.x) + (normal.y * normal.y) + (normal.z * normal.z));

	// 법선을 표준화합니다.
	normal.x = normal.x / length;
	normal.y = normal.y / length;
	normal.z = normal.z / length;
}