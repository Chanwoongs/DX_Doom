////////////////////////////////////////////////////////////////////////////////
// Filename: graphicsclass.h
////////////////////////////////////////////////////////////////////////////////
#ifndef _GRAPHICSCLASS_H_
#define _GRAPHICSCLASS_H_


//////////////
// INCLUDES //
//////////////
#include <vector>


///////////////////////
// MY CLASS INCLUDES //
///////////////////////
#include "d3dclass.h"
#include "cameraclass.h"
#include "modelclass.h"
#include "lightshaderclass.h"
#include "lightclass.h"
#include "textureshaderclass.h"
#include "bitmapclass.h"
#include "textclass.h"
#include "spriteclass.h"
#include "model2Dclass.h"
#include "enemyclass.h"


/////////////
// GLOBALS //
/////////////
const bool FULL_SCREEN = false;
const bool VSYNC_ENABLED = true;
const float SCREEN_DEPTH = 1000.0f;
const float SCREEN_NEAR = 0.1f;


////////////////////////////////////////////////////////////////////////////////
// Class name: GraphicsClass
////////////////////////////////////////////////////////////////////////////////
class GraphicsClass
{
public:
	GraphicsClass();
	GraphicsClass(const GraphicsClass&);
	~GraphicsClass();

	bool Initialize(int, int, HWND);
	void Shutdown();
	bool Frame();

	void toggleAmbient();
	void toggleDiffuse();
	void toggleSpecular();

	void SetPlanePosition();

	CameraClass* GetCamera();

private:
	bool Render(float);

private:
	D3DClass* m_D3D;
	CameraClass* m_Camera;

	ModelClass* m_Plane;

	TextureShaderClass* m_TextureShader;
	XMMATRIX m_BaseViewMatrix;

	int m_ScreenWidth, m_ScreenHeight;
	BitmapClass* m_Crosshair;

	TextClass* m_Text;

	LightShaderClass* m_LightShader;
	LightClass* m_Light;

	bool m_ambient;
	bool m_diffuse;
	bool m_specular;

	XMFLOAT3* m_planePosition;
	XMFLOAT3* m_monsterPosition;
	int m_planeCount;

	Model2DClass* m_Enemy;
	const WCHAR** m_textureFileNames;

	EnemyClass* m_Zombie;
	const WCHAR*** m_zombieTextureNames;
	int m_zombieAnimationCount;
	int m_zombieCurrentAnimationIndex;
	int* m_zombieMaxFrame;
	int frameNum;
};

#endif