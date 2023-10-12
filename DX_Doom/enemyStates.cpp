#include "enemyStates.h"
#include "state.h"
#include "enemyclass.h"
#include "astarclass.h"

#include <iostream>
using std::cout;


#ifdef TEXTOUTPUT
#include <fstream>
extern std::ofstream os;
#define cout os
#endif


//------------------------------------------------------------------------methods for Patrol
Patrol* Patrol::Instance()
{
  static Patrol instance;

  return &instance;
}


void Patrol::Enter(EnemyClass* pEnemyClass)
{
	pEnemyClass->SetStateChanged(true);
    return;
}


void Patrol::Execute(EnemyClass* pEnemyClass, float deltaTime)
{
	pEnemyClass->SetStateChanged(false);

	// Get Enemy's destination node position
	XMFLOAT3 destination = pEnemyClass->GetPath().at(pEnemyClass->GetPathIndex());
	pEnemyClass->SetSpeed(deltaTime * 0.4f);

	AStarClass::GetInstance()->setInfo(pEnemyClass->GetPosition(), destination);

	timer += deltaTime;
	if (timer > 50.0f && pEnemyClass->isPosUpdated())
	{
		// Find Path to destionation
		vector<XMFLOAT3> path = AStarClass::GetInstance()->findPath();

		if (path.size() == 0) return;

		XMFLOAT3 target;

		if (path.size() > 1) 
			path.erase(path.begin());

		target = *path.begin();

		XMFLOAT3 enemyPos = pEnemyClass->GetPosition();
		XMVECTOR enemyPositionVec = XMLoadFloat3(&enemyPos);
		XMVECTOR targetVec = XMLoadFloat3(&target);
		XMVECTOR diff = enemyPositionVec - targetVec;
		float distance = XMVectorGetX(XMVector3Length(diff));
		if (distance <= 0.1f)
		{
			if (path.size() > 1)
				path.erase(path.begin());

			target = *path.begin();
		}

		vector<XMFLOAT3>::iterator iter;
		iter = path.begin();
		int i = 0;
		pEnemyClass->SetShortestPathSize(0);
		for (; iter != path.end(); iter++)
		{
			pEnemyClass->GetShortestPath()[i].x = iter->x;
			pEnemyClass->GetShortestPath()[i].y = iter->y;
			pEnemyClass->GetShortestPath()[i].z = iter->z;
			i++;
		}
		pEnemyClass->SetShortestPathSize(i);

		// Set next current target node to enemy's path
		pEnemyClass->SetCurrentTargetPath(target);
		timer = 0.0f;
	}

	XMFLOAT3 enemyPos = pEnemyClass->GetPosition();
	XMVECTOR enemyPositionVec = XMLoadFloat3(&enemyPos);
	XMVECTOR enemyDestPathVec = XMLoadFloat3(&destination);
	XMFLOAT3 targetPos = pEnemyClass->GetTargetPosition();
	XMVECTOR targetPositionVec = XMLoadFloat3(&targetPos);

	// 플레이어 좌표가 Valid 한지 검사해야한다.
	if (AStarClass::GetInstance()->isValid((int)targetPos.z, (int)targetPos.x))
	{
		XMVECTOR diff = enemyPositionVec - enemyDestPathVec;
		float distance = XMVectorGetX(XMVector3Length(diff));
		if (distance <= pEnemyClass->GetAcceptDistance())
		{
			pEnemyClass->SetPathIndex(pEnemyClass->GetPathIndex() + 1);
			if (pEnemyClass->GetPathIndex() == pEnemyClass->GetPath().size())
			{
				pEnemyClass->SetPathIndex(0);
			}
		}

		diff = enemyPositionVec - targetPositionVec;
		distance = XMVectorGetX(XMVector3Length(diff));

		if (distance < pEnemyClass->GetDetectRange())
		{
			pEnemyClass->GetFSM()->ChangeState(Approach::Instance());
		}

		diff = XMVectorZero();
		distance = -1.0;
	}

	if (pEnemyClass->IsHitted())
	{
		pEnemyClass->GetFSM()->ChangeState(Hitted::Instance());
	}

    return;
}


void Patrol::Exit(EnemyClass* pEnemyClass)
{
    return;
}

int Patrol::GetStateID()
{
	return PATROL;
}

//------------------------------------------------------------------------methods for Approach

Approach* Approach::Instance()
{
  static Approach instance;

  return &instance;
}

void Approach::Enter(EnemyClass* pEnemyClass)
{
	pEnemyClass->SetStateChanged(true);
    return;
}


