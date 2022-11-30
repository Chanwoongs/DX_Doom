////////////////////////////////////////////////////////////////////////////////
// Filename: skyboxshaderclass.cpp
////////////////////////////////////////////////////////////////////////////////
#include "skyboxshaderclass.h"


SkyboxShaderClass::SkyboxShaderClass()
{
	VS = 0;
	PS = 0;
	D2D_PS = 0;
	SKYMAP_VS = 0;
	SKYMAP_PS = 0;
	m_layout = 0;
	m_sampleState = 0;

	m_numOfTexture = 0;
}


SkyboxShaderClass::SkyboxShaderClass(const SkyboxShaderClass& other)
{
}


SkyboxShaderClass::~SkyboxShaderClass()
{
}


bool SkyboxShaderClass::Initialize(ID3D11Device* device, HWND hwnd)
{
	bool result;


	// Initialize the vertex and pixel shaders.
	result = InitializeShader(device, hwnd, L"./data/Effect.fx");
	if (!result)
	{
		return false;
	}

	return true;
}


void SkyboxShaderClass::Shutdown()
{
	// Shutdown the vertex and pixel shaders as well as the related objects.
	ShutdownShader();

	return;
}


bool SkyboxShaderClass::Render(ID3D11DeviceContext* deviceContext, int indexCount, XMMATRIX worldMatrix, XMMATRIX viewMatrix,
	XMMATRIX projectionMatrix, ID3D11ShaderResourceView* texture)
{
	bool result;


	// Set the shader parameters that it will use for rendering.
	result = SetShaderParameters(deviceContext, worldMatrix, viewMatrix, projectionMatrix, texture);
	if (!result)
	{
		return false;
	}

	// Now render the prepared buffers with the shader.
	RenderShader(deviceContext, indexCount);

	return true;
}


