////////////////////////////////////////////////////////////////////////////////
// Filename: sphereclass.cpp
////////////////////////////////////////////////////////////////////////////////
#include "sphereclass.h"


SphereClass::SphereClass()
{
	m_vertexBuffer = 0;
	m_indexBuffer = 0;
	m_Texture = 0;
}


SphereClass::SphereClass(const SphereClass& other)
{
}


SphereClass::~SphereClass()
{
}


bool SphereClass::Initialize(ID3D11Device* device, const WCHAR* textureFilename, int latLines, int longLines)
{
	bool result;


	// Initialize the vertex and index buffers.s
	result = InitializeBuffers(device, latLines, longLines);
	if (!result)
	{
		return false;
	}

	// Load the texture for this model.
	result = LoadTexture(device, textureFilename);
	if (!result)
	{
		return false;
	}

	return true;
}


void SphereClass::Shutdown()
{
	// Release the model texture.
	ReleaseTexture();

	// Shutdown the vertex and index buffers.
	ShutdownBuffers();

	return;
}


void SphereClass::Render(ID3D11DeviceContext* deviceContext)
{
	// Put the vertex and index buffers on the graphics pipeline to prepare them for drawing.
	RenderBuffers(deviceContext);

	return;
}


int SphereClass::GetIndexCount()
{
	return m_faceCount;
}


ID3D11ShaderResourceView* SphereClass::GetTexture()
{
	return m_Texture->GetTexture();
}


bool SphereClass::InitializeBuffers(ID3D11Device* device, int latLines, int longLines)
{
	XMMATRIX rotationX, rotationY;
	D3D11_BUFFER_DESC vertexBufferDesc, indexBufferDesc;
	D3D11_SUBRESOURCE_DATA vertexData, indexData;
	HRESULT result;

	m_vertexCount = ((latLines - 2) * longLines) + 2;
	m_faceCount = ((latLines - 3) * (longLines) * 2) + (longLines * 2);

	float sphereYaw = 0.0f;
	float spherePitch = 0.0f;

	std::vector<VertexType> vertices(m_vertexCount);

	XMVECTOR currVertPos = XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f);

	vertices[0].pos.x = 0.0f;
	vertices[0].pos.y = 0.0f;
	vertices[0].pos.z = 1.0f;

	for (DWORD i = 0; i < DWORD(latLines - 2); ++i)
	{
		spherePitch = (float)(i + 1) * (3.14f / (float)(latLines - 1));
		rotationX = XMMatrixRotationX(spherePitch);
		for (DWORD j = 0; j < DWORD(longLines); ++j)
		{
			sphereYaw = (float)j * (6.28f / (float)(longLines));
			rotationY = XMMatrixRotationZ(sphereYaw);
			currVertPos = XMVector3TransformNormal(XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f), (rotationX * rotationY));
			currVertPos = XMVector3Normalize(currVertPos);
			vertices[i * longLines + j + 1].pos.x = XMVectorGetX(currVertPos);
			vertices[i * longLines + j + 1].pos.y = XMVectorGetY(currVertPos);
			vertices[i * longLines + j + 1].pos.z = XMVectorGetZ(currVertPos);
		}
	}

	vertices[m_vertexCount - 1].pos.x = 0.0f;
	vertices[m_vertexCount - 1].pos.y = 0.0f;
	vertices[m_vertexCount - 1].pos.z = -1.0f;

	ZeroMemory(&vertexBufferDesc, sizeof(vertexBufferDesc));

	vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	vertexBufferDesc.ByteWidth = sizeof(VertexType) *m_vertexCount;
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBufferDesc.CPUAccessFlags = 0;
	vertexBufferDesc.MiscFlags = 0;

	D3D11_SUBRESOURCE_DATA vertexBufferData;

	ZeroMemory(&vertexBufferData, sizeof(vertexBufferData));
	vertexBufferData.pSysMem = &vertices[0];
	result = device->CreateBuffer(&vertexBufferDesc, &vertexBufferData, &m_vertexBuffer);
	if (FAILED(result))
	{
		return false;
	}

	std::vector<DWORD> indices(m_faceCount * 3);

	int k = 0;
	for (DWORD l = 0; l < DWORD(longLines - 1); ++l)
	{
		indices[k] = 0;
		indices[k + 1] = l + 1;
		indices[k + 2] = l + 2;
		k += 3;
	}

	indices[k] = 0;
	indices[k + 1] = longLines;
	indices[k + 2] = 1;
	k += 3;

	for (DWORD i = 0; i < DWORD(latLines - 3); ++i)
	{
		for (DWORD j = 0; j < DWORD(longLines - 1); ++j)
		{
			indices[k] = i * longLines + j + 1;
			indices[k + 1] = i * longLines + j + 2;
			indices[k + 2] = (i + 1) * longLines + j + 1;

			indices[k + 3] = (i + 1) * longLines + j + 1;
			indices[k + 4] = i * longLines + j + 2;
			indices[k + 5] = (i + 1) * longLines + j + 2;

			k += 6; // next quad
		}

		indices[k] = (i * longLines) + longLines;
		indices[k + 1] = (i * longLines) + 1;
		indices[k + 2] = ((i + 1) * longLines) + longLines;

		indices[k + 3] = ((i + 1) * longLines) + longLines;
		indices[k + 4] = (i * longLines) + 1;
		indices[k + 5] = ((i + 1) * longLines) + 1;

		k += 6;
	}

	for (DWORD l = 0; l < (DWORD)(longLines - 1); ++l)
	{
		indices[k] = m_vertexCount - 1;
		indices[k + 1] = (m_vertexCount - 1) - (l + 1);
		indices[k + 2] = (m_vertexCount - 1) - (l + 2);
		k += 3;
	}

	indices[k] = m_vertexCount - 1;
	indices[k + 1] = (m_vertexCount - 1) - longLines;
	indices[k + 2] = m_vertexCount - 2;

	ZeroMemory(&indexBufferDesc, sizeof(indexBufferDesc));

	indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	indexBufferDesc.ByteWidth = sizeof(DWORD) * m_faceCount * 3;
	indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	indexBufferDesc.CPUAccessFlags = 0;
	indexBufferDesc.MiscFlags = 0;

	D3D11_SUBRESOURCE_DATA iinitData;

	iinitData.pSysMem = &indices[0];
	result = device->CreateBuffer(&indexBufferDesc, &iinitData, &m_indexBuffer);
	if (FAILED(result))
	{
		return false;
	}

	return true;
}


