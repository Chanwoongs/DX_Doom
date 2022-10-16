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

private:
	Model2DClass* m_model;
	const WCHAR*** m_textureNames;
	int m_animationCount;
	int* m_maxFrame;
	int m_modelWidth;
	int m_modelHeight;

	float m_positionX;
	float m_positionY;
};

#endif