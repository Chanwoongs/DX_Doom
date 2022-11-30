////////////////////////////////////////////////////////////////////////////////
// Filename: skyboxshaderclass.h
////////////////////////////////////////////////////////////////////////////////
#ifndef _SKYBOXSHADERCLASS_H_
#define _SKYBOXSHADERCLASS_H_


//////////////
// INCLUDES //
//////////////
#include <d3d11.h>
#include <directxmath.h>
#include <d3dcompiler.h>

#include <fstream>

using namespace std;
using namespace DirectX;

////////////////////////////////////////////////////////////////////////////////
// Class name: SkyboxShaderClass
////////////////////////////////////////////////////////////////////////////////
class SkyboxShaderClass
{
	struct cbPerObject
	{
		XMMATRIX  WVP;
		XMMATRIX World;
	};
public:
	SkyboxShaderClass();
	SkyboxShaderClass(const SkyboxShaderClass&);
	~SkyboxShaderClass();

	bool Initialize(ID3D11Device*, HWND);
	void Shutdown();
	bool Render(ID3D11DeviceContext*, int, XMMATRIX, XMMATRIX, XMMATRIX, ID3D11ShaderResourceView*);

private:
	bool InitializeShader(ID3D11Device*, HWND, const WCHAR*);
	void ShutdownShader();
	void OutputShaderErrorMessage(ID3D10Blob*, HWND, const WCHAR*);

	bool SetShaderParameters(ID3D11DeviceContext*, XMMATRIX, XMMATRIX, XMMATRIX, ID3D11ShaderResourceView*);
	void RenderShader(ID3D11DeviceContext*, int);

private:
	ID3D11VertexShader* VS;
	ID3D11PixelShader* PS;
	ID3D11PixelShader* D2D_PS;
	ID3D11VertexShader* SKYMAP_VS;
	ID3D11PixelShader* SKYMAP_PS;
	ID3D11InputLayout* m_layout;
	ID3D11Buffer* cbPerObjectBuffer;
	ID3D11SamplerState* m_sampleState;

	int m_numOfTexture;
	XMMATRIX WVP;
	cbPerObject cbPerObj;

};

#endif