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
	m_monsterPosition = new XMFLOAT3[1];
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
	SetPlanePosition();
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

	/*
	m_Enemy = new Model2DClass;
	if (!m_Enemy)
	{
		return false;
	}
	m_textureFileNames = new const WCHAR*[m_Enemy->GetMaxFrame()];
	m_textureFileNames[0] = L"./data/MT_Warewolf_0.dds";
	m_textureFileNames[1] = L"./data/MT_Warewolf_1.dds";
	m_textureFileNames[2] = L"./data/MT_Warewolf_0.dds";
	m_textureFileNames[3] = L"./data/MT_Warewolf_2.dds";
	// Initialize the bitmap object.
	result = m_Enemy->Initialize(m_D3D->GetDevice(), 10, 10,
		m_textureFileNames);
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the bitmap object.", L"Error", MB_OK);
		return false;
	}
	delete[] m_textureFileNames;
	*/


	m_zombieAnimationCount = 8;
	m_zombieCurrentAnimationIndex = 0;
	m_zombieMaxFrame = new int[m_zombieAnimationCount];
	for (int i = 0; i < 8; i++)
	{
		m_zombieMaxFrame[i] = 4;
	}
	
	m_zombieTextureNames = new const WCHAR**[m_zombieAnimationCount];
	for (int i = 0; i < m_zombieAnimationCount; i++)
	{
		m_zombieTextureNames[i] = new const WCHAR*[m_zombieMaxFrame[i]];
	}

	m_zombieTextureNames[0][0] = L"./data/zombie/MT_Zombie_F_1.dds";
	m_zombieTextureNames[0][1] = L"./data/zombie/MT_Zombie_F_2.dds";
	m_zombieTextureNames[0][2] = L"./data/zombie/MT_Zombie_F_3.dds";
	m_zombieTextureNames[0][3] = L"./data/zombie/MT_Zombie_F_4.dds";

	m_zombieTextureNames[1][0] = L"./data/zombie/MT_Zombie_FL_1.dds";
	m_zombieTextureNames[1][1] = L"./data/zombie/MT_Zombie_FL_2.dds";
	m_zombieTextureNames[1][2] = L"./data/zombie/MT_Zombie_FL_3.dds";
	m_zombieTextureNames[1][3] = L"./data/zombie/MT_Zombie_FL_4.dds";

	m_zombieTextureNames[2][0] = L"./data/zombie/MT_Zombie_L_1.dds";
	m_zombieTextureNames[2][1] = L"./data/zombie/MT_Zombie_L_2.dds";
	m_zombieTextureNames[2][2] = L"./data/zombie/MT_Zombie_L_3.dds";
	m_zombieTextureNames[2][3] = L"./data/zombie/MT_Zombie_L_4.dds";

	m_zombieTextureNames[3][0] = L"./data/zombie/MT_Zombie_BL_1.dds";
	m_zombieTextureNames[3][1] = L"./data/zombie/MT_Zombie_BL_2.dds";
	m_zombieTextureNames[3][2] = L"./data/zombie/MT_Zombie_BL_3.dds";
	m_zombieTextureNames[3][3] = L"./data/zombie/MT_Zombie_BL_4.dds";

	m_zombieTextureNames[4][0] = L"./data/zombie/MT_Zombie_B_1.dds";
	m_zombieTextureNames[4][1] = L"./data/zombie/MT_Zombie_B_2.dds";
	m_zombieTextureNames[4][2] = L"./data/zombie/MT_Zombie_B_3.dds";
	m_zombieTextureNames[4][3] = L"./data/zombie/MT_Zombie_B_4.dds";

	m_zombieTextureNames[5][0] = L"./data/zombie/MT_Zombie_BR_1.dds";
	m_zombieTextureNames[5][1] = L"./data/zombie/MT_Zombie_BR_2.dds";
	m_zombieTextureNames[5][2] = L"./data/zombie/MT_Zombie_BR_3.dds";
	m_zombieTextureNames[5][3] = L"./data/zombie/MT_Zombie_BR_4.dds";

	m_zombieTextureNames[6][0] = L"./data/zombie/MT_Zombie_R_1.dds";
	m_zombieTextureNames[6][1] = L"./data/zombie/MT_Zombie_R_2.dds";
	m_zombieTextureNames[6][2] = L"./data/zombie/MT_Zombie_R_3.dds";
	m_zombieTextureNames[6][3] = L"./data/zombie/MT_Zombie_R_4.dds";

	m_zombieTextureNames[7][0] = L"./data/zombie/MT_Zombie_FR_1.dds";
	m_zombieTextureNames[7][1] = L"./data/zombie/MT_Zombie_FR_2.dds";
	m_zombieTextureNames[7][2] = L"./data/zombie/MT_Zombie_FR_3.dds";
	m_zombieTextureNames[7][3] = L"./data/zombie/MT_Zombie_FR_4.dds";

	m_Zombie = new EnemyClass(m_zombieAnimationCount, m_zombieMaxFrame, 10, 10, m_zombieTextureNames);

	result = m_Zombie->Initialize(m_D3D->GetDevice());
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the bitmap object.", L"Error", MB_OK);
		return false;
	}
	for (int i = 0; i < m_zombieAnimationCount; i++)
	{
		delete[] m_zombieTextureNames[i];
	}
	delete[] m_zombieTextureNames;

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

