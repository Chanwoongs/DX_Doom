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
#include <vector>

using namespace DirectX;
using namespace std;


///////////////////////
// MY CLASS INCLUDES //
///////////////////////
#include "textureclass.h"
#include "texturearrayclass.h"
#include "spriteclass.h"

struct Animation
{
	vector<SpriteClass*> sprites;
};

////////////////////////////////////////////////////////////////////////////////
// Class name: Model2DClass
////////////////////////////////////////////////////////////////////////////////
class Model2DClass
{
public:
	Model2DClass();
	Model2DClass(const Model2DClass&);
	~Model2DClass();

	bool Initialize(ID3D11Device*, int, int[], int, int, const WCHAR**[], const WCHAR*, const WCHAR*);
	void Shutdown();
	bool Render(ID3D11DeviceContext*, int, int, int, int);

	vector<Animation*> GetAnimations() { return m_animations; }
	int GetMaxFrameNum(int animationIndex) { return m_maxFrame[animationIndex]; }
	int GetSpriteIndexCount(int, int);
	ID3D11ShaderResourceView* GetSpriteTexture(int, int);
	ID3D11ShaderResourceView** GetTextureArray() { return m_TextureArray->GetTextureArray(); }
	bool UpdateTextures(ID3D11Device*, const WCHAR*);
	bool LoadTextures(ID3D11Device*, const WCHAR*, const WCHAR*, const WCHAR*);


private:
	ID3D11Buffer* m_vertexBuffer, * m_indexBuffer;
	int m_spriteWidth, m_spriteHeight;
	int m_previousPosX, m_previousPos;

	vector<Animation*> m_animations;
	int m_animationCount;
	int* m_maxFrame;
	int m_currentAnimationIndex;
	int m_currentSpriteIndex;

	TextureArrayClass* m_TextureArray;
	const WCHAR* m_multiTextureFilename;
	const WCHAR* m_bumpTextureFilename;

};

#endif