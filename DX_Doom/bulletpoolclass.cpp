////////////////////////////////////////////////////////////////////////////////
// Filename: bulletpoolclass.cpp
////////////////////////////////////////////////////////////////////////////////
#include "bulletpoolclass.h"

BulletPoolClass::BulletPoolClass(int poolSize)
	: m_poolSize(poolSize)
{
	m_Bullets = new BulletClass[m_poolSize];

	m_FirstAvailable = &m_Bullets[0];

	for (int i = 0; i < m_poolSize - 1; i++)
	{
		m_Bullets[i].SetNext(&m_Bullets[i + 1]);
	}

	m_Bullets[m_poolSize - 1].SetNext(NULL);
}

BulletPoolClass::~BulletPoolClass()
{     
}

bool BulletPoolClass::Initialize(ID3D11Device* device, int modelWidth, int modelHeight, const WCHAR* textureFileName)
{
	for (int i = 0; i < m_poolSize - 1; i++)
	{
		m_Bullets[i].Initialize(device, modelWidth, modelHeight, textureFileName);
	}

	return true;
}

void BulletPoolClass::Create(XMVECTOR directionVec, XMVECTOR positionVec)
{
	assert(m_FirstAvailable != NULL);

	BulletClass* newBullet = m_FirstAvailable;
	m_FirstAvailable = newBullet->GetNext();

	newBullet->SetUsage(true);
	newBullet->SetForwardVector(directionVec);
	newBullet->SetPositionVector(positionVec);
	m_numUsingBullets++;

}

void BulletPoolClass::ShutDown()
{
	for (int i = 0; i < m_poolSize; i++)
	{
		m_Bullets[i].Shutdown();
	}
	delete[] m_Bullets;
}

void BulletPoolClass::UpdateBullets()
{
	for (int i = 0; i < m_poolSize; i++)
	{
		if (!m_Bullets[i].IsUse()) continue;
		if (!m_Bullets[i].IsJustDied()) continue;

		m_Bullets[i].Clean();
		m_Bullets[i].SetNext(m_FirstAvailable);
		m_FirstAvailable = &m_Bullets[i];
		m_numUsingBullets--;
	}
}

