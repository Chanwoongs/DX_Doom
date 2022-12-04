////////////////////////////////////////////////////////////////////////////////
// Filename: bulletclass.cpp
////////////////////////////////////////////////////////////////////////////////
#include "bulletclass.h"

BulletClass::BulletClass()
{
	m_isUse = false;
	m_justDied = false;
	m_life = 0.0f;
	m_lifeTime = 50.0f;
	m_speed = 2.0f;
}

BulletClass::~BulletClass()
{
}

bool BulletClass::Initialize(ID3D11Device* device, int modelWidth, int modelHeight, const WCHAR* textureFileName)
{
	bool result;

	m_Model = new SpriteClass();

	m_modelWidth = modelWidth;
	m_modelHeight = modelHeight;
	m_textureName = textureFileName;

	// ¸ðµ¨ ÃÊ±âÈ­
	result = m_Model->Initialize(device, m_modelWidth, m_modelHeight, m_textureName);
	if (!result)
	{
		return false;
	}

	return true;
}

void BulletClass::Shutdown()
{
	// ¸ðµ¨ »èÁ¦
	m_Model->Shutdown();
	delete m_Model;

	return;
}


bool BulletClass::Render(ID3D11DeviceContext* deviceContext, float positionX, float positionY)
{
	bool result;

	result = m_Model->Render(deviceContext, positionX, positionY);
	if (!result)
	{
		return false;
	}

	return true;
}

void BulletClass::Clean()
{
	m_position = XMFLOAT3(0, 0, 0);
	m_forwardVec = XMVectorZero();
	m_life = 0.0f;
	m_isUse = false;
	m_justDied = false;
}

XMMATRIX BulletClass::Update(float deltaTime, XMFLOAT3 camPosition)
{
	XMMATRIX bulletMoveMatrix = XMMatrixIdentity();
	bulletMoveMatrix = XMMatrixTranslationFromVector(m_forwardVec / deltaTime * m_speed * 1.0f);
	m_positionVec = XMVector3Transform(m_positionVec, bulletMoveMatrix);
	XMFLOAT3 tempBulletPos;
	XMStoreFloat3(&tempBulletPos, m_positionVec);
	SetPosition(tempBulletPos);
	SetForwardVector(XMVector3Normalize(m_forwardVec));

	m_Sphere.Center = m_position;
	m_Sphere.Radius = 1.0f;

	// Calculate the rotation that needs to be applied to the billboard model to face the current camera position using the arc tangent function.
	auto yawAngle = atan2(m_position.x - camPosition.x, m_position.z - camPosition.z) * (180.0 / XM_PI);

	// Convert rotation into radians.
	auto billboardYawRotation = (float)yawAngle * 0.0174532925f;

	XMMATRIX bulletWorldMatrix = XMMatrixIdentity();
	// Setup the rotation the billboard at the origin using the world matrix.
	bulletWorldMatrix *= XMMatrixRotationY(billboardYawRotation);

	// Setup the translation matrix from the billboard model.
	XMMATRIX translateMatrix = XMMatrixTranslation(m_position.x, m_position.y, m_position.z);
	// Finally combine the rotation and translation matrices to create the final world matrix for the billboard model.
	bulletWorldMatrix = XMMatrixMultiply(bulletWorldMatrix, translateMatrix);

	m_life += deltaTime * 0.01f;
	if (m_life > m_lifeTime)
	{
		m_justDied = true;
	}

	return bulletWorldMatrix;
}