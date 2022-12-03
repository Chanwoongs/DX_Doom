////////////////////////////////////////////////////////////////////////////////
// Filename: spriteclass.h
////////////////////////////////////////////////////////////////////////////////
#ifndef _SPRITECLASS_H_
#define _SPRITECLASS_H_


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
// Class name: SpriteClass
////////////////////////////////////////////////////////////////////////////////
class SpriteClass
{
private:
	struct VertexType
	{
		XMFLOAT3 position;
		XMFLOAT2 texture;
	};

public:
	SpriteClass();
	SpriteClass(const SpriteClass&);
	~SpriteClass();

	bool Initialize(ID3D11Device*, int, int, const WCHAR*);
	void Shutdown();
	bool Render(ID3D11DeviceContext*, int, int);

	int GetVertexCount() { return m_vertexCount; }
	int GetIndexCount();
	ID3D11ShaderResourceView* GetTexture();

private:
	bool InitializeBuffers(ID3D11Device*);
	void ShutdownBuffers();
	bool UpdateBuffers(ID3D11DeviceContext*, float, float);
	void RenderBuffers(ID3D11DeviceContext*);

	bool LoadTexture(ID3D11Device*, const WCHAR*);
	void ReleaseTexture();

private:
	ID3D11Buffer* m_vertexBuffer, * m_indexBuffer;
	int m_vertexCount, m_indexCount;
	TextureClass* m_Texture;
	int m_spriteWidth, m_spriteHeight;
	int m_previousPosX, m_previousPosY;
};

#endif