void Approach::Execute(EnemyClass* pEnemyClass, float deltaTime)
{
	pEnemyClass->SetStateChanged(false);

	XMFLOAT3 destination = XMFLOAT3(pEnemyClass->GetTargetPosition().x, 0, pEnemyClass->GetTargetPosition().z);
	pEnemyClass->SetSpeed(deltaTime);

	timer += deltaTime;
	if (timer > 50.0f && pEnemyClass->isPosUpdated())
	{
		// Find Path to destionation
		AStarClass::GetInstance()->setInfo(pEnemyClass->GetPosition(), destination);
		vector<XMFLOAT3> path = AStarClass::GetInstance()->findPath();

		if (path.size() == 0)
		{
			pEnemyClass->GetFSM()->ChangeState(Patrol::Instance());
			return;
		}

		XMFLOAT3 target;

		if (path.size() > 2)
		{
			path.erase(path.begin(), path.begin() + 2);
		}
		target = *path.begin();

		XMFLOAT3 enemyPos = pEnemyClass->GetPosition();
		XMVECTOR enemyPositionVec = XMLoadFloat3(&enemyPos);
		XMVECTOR targetVec = XMLoadFloat3(&target);
		XMVECTOR diff = enemyPositionVec - targetVec;
		float distance = XMVectorGetX(XMVector3Length(diff));
		if (distance <= 0.1f)
		{
			if (path.size() > 2)
			{
				path.erase(path.begin(), path.begin() + 2);
			}
			target = *path.begin();
		}

		vector<XMFLOAT3>::iterator iter;
		iter = path.begin();
		int i = 0;
		pEnemyClass->SetShortestPathSize(0);
		for (; iter != path.end(); iter++)
		{
			pEnemyClass->GetShortestPath()[i].x = iter->x;
			pEnemyClass->GetShortestPath()[i].y = iter->y;
			pEnemyClass->GetShortestPath()[i].z = iter->z;
			i++;
		}
		pEnemyClass->SetShortestPathSize(i);

		// Set next current target node to enemy's path
		pEnemyClass->SetCurrentTargetPath(target);
		timer = 0.0f;
	}

	XMFLOAT3 enemyPos = pEnemyClass->GetPosition();
	XMVECTOR enemyPositionVec = XMLoadFloat3(&enemyPos);
	XMVECTOR enemyDestPathVec = XMLoadFloat3(&destination);
	XMFLOAT3 targetPos = pEnemyClass->GetTargetPosition();
	XMVECTOR targetPositionVec = XMLoadFloat3(&targetPos);

	XMVECTOR diff = enemyPositionVec - enemyDestPathVec;
	float distance = XMVectorGetX(XMVector3Length(diff));

	if (distance >= pEnemyClass->GetDetectRange())
	{
		pEnemyClass->GetFSM()->ChangeState(Patrol::Instance());
	}
	else if (distance <= pEnemyClass->GetAttackRange() - 0.5f)
	{
		pEnemyClass->GetFSM()->ChangeState(Attack::Instance());
	}

	if (pEnemyClass->IsHitted())
	{
		pEnemyClass->GetFSM()->ChangeState(Hitted::Instance());
	}

	diff = XMVectorZero();
	distance = -1.0;

    return;
}


void Approach::Exit(EnemyClass* pEnemyClass)
{
    return;
}

int Approach::GetStateID()
{
	return APPROACH;
}

//------------------------------------------------------------------------methods for Attack

Attack* Attack::Instance()
{
  static Attack instance;

  return &instance;
}

void Attack::Enter(EnemyClass* pEnemyClass)
{
	pEnemyClass->SetStateChanged(true);
	pEnemyClass->SetAttackPosition(pEnemyClass->GetTargetPosition());
    
	return;
}

void Attack::Execute(EnemyClass* pEnemyClass, float deltaTime)
{
	pEnemyClass->SetStateChanged(false);

	pEnemyClass->SetCurrentTargetPath(XMFLOAT3(pEnemyClass->GetTargetPosition().x, 0, pEnemyClass->GetTargetPosition().z));
	pEnemyClass->SetSpeed(0.0f);
	XMFLOAT3 enemyPos = pEnemyClass->GetPosition();
	XMVECTOR enemyPositionVec = XMLoadFloat3(&enemyPos);
	XMFLOAT3 enemyCurrentPath = pEnemyClass->GetCurrentTargetPath();
	XMVECTOR enemyCurrentPathVec = XMLoadFloat3(&enemyCurrentPath);
	XMFLOAT3 targetPos = pEnemyClass->GetTargetPosition();
	XMVECTOR targetPositionVec = XMLoadFloat3(&targetPos);

	XMVECTOR diff = enemyPositionVec - targetPositionVec;
	float distance = XMVectorGetX(XMVector3Length(diff));

	if (distance > pEnemyClass->GetAttackRange())
	{
		pEnemyClass->GetFSM()->ChangeState(Approach::Instance());
	}

	pEnemyClass->SetAttacking(true);

	if (pEnemyClass->IsHitted())
	{
		pEnemyClass->GetFSM()->ChangeState(Hitted::Instance());
	}

    return;
}

void Attack::Exit(EnemyClass* pEnemyClass)
{
    return;
}

int Attack::GetStateID()
{
	return ATTACK;
}

//------------------------------------------------------------------------methods for Hitted

Hitted* Hitted::Instance()
{
  static Hitted instance;

  return &instance;
}


void Hitted::Enter(EnemyClass* pEnemyClass)
{
	pEnemyClass->SetStateChanged(true);

	pEnemyClass->SetHp(pEnemyClass->GetHp() - 25.0f);

    return;
}


void Hitted::Execute(EnemyClass* pEnemyClass, float deltaTime)
{
	pEnemyClass->SetStateChanged(false);

	if (pEnemyClass->GetHp() <= 0)
	{
		pEnemyClass->GetFSM()->ChangeState(Dead::Instance());
	}

    return;
}

void Hitted::Exit(EnemyClass* pEnemyClass)
{
	pEnemyClass->SetHitted(false);

    return;
}

int Hitted::GetStateID()
{
	return HITTED;
}

//------------------------------------------------------------------------methods for Dead

Dead* Dead::Instance()
{
	static Dead instance;

	return &instance;
}


void Dead::Enter(EnemyClass* pEnemyClass)
{
	pEnemyClass->SetStateChanged(true);
	pEnemyClass->SetSpeed(0.0f);
	return;
}


void Dead::Execute(EnemyClass* pEnemyClass, float deltaTime)
{
	pEnemyClass->SetStateChanged(false);

	return;
}

void Dead::Exit(EnemyClass* pEnemyClass)
{
	return;
}

int Dead::GetStateID()
{
	return DEAD;
}

