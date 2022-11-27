////////////////////////////////////////////////////////////////////////////////
// Filename: sphereclass.h
////////////////////////////////////////////////////////////////////////////////
#ifndef _SPHERECLASS_H_
#define _SPHERECLASS_H_


//////////////
// INCLUDES //
//////////////
#include <d3d11.h>
#include <directxmath.h>
#include <vector>

using namespace DirectX;


///////////////////////
// MY CLASS INCLUDES //
///////////////////////
#include "textureclass.h"

////////////////////////////////////////////////////////////////////////////////
// Class name: SphereClass
////////////////////////////////////////////////////////////////////////////////
class SphereClass
{
private:
	struct VertexType
	{
		VertexType() {}
		VertexType(float x, float y, float z,
			float u, float v,
			float nx, float ny, float nz)
			: pos(x, y, z), texCoord(u, v), normal(nx, ny, nz) {}

		XMFLOAT3 pos;
		XMFLOAT2 texCoord;
		XMFLOAT3 normal;
	};
public:
	SphereClass();
	SphereClass(const SphereClass&);
	~SphereClass();

	bool Initialize(ID3D11Device*, const WCHAR*, int, int);
	void Shutdown();
	void Render(ID3D11DeviceContext*);

	int GetIndexCount();
	ID3D11ShaderResourceView* GetTexture();
	 

private:
	bool InitializeBuffers(ID3D11Device*, int, int);
	void ShutdownBuffers();
	void RenderBuffers(ID3D11DeviceContext*);

	bool LoadTexture(ID3D11Device*, const WCHAR*);
	void ReleaseTexture();

private:
	ID3D11Buffer* m_vertexBuffer, * m_indexBuffer;
	int m_vertexCount, m_faceCount;
	TextureClass* m_Texture;
};

#endif