////////////////////////////////////////////////////////////////////////////////
// Filename: graphicsclass.cpp
////////////////////////////////////////////////////////////////////////////////
#include "graphicsclass.h"

#ifndef _ZOMBIE_TEXTURE_

#define ZOMBIE_F 0
#define ZOMBIE_FL 1
#define ZOMBIE_L 2
#define ZOMBIE_BL 3
#define ZOMBIE_B 4
#define ZOMBIE_BR 5
#define ZOMBIE_R 6
#define ZOMBIE_FR 7

#define ZOMBIE_AF 8
#define ZOMBIE_AFL 9
#define ZOMBIE_AL 10
#define ZOMBIE_ABL 11
#define ZOMBIE_AB 12
#define ZOMBIE_ABR 13
#define ZOMBIE_AR 14
#define ZOMBIE_AFR 15

#define ZOMBIE_HF  16
#define ZOMBIE_HFL 17
#define ZOMBIE_HL  18
#define ZOMBIE_HBL 19
#define ZOMBIE_HB  20
#define ZOMBIE_HBR 21
#define ZOMBIE_HR  22
#define ZOMBIE_HFR 23

#define ZOMBIE_D 24

#endif 

GraphicsClass::GraphicsClass()
{
	m_D3D = 0;
	m_Camera = 0;	

	m_TextureShader = 0;
	m_Text = 0;

	m_LightShader = 0;
	m_Light = 0;

	m_ambient = true;
	m_diffuse = true;
	m_specular = true;

	m_planeCount = 1;
	m_planePosition = new XMFLOAT3[1];

	m_stageCount = 1;
	m_stagePosition = new XMFLOAT3[1];

	m_navmeshCount = 1;
	m_navmeshPosition = new XMFLOAT3[1];

	m_zombieInterval = 25;
	m_zombieCount = 4;

	m_isBulletReloaded = true;

	m_StageBox.push_back(new BoundingBox(XMFLOAT3(17.5f, 10.0f, 20.0f), XMFLOAT3(0.2f, 10.0f, 25.0f)));
	m_StageBox.push_back(new BoundingBox(XMFLOAT3(-17.5f, 10.0f, 20.0f), XMFLOAT3(0.2f, 10.0f, 25.0f)));
	m_StageBox.push_back(new BoundingBox(XMFLOAT3(0.0f, 10.0f, 21.0f), XMFLOAT3(7.0f, 10.0f, 6.0f)));
	m_StageBox.push_back(new BoundingBox(XMFLOAT3(23.0f, 10.0f, 45.0f), XMFLOAT3(20.0f, 10.0f, 6.5f)));
	m_StageBox.push_back(new BoundingBox(XMFLOAT3(-23.0f, 10.0f, 45.0f), XMFLOAT3(20.0f, 10.0f, 6.5f)));
	m_StageBox.push_back(new BoundingBox(XMFLOAT3(0.0f, 10.0f, 61.0f), XMFLOAT3(5.0f, 10.0f, 6.0f)));
	m_StageBox.push_back(new BoundingBox(XMFLOAT3(21.0f, 10.0f, 66.0f), XMFLOAT3(13.0f, 10.0f, 5.0f)));
	m_StageBox.push_back(new BoundingBox(XMFLOAT3(-21.0f, 10.0f, 66.0f), XMFLOAT3(13.0f, 10.0f, 5.0f)));

	m_playerHP = 100.0f;
	m_aliveEnemies = 4;
}


GraphicsClass::GraphicsClass(const GraphicsClass& other)
{
}


GraphicsClass::~GraphicsClass()
{
}