void GraphicsClass::SetPlanePosition()
{
	m_planePosition[0] = XMFLOAT3(0.0f, 0.0f, 0.0f);
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

bool GraphicsClass::Frame()
{
	bool result;

	static float rotation = 0.0f;


	// Update the rotation variable each frame.
	rotation += XM_PI * 0.005f;
	if (rotation > 360.0f)
	{
		rotation -= 360.0f;
	}

	// Render the graphics scene.
	result = Render(rotation);
	if(!result)
	{
		return false;
	}

	return true;
}

bool GraphicsClass::Render(float rotation)
{
	XMMATRIX worldMatrix, viewMatrix, projectionMatrix, orthoMatrix;
	bool result;
	
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
		worldMatrix * XMMatrixScaling(25.0f, 0.01f, 25.0f) * XMMatrixTranslation(0.0f, 0.0f, 0.0f), viewMatrix, projectionMatrix,
		m_Plane->GetTexture(),
		m_Light->GetDirection(), m_Light->GetAmbientColor(), m_Light->GetDiffuseColor(),
		m_Camera->GetPosition(), m_Light->GetSpecularColor(), m_Light->GetSpecularPower(),
		m_Light->GetAmbientToggle(), m_Light->GetDiffuseToggle(), m_Light->GetSpecularToggle());
	if (!result)
	{
		return false;
	}

	/////////////////////////////////////////////////////// 2.5D Render
	// Turn on the alpha blending before rendering the text.
	m_D3D->TurnOnAlphaBlending();

	// Put the bitmap vertex and index buffers on the graphics pipeline to prepare them for drawing.
	result = m_Zombie->Render(m_D3D->GetDeviceContext(), 0, 0, m_zombieCurrentAnimationIndex, frameNum / 25);
	if (!result)
	{
		return false;
	}
	if (frameNum + 1 == m_Zombie->GetModel()->GetMaxFrameNum(m_zombieCurrentAnimationIndex) * 25)
	{
		frameNum = 0;
	}
	else frameNum++;

	// Render the bitmap with the texture shader.
	result = m_TextureShader->Render(m_D3D->GetDeviceContext(), 
		m_Zombie->GetModel()->GetSpriteIndexCount(m_zombieCurrentAnimationIndex, frameNum / 25),
		worldMatrix, viewMatrix, projectionMatrix,
		m_Zombie->GetModel()->GetSpriteTexture(m_zombieCurrentAnimationIndex, frameNum / 25));
	if (!result)
	{
		return false;
	}

	// Turn off alpha blending after rendering the text.
	m_D3D->TurnOffAlphaBlending();

	/////////////////////////////////////////////////////// 2D Render
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

	// Turn off alpha blending after rendering the text.
	m_D3D->TurnOffAlphaBlending();

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