bool SkyboxShaderClass::InitializeShader(ID3D11Device* device, HWND hwnd,
	const WCHAR* filename)
{
	HRESULT result;
	ID3D10Blob* errorMessage;
	ID3D10Blob* D2D_PS_Buffer;
	ID3D10Blob* VS_Buffer;
	ID3D10Blob* PS_Buffer;
	ID3D10Blob* SKYMAP_VS_Buffer;
	ID3D10Blob* SKYMAP_PS_Buffer;
	D3D11_INPUT_ELEMENT_DESC polygonLayout[3];
	unsigned int numElements;
	D3D11_BUFFER_DESC matrixBufferDesc;
	D3D11_BUFFER_DESC textureBufferDesc;
	D3D11_BUFFER_DESC cbbd;
	D3D11_SAMPLER_DESC samplerDesc;


	// Initialize the pointers this function will use to null.
	errorMessage = 0;
	SKYMAP_VS_Buffer = 0;
	SKYMAP_PS_Buffer = 0;
	VS_Buffer = 0;
	PS_Buffer = 0;
	D2D_PS_Buffer = 0;

	result = D3DCompileFromFile(L"Effects.fx", NULL, NULL, "VS", "vs_5_0", D3D10_SHADER_ENABLE_STRICTNESS, 0,
		&VS_Buffer, &errorMessage);
	if (FAILED(result))
	{
		// If the shader failed to compile it should have writen something to the error message.
		if (errorMessage)
		{
			OutputShaderErrorMessage(errorMessage, hwnd, L"Effects.fx");
		}
		// If there was nothing in the error message then it simply could not find the shader file itself.
		else
		{
			MessageBox(hwnd, L"Effects.fx", L"Missing Shader File", MB_OK);
		}

		return false;
	}

	result = D3DCompileFromFile(L"Effects.fx", NULL, NULL, "PS", "ps_5_0", D3D10_SHADER_ENABLE_STRICTNESS, 0,
		&PS_Buffer, &errorMessage);
	if (FAILED(result))
	{
		// If the shader failed to compile it should have writen something to the error message.
		if (errorMessage)
		{
			OutputShaderErrorMessage(errorMessage, hwnd, L"Effects.fx");
		}
		// If there was nothing in the error message then it simply could not find the shader file itself.
		else
		{
			MessageBox(hwnd, L"Effects.fx", L"Missing Shader File", MB_OK);
		}

		return false;
	}

	result = D3DCompileFromFile(L"Effects.fx", NULL, NULL, "D2D_PS", "ps_5_0", D3D10_SHADER_ENABLE_STRICTNESS, 0,
		&D2D_PS_Buffer, &errorMessage);
	if (FAILED(result))
	{
		// If the shader failed to compile it should have writen something to the error message.
		if (errorMessage)
		{
			OutputShaderErrorMessage(errorMessage, hwnd, L"Effects.fx");
		}
		// If there was nothing in the error message then it simply could not find the shader file itself.
		else
		{
			MessageBox(hwnd, L"Effects.fx", L"Missing Shader File", MB_OK);
		}

		return false;
	}

	// Compile the vertex shader code.
	result = D3DCompileFromFile(L"Effects.fx", NULL, NULL, "SKYMAP_VS", "vs_5_0", D3D10_SHADER_ENABLE_STRICTNESS, 0,
		&SKYMAP_VS_Buffer, &errorMessage);
	if (FAILED(result))
	{
		// If the shader failed to compile it should have writen something to the error message.
		if (errorMessage)
		{
			OutputShaderErrorMessage(errorMessage, hwnd, L"Effects.fx");
		}
		// If there was nothing in the error message then it simply could not find the shader file itself.
		else
		{
			MessageBox(hwnd, L"Effects.fx", L"Missing Shader File", MB_OK);
		}

		return false;
	}

	// Compile the pixel shader code.
	result = D3DCompileFromFile(L"Effects.fx", NULL, NULL, "SKYMAP_PS", "ps_5_0", D3D10_SHADER_ENABLE_STRICTNESS, 0,
		&SKYMAP_PS_Buffer, &errorMessage);
	if (FAILED(result))
	{
		// If the shader failed to compile it should have writen something to the error message.
		if (errorMessage)
		{
			OutputShaderErrorMessage(errorMessage, hwnd, L"Effects.fx");
		}
		// If there was  nothing in the error message then it simply could not find the file itself.
		else
		{
			MessageBox(hwnd, L"Effects.fx", L"Missing Shader File", MB_OK);
		}

		return false;
	}

	// Create the vertex shader from the buffer.
	result = device->CreateVertexShader(VS_Buffer->GetBufferPointer(), VS_Buffer->GetBufferSize(), NULL, &VS);
	if (FAILED(result))
	{
		return false;
	}

	// Create the vertex shader from the buffer.
	result = device->CreatePixelShader(PS_Buffer->GetBufferPointer(), PS_Buffer->GetBufferSize(), NULL, &PS);
	if (FAILED(result))
	{
		return false;
	}

	// Create the vertex shader from the buffer.
	result = device->CreatePixelShader(D2D_PS_Buffer->GetBufferPointer(), D2D_PS_Buffer->GetBufferSize(), NULL, &D2D_PS);
	if (FAILED(result))
	{
		return false;
	}

	// Create the vertex shader from the buffer.
	result = device->CreateVertexShader(SKYMAP_VS_Buffer->GetBufferPointer(), SKYMAP_VS_Buffer->GetBufferSize(), NULL, &SKYMAP_VS);
	if (FAILED(result))
	{
		return false;
	}

	// Create the pixel shader from the buffer.
	result = device->CreatePixelShader(SKYMAP_PS_Buffer->GetBufferPointer(), SKYMAP_PS_Buffer->GetBufferSize(), NULL, &SKYMAP_PS);
	if (FAILED(result))
	{
		return false;
	}

	// Create the vertex input layout description.
	// This setup needs to match the VertexType stucture in the ModelClass and in the shader.
	polygonLayout[0].SemanticName = "POSITION";
	polygonLayout[0].SemanticIndex = 0;
	polygonLayout[0].Format = DXGI_FORMAT_R32G32B32_FLOAT;
	polygonLayout[0].InputSlot = 0;
	polygonLayout[0].AlignedByteOffset = 0;
	polygonLayout[0].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	polygonLayout[0].InstanceDataStepRate = 0;

	polygonLayout[1].SemanticName = "TEXCOORD";
	polygonLayout[1].SemanticIndex = 0;
	polygonLayout[1].Format = DXGI_FORMAT_R32G32_FLOAT;
	polygonLayout[1].InputSlot = 0;
	polygonLayout[1].AlignedByteOffset = 12;
	polygonLayout[1].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	polygonLayout[1].InstanceDataStepRate = 0;

	polygonLayout[2].SemanticName = "NORMAL";
	polygonLayout[2].SemanticIndex = 0;
	polygonLayout[2].Format = DXGI_FORMAT_R32G32B32_FLOAT;
	polygonLayout[2].InputSlot = 0;
	polygonLayout[2].AlignedByteOffset = 20;
	polygonLayout[2].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	polygonLayout[2].InstanceDataStepRate = 0;

	// Get a count of the elements in the layout.
	numElements = sizeof(polygonLayout) / sizeof(polygonLayout[0]);

	// Create the vertex input layout.
	result = device->CreateInputLayout(polygonLayout, numElements, VS_Buffer->GetBufferPointer(),
		VS_Buffer->GetBufferSize(), &m_layout);
	if (FAILED(result))
	{
		return false;
	}

	// Release the vertex shader buffer and pixel shader buffer since they are no longer needed.
	VS_Buffer->Release();
	VS_Buffer = 0;

	PS_Buffer->Release();
	PS_Buffer = 0;

	SKYMAP_VS_Buffer->Release();
	SKYMAP_VS_Buffer = 0;

	SKYMAP_PS_Buffer->Release();
	SKYMAP_PS_Buffer = 0;

	D2D_PS_Buffer->Release();
	D2D_PS_Buffer = 0;

	ZeroMemory(&cbbd, sizeof(D3D11_BUFFER_DESC));

	cbbd.Usage = D3D11_USAGE_DEFAULT;
	cbbd.ByteWidth = sizeof(cbPerObject);
	cbbd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	cbbd.CPUAccessFlags = 0;
	cbbd.MiscFlags = 0;

	result = device->CreateBuffer(&cbbd, NULL, &cbPerObjectBuffer);
	if (FAILED(result))
	{
		return false;
	}

	// Describe the Sample State
	D3D11_SAMPLER_DESC sampDesc;
	ZeroMemory(&sampDesc, sizeof(sampDesc));
	sampDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	sampDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	sampDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	sampDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	sampDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
	sampDesc.MinLOD = 0;
	sampDesc.MaxLOD = D3D11_FLOAT32_MAX;

	// Create the Sample State
	result = device->CreateSamplerState(&sampDesc, &m_sampleState);
	if (FAILED(result))
	{
		return false;
	}

	return true;
}


