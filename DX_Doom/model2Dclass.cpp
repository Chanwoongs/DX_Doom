////////////////////////////////////////////////////////////////////////////////
// Filename: model2Dclass.cpp
////////////////////////////////////////////////////////////////////////////////
#include "model2Dclass.h"


Model2DClass::Model2DClass()
	:
	m_currentSpriteIndex(0)
{
}


Model2DClass::Model2DClass(const Model2DClass& other)
{
}


Model2DClass::~Model2DClass()
{
}


bool Model2DClass::Initialize(ID3D11Device* device, int animationCount, int maxFrame[], int spriteWidth, int spriteHeight, const WCHAR** textureFilenames[])
{
	bool result;

	m_maxFrame = maxFrame;
	m_animationCount = animationCount;

	// �ִϸ��̼� ���� ����
	for (int i = 0; i < m_animationCount; i++)
	{
		m_animations.push_back(new Animation());
	}

	// �ִϸ��̼� �ȿ� �ִ� �̹����� ����
	int j = 0;
	for (auto ani : m_animations)
	{
		for (int i = 0; i < m_maxFrame[j]; i++)
		{
			ani->sprites.push_back(new SpriteClass());
		}
		j++;
	}

	// �ִϸ��̼� �ȿ� �ִ� �̹����� �ʱ�ȭ
	j = 0;
	for (auto ani : m_animations)
	{
		for (int i = 0; i < m_maxFrame[j]; i++)
		{
			result = ani->sprites.at(i)->Initialize(device, spriteWidth, spriteHeight, textureFilenames[j][i]);
			if (!result)
			{
				return false;
			}
		}
		j++;
	}

	return true;
}


void Model2DClass::Shutdown()
{
	// �ִϸ��̼��� �̹����� ����
	int j = 0;
	for (auto ani : m_animations)
	{
		for (int i = 0; i < m_maxFrame[j]; i++)
		{
			ani->sprites.at(i)->Shutdown();
			delete ani->sprites.at(i);
			ani->sprites.at(i) = 0;
		}
		j++;
	}
	// �ִϸ��̼� ����
	for (int i = 0; i < m_animationCount; i++)
	{
		delete m_animations.at(i);
		m_animations.at(i) = 0;
	}

	return;
}


bool Model2DClass::Render(ID3D11DeviceContext* deviceContext, int positionX, int positionY, int animationIndex, int frameIndex)
{
	bool result;
	m_currentAnimationIndex = animationIndex;
	m_currentSpriteIndex = frameIndex;

	// ���ϴ� �ִϸ��̼��� ���ϴ� ������ ���
	result = m_animations.at(m_currentAnimationIndex)->sprites.at(m_currentSpriteIndex)->Render(deviceContext, positionX, positionY);
	if (!result)
	{
		return false;
	}

	return true;
}

int Model2DClass::GetSpriteIndexCount(int animationIndex, int spriteIndex)
{
	return m_animations.at(m_currentAnimationIndex)->sprites.at(m_currentSpriteIndex)->GetIndexCount();
}

ID3D11ShaderResourceView* Model2DClass::GetSpriteTexture(int animationIndex, int spriteIndex)
{
	return m_animations.at(m_currentAnimationIndex)->sprites.at(m_currentSpriteIndex)->GetTexture();
}
