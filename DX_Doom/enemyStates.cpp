#include "enemyStates.h"
#include "state.h"
#include "enemyclass.h"

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
    return;
}


void Patrol::Execute(EnemyClass* pEnemyClass)
{
	pEnemyClass->SetCurrentTargetPath(pEnemyClass->GetPath().at(pEnemyClass->GetPathIndex()));
	pEnemyClass->SetSpeed(1.0f);
	XMFLOAT3 enemyPos = pEnemyClass->GetPosition();
	XMVECTOR enemyPositionVec = XMLoadFloat3(&enemyPos);
	XMFLOAT3 enemyCurrentPath = pEnemyClass->GetCurrentTargetPath();
	XMVECTOR enemyCurrentPathVec = XMLoadFloat3(&enemyCurrentPath);
	XMFLOAT3 targetPos = pEnemyClass->GetTargetPosition();
	XMVECTOR targetPositionVec = XMLoadFloat3(&targetPos);

	XMVECTOR diff = enemyPositionVec - enemyCurrentPathVec;
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
    return;
}


void Approach::Execute(EnemyClass* pEnemyClass)
{
	pEnemyClass->SetCurrentTargetPath(XMFLOAT3(pEnemyClass->GetTargetPosition().x, 0, pEnemyClass->GetTargetPosition().z));
	pEnemyClass->SetSpeed(2.5f);
	XMFLOAT3 enemyPos = pEnemyClass->GetPosition();
	XMVECTOR enemyPositionVec = XMLoadFloat3(&enemyPos);
	XMFLOAT3 enemyCurrentPath = pEnemyClass->GetCurrentTargetPath();
	XMVECTOR enemyCurrentPathVec = XMLoadFloat3(&enemyCurrentPath);
	XMFLOAT3 targetPos = pEnemyClass->GetTargetPosition();
	XMVECTOR targetPositionVec = XMLoadFloat3(&targetPos);

	XMVECTOR diff = enemyPositionVec - enemyCurrentPathVec;
	float distance = XMVectorGetX(XMVector3Length(diff));

	if (distance >= pEnemyClass->GetDetectRange())
	{
		pEnemyClass->GetFSM()->ChangeState(Patrol::Instance());
	}
	else if (distance <= pEnemyClass->GetAttackRange())
	{
		pEnemyClass->GetFSM()->ChangeState(Attack::Instance());
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
	pEnemyClass->SetAttackPosition(pEnemyClass->GetTargetPosition());
    
	return;
}

void Attack::Execute(EnemyClass* pEnemyClass)
{
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

	if (distance >= pEnemyClass->GetAttackRange() + 1.0f)
	{
		pEnemyClass->GetFSM()->ChangeState(Approach::Instance());
	}

	pEnemyClass->SetAttacking(true);
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
    return;
}


void Hitted::Execute(EnemyClass* pEnemyClass)
{
    return;
}

void Hitted::Exit(EnemyClass* pEnemyClass)
{
    return;
}

int Hitted::GetStateID()
{
	return HITTED;
}