bool GraphicsClass::Initialize(int screenWidth, int screenHeight, HWND hwnd)
{
	bool result;
	m_ScreenWidth = screenWidth;
	m_ScreenHeight = screenHeight;

	// Create the Direct3D object.
	m_D3D = new D3DClass;
	if(!m_D3D)
	{
		return false;
	}

	// Initialize the Direct3D object.
	result = m_D3D->Initialize(screenWidth, screenHeight, VSYNC_ENABLED, hwnd, FULL_SCREEN, SCREEN_DEPTH, SCREEN_NEAR);
	if(!result)
	{
		MessageBox(hwnd, L"Could not initialize Direct3D.", L"Error", MB_OK);
		return false;
	}

	// Create the camera object.
	m_Camera = new CameraClass;
	if(!m_Camera)
	{
		return false;
	}

	// Set the initial position of the camera.
	m_Camera->SetPosition(0.0f, 2.0f, -5.0f);	// for cube
	// Initialize a base view matrix with the camera for 2D user interface rendering.
	m_Camera->Render();
	m_Camera->GetViewMatrix(m_BaseViewMatrix);
	m_Camera->SetPosition(0.0f, 2.0f, 0.0f);	// for cube

	// Create the texture shader object.
	m_TextureShader = new TextureShaderClass;
	if (!m_TextureShader)
	{
		return false;
	}

	// Initialize the texture shader object.
	result = m_TextureShader->Initialize(m_D3D->GetDevice(), hwnd);
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the texture shader object.", L"Error", MB_OK);
		return false;
	}

	// Create the text object.
	m_Text = new TextClass;
	if (!m_Text)
	{
		return false;
	}
	// Initialize the text object.
	result = m_Text->Initialize(m_D3D->GetDevice(), m_D3D->GetDeviceContext(), hwnd, screenWidth,
		screenHeight, m_BaseViewMatrix);
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the text object.", L"Error", MB_OK);
		return false;
	}
		
	// Create the light shader object.
	m_LightShader = new LightShaderClass;
	if (!m_LightShader)
	{
		return false;
	}

	// Initialize the light shader object.
	result = m_LightShader->Initialize(m_D3D->GetDevice(), hwnd);
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the light shader object.", L"Error", MB_OK);
		return false;
	}

	// Create the light object.
	m_Light = new LightClass;
	if (!m_Light)
	{
		return false;
	}

	// Initialize the light object.
	m_Light->SetAmbientColor(0.3f, 0.3f, 0.3f, 1.0f);
	m_Light->SetDiffuseColor(1.0f, 1.0f, 1.0f, 1.0f);
	m_Light->SetDirection(1.0f, -1.0f, 1.0f);
	m_Light->SetSpecularColor(1.0f, 1.0f, 1.0f, 1.0f);
	m_Light->SetSpecularPower(32.0f);

	// Create the skybox shader object.
	m_SkyboxShader = new SkyboxShaderClass;
	if (!m_SkyboxShader)
	{
		return false;
	}

	// Initialize the skybox shader object.
	result = m_SkyboxShader->Initialize(m_D3D->GetDevice(), hwnd);
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the skybox shader object.", L"Error", MB_OK);
		return false;
	}

	// Create the model object.
	SetModelPosition();
	m_Plane = new ModelClass(m_planePosition, m_planeCount, false);
	if (!m_Plane)
	{
		return false;
	}
	// Initialize the model object.
	result = m_Plane->Initialize(m_D3D->GetDevice(), L"./data/EM_Cube.obj", L"./data/ET_Plane.dds", L"./data/ET_Dirt.dds", L"./data/MT_White.dds");
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the model object.", L"Error", MB_OK);
		return false;
	}

	m_Stage = new ModelClass(m_stagePosition, m_stageCount, false);
	if (!m_Stage)
	{
		return false;
	}
	// Initialize the model object.
	result = m_Stage->Initialize(m_D3D->GetDevice(), L"./data/EM_Stage.obj", L"./data/ET_Stone.dds", L"./data/ET_Dirt.dds", L"./data/NT_Stone.dds");
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the model object.", L"Error", MB_OK);
		return false;
	}

	XMFLOAT3 temp = XMFLOAT3(0, 0, 0);
	m_Cube = new ModelClass(&temp, 1, false);
	if (!m_Cube)
	{
		return false;
	}
	// Initialize the model object.
	result = m_Cube->Initialize(m_D3D->GetDevice(), L"./data/EM_Cube.obj", L"./data/MT_White.dds", L"./data/MT_White.dds", L"./data/MT_White.dds");
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the model object.", L"Error", MB_OK);
		return false;
	}


	// Create the model object.
	m_Sphere = new SphereClass;
	if (!m_Sphere)
	{
		return false;
	}

	// Initialize the model object.
	result = m_Sphere->Initialize(m_D3D->GetDevice(), L"./data/ET_Skybox.dds", 10, 10);
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the sphere object.", L"Error", MB_OK);
		return false;
	}

	// Create the bitmap object.
	m_Crosshair = new BitmapClass;
	if (!m_Crosshair)
	{
		return false;
	}
	// Initialize the bitmap object.
	result = m_Crosshair->Initialize(m_D3D->GetDevice(), screenWidth, screenHeight,
		L"./data/MT_Crosshair.dds", 200, 200);
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the bitmap object.", L"Error", MB_OK);
		return false;
	}

	// Initialize Gun Info
	m_GunBitmapInfo.maxFrame = 4;
	m_GunBitmapInfo.bitmapsWidth = new int[m_GunBitmapInfo.maxFrame];
	m_GunBitmapInfo.bitmapsHeight = new int[m_GunBitmapInfo.maxFrame];
	m_GunBitmapInfo.bitmapsPos = new Position[m_GunBitmapInfo.maxFrame];

	m_GunBitmapInfo.bitmapsWidth[0] = 116;
	m_GunBitmapInfo.bitmapsHeight[0] = 92;
	m_GunBitmapInfo.bitmapsPos[0].x = m_ScreenWidth / 2 - (m_GunBitmapInfo.bitmapsWidth[0] / 2);
	m_GunBitmapInfo.bitmapsWidth[1] = 116;
	m_GunBitmapInfo.bitmapsHeight[1] = 120;
	m_GunBitmapInfo.bitmapsPos[1].x = m_ScreenWidth / 2 - (m_GunBitmapInfo.bitmapsWidth[1] / 2) - 50;
	m_GunBitmapInfo.bitmapsWidth[2] = 84;
	m_GunBitmapInfo.bitmapsHeight[2] = 148;
	m_GunBitmapInfo.bitmapsPos[2].x = m_ScreenWidth / 2 - (m_GunBitmapInfo.bitmapsWidth[2] / 2) - 100;
	m_GunBitmapInfo.bitmapsWidth[3] = 112;
	m_GunBitmapInfo.bitmapsHeight[3] = 128;
	m_GunBitmapInfo.bitmapsPos[3].x = m_ScreenWidth / 2 - (m_GunBitmapInfo.bitmapsWidth[3] / 2) - 150;

	m_GunBitmapInfo.textureNames = new const WCHAR * [m_GunBitmapInfo.maxFrame];

	m_GunBitmapInfo.textureNames[0] = L"./data/Gun/MT_Gun_1.dds";
	m_GunBitmapInfo.textureNames[1] = L"./data/Gun/MT_Gun_2.dds";
	m_GunBitmapInfo.textureNames[2] = L"./data/Gun/MT_Gun_3.dds";
	m_GunBitmapInfo.textureNames[3] = L"./data/Gun/MT_Gun_4.dds";

	m_Gun = new Bitmaps;

	for (int i = 0; i < m_GunBitmapInfo.maxFrame; i++)
	{
		m_Gun->bitmaps.push_back(new BitmapClass());
	}

	// Initialize the bitmap object.
	for (size_t i = 0; i < m_GunBitmapInfo.maxFrame; i++)
	{
		result = m_Gun->bitmaps.at(i)->Initialize(m_D3D->GetDevice(), screenWidth, screenHeight,
			m_GunBitmapInfo.textureNames[i], m_GunBitmapInfo.bitmapsWidth[i], m_GunBitmapInfo.bitmapsHeight[i]);
		if (!result)
		{
			MessageBox(hwnd, L"Could not initialize the bitmap object.", L"Error", MB_OK);
			return false;
		}
	}

	// Initialize Muzzle Flash Info
	m_MuzzleFlashBitmapInfo.maxFrame = 2;
	m_MuzzleFlashBitmapInfo.currentFrameNum = 0;
	m_MuzzleFlashBitmapInfo.bitmapsWidth = new int[m_MuzzleFlashBitmapInfo.maxFrame];
	m_MuzzleFlashBitmapInfo.bitmapsHeight = new int[m_MuzzleFlashBitmapInfo.maxFrame];
	m_MuzzleFlashBitmapInfo.bitmapsPos = new Position[m_MuzzleFlashBitmapInfo.maxFrame];
	
	m_MuzzleFlashBitmapInfo.bitmapsWidth[0] = 80;
	m_MuzzleFlashBitmapInfo.bitmapsHeight[0] = 50;
	m_MuzzleFlashBitmapInfo.bitmapsPos[0] = Position(m_ScreenWidth / 2 - (m_MuzzleFlashBitmapInfo.bitmapsWidth[0] / 2) + 16,
		m_ScreenHeight - 120);
	m_MuzzleFlashBitmapInfo.bitmapsWidth[1] = 80;
	m_MuzzleFlashBitmapInfo.bitmapsHeight[1] = 65;
	m_MuzzleFlashBitmapInfo.bitmapsPos[1] = Position(m_ScreenWidth / 2 - (m_MuzzleFlashBitmapInfo.bitmapsWidth[1] / 2) + 13,
		m_ScreenHeight - 125);

	m_MuzzleFlashBitmapInfo.textureNames = new const WCHAR*[m_MuzzleFlashBitmapInfo.maxFrame];
	m_MuzzleFlashBitmapInfo.textureNames[0] = L"./data/Gun/MT_MuzzleFlash_1.dds";
	m_MuzzleFlashBitmapInfo.textureNames[1] = L"./data/Gun/MT_MuzzleFlash_2.dds";

	m_MuzzleFlash = new Bitmaps;

	for (int i = 0; i < m_MuzzleFlashBitmapInfo.maxFrame; i++)
	{
		m_MuzzleFlash->bitmaps.push_back(new BitmapClass());
	}

	// Initialize the bitmap object.
	for (size_t i = 0; i < m_MuzzleFlashBitmapInfo.maxFrame; i++)
	{
		result = m_MuzzleFlash->bitmaps.at(i)->Initialize(m_D3D->GetDevice(), screenWidth, screenHeight,
			m_MuzzleFlashBitmapInfo.textureNames[i], m_MuzzleFlashBitmapInfo.bitmapsWidth[i],
			m_MuzzleFlashBitmapInfo.bitmapsHeight[i]);
		if (!result)
		{
			MessageBox(hwnd, L"Could not initialize the bitmap object.", L"Error", MB_OK);
			return false;
		}
	}

	// Create the bullet objects.
	m_BulletPool = new BulletPoolClass(10);
	if (!m_BulletPool)
	{
		return false;
	}
	// Initialize the bullet objects.
	result = m_BulletPool->Initialize(m_D3D->GetDevice(), 4, 4,
		L"./data/Gun/MT_Bullet.dds");
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the bullet object.", L"Error", MB_OK);
		return false;
	}

	// Initialize Enemies
	for (int i = 0; i < m_zombieCount; i++)
	{
		m_ZombieAnimInfos.push_back(new AnimationInfo);
	}
	for (int i = 0; i < m_zombieCount; i++)
	{
		SetZombieAnimInfo(*m_ZombieAnimInfos.at(i), 25);
		SetModels2DTextures(i);

		m_Zombies.push_back(new EnemyClass(100.0f, m_ZombieAnimInfos.at(i)->animationCount, m_ZombieAnimInfos.at(i)->maxFrame, 3, 3,
			m_ZombieAnimInfos.at(i)->textureNames, 0.5f, 1.0f, 0.5f));
	}
	m_Zombies.at(0)->SetPosition(4, 0, 30);
	m_Zombies.at(0)->SetForwardVector(0, 0, -1);
	m_Zombies.at(0)->SetAcceptDistance(2.0f);
	m_Zombies.at(0)->SetDetectRange(30.0f);
	m_Zombies.at(0)->SetAttackRange(3.0f);
	m_Zombies.at(0)->SetSpeed(1.0f);
	m_Zombies.at(0)->AddPath(XMFLOAT3(0, 0, 29));
	m_Zombies.at(0)->AddPath(XMFLOAT3(0, 0, 1));
	m_Zombies.at(0)->SetPathIndex(0);
	m_Zombies.at(0)->SetCurrentTargetPath(m_Zombies.at(0)->GetPath().at(m_Zombies.at(0)->GetPathIndex()));

	m_Zombies.at(1)->SetPosition(-41, 0, 54);
	m_Zombies.at(1)->SetForwardVector(0, 0, -1);
	m_Zombies.at(1)->SetAcceptDistance(2.0f);
	m_Zombies.at(1)->SetDetectRange(30.0f);
	m_Zombies.at(1)->SetAttackRange(3.0f);
	m_Zombies.at(1)->SetSpeed(1.0f);
	m_Zombies.at(1)->AddPath(XMFLOAT3(41, 0, 89));
	m_Zombies.at(1)->AddPath(XMFLOAT3(41, 0, 54));
	m_Zombies.at(1)->AddPath(XMFLOAT3(-41, 0, 98));
	m_Zombies.at(1)->AddPath(XMFLOAT3(-41, 0, 54));
	m_Zombies.at(1)->SetPathIndex(0);
	m_Zombies.at(1)->SetCurrentTargetPath(m_Zombies.at(1)->GetPath().at(m_Zombies.at(1)->GetPathIndex()));

	m_Zombies.at(2)->SetPosition(-10, 0, 88);
	m_Zombies.at(2)->SetForwardVector(0, 0, -1);
	m_Zombies.at(2)->SetAcceptDistance(2.0f);
	m_Zombies.at(2)->SetDetectRange(30.0f);
	m_Zombies.at(2)->SetAttackRange(3.0f);
	m_Zombies.at(2)->SetSpeed(1.0f);
	m_Zombies.at(2)->AddPath(XMFLOAT3(38, 0, 163));
	m_Zombies.at(2)->AddPath(XMFLOAT3(-10, 0, 88));
	m_Zombies.at(2)->SetPathIndex(0);
	m_Zombies.at(2)->SetCurrentTargetPath(m_Zombies.at(2)->GetPath().at(m_Zombies.at(2)->GetPathIndex()));

	m_Zombies.at(3)->SetPosition(36, 0, 178);
	m_Zombies.at(3)->SetForwardVector(0, 0, -1);
	m_Zombies.at(3)->SetAcceptDistance(2.0f);
	m_Zombies.at(3)->SetDetectRange(30.0f);
	m_Zombies.at(3)->SetAttackRange(3.0f);
	m_Zombies.at(3)->SetSpeed(1.0f);
	m_Zombies.at(3)->AddPath(XMFLOAT3(-36, 0, 165));
	m_Zombies.at(3)->AddPath(XMFLOAT3(-37, 0, 192));
	m_Zombies.at(3)->AddPath(XMFLOAT3(39, 0, 189));
	m_Zombies.at(3)->SetPathIndex(0);
	m_Zombies.at(3)->SetCurrentTargetPath(m_Zombies.at(3)->GetPath().at(m_Zombies.at(3)->GetPathIndex()));

	for (int i = 0; i < m_zombieCount; i++)
	{
		result = m_Zombies.at(i)->Initialize(m_D3D->GetDevice());
		if (!result)
		{
			MessageBox(hwnd, L"Could not initialize the bitmap object.", L"Error", MB_OK);
			return false;
		}
	}



	return true;
}

