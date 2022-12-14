////////////////////////////////////////////////////////////////////////////////
// Filename: enemyclass.h
////////////////////////////////////////////////////////////////////////////////
#ifndef _ENEMYCLASS_H_
#define _ENEMYCLASS_H_


//////////////
// INCLUDES //
//////////////
#include <DirectXCollision.h>

///////////////////////
// MY CLASS INCLUDES //
///////////////////////
#include "model2Dclass.h"
#include "spriteclass.h"
#include "enemyStates.h"
#include "StateMachine.h"

using namespace DirectX;

////////////////////////////////////////////////////////////////////////////////
// Class name: EnemyClass
////////////////////////////////////////////////////////////////////////////////

template <class type> class State; //pre-fixed with "template <class entity_type> " for vs8 compatibility

class EnemyClass
{
public:
	EnemyClass(float, int, int[], int, int, const WCHAR**[], float, float, float);
	EnemyClass(const EnemyClass&);
	~EnemyClass();

	void Update(float);

	StateMachine<EnemyClass>* GetFSM()const { return m_pStateMachine; }

	bool Initialize(ID3D11Device*);
	void Shutdown();
	bool Render(ID3D11DeviceContext*, int, int, int, int);
	Model2DClass* GetModel() { return m_model; }

	XMFLOAT3 GetPosition() { return m_position; }
	void SetPosition(float, float, float);
	void SetPosition(XMFLOAT3);

	XMVECTOR GetForwardVector() { return m_forwardVec; }
	void SetForwardVector(float ,float, float);
	void SetForwardVector(XMVECTOR);

	vector<XMFLOAT3> GetPath() { return m_path; }
	void AddPath(XMFLOAT3);
	int GetPathIndex() { return m_pathIndex; }
	void SetPathIndex(int index) { m_pathIndex = index; }

	XMFLOAT3* GetShortestPath() { return m_shortestPath; }
	int GetShortestPathSize() { return m_shortestPathSize; }
	void SetShortestPathSize(int size) { m_shortestPathSize = size; }

	float GetAcceptDistance() { return m_acceptDistance; }
	void SetAcceptDistance(float acceptDistance) { m_acceptDistance = acceptDistance; }
	float GetDetectRange() { return m_detectRange; }
	void SetDetectRange(float detectRange) { m_detectRange = detectRange; }
	float GetAttackRange() { return m_attackRange; }
	void SetAttackRange(float attackRange) { m_attackRange = attackRange; }
	
	XMFLOAT3 GetCurrentTargetPath() { return m_currentTargetPath; }
	void SetCurrentTargetPath(XMFLOAT3 path) { m_currentTargetPath = path; }

	XMFLOAT3 GetTargetPosition() { return m_targetPosition; }
	void SetTargetPosition(XMFLOAT3 pos) { m_targetPosition = pos; }
	XMFLOAT3 GetAttackPosition() { return m_attackPosition; }
	void SetAttackPosition(XMFLOAT3 pos) { m_attackPosition = pos; }

	float GetSpeed() { return m_speed; }
	void SetSpeed(float speed) { m_speed = speed; }

	bool IsAttacking() { return m_isAttacking == true; }
	void SetAttacking(bool attack) { m_isAttacking = attack; }

	bool IsHitted() { return m_isHitted == true; }
	void SetHitted(bool hitted) { m_isHitted = hitted; }

	BoundingBox GetBoundingBox() { return m_Box; }

	bool IsStateChanged() { return m_stateChanged == true; }
	void SetStateChanged(bool stateChanged) { m_stateChanged = stateChanged; }

	float GetHp() { return m_hp; }
	void SetHp(float hp) { m_hp = hp; }

	bool IsAlive() { return m_isAlive == true; }
	void SetAlive(bool alive) { m_isAlive = alive; }

	bool IsSpawn() { return m_isSpawn == true; }
	void SetSpawn(bool spwan) { m_isSpawn = spwan; }

	bool IsReturning() { return m_isReturning == true; }
	void SetReturning(bool returning) { m_isReturning = returning; }

private:
	StateMachine<EnemyClass>* m_pStateMachine;
	bool m_stateChanged;

	Model2DClass* m_model;
	const WCHAR*** m_textureNames;
	int m_animationCount;
	int* m_maxFrame;
	int m_modelWidth;
	int m_modelHeight;

	float m_positionX;
	float m_positionY;

	XMFLOAT3 m_position;
	XMVECTOR m_forwardVec;

	vector<XMFLOAT3> m_path;
	XMFLOAT3 m_currentTargetPath;
	XMFLOAT3* m_shortestPath;
	int m_shortestPathSize;
	int m_pathIndex;
	float m_acceptDistance;
	float m_detectRange;
	float m_attackRange;
	float m_speed;

	XMFLOAT3 m_targetPosition;
	XMFLOAT3 m_attackPosition;

	bool m_isAttacking;
	bool m_isHitted;
	bool m_isReturning;

	BoundingBox m_Box;

	float m_hp;
	bool m_isAlive;
	bool m_isSpawn;
};

#endif