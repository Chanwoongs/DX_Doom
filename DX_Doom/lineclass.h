////////////////////////////////////////////////////////////////////////////////
// Filename: lineclass.h
////////////////////////////////////////////////////////////////////////////////
#ifndef _LINECLASS_H_
#define _LINECLASS_H_


//////////////
// INCLUDES //
//////////////
#include <d3d11.h>
#include <directxmath.h>

using namespace DirectX;


///////////////////////
// MY CLASS INCLUDES //
///////////////////////
#include "textureclass.h"


////////////////////////////////////////////////////////////////////////////////
// Class name: LineClass
////////////////////////////////////////////////////////////////////////////////
class LineClass
{
private:
	struct VertexType
	{
		XMFLOAT3 position;
		XMFLOAT2 texture;
	};

public:
	LineClass();
	LineClass(const LineClass&);
	~LineClass();

	bool Initialize(ID3D11Device*, XMFLOAT3);
	void Shutdown();
	void Render(ID3D11DeviceContext*);

	int GetIndexCount();
	ID3D11ShaderResourceView* GetTexture();

	bool UpdateTexture(ID3D11Device* device, const WCHAR* textureFilename);

private:
	bool InitializeBuffers(ID3D11Device*, XMFLOAT3);
	void ShutdownBuffers();
	void RenderBuffers(ID3D11DeviceContext*);

	bool LoadTexture(ID3D11Device*, const WCHAR*);
	void ReleaseTexture();


private:
	ID3D11Buffer* m_vertexBuffer, * m_indexBuffer;
	int m_vertexCount, m_indexCount;
	TextureClass* m_Texture;
};

#endif