void GraphicsClass::toggleAmbient()
{
	if (m_Light->GetAmbientToggle() == 0.0f)
	{
		m_Light->SetToggleAmbient(1.0f);
	}
	else
	{
		m_Light->SetToggleAmbient(0.0f);
	}
}

void GraphicsClass::toggleDiffuse()
{
	if (m_Light->GetDiffuseToggle() == 0.0f)
	{
		m_Light->SetToggleDiffuse(1.0f);
	}
	else
	{
		m_Light->SetToggleDiffuse(0.0f);
	}
}

void GraphicsClass::toggleSpecular()
{
	if (m_Light->GetSpecularToggle() == 0.0f)
	{
		m_Light->SetToggleSpecular(1.0f);
	}
	else
	{
		m_Light->SetToggleSpecular(0.0f);
	}
}

void GraphicsClass::SetModelPosition()
{
	m_planePosition[0] = XMFLOAT3(0.0f, 0.0f, 0.0f);
	m_stagePosition[0] = XMFLOAT3(0.0f, 0.0f, 0.0f);
	m_navmeshPosition[0] = XMFLOAT3(0.0f, -8.0f, 0.0f);
}


CameraClass* GraphicsClass::GetCamera()
{
	return m_Camera;
}

void GraphicsClass::StartShoot()
{
	if (m_isShoot == false)
	{
		m_isShoot = true;
		m_isGunAnimPlay = true;
		PlayMuzzleFlashAnim();
		ShootBullet();
	}
}

void GraphicsClass::ShootBullet()
{
	if (!m_isBulletReloaded) return;

	XMMATRIX bulletMatrix = XMMatrixIdentity();
	XMFLOAT3 tempCamPos = m_Camera->GetPosition();
	XMVECTOR tempCamPosVec = XMLoadFloat3(&tempCamPos);

	m_BulletPool->Create(m_Camera->GetTargetVector() - tempCamPosVec, tempCamPosVec);
	m_isBulletReloaded = false;
}

void GraphicsClass::PlayGunAnim()
{
	if (m_isGunAnimPlay == true)
	{
		if (m_isGunAnimReversed == false)
		{
			m_GunBitmapInfo.currentFrameNum++;
		}
		else if (m_isGunAnimReversed == true)
		{
			m_GunBitmapInfo.currentFrameNum--;
		}
		
		if (m_GunBitmapInfo.currentFrameNum + 1 == m_GunBitmapInfo.maxFrame * 25)
		{
			m_isGunAnimReversed = true;
		}

		if (m_isGunAnimReversed == true && m_GunBitmapInfo.currentFrameNum == 0)
		{

			m_isGunAnimPlay = false;
			m_isGunAnimReversed = false;
			m_isShoot = false;

			m_MuzzleFlashBitmapInfo.currentFrameNum = 0;
			m_isMuzzleAnimPlay = false;
			m_isBulletReloaded = true;

			return;
		}
	}
}

void GraphicsClass::PlayMuzzleFlashAnim()
{
	m_isMuzzleAnimPlay = true;
}

void GraphicsClass::FinishShoot()
{
	m_isShoot = false;
}

bool GraphicsClass::SameSide(XMFLOAT3 p1, XMFLOAT3 p2, XMFLOAT3 a, XMFLOAT3 b)
{
	XMVECTOR p1V, p2V, aV, bV;
	p1V = XMLoadFloat3(&p1);
	p2V = XMLoadFloat3(&p2);
	aV = XMLoadFloat3(&a);
	bV = XMLoadFloat3(&b);

	auto cp1 = XMVector3Cross(bV - aV, p1V - aV);
	auto cp2 = XMVector3Cross(bV - aV, p2V - aV);

	if (XMVectorGetX(XMVector3Dot(cp1, cp2)) >= 0.0f)
	{
		return true;
	}

	return false;
}

bool GraphicsClass::PointInTriangle(XMFLOAT3 p, XMFLOAT3 a, XMFLOAT3 b, XMFLOAT3 c)
{
	if (SameSide(p, a, b, c) && SameSide(p, b, a, c) && SameSide(p, c, a, b))
	{
		return true;
	}
	return false;
}

void GraphicsClass::Shutdown()
{
	// Release the D3D object.
	if (m_D3D)
	{
		m_D3D->Shutdown();
		delete m_D3D;
		m_D3D = 0;
	}

	// Release the camera object.
	if (m_Camera)
	{
		delete m_Camera;
		m_Camera = 0;
	}

	// Release the texture shader object.
	if (m_TextureShader)
	{
		m_TextureShader->Shutdown();
		delete m_TextureShader;
		m_TextureShader = 0;
	}

	// Release the text object.
	if (m_Text)
	{
		m_Text->Shutdown();
		delete m_Text;
		m_Text = 0;
	}

	// Release the model object.
	if (m_Navmesh)
	{
		m_Navmesh->Shutdown();
		delete m_Navmesh;
		m_Navmesh = 0;
	}

	// Release the model object.
	if (m_Plane)
	{
		delete m_planePosition;
		m_Plane->Shutdown();
		delete m_Plane;
		m_Plane = 0;
	}

	// Release the model object.
	if (m_Stage)
	{
		delete m_stagePosition;
		m_Stage->Shutdown();
		delete m_Stage;
		m_Stage = 0;
	}

	// Release the model object.
	if (m_Cube)
	{
		m_Cube->Shutdown();
		delete m_Cube;
		m_Cube = 0;
	}

	// Release the model object.
	if (m_NavmeshModel)
	{
		delete m_navmeshPosition;
		m_NavmeshModel->Shutdown();
		delete m_NavmeshModel;
		m_NavmeshModel = 0;
	}

	// Release the model object.
	if (m_Sphere)
	{
		m_Sphere->Shutdown();
		delete m_Sphere;
		m_Sphere = 0;
	}

	// Release the model object.
	if (m_Crosshair)
	{
		m_Crosshair->Shutdown();
		delete m_Crosshair;
		m_Crosshair = 0;
	}

	// Release the model object.
	if (m_Gun)
	{
		delete[] m_GunBitmapInfo.bitmapsWidth;
		delete[] m_GunBitmapInfo.bitmapsHeight;
		delete[] m_GunBitmapInfo.bitmapsPos;
		delete[] m_GunBitmapInfo.textureNames;

		for (int i = 0; i < m_GunBitmapInfo.maxFrame; i++)
		{
			m_Gun->bitmaps.at(i)->Shutdown();
			delete m_Gun->bitmaps.at(i);
		}
		delete m_Gun;
		m_Gun = 0;
	}

	if (m_MuzzleFlash)
	{
		delete[] m_MuzzleFlashBitmapInfo.bitmapsWidth;
		delete[] m_MuzzleFlashBitmapInfo.bitmapsHeight;
		delete[] m_MuzzleFlashBitmapInfo.bitmapsPos;
		delete[] m_MuzzleFlashBitmapInfo.textureNames;

		for (int i = 0; i < m_MuzzleFlashBitmapInfo.maxFrame; i++)
		{
			m_MuzzleFlash->bitmaps.at(i)->Shutdown();
			delete m_MuzzleFlash->bitmaps.at(i);
		}
		delete m_MuzzleFlash;
		m_MuzzleFlash = 0;
	}

	// Release the bullet objects.
	if (m_BulletPool)
	{
		m_BulletPool->ShutDown();
		delete m_BulletPool;
		m_BulletPool = 0;
	}

	// Release the model object.
	if (m_Zombies.size() > 0)
	{
		for (int i = 0; i < m_zombieCount; i++)
		{
			for (int i = 0; i < m_ZombieAnimInfos.at(i)->animationCount; i++)
			{
				delete[] m_ZombieAnimInfos.at(i)->textureNames[i];
			}
			delete[] m_ZombieAnimInfos.at(i)->textureNames;

			m_Zombies.at(i)->Shutdown();
			delete m_Zombies.at(i);
		}
	}

	// Release the light object.
	if (m_Light)
	{
		delete m_Light;
		m_Light = 0;
	}

	// Release the light shader object.
	if (m_LightShader)
	{
		m_LightShader->Shutdown();
		delete m_LightShader;
		m_LightShader = 0;
	}
	
	return;
}

