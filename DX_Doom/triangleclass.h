////////////////////////////////////////////////////////////////////////////////
// Filename: triangleclass.h
////////////////////////////////////////////////////////////////////////////////
#ifndef _TRIANGLECLASS_H_
#define _TRIANGLECLASS_H_


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
// Class name: TriangleClass
////////////////////////////////////////////////////////////////////////////////
class TriangleClass
{
private:
	struct VertexType
	{
		XMFLOAT3 position;
		XMFLOAT2 texture;
	};

public:
	TriangleClass();
	TriangleClass(const TriangleClass&);
	~TriangleClass();

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