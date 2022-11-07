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


////////////////////////////////////////////////////////////////////////////////
// Class name: EnemyClass
////////////////////////////////////////////////////////////////////////////////

class EnemyClass
{
public:
	EnemyClass(int, int[], int, int, const WCHAR**[]);
	EnemyClass(const EnemyClass&);
	~EnemyClass();

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
	
	XMFLOAT3 GetCurrentTargetPath() { return m_currentTargetPath; }
	void SetCurrentTargetPath(XMFLOAT3 path) { m_currentTargetPath = path; }


private:
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
};

#endif