bool GraphicsClass::Frame(int fps, int cpu, float frameTime)
{
	bool result;
	static float bobAngle = 0.0f;

	auto deltaTime = frameTime;
	 
	// Set the frames per second.
	result = m_Text->SetFps(fps, m_D3D->GetDeviceContext());
	if (!result)
	{
		return false;
	}
	// Set the cpu usage.
	result = m_Text->SetCpu(cpu, m_D3D->GetDeviceContext());
	if (!result)
	{
		return false;
	}
	// Set the position.
	result = m_Text->SetPosition(m_Camera->GetPosition() , m_D3D->GetDeviceContext());
	if (!result)
	{
		return false;
	}
	// Set the deltaTime.
	result = m_Text->SetDeltaTime(deltaTime, m_D3D->GetDeviceContext());
	if (!result)
	{
		return false;
	}
	// Set the hp.
	result = m_Text->SetHp(m_playerHP, m_D3D->GetDeviceContext());
	if (!result)
	{
		return false;
	}
	// Set the remaining enemies.
	result = m_Text->SetNumEnemies(m_aliveEnemies, m_D3D->GetDeviceContext());
	if (!result)
	{
		return false;
	}


	// update camera Headbob
	bobAngle += XM_PI * 0.025f;
	if (bobAngle > 360.0f)
	{
		bobAngle = 0.0f;
	}
	
	m_Camera->StartHeadbob(bobAngle);
	m_PlayerBox.Center = m_Camera->GetPosition();
	m_PlayerBox.Extents = XMFLOAT3(0.5f, 5.0f, 0.5f);

	vector<BoundingBox*>::iterator iter;
	iter = m_StageBox.begin();
	for (; iter != m_StageBox.end(); iter++)
	{
		XMFLOAT3 camPos = m_Camera->GetPosition();
		XMVECTOR camPosVec = XMLoadFloat3(&camPos);
		float a = 0.0f;
		if ((*iter)->Intersects(camPosVec, m_Camera->GetForwardVector(), a))
		{
			if (a < 2.0f)
				m_forwardHit = true;
			else
				m_forwardHit = false;
		}
		if ((*iter)->Intersects(camPosVec, -m_Camera->GetForwardVector(), a))
		{
			if (a < 2.0f)
				m_backwardHit = true;
			else
				m_backwardHit = false;
		}
		if ((*iter)->Intersects(camPosVec, m_Camera->GetRightVector(), a))
		{
			if (a < 2.0f)
				m_rightHit = true;
			else
				m_rightHit = false;
		}
		if ((*iter)->Intersects(camPosVec, -m_Camera->GetRightVector(), a))
		{
			if (a < 2.0f)
				m_leftHit = true;
			else
				m_leftHit = false;
		}
	}

	// Play Gun Animation
	PlayGunAnim();

	// Update FSM
	for (int i = 0; i < m_zombieCount; i++)
	{
		m_Zombies.at(i)->SetTargetPosition(m_Camera->GetPosition());
		m_Zombies.at(i)->Update(deltaTime);
		if (m_Zombies.at(i)->IsStateChanged())
		{
			m_ZombieAnimInfos.at(i)->currentFrameNum = 0;
		}
	}

	// Render the graphics scene.
	result = Render(deltaTime);
	if(!result)
	{
		return false;
	}

	return true;
}

