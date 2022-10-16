////////////////////////////////////////////////////////////////////////////////
// Filename: enemyclass.cpp
////////////////////////////////////////////////////////////////////////////////
#include "enemyclass.h"

EnemyClass::EnemyClass(int animationCount, int maxFrame[], int modelWidth, int modelHeight, const WCHAR** textureFilenames[])
	: 
	m_animationCount(animationCount), m_maxFrame(maxFrame),
	m_modelWidth(modelWidth), m_modelHeight(modelHeight), m_textureNames(textureFilenames)
{
	m_model = new Model2DClass();
}

EnemyClass::~EnemyClass()
{
}

bool EnemyClass::Initialize(ID3D11Device* device)
{
	bool result;

	// ¸ðµ¨ ÃÊ±âÈ­
	result = m_model->Initialize(device, m_animationCount, m_maxFrame, m_modelWidth, m_modelHeight, m_textureNames);
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

	delete m_model;

	return;
}


bool EnemyClass::Render(ID3D11DeviceContext* deviceContext, int positionX, int positionY, int animationIndex, int frameIndex)
{
	bool result;

	m_positionX = positionX;
	m_positionY = positionY;

	result = m_model->Render(deviceContext, positionX, positionY, animationIndex, frameIndex);
	if (!result)
	{
		return false;
	}

	return true;
}
