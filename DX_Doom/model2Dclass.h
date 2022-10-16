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
#include "spriteclass.h"


////////////////////////////////////////////////////////////////////////////////
// Class name: Model2DClass
////////////////////////////////////////////////////////////////////////////////
class Model2DClass
{
private:
	struct Animation
	{
		vector<SpriteClass*> sprites;
	};

public:
	Model2DClass();
	Model2DClass(const Model2DClass&);
	~Model2DClass();

	bool Initialize(ID3D11Device*, int, int[], int, int, const WCHAR**[]);
	void Shutdown();
	bool Render(ID3D11DeviceContext*, int, int, int, int);

	vector<Animation*> GetAnimations() { return m_animations; }
	int GetMaxFrameNum(int animationIndex) { return m_maxFrame[animationIndex]; }
	int GetSpriteIndexCount(int, int);
	ID3D11ShaderResourceView* GetSpriteTexture(int, int);

private:
	ID3D11Buffer* m_vertexBuffer, * m_indexBuffer;
	int m_spriteWidth, m_spriteHeight;
	int m_previousPosX, m_previousPos;

	vector<Animation*> m_animations;
	int m_animationCount;
	int* m_maxFrame;
	int m_currentAnimationIndex;
	int m_currentSpriteIndex;

};

#endif