void SkyboxShaderClass::ShutdownShader()
{
	// Release the sampler state.
	if (m_sampleState)
	{
		m_sampleState->Release();
		m_sampleState = 0;
	}

	if (cbPerObjectBuffer)
	{
		cbPerObjectBuffer->Release();
		cbPerObjectBuffer = 0;
	}

	// Release the layout.
	if (m_layout)
	{
		m_layout->Release();
		m_layout = 0;
	}

	// Release the pixel shader.
	if (SKYMAP_PS)
	{
		SKYMAP_PS->Release();
		SKYMAP_PS = 0;
	}

	// Release the vertex shader.
	if (SKYMAP_VS)
	{
		SKYMAP_VS->Release();
		SKYMAP_VS = 0;
	}

	// Release the vertex shader.
	if (VS)
	{
		VS->Release();
		VS = 0;
	}

	// Release the vertex shader.
	if (PS)
	{
		PS->Release();
		PS = 0;
	}

	// Release the vertex shader.
	if (D2D_PS)
	{
		D2D_PS->Release();
		D2D_PS = 0;
	}

	return;
}


void SkyboxShaderClass::OutputShaderErrorMessage(ID3D10Blob* errorMessage, HWND hwnd,
	const WCHAR* shaderFilename)
{
	char* compileErrors;
	unsigned long bufferSize, i;
	ofstream fout;


	// Get a pointer to the error message text buffer.
	compileErrors = (char*)(errorMessage->GetBufferPointer());

	// Get the length of the message.
	bufferSize = errorMessage->GetBufferSize();

	// Open a file to write the error message to.
	fout.open("shader-error.txt");

	// Write out the error message.
	for (i = 0; i < bufferSize; i++)
	{
		fout << compileErrors[i];
	}

	// Close the file.
	fout.close();

	// Release the error message.
	errorMessage->Release();
	errorMessage = 0;

	// Pop a message up on the screen to notify the user to check the text file for compile errors.
	MessageBox(hwnd, L"Error compiling shader.  Check shader-error.txt for message.", shaderFilename, MB_OK);

	return;
}


bool SkyboxShaderClass::SetShaderParameters(ID3D11DeviceContext* deviceContext, XMMATRIX worldMatrix, 
	XMMATRIX viewMatrix, XMMATRIX projectionMatrix, ID3D11ShaderResourceView* texture)
{
	// Set the world view projection matrix and send it to the constant buffer in effect file
	WVP = worldMatrix * viewMatrix * projectionMatrix;
	cbPerObj.WVP = XMMatrixTranspose(WVP);
	cbPerObj.World = XMMatrixTranspose(worldMatrix);
	deviceContext->UpdateSubresource(cbPerObjectBuffer, 0, NULL, &cbPerObj, 0, 0);
	deviceContext->VSSetConstantBuffers(0, 1, &cbPerObjectBuffer);
	// Send our skymap resource view to pixel shader
	deviceContext->PSSetShaderResources(0, 1, &texture);

	return true;
}


void SkyboxShaderClass::RenderShader(ID3D11DeviceContext* deviceContext, int numSphereFaces)
{
	// Set the vertex input layout.
	deviceContext->IASetInputLayout(m_layout);

	// Set the vertex and pixel shaders that will be used to render this triangle.
	deviceContext->VSSetShader(SKYMAP_VS, NULL, 0);
	deviceContext->PSSetShader(SKYMAP_PS, NULL, 0);

	// Set the sampler state in the pixel shader.
	deviceContext->PSSetSamplers(0, 1, &m_sampleState);

	// Render the triangle.
	deviceContext->DrawIndexed(numSphereFaces * 3, 0, 0);

	return;
}