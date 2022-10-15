////////////////////////////////////////////////////////////////////////////////
// Filename: model2Dclass.h
////////////////////////////////////////////////////////////////////////////////
#ifndef _MODEL2DCLASS_H_
#define _MODEL2DCLASS_H_


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
#include "spriteclass.h"


////////////////////////////////////////////////////////////////////////////////
// Class name: Model2DClass
////////////////////////////////////////////////////////////////////////////////
class Model2DClass
{
private:
	struct Animation
	{
		SpriteClass* sprites;
	};

public:
	Model2DClass();
	Model2DClass(const Model2DClass&);
	~Model2DClass();

	bool Initialize(ID3D11Device*, int, int, const WCHAR*[]);
	void Shutdown();
	bool Render(ID3D11DeviceContext*, int, int, int);

	int GetMaxFrame() { return m_maxFrame; }
	int GetIndexCount();
	ID3D11ShaderResourceView* GetTexture();

private:
	ID3D11Buffer* m_vertexBuffer, * m_indexBuffer;
	int m_spriteWidth, m_spriteHeight;
	int m_previousPosX, m_previousPosY;

	Animation m_Idle;
	int m_maxFrame;
	int m_currentSpriteIndex;
};

#endif