void SphereClass::ShutdownBuffers()
{
	// Release the index buffer.
	if (m_indexBuffer)
	{
		m_indexBuffer->Release();
		m_indexBuffer = 0;
	}

	// Release the vertex buffer.
	if (m_vertexBuffer)
	{
		m_vertexBuffer->Release();
		m_vertexBuffer = 0;
	}

	return;
}


void SphereClass::RenderBuffers(ID3D11DeviceContext* deviceContext)
{
	unsigned int stride;
	unsigned int offset;


	// Set vertex buffer stride and offset.
	stride = sizeof(VertexType);
	offset = 0;

	// Set the vertex buffer to active in the input assembler so it can be rendered.
	deviceContext->IASetVertexBuffers(0, 1, &m_vertexBuffer, &stride, &offset);

	// Set the index buffer to active in the input assembler so it can be rendered.
	deviceContext->IASetIndexBuffer(m_indexBuffer, DXGI_FORMAT_R32_UINT, 0);

	// Set the type of primitive that should be rendered from this vertex buffer, in this case triangles.
	deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	return;
}


bool SphereClass::LoadTexture(ID3D11Device* device, const WCHAR* filename)
{
	bool result;


	// Create the texture object.
	m_Texture = new TextureClass;
	if (!m_Texture)
	{
		return false;
	}

	// Initialize the texture object.
	result = m_Texture->Initialize(device, filename);
	if (!result)
	{
		return false;
	}

	return true;
}


void SphereClass::ReleaseTexture()
{
	// Release the texture object.
	if (m_Texture)
	{
		m_Texture->Shutdown();
		delete m_Texture;
		m_Texture = 0;
	}

	return;
}