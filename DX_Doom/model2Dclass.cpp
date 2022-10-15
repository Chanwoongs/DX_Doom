////////////////////////////////////////////////////////////////////////////////
// Filename: model2Dclass.cpp
////////////////////////////////////////////////////////////////////////////////
#include "model2Dclass.h"


Model2DClass::Model2DClass()
{
	m_maxFrame = 4;
	m_currentSpriteIndex = 0;

	for (int i = 0; i < m_maxFrame; i++)
	{
		m_Idle.sprites = new SpriteClass[m_maxFrame];
	}
}


Model2DClass::Model2DClass(const Model2DClass& other)
{
}


Model2DClass::~Model2DClass()
{
}


bool Model2DClass::Initialize(ID3D11Device* device, int spriteWidth, int spriteHeight, const WCHAR* textureFilenames[])
{
	bool result;
	
	for (int i = 0; i < m_maxFrame; i++)
	{
		result = m_Idle.sprites[i].Initialize(device, spriteWidth, spriteHeight, textureFilenames[i]);
		if (!result)
		{
			return false;
		}
	}

	return true;
}


void Model2DClass::Shutdown()
{
	for (int i = 0; i < m_maxFrame; i++)
	{
		m_Idle.sprites[i].Shutdown();
	}

	delete[] m_Idle.sprites;

	return;
}


bool Model2DClass::Render(ID3D11DeviceContext* deviceContext, int positionX, int positionY, int frameNum)
{
	bool result;
	m_currentSpriteIndex = frameNum;

	result = m_Idle.sprites[m_currentSpriteIndex].Render(deviceContext, positionX, positionY);
	if (!result)
	{
		return false;
	}

	return true;
}

int Model2DClass::GetIndexCount()
{
	return m_Idle.sprites[m_currentSpriteIndex].GetIndexCount();
}


ID3D11ShaderResourceView* Model2DClass::GetTexture()
{
	return  m_Idle.sprites[m_currentSpriteIndex].GetTexture();
}

