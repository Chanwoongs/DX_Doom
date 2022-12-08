////////////////////////////////////////////////////////////////////////////////
// Filename: graphicsclass.h
////////////////////////////////////////////////////////////////////////////////
#ifndef _GRAPHICSCLASS_H_
#define _GRAPHICSCLASS_H_


//////////////
// INCLUDES //
//////////////
#include <vector>
#include <DirectXCollision.h>

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
#include "sphereclass.h"
#include "astarclass.h"
#include "nodeclass.h"
#include "navmeshclass.h"
#include "skyboxshaderclass.h"
#include "bulletpoolclass.h"
#include "particleshaderclass.h"
#include "particlesystemclass.h"
#include "soundclass.h"

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
	
	void SetZombieAnimInfo(AnimationInfo&, int);

	void SetModels2DTextures(int);

	CameraClass* GetCamera();
	bool IsForwardHit() { return m_forwardHit == true; }
	bool IsBackwardHit() { return m_backwardHit == true; }
	bool IsRightHit() { return m_rightHit == true; }
	bool IsLeftHit() { return m_leftHit == true; }

	void StartShoot();
	void ShootBullet();
	void PlayGunAnim();
	void PlayMuzzleFlashAnim();
	void FinishShoot();

	void UpdateStageNum();

	bool SameSide(XMFLOAT3, XMFLOAT3, XMFLOAT3, XMFLOAT3);
	bool PointInTriangle(XMFLOAT3, XMFLOAT3, XMFLOAT3, XMFLOAT3);

	int GetSceneNum() { return m_SceneNum; }
	void SetSceneNum(int num) { m_SceneNum = num; }

private:
	bool Render(float);
	XMMATRIX UpdateEnemyWalkingAnimation(EnemyClass*, AnimationInfo&, float);

private:
	D3DClass* m_D3D;
	CameraClass* m_Camera;
	bool m_forwardHit;
	bool m_backwardHit;
	bool m_rightHit;
	bool m_leftHit;

	BoundingBox m_PlayerBox;

	int m_ScreenWidth, m_ScreenHeight;

	TextureShaderClass* m_TextureShader;

	LightShaderClass* m_LightShader;
	LightClass* m_Light;

	SkyboxShaderClass* m_SkyboxShader;
	SphereClass* m_Sphere;

	ParticleShaderClass* m_ParticleShader;
	ParticleSystemClass* m_ParticleSystem;
	XMFLOAT3* m_ParticlePosition;
	int m_ParticleCount;

	bool m_ambient;
	bool m_diffuse;
	bool m_specular;

	XMMATRIX m_BaseViewMatrix;

	BitmapClass* m_Title;
	BitmapClass* m_Clear;
	BitmapClass* m_Fail;
	BitmapClass* m_Crosshair;
	BitmapClass* m_Hit;

	SoundClass* m_BGM;
	SoundClass* m_FireSound;
	SoundClass* m_HittedSound;
	SoundClass* m_EnemyHittedSound;
	SoundClass* m_EnemyDieSound;
	SoundClass* m_StageClearSound;
	SoundClass* m_AllClearSound;

	NavmeshClass* m_Navmesh;
	ModelClass* m_Cube;

	ModelClass* m_Plane;
	ModelClass* m_Stage;
	ModelClass* m_NavmeshModel;
	vector<BoundingBox*> m_StageBox;
	vector<BoundingBox*> m_DoorBox;

	Bitmaps* m_Gun;
	BitmapInfo m_GunBitmapInfo;
	Bitmaps* m_MuzzleFlash;
	BitmapInfo m_MuzzleFlashBitmapInfo;

	BulletPoolClass* m_BulletPool;

	TextClass* m_Text;

	XMFLOAT3* m_planePosition;
	int m_planeCount;

	XMFLOAT3* m_stagePosition;
	int m_stageCount;

	XMFLOAT3* m_navmeshPosition;
	int m_navmeshCount;

	vector<EnemyClass*> m_Zombies;
	vector<AnimationInfo*> m_ZombieAnimInfos;
	EnemyClass* m_Zombie;
	AnimationInfo m_ZombieAnimInfo;
	int m_zombieInterval;
	int m_zombieCount;

	bool m_isShoot;
	bool m_isGunAnimPlay;
	bool m_isGunAnimReversed;
	bool m_isMuzzleAnimPlay;
	bool m_isBulletReloaded;
	bool m_playerHit;
	float m_hitTimer;

	float m_playerHP;
	int m_aliveEnemies;
	int m_currentStage;
	int m_SceneNum;
};

#endif