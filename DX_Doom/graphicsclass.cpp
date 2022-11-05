////////////////////////////////////////////////////////////////////////////////
// Filename: graphicsclass.cpp
////////////////////////////////////////////////////////////////////////////////
#include "graphicsclass.h"


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
	m_Light->SetAmbientColor(0.15f, 0.15f, 0.15f, 1.0f);
	m_Light->SetDiffuseColor(1.0f, 1.0f, 1.0f, 1.0f);
	m_Light->SetDirection(1.0f, -0.5f, 1.0f);
	m_Light->SetSpecularColor(1.0f, 1.0f, 1.0f, 1.0f);
	m_Light->SetSpecularPower(32.0f);

	// Create the model object.
	SetModelPosition();
	m_Plane = new ModelClass(m_planePosition, m_planeCount);
	if (!m_Plane)
	{
		return false;
	}
	// Initialize the model object.
	result = m_Plane->Initialize(m_D3D->GetDevice(), L"./data/EM_Cube.obj", L"./data/ET_Plane.dds");
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the model object.", L"Error", MB_OK);
		return false;
	}

	m_Stage = new ModelClass(m_stagePosition, m_stageCount);
	if (!m_Stage)
	{
		return false;
	}
	// Initialize the model object.
	result = m_Stage->Initialize(m_D3D->GetDevice(), L"./data/EM_Stage.obj", L"./data/ET_Seafloor.dds");
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the model object.", L"Error", MB_OK);
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
		L"./data/MT_Crosshair.dds", 50, 50);
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the bitmap object.", L"Error", MB_OK);
		return false;
	}


	SetModel2DAnimInfo(m_ZombieAnimInfo, 8, 4);
	SetModels2DTextures();

	// Zombie
	m_Zombie = new EnemyClass(m_ZombieAnimInfo.animationCount, m_ZombieAnimInfo.maxFrame, 3, 3, m_ZombieAnimInfo.textureNames);
	m_Zombie->SetPosition(0, 0, 0);
	m_Zombie->SetForwardVector(0, 0, -1);

	result = m_Zombie->Initialize(m_D3D->GetDevice());
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the bitmap object.", L"Error", MB_OK);
		return false;
	}
	for (int i = 0; i < m_ZombieAnimInfo.animationCount; i++)
	{
		delete[] m_ZombieAnimInfo.textureNames[i];
	}
	delete[] m_ZombieAnimInfo.textureNames;

	m_GunBitmapInfo.maxFrame = 4;
	m_GunBitmapInfo.bitmapsWidth = new int[m_GunBitmapInfo.maxFrame];
	m_GunBitmapInfo.bitmapsHeight = new int[m_GunBitmapInfo.maxFrame];

	m_GunBitmapInfo.bitmapsWidth[0] = 116;
	m_GunBitmapInfo.bitmapsHeight[0] = 62;
	m_GunBitmapInfo.bitmapsWidth[1] = 116;
	m_GunBitmapInfo.bitmapsHeight[1] = 120;
	m_GunBitmapInfo.bitmapsWidth[2] = 84;
	m_GunBitmapInfo.bitmapsHeight[2] = 148;
	m_GunBitmapInfo.bitmapsWidth[3] = 112;
	m_GunBitmapInfo.bitmapsHeight[3] = 128;

	m_GunBitmapInfo.textureNames = new const WCHAR*[m_GunBitmapInfo.maxFrame];

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
}