bool GraphicsClass::Render(float deltaTime)
{
	XMMATRIX worldMatrix, viewMatrix, projectionMatrix, orthoMatrix, translateMatrix, skyboxMatrix;
	bool result;
	XMFLOAT3 cameraPosition;
	double angle, tempAngle;
	float billboardRotation;
	
	// Clear the buffers to begin the scene.
	m_D3D->BeginScene(0.0f, 0.0f, 0.0f, 1.0f);

	// Generate the view matrix based on the camera's position.
	m_Camera->Render();
	// Get the world, view, and projection matrices from the camera and d3d objects.
	m_D3D->GetWorldMatrix(worldMatrix);
	m_D3D->GetProjectionMatrix(projectionMatrix);
	m_D3D->GetOrthoMatrix(orthoMatrix);
	m_Camera->GetViewMatrix(viewMatrix);

	// Skybox rendering
	m_D3D->TurnOffCulling();
	m_D3D->TurnOnDS();

	// Reset skyboxMatrix
	skyboxMatrix = XMMatrixIdentity();
	skyboxMatrix = XMMatrixScaling(5.0f, 5.0f, 5.0f) * 
		XMMatrixTranslation(m_Camera->GetPosition().x, m_Camera->GetPosition().y, m_Camera->GetPosition().z);

	// Put the model vertex and index buffers on the graphics pipeline to prepare them for drawing.
	m_Sphere->Render(m_D3D->GetDeviceContext());

	// Render the model using the texture shader.
	result = m_SkyboxShader->Render(m_D3D->GetDeviceContext(), m_Sphere->GetFaceCount(), 
		skyboxMatrix, viewMatrix, projectionMatrix,	m_Sphere->GetTexture());
	if (!result)
	{
		return false;
	}

	m_D3D->TurnOnCulling();
	m_D3D->TurnOffDS();

	/////////////////////////////////////////////////////// 3D Render
	m_Plane->Render(m_D3D->GetDeviceContext());

	// Render the model using the light shader.
	result = m_LightShader->Render(m_D3D->GetDeviceContext(), m_Plane->GetVertexCount(), m_Plane->GetInstanceCount(),
		worldMatrix * XMMatrixScaling(150.0f, 0.01f, 500.0f) * XMMatrixTranslation(0.0f, 0.0f, 100.0f), viewMatrix, projectionMatrix,
		m_Plane->GetTextureArray(),
		m_Light->GetDirection(), m_Light->GetAmbientColor(), m_Light->GetDiffuseColor(),
		m_Camera->GetPosition(), m_Light->GetSpecularColor(), m_Light->GetSpecularPower(),
		m_Light->GetAmbientToggle(), m_Light->GetDiffuseToggle(), m_Light->GetSpecularToggle(), 10);
	if (!result)
	{
		return false;
	}

	m_Stage->Render(m_D3D->GetDeviceContext());

	// Render the model using the light shader.
	result = m_LightShader->Render(m_D3D->GetDeviceContext(), m_Stage->GetVertexCount(), m_Stage->GetInstanceCount(),
		worldMatrix * XMMatrixScaling(0.5f, 0.2f, 0.5f) * XMMatrixTranslation(0.0f, 0.0f, 0.0f), viewMatrix, projectionMatrix,
		m_Stage->GetTextureArray(),
		m_Light->GetDirection(), m_Light->GetAmbientColor(), m_Light->GetDiffuseColor(),
		m_Camera->GetPosition(), m_Light->GetSpecularColor(), m_Light->GetSpecularPower(),
		m_Light->GetAmbientToggle(), m_Light->GetDiffuseToggle(), m_Light->GetSpecularToggle(), 10);
	if (!result)
	{
		return false;
	}

	/////////////////////////////////////////////////////// 2.5D Render
	// Turn on the alpha blending before rendering the text.
	m_D3D->TurnOnAlphaBlending();

	// billboarding Enemy
	for (int i = 0; i < m_zombieCount; i++)
	{
		// Zombie
		if (m_Zombies.at(i)->IsAlive())
		{
			XMMATRIX zombieBillboardWorldMatrix = UpdateEnemyWalkingAnimation(m_Zombies.at(i), *m_ZombieAnimInfos.at(i), deltaTime);

			// Put the bitmap vertex and index buffers on the graphics pipeline to prepare them for drawing.
			result = m_Zombies.at(i)->Render(m_D3D->GetDeviceContext(), 0, 0, m_ZombieAnimInfos.at(i)->currentAnimationIndex,
				m_ZombieAnimInfos.at(i)->currentFrameNum / m_zombieInterval);
			if (!result)
			{
				return false;
			}
			if (m_ZombieAnimInfos.at(i)->currentFrameNum + 1 == m_Zombies.at(i)->GetModel()->GetMaxFrameNum(m_ZombieAnimInfos.at(i)->currentAnimationIndex) * m_zombieInterval)
			{
				m_ZombieAnimInfos.at(i)->currentFrameNum = 0;
			}
			else m_ZombieAnimInfos.at(i)->currentFrameNum++;

			result = m_Zombies.at(i)->GetModel()->UpdateTextures(m_D3D->GetDevice(),
				m_ZombieAnimInfos.at(i)->textureNames[m_ZombieAnimInfos.at(i)->currentAnimationIndex][m_ZombieAnimInfos.at(i)->currentFrameNum / m_zombieInterval]);
			if (!result)
			{
				return false;
			}
			// Render the model using the light shader.
			// 신축 회전 이동 순
			result = m_LightShader->Render(m_D3D->GetDeviceContext(), 6, 1,
				zombieBillboardWorldMatrix,
				viewMatrix, projectionMatrix,
				m_Zombies.at(i)->GetModel()->GetTextureArray(),
				m_Light->GetDirection(), m_Light->GetAmbientColor(), m_Light->GetDiffuseColor(),
				m_Camera->GetPosition(), m_Light->GetSpecularColor(), m_Light->GetSpecularPower(),
				m_Light->GetAmbientToggle(), m_Light->GetDiffuseToggle(), m_Light->GetSpecularToggle(), 1);
			if (!result)
			{
				return false;
			}
		}

		for (int j = 0; j < m_Zombies.at(i)->GetShortestPathSize(); j++)
		{
			m_Cube->Render(m_D3D->GetDeviceContext());

			// Render the model using the light shader.
			result = m_LightShader->Render(m_D3D->GetDeviceContext(), m_Cube->GetVertexCount(), m_Cube->GetInstanceCount(),
				worldMatrix * XMMatrixScaling(0.2f, 0.2f, 0.2f) *
				XMMatrixTranslation(m_Zombies.at(i)->GetShortestPath()[j].x, m_Zombies.at(i)->GetShortestPath()[j].y, m_Zombies.at(i)->GetShortestPath()[j].z),
				viewMatrix, projectionMatrix,
				m_Cube->GetTextureArray(),
				m_Light->GetDirection(), m_Light->GetAmbientColor(), m_Light->GetDiffuseColor(),
				m_Camera->GetPosition(), m_Light->GetSpecularColor(), m_Light->GetSpecularPower(),
				m_Light->GetAmbientToggle(), m_Light->GetDiffuseToggle(), m_Light->GetSpecularToggle(), 1);
			if (!result)
			{
				return false;
			}
		}
	}

	// billboarding Bullet
	// Update and Render Bullet
	m_BulletPool->UpdateBullets();
	for (int i = 0; i < m_BulletPool->GetPoolSize(); i++)
	{
		if (!m_BulletPool->GetBullets()[i].IsUse()) continue;

		XMMATRIX bulletMatrix;
		bulletMatrix = m_BulletPool->GetBullets()[i].Update(deltaTime, m_Camera->GetPosition());

		result = m_BulletPool->GetBullets()[i].Render(m_D3D->GetDeviceContext(), -0.5f, 2.5f);

		// Render the bitmap with the texture shader.
		result = m_TextureShader->Render(m_D3D->GetDeviceContext(), 
			m_BulletPool->GetBullets()[i].GetModel()->GetIndexCount(),
			bulletMatrix, viewMatrix, projectionMatrix,
			m_BulletPool->GetBullets()[i].GetModel()->GetTexture());
		if (!result)
		{
			return false;
		}

		for (int i = 0; i < m_zombieCount; i++)
		{
			if (m_BulletPool->GetBullets()[i].GetBoundingSphere().Intersects(m_Zombies.at(i)->GetBoundingBox()))
			{
				m_BulletPool->GetBullets()[i].SetJustDied(true);
				m_Zombies.at(i)->SetHitted(true);
			}
		}
	}


	// Turn off alpha blending after rendering the text.
	m_D3D->TurnOffAlphaBlending();

	/////////////////////////////////////////////////////// 2D Image Render
	// Turn off the Z buffer to begin all 2D rendering.
	m_D3D->TurnZBufferOff();

	// Turn on the alpha blending before rendering the text.
	m_D3D->TurnOnAlphaBlending();

	// Put the bitmap vertex and index buffers on the graphics pipeline to prepare them for drawing.
	result = m_Crosshair->Render(m_D3D->GetDeviceContext(), m_ScreenWidth / 2 - 100, m_ScreenHeight / 2 - 100);
	if (!result)
	{
		return false;
	}
	// Render the bitmap with the texture shader.
	result = m_TextureShader->Render(m_D3D->GetDeviceContext(), m_Crosshair->GetIndexCount(),
		worldMatrix, m_BaseViewMatrix, orthoMatrix, m_Crosshair->GetTexture());
	if (!result)
	{
		return false;
	}

	// Play Muzzle Flash Anim
	if (m_isMuzzleAnimPlay == true && m_MuzzleFlashBitmapInfo.currentFrameNum != -1)
	{
		// Put the bitmap vertex and index buffers on the graphics pipeline to prepare them for drawing.
		result = m_MuzzleFlash->bitmaps.at(m_MuzzleFlashBitmapInfo.currentFrameNum / 10)->Render(m_D3D->GetDeviceContext(),
			m_MuzzleFlashBitmapInfo.bitmapsPos[m_MuzzleFlashBitmapInfo.currentFrameNum / 10].x,
			m_MuzzleFlashBitmapInfo.bitmapsPos[m_MuzzleFlashBitmapInfo.currentFrameNum / 10].y);
		if (!result)
		{
			return false;
		}
		// Render the bitmap with the texture shader.
		result = m_TextureShader->Render(m_D3D->GetDeviceContext(), m_MuzzleFlash->bitmaps.at(m_MuzzleFlashBitmapInfo.currentFrameNum / 10)->GetIndexCount(),
			worldMatrix, m_BaseViewMatrix, orthoMatrix, m_MuzzleFlash->bitmaps.at(m_MuzzleFlashBitmapInfo.currentFrameNum / 10)->GetTexture());
		if (!result)
		{
			return false;
		}
		m_MuzzleFlashBitmapInfo.currentFrameNum++;
		if (m_MuzzleFlashBitmapInfo.currentFrameNum == m_MuzzleFlashBitmapInfo.maxFrame * 10)
		{
			m_isMuzzleAnimPlay = false;
			m_MuzzleFlashBitmapInfo.currentFrameNum = -1;
		}
	}

	// Put the bitmap vertex and index buffers on the graphics pipeline to prepare them for drawing.
	result = m_Gun->bitmaps.at(m_GunBitmapInfo.currentFrameNum / 25)->Render(m_D3D->GetDeviceContext(),
		m_GunBitmapInfo.bitmapsPos[m_GunBitmapInfo.currentFrameNum / 25].x,
		m_ScreenHeight - m_GunBitmapInfo.bitmapsHeight[m_GunBitmapInfo.currentFrameNum / 25]);
	if (!result)
	{
		return false;
	}
	// Render the bitmap with the texture shader.
	result = m_TextureShader->Render(m_D3D->GetDeviceContext(), m_Gun->bitmaps.at(m_GunBitmapInfo.currentFrameNum / 25)->GetIndexCount(),
		worldMatrix, m_BaseViewMatrix, orthoMatrix, m_Gun->bitmaps.at(m_GunBitmapInfo.currentFrameNum / 25)->GetTexture());
	if (!result)
	{
		return false;
	}

	// Turn off alpha blending after rendering the text.
	m_D3D->TurnOffAlphaBlending();

	/////////////////////////////////////////////////////// 2D Text Render

	// Turn on the alpha blending before rendering the text.
	m_D3D->TurnOnAlphaBlending();

	// Render the text strings.
	result = m_Text->Render(m_D3D->GetDeviceContext(), worldMatrix, orthoMatrix);
	if (!result)
	{
		return false;
	}

	// Turn off alpha blending after rendering the text.
	m_D3D->TurnOffAlphaBlending();

	// Turn the Z buffer back on now that all 2D rendering has completed.
	m_D3D->TurnZBufferOn();

	// Present the rendered scene to the screen.
	m_D3D->EndScene();

	return true;
}

