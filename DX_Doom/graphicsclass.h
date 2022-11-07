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
#include "position.h"


/////////////
// GLOBALS //
/////////////
const bool FULL_SCREEN = false;
const bool VSYNC_ENABLED = true;
const float SCREEN_DEPTH = 1000.0f;
const float SCREEN_NEAR = 0.1f;

struct AnimationInfo
{
	const WCHAR*** textureNames;
	int animationCount;
	int currentAnimationIndex;
	int* maxFrame;
	int currentFrameNum;
};

struct Bitmaps
{
	vector <BitmapClass*> bitmaps;

};

struct BitmapInfo
{
	const WCHAR** textureNames;
	int maxFrame;
	int currentFrameNum;
	int* bitmapsHeight;
	int* bitmapsWidth;
	Position* bitmapsPos;
};


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
	bool Frame(int, int, float);

	void toggleAmbient();
	void toggleDiffuse();
	void toggleSpecular();

	void SetModelPosition();
	
	void SetModel2DAnimInfo(AnimationInfo&, int, int);

	void SetModels2DTextures();

	CameraClass* GetCamera();

	void StartShoot();
	void PlayGunAnim();
	void PlayMuzzleFlashAnim();
	void FinishShoot();

private:
	bool Render(float);
	XMMATRIX UpdateEnemyWalkingAnimation(EnemyClass*, AnimationInfo&);

private:
	D3DClass* m_D3D;
	CameraClass* m_Camera;

	int m_ScreenWidth, m_ScreenHeight;

	TextureShaderClass* m_TextureShader;

	LightShaderClass* m_LightShader;
	LightClass* m_Light;

	bool m_ambient;
	bool m_diffuse;
	bool m_specular;

	XMMATRIX m_BaseViewMatrix;

	BitmapClass* m_Crosshair;

	ModelClass* m_Plane;
	ModelClass* m_Stage;

	Bitmaps* m_Gun;
	BitmapInfo m_GunBitmapInfo;
	Bitmaps* m_MuzzleFlash;
	BitmapInfo m_MuzzleFlashBitmapInfo;

	TextClass* m_Text;

	XMFLOAT3* m_planePosition;
	int m_planeCount;

	XMFLOAT3* m_stagePosition;
	int m_stageCount;

	EnemyClass* m_Zombie;
	AnimationInfo m_ZombieAnimInfo;

	float m_acceptDistance;

	bool m_isShoot;
	bool m_isGunAnimPlay;
	bool m_isGunAnimReversed;
	bool m_isMuzzleAnimPlay;
};

#endif