CameraClass* GraphicsClass::GetCamera()
{
	return m_Camera;
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
	if (m_Plane)
	{
		m_Plane->Shutdown();
		delete m_Plane;
		m_Plane = 0;
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
		delete[] m_GunBitmapInfo.textureNames;

		for (int i = 0; i < m_GunBitmapInfo.maxFrame; i++)
		{
			m_Gun->bitmaps.at(i)->Shutdown();
			delete m_Gun->bitmaps.at(i);
		}
		delete m_Gun;
		m_Gun = 0;
	}

	// Release the model object.
	if (m_Zombie)
	{
		m_Zombie->Shutdown();
		delete m_Zombie;
		m_Zombie = 0;
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

	// Update the rotation variable each frame.
	bobAngle += XM_PI * 0.025f;

	if (bobAngle > 360.0f)
	{
		bobAngle = 0.0f;
	}
	m_Camera->StartHeadbob(bobAngle);

	// Render the graphics scene.
	result = Render();
	if(!result)
	{
		return false;
	}

	return true;
}

bool GraphicsClass::Render()
{
	XMMATRIX worldMatrix, viewMatrix, projectionMatrix, orthoMatrix, translateMatrix;
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

	/////////////////////////////////////////////////////// 3D Render
	m_Plane->Render(m_D3D->GetDeviceContext());

	// Render the model using the light shader.
	result = m_LightShader->Render(m_D3D->GetDeviceContext(), m_Plane->GetVertexCount(), m_Plane->GetInstanceCount(),
		worldMatrix * XMMatrixScaling(100.0f, 0.01f, 100.0f) * XMMatrixTranslation(0.0f, 0.0f, 0.0f), viewMatrix, projectionMatrix,
		m_Plane->GetTexture(),
		m_Light->GetDirection(), m_Light->GetAmbientColor(), m_Light->GetDiffuseColor(),
		m_Camera->GetPosition(), m_Light->GetSpecularColor(), m_Light->GetSpecularPower(),
		m_Light->GetAmbientToggle(), m_Light->GetDiffuseToggle(), m_Light->GetSpecularToggle());
	if (!result)
	{
		return false;
	}

	m_Stage->Render(m_D3D->GetDeviceContext());

	// Render the model using the light shader.
	result = m_LightShader->Render(m_D3D->GetDeviceContext(), m_Stage->GetVertexCount(), m_Stage->GetInstanceCount(),
		worldMatrix * XMMatrixScaling(0.5f, 0.2f, 0.5f) * XMMatrixTranslation(0.0f, 0.0f, 0.0f), viewMatrix, projectionMatrix,
		m_Stage->GetTexture(),
		m_Light->GetDirection(), m_Light->GetAmbientColor(), m_Light->GetDiffuseColor(),
		m_Camera->GetPosition(), m_Light->GetSpecularColor(), m_Light->GetSpecularPower(),
		m_Light->GetAmbientToggle(), m_Light->GetDiffuseToggle(), m_Light->GetSpecularToggle());
	if (!result)
	{
		return false;
	}

	/////////////////////////////////////////////////////// 2.5D Render
	// billboarding Enemy
	auto tempWorldMatrix = UpdateEnemyWalkingAnimation(m_Zombie, m_ZombieAnimInfo);
	// Turn on the alpha blending before rendering the text.
	m_D3D->TurnOnAlphaBlending();

	// Put the bitmap vertex and index buffers on the graphics pipeline to prepare them for drawing.
	result = m_Zombie->Render(m_D3D->GetDeviceContext(), 0, 0, m_ZombieAnimInfo.currentAnimationIndex, m_ZombieAnimInfo.currentFrameNum / 25);
	if (!result)
	{
		return false;
	}
	if (m_ZombieAnimInfo.currentFrameNum + 1 == m_Zombie->GetModel()->GetMaxFrameNum(m_ZombieAnimInfo.currentAnimationIndex) * 25)
	{
		m_ZombieAnimInfo.currentFrameNum = 0;
	}
	else m_ZombieAnimInfo.currentFrameNum++;

	// Render the bitmap with the texture shader.
	result = m_TextureShader->Render(m_D3D->GetDeviceContext(), 
		m_Zombie->GetModel()->GetSpriteIndexCount(m_ZombieAnimInfo.currentAnimationIndex, m_ZombieAnimInfo.currentFrameNum / 25),
		tempWorldMatrix, viewMatrix, projectionMatrix,
		m_Zombie->GetModel()->GetSpriteTexture(m_ZombieAnimInfo.currentAnimationIndex, m_ZombieAnimInfo.currentFrameNum / 25));
	if (!result)
	{
		return false;
	}

	// Turn off alpha blending after rendering the text.
	m_D3D->TurnOffAlphaBlending();

	/////////////////////////////////////////////////////// 2D Image Render
	// Turn off the Z buffer to begin all 2D rendering.
	m_D3D->TurnZBufferOff();

	// Turn on the alpha blending before rendering the text.
	m_D3D->TurnOnAlphaBlending();

	// Put the bitmap vertex and index buffers on the graphics pipeline to prepare them for drawing.
	result = m_Crosshair->Render(m_D3D->GetDeviceContext(), m_ScreenWidth / 2 - 25, m_ScreenHeight / 2 - 25);
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

	// Put the bitmap vertex and index buffers on the graphics pipeline to prepare them for drawing.
	result = m_Gun->bitmaps.at(0)->Render(m_D3D->GetDeviceContext(), 
		m_ScreenWidth / 2 - m_GunBitmapInfo.bitmapsHeight[0], m_ScreenHeight - m_GunBitmapInfo.bitmapsWidth[0]);
	if (!result)
	{
		return false;
	}
	// Render the bitmap with the texture shader.
	result = m_TextureShader->Render(m_D3D->GetDeviceContext(), m_Gun->bitmaps.at(0)->GetIndexCount(),
		worldMatrix, m_BaseViewMatrix, orthoMatrix, m_Gun->bitmaps.at(0)->GetTexture());
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

XMMATRIX GraphicsClass::UpdateEnemyWalkingAnimation(EnemyClass* enemy, AnimationInfo& anim)
{
	XMMATRIX worldMatrix, translateMatrix;
	XMFLOAT3 cameraPosition;
	double angle, tempAngle;
	float billboardRotation;

	m_D3D->GetWorldMatrix(worldMatrix);

	// Get the position of the camera.
	cameraPosition = XMFLOAT3(m_Camera->GetPosition().x, m_Camera->GetPosition().y, m_Camera->GetPosition().z);

	// Calculate the rotation that needs to be applied to the billboard model to face the current camera position using the arc tangent function.
	angle = atan2(enemy->GetPosition().x - cameraPosition.x, enemy->GetPosition().z - cameraPosition.z) * (180.0 / XM_PI);
	// Convert rotation into radians.
	billboardRotation = (float)angle * 0.0174532925f;

	XMMATRIX tempWorldMatrix = worldMatrix;
	// Setup the rotation the billboard at the origin using the world matrix.
	tempWorldMatrix *= XMMatrixRotationY(billboardRotation);
	// Setup the translation matrix from the billboard model.
	translateMatrix = XMMatrixTranslation(enemy->GetPosition().x, enemy->GetPosition().y, enemy->GetPosition().z);
	// Finally combine the rotation and translation matrices to create the final world matrix for the billboard model.
	tempWorldMatrix = XMMatrixMultiply(tempWorldMatrix, translateMatrix);

	// change animation
	XMVECTOR enemyToPlayerVec = XMVector3Normalize(XMVectorSet(cameraPosition.x - enemy->GetPosition().x, 0,
		cameraPosition.z - enemy->GetPosition().z, 0));
	tempAngle = acos(XMVectorGetX(XMVector3Dot(enemyToPlayerVec, enemy->GetForwardVector()))) * (180.0 / XM_PI);
	float tempCross = XMVectorGetY(XMVector3Cross(enemy->GetForwardVector(), enemyToPlayerVec));

	// case Forward
	if (tempAngle < 22.5f && tempAngle >= 0.0f)
	{
		anim.currentAnimationIndex = 0;
	}
	// case ForwardLeft
	else if (tempAngle < 67.5f && tempAngle >= 22.5f && tempCross < 0)
	{
		anim.currentAnimationIndex = 1;
	}
	// case Left
	else if (tempAngle < 112.5f && tempAngle >= 67.5f && tempCross < 0)
	{
		anim.currentAnimationIndex = 2;
	}
	// case BackLeft
	else if (tempAngle < 157.5f && tempAngle >= 112.5f && tempCross < 0)
	{
		anim.currentAnimationIndex = 3;
	}
	// case Back
	else if (tempAngle < 180.0f && tempAngle >= 157.5f)
	{
		anim.currentAnimationIndex = 4;
	}
	// case BackRight
	else if (tempAngle < 157.5 && tempAngle >= 112.5f)
	{
		anim.currentAnimationIndex = 5;
	}
	// case Right
	else if (tempAngle < 112.5f && tempAngle >= 67.5f)
	{
		anim.currentAnimationIndex = 6;
	}
	// case Back
	else if (tempAngle < 67.5f && tempAngle >= 22.5f)
	{
		anim.currentAnimationIndex = 7;
	}

	return tempWorldMatrix;
}

void GraphicsClass::SetModel2DAnimInfo(AnimationInfo& anim, int animationCount, int maxFrame)
{
	anim.animationCount = animationCount;
	anim.currentAnimationIndex = 0;
	anim.maxFrame = new int[anim.animationCount];
	for (int i = 0; i < anim.animationCount; i++)
	{
		anim.maxFrame[i] = maxFrame;
	}

	anim.textureNames = new const WCHAR**[anim.animationCount];
	for (int i = 0; i < anim.animationCount; i++)
	{
		anim.textureNames[i] = new const WCHAR*[anim.maxFrame[i]];
	}
}


void GraphicsClass::SetModels2DTextures()
{
	// Zombie
	m_ZombieAnimInfo.textureNames[0][0] = L"./data/Zombie/MT_Zombie_F_1.dds";
	m_ZombieAnimInfo.textureNames[0][1] = L"./data/Zombie/MT_Zombie_F_2.dds";
	m_ZombieAnimInfo.textureNames[0][2] = L"./data/Zombie/MT_Zombie_F_3.dds";
	m_ZombieAnimInfo.textureNames[0][3] = L"./data/Zombie/MT_Zombie_F_4.dds";

	m_ZombieAnimInfo.textureNames[1][0] = L"./data/Zombie/MT_Zombie_FL_1.dds";
	m_ZombieAnimInfo.textureNames[1][1] = L"./data/Zombie/MT_Zombie_FL_2.dds";
	m_ZombieAnimInfo.textureNames[1][2] = L"./data/Zombie/MT_Zombie_FL_3.dds";
	m_ZombieAnimInfo.textureNames[1][3] = L"./data/Zombie/MT_Zombie_FL_4.dds";

	m_ZombieAnimInfo.textureNames[2][0] = L"./data/Zombie/MT_Zombie_L_1.dds";
	m_ZombieAnimInfo.textureNames[2][1] = L"./data/Zombie/MT_Zombie_L_2.dds";
	m_ZombieAnimInfo.textureNames[2][2] = L"./data/Zombie/MT_Zombie_L_3.dds";
	m_ZombieAnimInfo.textureNames[2][3] = L"./data/Zombie/MT_Zombie_L_4.dds";

	m_ZombieAnimInfo.textureNames[3][0] = L"./data/Zombie/MT_Zombie_BL_1.dds";
	m_ZombieAnimInfo.textureNames[3][1] = L"./data/Zombie/MT_Zombie_BL_2.dds";
	m_ZombieAnimInfo.textureNames[3][2] = L"./data/Zombie/MT_Zombie_BL_3.dds";
	m_ZombieAnimInfo.textureNames[3][3] = L"./data/Zombie/MT_Zombie_BL_4.dds";

	m_ZombieAnimInfo.textureNames[4][0] = L"./data/Zombie/MT_Zombie_B_1.dds";
	m_ZombieAnimInfo.textureNames[4][1] = L"./data/Zombie/MT_Zombie_B_2.dds";
	m_ZombieAnimInfo.textureNames[4][2] = L"./data/Zombie/MT_Zombie_B_3.dds";
	m_ZombieAnimInfo.textureNames[4][3] = L"./data/Zombie/MT_Zombie_B_4.dds";

	m_ZombieAnimInfo.textureNames[5][0] = L"./data/Zombie/MT_Zombie_BR_1.dds";
	m_ZombieAnimInfo.textureNames[5][1] = L"./data/Zombie/MT_Zombie_BR_2.dds";
	m_ZombieAnimInfo.textureNames[5][2] = L"./data/Zombie/MT_Zombie_BR_3.dds";
	m_ZombieAnimInfo.textureNames[5][3] = L"./data/Zombie/MT_Zombie_BR_4.dds";

	m_ZombieAnimInfo.textureNames[6][0] = L"./data/Zombie/MT_Zombie_R_1.dds";
	m_ZombieAnimInfo.textureNames[6][1] = L"./data/Zombie/MT_Zombie_R_2.dds";
	m_ZombieAnimInfo.textureNames[6][2] = L"./data/Zombie/MT_Zombie_R_3.dds";
	m_ZombieAnimInfo.textureNames[6][3] = L"./data/Zombie/MT_Zombie_R_4.dds";

	m_ZombieAnimInfo.textureNames[7][0] = L"./data/Zombie/MT_Zombie_FR_1.dds";
	m_ZombieAnimInfo.textureNames[7][1] = L"./data/Zombie/MT_Zombie_FR_2.dds";
	m_ZombieAnimInfo.textureNames[7][2] = L"./data/Zombie/MT_Zombie_FR_3.dds";
	m_ZombieAnimInfo.textureNames[7][3] = L"./data/Zombie/MT_Zombie_FR_4.dds";

}