XMMATRIX GraphicsClass::UpdateEnemyWalkingAnimation(EnemyClass* enemy, AnimationInfo& anim, float deltaTime)
{
	XMMATRIX worldMatrix, translateMatrix;
	XMFLOAT3 cameraPosition;
	double angle, billboardAngle;
	float billboardRotation;

	m_D3D->GetWorldMatrix(worldMatrix);

	// Get the position of the camera.
	cameraPosition = XMFLOAT3(m_Camera->GetPosition().x, m_Camera->GetPosition().y, m_Camera->GetPosition().z);

	// Calculate the rotation that needs to be applied to the billboard model to face the current camera position using the arc tangent function.
	angle = atan2(enemy->GetPosition().x - cameraPosition.x, enemy->GetPosition().z - cameraPosition.z) * (180.0 / XM_PI);

	// Convert rotation into radians.
	billboardRotation = (float)angle * 0.0174532925f;

	// Update Zombie Path
	XMFLOAT3 enemyPos = enemy->GetPosition();
	XMVECTOR enemyPositionVec = XMLoadFloat3(&enemyPos);
	XMFLOAT3 enemyCurrentPath = enemy->GetCurrentTargetPath();
	XMVECTOR enemyCurrentPathVec = XMLoadFloat3(&enemyCurrentPath);	
	XMFLOAT3 targetPos = enemy->GetTargetPosition();
	XMVECTOR targetPositionVec = XMLoadFloat3(&targetPos);

	XMMATRIX enemyMoveMatrix = XMMatrixIdentity();
	XMVECTOR enemyDirection = XMVector3Normalize(enemyCurrentPathVec - enemyPositionVec);
	enemyMoveMatrix = XMMatrixTranslationFromVector(enemyDirection / deltaTime * enemy->GetSpeed() * 0.1f);
	enemyPositionVec = XMVector3Transform(enemyPositionVec, enemyMoveMatrix);
	XMFLOAT3 tempZombiePos;
	XMStoreFloat3(&tempZombiePos, enemyPositionVec);
	enemy->SetPosition(tempZombiePos);
	enemy->SetForwardVector(XMVector3Normalize(enemyCurrentPathVec - enemyPositionVec));

	XMMATRIX billboardWorldMatrix = worldMatrix;
	// Setup the rotation the billboard at the origin using the world matrix.
	billboardWorldMatrix *= XMMatrixRotationY(billboardRotation);
	// Setup the translation matrix from the billboard model.
	translateMatrix = XMMatrixTranslation(enemy->GetPosition().x, enemy->GetPosition().y, enemy->GetPosition().z);
	// Finally combine the rotation and translation matrices to create the final world matrix for the billboard model.
	billboardWorldMatrix = XMMatrixMultiply(billboardWorldMatrix, translateMatrix);

	// change animation
	XMVECTOR enemyToPlayerVec = XMVector3Normalize(XMVectorSet(cameraPosition.x - enemy->GetPosition().x, 0,
		cameraPosition.z - enemy->GetPosition().z, 0));
	billboardAngle = acos(XMVectorGetX(XMVector3Dot(enemyToPlayerVec, enemy->GetForwardVector()))) * (180.0 / XM_PI);
	float tempCross = XMVectorGetY(XMVector3Cross(enemy->GetForwardVector(), enemyToPlayerVec));

	// case Forward
	if (billboardAngle < 22.5f && billboardAngle >= 0.0f)
	{
		if (enemy->GetFSM()->CurrentState()->GetStateID() == PATROL || enemy->GetFSM()->CurrentState()->GetStateID() == APPROACH)
		{
			anim.currentAnimationIndex = ZOMBIE_F;
		}
		else if (enemy->GetFSM()->CurrentState()->GetStateID() == ATTACK)
		{
			if ((anim.currentFrameNum + 1) / m_zombieInterval >= 3)
			{
				float a = 0.0f;
				if (m_PlayerBox.Intersects(enemyPositionVec, enemyToPlayerVec, a))
				{
					if (a < 3.0f)
					{
						m_playerHP -= 10.0f;
						m_Camera->MoveBack(0.5f);
					}
				}
			}
			if (anim.currentFrameNum / m_zombieInterval == 2) enemy->SetAttacking(false);

			anim.currentAnimationIndex = ZOMBIE_AF;
		}
		else if (enemy->GetFSM()->CurrentState()->GetStateID() == HITTED)
		{
			if (anim.currentFrameNum / (m_zombieInterval - 1) == 1) 
				enemy->GetFSM()->ChangeState(enemy->GetFSM()->PreviousState());

			anim.currentAnimationIndex = ZOMBIE_HF;
		}
		else if (enemy->GetFSM()->CurrentState()->GetStateID() == DEAD)
		{
			if (anim.currentFrameNum / m_zombieInterval == 7)
			{
				enemy->SetAlive(false); 
			}

			anim.currentAnimationIndex = ZOMBIE_D;
		}
	}
	// case ForwardLeft
	else if (billboardAngle < 67.5f && billboardAngle >= 22.5f && tempCross < 0)
	{
		if (enemy->GetFSM()->CurrentState()->GetStateID() == PATROL || enemy->GetFSM()->CurrentState()->GetStateID() == APPROACH)
		{
			anim.currentAnimationIndex = ZOMBIE_FL;
		}
		else if (enemy->GetFSM()->CurrentState()->GetStateID() == ATTACK)
		{
			if (anim.currentFrameNum / m_zombieInterval == 2) enemy->SetAttacking(false);

			anim.currentAnimationIndex = ZOMBIE_AFL;
		}
		else if (enemy->GetFSM()->CurrentState()->GetStateID() == HITTED)
		{
			if (anim.currentFrameNum / (m_zombieInterval - 1) == 1) 
				enemy->GetFSM()->ChangeState(enemy->GetFSM()->PreviousState());

			anim.currentAnimationIndex = ZOMBIE_HFL;
		}
		else if (enemy->GetFSM()->CurrentState()->GetStateID() == DEAD)
		{
			if (anim.currentFrameNum / m_zombieInterval == 7)
			{
				enemy->SetAlive(false);
			}

			anim.currentAnimationIndex = ZOMBIE_D;
		}
	}
	// case Left
	else if (billboardAngle < 112.5f && billboardAngle >= 67.5f && tempCross < 0)
	{
		if (enemy->GetFSM()->CurrentState()->GetStateID() == PATROL || enemy->GetFSM()->CurrentState()->GetStateID() == APPROACH)
		{
			anim.currentAnimationIndex = ZOMBIE_L;
		}
		else if (enemy->GetFSM()->CurrentState()->GetStateID() == ATTACK)
		{
			if (anim.currentFrameNum / m_zombieInterval == 2) enemy->SetAttacking(false);

			anim.currentAnimationIndex = ZOMBIE_AL;
		}
		else if (enemy->GetFSM()->CurrentState()->GetStateID() == HITTED)
		{
			if (anim.currentFrameNum / (m_zombieInterval - 1) == 1) 
				enemy->GetFSM()->ChangeState(enemy->GetFSM()->PreviousState());

			anim.currentAnimationIndex = ZOMBIE_HL;
		}
		else if (enemy->GetFSM()->CurrentState()->GetStateID() == DEAD)
		{
			if (anim.currentFrameNum / m_zombieInterval == 7)
			{
				enemy->SetAlive(false);
			}

			anim.currentAnimationIndex = ZOMBIE_D;
		}
	}
	// case BackLeft
	else if (billboardAngle < 157.5f && billboardAngle >= 112.5f && tempCross < 0)
	{
		if (enemy->GetFSM()->CurrentState()->GetStateID() == PATROL || enemy->GetFSM()->CurrentState()->GetStateID() == APPROACH)
		{
			anim.currentAnimationIndex = ZOMBIE_BL;
		}
		else if (enemy->GetFSM()->CurrentState()->GetStateID() == ATTACK)
		{
			if (anim.currentFrameNum / m_zombieInterval == 2) enemy->SetAttacking(false);

			anim.currentAnimationIndex = ZOMBIE_ABL;
		}
		else if (enemy->GetFSM()->CurrentState()->GetStateID() == HITTED)
		{
			if (anim.currentFrameNum / (m_zombieInterval - 1) == 1) 
				enemy->GetFSM()->ChangeState(enemy->GetFSM()->PreviousState());

			anim.currentAnimationIndex = ZOMBIE_HBL;
		}
		else if (enemy->GetFSM()->CurrentState()->GetStateID() == DEAD)
		{
			if (anim.currentFrameNum / m_zombieInterval == 7)
			{
				enemy->SetAlive(false);
			}

			anim.currentAnimationIndex = ZOMBIE_D;
		}
	}
	// case Back
	else if (billboardAngle < 180.0f && billboardAngle >= 157.5f)
	{
		if (enemy->GetFSM()->CurrentState()->GetStateID() == PATROL || enemy->GetFSM()->CurrentState()->GetStateID() == APPROACH)
		{
			anim.currentAnimationIndex = ZOMBIE_B;
		}
		else if (enemy->GetFSM()->CurrentState()->GetStateID() == ATTACK)
		{
			if (anim.currentFrameNum / m_zombieInterval == 2) enemy->SetAttacking(false);

			anim.currentAnimationIndex = ZOMBIE_AB;
		}
		else if (enemy->GetFSM()->CurrentState()->GetStateID() == HITTED)
		{
			if (anim.currentFrameNum / (m_zombieInterval - 1) == 1) 
				enemy->GetFSM()->ChangeState(enemy->GetFSM()->PreviousState());

			anim.currentAnimationIndex = ZOMBIE_HB;
		}
		else if (enemy->GetFSM()->CurrentState()->GetStateID() == DEAD)
		{
			if (anim.currentFrameNum / m_zombieInterval == 7)
			{
				enemy->SetAlive(false);
			}

			anim.currentAnimationIndex = ZOMBIE_D;
		}
	}
	// case BackRight
	else if (billboardAngle < 157.5 && billboardAngle >= 112.5f && tempCross > 0)
	{
		if (enemy->GetFSM()->CurrentState()->GetStateID() == PATROL || enemy->GetFSM()->CurrentState()->GetStateID() == APPROACH)
		{
			anim.currentAnimationIndex = ZOMBIE_BR;
		}
		else if (enemy->GetFSM()->CurrentState()->GetStateID() == ATTACK)
		{
			if (anim.currentFrameNum / m_zombieInterval == 2) enemy->SetAttacking(false);

			anim.currentAnimationIndex = ZOMBIE_ABR;
		}
		else if (enemy->GetFSM()->CurrentState()->GetStateID() == HITTED)
		{
			if (anim.currentFrameNum / (m_zombieInterval - 1) == 1) 
				enemy->GetFSM()->ChangeState(enemy->GetFSM()->PreviousState());

			anim.currentAnimationIndex = ZOMBIE_HBR;
		}
		else if (enemy->GetFSM()->CurrentState()->GetStateID() == DEAD)
		{
			if (anim.currentFrameNum / m_zombieInterval == 7)
			{
				enemy->SetAlive(false);
			}

			anim.currentAnimationIndex = ZOMBIE_D;
		}
	}
	// case Right
	else if (billboardAngle < 112.5f && billboardAngle >= 67.5f && tempCross > 0)
	{
		if (enemy->GetFSM()->CurrentState()->GetStateID() == PATROL || enemy->GetFSM()->CurrentState()->GetStateID() == APPROACH)
		{
			anim.currentAnimationIndex = ZOMBIE_R;
		}
		else if (enemy->GetFSM()->CurrentState()->GetStateID() == ATTACK)
		{
			if (anim.currentFrameNum / m_zombieInterval == 2) enemy->SetAttacking(false);

			anim.currentAnimationIndex = ZOMBIE_AR;
		}
		else if (enemy->GetFSM()->CurrentState()->GetStateID() == HITTED)
		{
			if (anim.currentFrameNum / (m_zombieInterval - 1) == 1) 
				enemy->GetFSM()->ChangeState(enemy->GetFSM()->PreviousState());

			anim.currentAnimationIndex = ZOMBIE_HR;
		}
		else if (enemy->GetFSM()->CurrentState()->GetStateID() == DEAD)
		{
			if (anim.currentFrameNum / m_zombieInterval == 7)
			{
				enemy->SetAlive(false);
			}

			anim.currentAnimationIndex = ZOMBIE_D;
		}
	}
	// case ForwardRight
	else if (billboardAngle < 67.5f && billboardAngle >= 22.5f && tempCross > 0)
	{
		if (enemy->GetFSM()->CurrentState()->GetStateID() == PATROL || enemy->GetFSM()->CurrentState()->GetStateID() == APPROACH)
		{
			anim.currentAnimationIndex = ZOMBIE_FR;
		}
		else if (enemy->GetFSM()->CurrentState()->GetStateID() == ATTACK)
		{
			if (anim.currentFrameNum / m_zombieInterval == 2) enemy->SetAttacking(false);

			anim.currentAnimationIndex = ZOMBIE_AFR;
		}
		else if (enemy->GetFSM()->CurrentState()->GetStateID() == HITTED)
		{
			if (anim.currentFrameNum / (m_zombieInterval - 1) == 1) 
				enemy->GetFSM()->ChangeState(enemy->GetFSM()->PreviousState());

			anim.currentAnimationIndex = ZOMBIE_HFR;
		}
		else if (enemy->GetFSM()->CurrentState()->GetStateID() == DEAD)
		{
			if (anim.currentFrameNum / m_zombieInterval == 7)
			{
				enemy->SetAlive(false);
			}

			anim.currentAnimationIndex = ZOMBIE_D;
		}
	}

	return billboardWorldMatrix;
}

