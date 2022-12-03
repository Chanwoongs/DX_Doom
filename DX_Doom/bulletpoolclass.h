////////////////////////////////////////////////////////////////////////////////
// Filename: bulletpoolclass.h
////////////////////////////////////////////////////////////////////////////////
#ifndef _BULLETPOOLCLASS_H_
#define _BULLETPOOLCLASS_H_


//////////////
// INCLUDES //
//////////////


///////////////////////
// MY CLASS INCLUDES //
///////////////////////
#include "d3dclass.h"
#include "bulletclass.h"


using namespace DirectX;
using namespace std;

////////////////////////////////////////////////////////////////////////////////
// Class name: BulletPoolClass
////////////////////////////////////////////////////////////////////////////////

class BulletPoolClass
{
public:
	BulletPoolClass(int);
	BulletPoolClass(const BulletPoolClass&);
	~BulletPoolClass();

	bool Initialize(ID3D11Device*, int, int, const WCHAR*);
	void Create(XMVECTOR, XMVECTOR);
	void ShutDown();
	void UpdateBullets();

	BulletClass* GetBullets() { return m_Bullets; }
	int GetPoolSize() { return m_poolSize; }
	
private:
	BulletClass* m_Bullets;
	BulletClass* m_FirstAvailable;

	int m_poolSize;
	int m_numUsingBullets;
};

#endif

