////////////////////////////////////////////////////////////////////////////////
// Filename: enemyclass.h
////////////////////////////////////////////////////////////////////////////////
#ifndef _ENEMYCLASS_H_
#define _ENEMYCLASS_H_


///////////////////////
// MY CLASS INCLUDES //
///////////////////////
#include "model2Dclass.h"
#include "spriteclass.h"
#include "enemyStates.h"
#include "StateMachine.h"


////////////////////////////////////////////////////////////////////////////////
// Class name: EnemyClass
////////////////////////////////////////////////////////////////////////////////

template <class type> class State; //pre-fixed with "template <class entity_type> " for vs8 compatibility

class EnemyClass
{
public:
	EnemyClass(int, int[], int, int, const WCHAR**[]);
	EnemyClass(const EnemyClass&);
	~EnemyClass();

	void Update();

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

private:
	StateMachine<EnemyClass>* m_pStateMachine;

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
	int m_pathIndex;
	float m_acceptDistance;
	float m_detectRange;
	float m_attackRange;

	XMFLOAT3 m_targetPosition;
	XMFLOAT3 m_attackPosition;

};

#endif