void GraphicsClass::SetZombieAnimInfo(AnimationInfo& anim, int animationCount)
{
	anim.animationCount = animationCount;
	anim.currentAnimationIndex = 0;
	anim.textureNames = new const WCHAR**[anim.animationCount];
	anim.maxFrame = new int[anim.animationCount];
	anim.currentFrameNum = 0;

	// Zombie Move
	for (int i = ZOMBIE_F; i < ZOMBIE_FR + 1; i++)
	{
		anim.maxFrame[i] = 4;
		anim.textureNames[i] = new const WCHAR*[anim.maxFrame[i]];
	}
	// Zombie Attack
	for (int i = ZOMBIE_AF; i < ZOMBIE_AFR + 1; i++)
	{
		anim.maxFrame[i] = 3;
		anim.textureNames[i] = new const WCHAR * [anim.maxFrame[i]];
	}
	// Zombie Hitted
	for (int i = ZOMBIE_HF; i < ZOMBIE_HFR + 1; i++)
	{
		anim.maxFrame[i] = 1;
		anim.textureNames[i] = new const WCHAR * [anim.maxFrame[i]];
	}
	// Zombie Die
	anim.maxFrame[ZOMBIE_D] = 8;
	anim.textureNames[ZOMBIE_D] = new const WCHAR * [anim.maxFrame[ZOMBIE_D]];
}


