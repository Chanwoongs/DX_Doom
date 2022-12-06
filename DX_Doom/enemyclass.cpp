////////////////////////////////////////////////////////////////////////////////
// Filename: enemyclass.cpp
////////////////////////////////////////////////////////////////////////////////
#include "enemyclass.h"

EnemyClass::EnemyClass(float hp, int animationCount, int maxFrame[], int modelWidth, int modelHeight, 
	const WCHAR** textureFilenames[], float boxExtentX, float boxExtentY, float boxExtentZ)
	: 
	m_animationCount(animationCount), m_maxFrame(maxFrame),
	m_modelWidth(modelWidth), m_modelHeight(modelHeight), m_textureNames(textureFilenames)
{
	m_model = new Model2DClass();

	m_pStateMachine = new StateMachine<EnemyClass>(this);

	m_pStateMachine->SetCurrentState(Patrol::Instance());

	m_Box.Center = XMFLOAT3(0.0f, 0.0f, 0.0f);
	m_Box.Extents = XMFLOAT3(boxExtentX, boxExtentY, boxExtentZ);

	m_hp = hp;
	m_isAlive = true;

	m_shortestPath = new XMFLOAT3[100];
}

EnemyClass::~EnemyClass()
{
}

void EnemyClass::Update(float deltaTime)
{
	m_pStateMachine->Update(deltaTime);
}

bool EnemyClass::Initialize(ID3D11Device* device)
{
	bool result;

	// ¸ðµ¨ ÃÊ±âÈ­
	result = m_model->Initialize(device, m_animationCount, m_maxFrame, m_modelWidth, m_modelHeight, m_textureNames,
		L"./data/MT_White.dds", L"./data/MT_White.dds");
	if (!result)
	{
		return false;
	}

	return true;
}


void EnemyClass::Shutdown()
{
	// ¸ðµ¨ »èÁ¦
	m_model->Shutdown();

	delete m_pStateMachine;
	delete m_model;
	delete[] m_shortestPath;

	return;
}


bool EnemyClass::Render(ID3D11DeviceContext* deviceContext, int positionX, int positionY, int animationIndex, int frameIndex)
{
	bool result;

	m_positionX = positionX;
	m_positionY = positionY;

	result = m_model->Render(deviceContext, m_positionX, m_positionY, animationIndex, frameIndex);
	if (!result)
	{
		return false;
	}

	return true;
}

void EnemyClass::SetPosition(float x, float y, float z)
{
	m_position.x = x;
	m_position.y = y;
	m_position.z = z;
}

void EnemyClass::SetPosition(XMFLOAT3 position)
{
	m_position = position;
	m_Box.Center = XMFLOAT3(m_position.x, m_modelHeight / 2, m_position.z); ;
}

void EnemyClass::SetForwardVector(float x, float y, float z)
{
	m_forwardVec = XMVectorSet(x, y, z, 0);
}

void EnemyClass::SetForwardVector(XMVECTOR forwardVec)
{
	m_forwardVec = forwardVec;
}

void EnemyClass::AddPath(XMFLOAT3 path)
{
	m_path.push_back(path);
}