void GraphicsClass::SetModels2DTextures(int i)
{
	// Zombie Move
	m_ZombieAnimInfos.at(i)->textureNames[ZOMBIE_F][0] = L"./data/Zombie/MT_Zombie_F_1.dds";
	m_ZombieAnimInfos.at(i)->textureNames[ZOMBIE_F][1] = L"./data/Zombie/MT_Zombie_F_2.dds";
	m_ZombieAnimInfos.at(i)->textureNames[ZOMBIE_F][2] = L"./data/Zombie/MT_Zombie_F_3.dds";
	m_ZombieAnimInfos.at(i)->textureNames[ZOMBIE_F][3] = L"./data/Zombie/MT_Zombie_F_4.dds";

	m_ZombieAnimInfos.at(i)->textureNames[ZOMBIE_FL][0] = L"./data/Zombie/MT_Zombie_FL_1.dds";
	m_ZombieAnimInfos.at(i)->textureNames[ZOMBIE_FL][1] = L"./data/Zombie/MT_Zombie_FL_2.dds";
	m_ZombieAnimInfos.at(i)->textureNames[ZOMBIE_FL][2] = L"./data/Zombie/MT_Zombie_FL_3.dds";
	m_ZombieAnimInfos.at(i)->textureNames[ZOMBIE_FL][3] = L"./data/Zombie/MT_Zombie_FL_4.dds";

	m_ZombieAnimInfos.at(i)->textureNames[ZOMBIE_L][0] = L"./data/Zombie/MT_Zombie_L_1.dds";
	m_ZombieAnimInfos.at(i)->textureNames[ZOMBIE_L][1] = L"./data/Zombie/MT_Zombie_L_2.dds";
	m_ZombieAnimInfos.at(i)->textureNames[ZOMBIE_L][2] = L"./data/Zombie/MT_Zombie_L_3.dds";
	m_ZombieAnimInfos.at(i)->textureNames[ZOMBIE_L][3] = L"./data/Zombie/MT_Zombie_L_4.dds";

	m_ZombieAnimInfos.at(i)->textureNames[ZOMBIE_BL][0] = L"./data/Zombie/MT_Zombie_BL_1.dds";
	m_ZombieAnimInfos.at(i)->textureNames[ZOMBIE_BL][1] = L"./data/Zombie/MT_Zombie_BL_2.dds";
	m_ZombieAnimInfos.at(i)->textureNames[ZOMBIE_BL][2] = L"./data/Zombie/MT_Zombie_BL_3.dds";
	m_ZombieAnimInfos.at(i)->textureNames[ZOMBIE_BL][3] = L"./data/Zombie/MT_Zombie_BL_4.dds";

	m_ZombieAnimInfos.at(i)->textureNames[ZOMBIE_B][0] = L"./data/Zombie/MT_Zombie_B_1.dds";
	m_ZombieAnimInfos.at(i)->textureNames[ZOMBIE_B][1] = L"./data/Zombie/MT_Zombie_B_2.dds";
	m_ZombieAnimInfos.at(i)->textureNames[ZOMBIE_B][2] = L"./data/Zombie/MT_Zombie_B_3.dds";
	m_ZombieAnimInfos.at(i)->textureNames[ZOMBIE_B][3] = L"./data/Zombie/MT_Zombie_B_4.dds";

	m_ZombieAnimInfos.at(i)->textureNames[ZOMBIE_BR][0] = L"./data/Zombie/MT_Zombie_BR_1.dds";
	m_ZombieAnimInfos.at(i)->textureNames[ZOMBIE_BR][1] = L"./data/Zombie/MT_Zombie_BR_2.dds";
	m_ZombieAnimInfos.at(i)->textureNames[ZOMBIE_BR][2] = L"./data/Zombie/MT_Zombie_BR_3.dds";
	m_ZombieAnimInfos.at(i)->textureNames[ZOMBIE_BR][3] = L"./data/Zombie/MT_Zombie_BR_4.dds";

	m_ZombieAnimInfos.at(i)->textureNames[ZOMBIE_R][0] = L"./data/Zombie/MT_Zombie_R_1.dds";
	m_ZombieAnimInfos.at(i)->textureNames[ZOMBIE_R][1] = L"./data/Zombie/MT_Zombie_R_2.dds";
	m_ZombieAnimInfos.at(i)->textureNames[ZOMBIE_R][2] = L"./data/Zombie/MT_Zombie_R_3.dds";
	m_ZombieAnimInfos.at(i)->textureNames[ZOMBIE_R][3] = L"./data/Zombie/MT_Zombie_R_4.dds";

	m_ZombieAnimInfos.at(i)->textureNames[ZOMBIE_FR][0] = L"./data/Zombie/MT_Zombie_FR_1.dds";
	m_ZombieAnimInfos.at(i)->textureNames[ZOMBIE_FR][1] = L"./data/Zombie/MT_Zombie_FR_2.dds";
	m_ZombieAnimInfos.at(i)->textureNames[ZOMBIE_FR][2] = L"./data/Zombie/MT_Zombie_FR_3.dds";
	m_ZombieAnimInfos.at(i)->textureNames[ZOMBIE_FR][3] = L"./data/Zombie/MT_Zombie_FR_4.dds";

	// Zombie Attack
	m_ZombieAnimInfos.at(i)->textureNames[ZOMBIE_AF][0] = L"./data/Zombie/MT_Zombie_AF_1.dds";
	m_ZombieAnimInfos.at(i)->textureNames[ZOMBIE_AF][1] = L"./data/Zombie/MT_Zombie_AF_2.dds";
	m_ZombieAnimInfos.at(i)->textureNames[ZOMBIE_AF][2] = L"./data/Zombie/MT_Zombie_AF_3.dds";

	m_ZombieAnimInfos.at(i)->textureNames[ZOMBIE_AFL][0] = L"./data/Zombie/MT_Zombie_AFL_1.dds";
	m_ZombieAnimInfos.at(i)->textureNames[ZOMBIE_AFL][1] = L"./data/Zombie/MT_Zombie_AFL_2.dds";
	m_ZombieAnimInfos.at(i)->textureNames[ZOMBIE_AFL][2] = L"./data/Zombie/MT_Zombie_AFL_3.dds";

	m_ZombieAnimInfos.at(i)->textureNames[ZOMBIE_AL][0] = L"./data/Zombie/MT_Zombie_AL_1.dds";
	m_ZombieAnimInfos.at(i)->textureNames[ZOMBIE_AL][1] = L"./data/Zombie/MT_Zombie_AL_2.dds";
	m_ZombieAnimInfos.at(i)->textureNames[ZOMBIE_AL][2] = L"./data/Zombie/MT_Zombie_AL_3.dds";

	m_ZombieAnimInfos.at(i)->textureNames[ZOMBIE_ABL][0] = L"./data/Zombie/MT_Zombie_ABL_1.dds";
	m_ZombieAnimInfos.at(i)->textureNames[ZOMBIE_ABL][1] = L"./data/Zombie/MT_Zombie_ABL_2.dds";
	m_ZombieAnimInfos.at(i)->textureNames[ZOMBIE_ABL][2] = L"./data/Zombie/MT_Zombie_ABL_3.dds";

	m_ZombieAnimInfos.at(i)->textureNames[ZOMBIE_AB][0] = L"./data/Zombie/MT_Zombie_AB_1.dds";
	m_ZombieAnimInfos.at(i)->textureNames[ZOMBIE_AB][1] = L"./data/Zombie/MT_Zombie_AB_2.dds";
	m_ZombieAnimInfos.at(i)->textureNames[ZOMBIE_AB][2] = L"./data/Zombie/MT_Zombie_AB_3.dds";

	m_ZombieAnimInfos.at(i)->textureNames[ZOMBIE_ABR][0] = L"./data/Zombie/MT_Zombie_ABR_1.dds";
	m_ZombieAnimInfos.at(i)->textureNames[ZOMBIE_ABR][1] = L"./data/Zombie/MT_Zombie_ABR_2.dds";
	m_ZombieAnimInfos.at(i)->textureNames[ZOMBIE_ABR][2] = L"./data/Zombie/MT_Zombie_ABR_3.dds";

	m_ZombieAnimInfos.at(i)->textureNames[ZOMBIE_AR][0] = L"./data/Zombie/MT_Zombie_AR_1.dds";
	m_ZombieAnimInfos.at(i)->textureNames[ZOMBIE_AR][1] = L"./data/Zombie/MT_Zombie_AR_2.dds";
	m_ZombieAnimInfos.at(i)->textureNames[ZOMBIE_AR][2] = L"./data/Zombie/MT_Zombie_AR_3.dds";

	m_ZombieAnimInfos.at(i)->textureNames[ZOMBIE_AFR][0] = L"./data/Zombie/MT_Zombie_AFR_1.dds";
	m_ZombieAnimInfos.at(i)->textureNames[ZOMBIE_AFR][1] = L"./data/Zombie/MT_Zombie_AFR_2.dds";
	m_ZombieAnimInfos.at(i)->textureNames[ZOMBIE_AFR][2] = L"./data/Zombie/MT_Zombie_AFR_3.dds";

	// Zombie Hitted
	m_ZombieAnimInfos.at(i)->textureNames[ZOMBIE_HF][0] = L"./data/Zombie/MT_Zombie_HF.dds";

	m_ZombieAnimInfos.at(i)->textureNames[ZOMBIE_HFL][0] = L"./data/Zombie/MT_Zombie_HFL.dds";

	m_ZombieAnimInfos.at(i)->textureNames[ZOMBIE_HL][0] = L"./data/Zombie/MT_Zombie_HL.dds";

	m_ZombieAnimInfos.at(i)->textureNames[ZOMBIE_HBL][0] = L"./data/Zombie/MT_Zombie_HBL.dds";

	m_ZombieAnimInfos.at(i)->textureNames[ZOMBIE_HB][0] = L"./data/Zombie/MT_Zombie_HB.dds";

	m_ZombieAnimInfos.at(i)->textureNames[ZOMBIE_HBR][0] = L"./data/Zombie/MT_Zombie_HBR.dds";

	m_ZombieAnimInfos.at(i)->textureNames[ZOMBIE_HR][0] = L"./data/Zombie/MT_Zombie_HR.dds";

	m_ZombieAnimInfos.at(i)->textureNames[ZOMBIE_HFR][0] = L"./data/Zombie/MT_Zombie_HFR.dds";

	// Zombie Die
	m_ZombieAnimInfos.at(i)->textureNames[ZOMBIE_D][0] = L"./data/Zombie/MT_Zombie_D_1.dds";
	m_ZombieAnimInfos.at(i)->textureNames[ZOMBIE_D][1] = L"./data/Zombie/MT_Zombie_D_2.dds";
	m_ZombieAnimInfos.at(i)->textureNames[ZOMBIE_D][2] = L"./data/Zombie/MT_Zombie_D_3.dds";
	m_ZombieAnimInfos.at(i)->textureNames[ZOMBIE_D][3] = L"./data/Zombie/MT_Zombie_D_4.dds";
	m_ZombieAnimInfos.at(i)->textureNames[ZOMBIE_D][4] = L"./data/Zombie/MT_Zombie_D_5.dds";
	m_ZombieAnimInfos.at(i)->textureNames[ZOMBIE_D][5] = L"./data/Zombie/MT_Zombie_D_5.dds";
	m_ZombieAnimInfos.at(i)->textureNames[ZOMBIE_D][6] = L"./data/Zombie/MT_Zombie_D_5.dds";
	m_ZombieAnimInfos.at(i)->textureNames[ZOMBIE_D][7] = L"./data/Zombie/MT_Zombie_D_5.dds";
}
