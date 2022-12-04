////////////////////////////////////////////////////////////////////////////////
// Filename: bulletclass.h
////////////////////////////////////////////////////////////////////////////////
#ifndef _BULLETCLASS_H_
#define _BULLETCLASS_H_

///////////////////////
// MY CLASS INCLUDES //
///////////////////////
#include "spriteclass.h"

////////////////////////////////////////////////////////////////////////////////
// Class name: BulletClass
////////////////////////////////////////////////////////////////////////////////
class BulletClass
{
public:
	BulletClass();
	BulletClass(const BulletClass&);
	~BulletClass();

	XMMATRIX Update(float, XMFLOAT3);

	bool Initialize(ID3D11Device*, int, int, const WCHAR*);
	void Shutdown();
	bool Render(ID3D11DeviceContext*, float, float);

	void Clean();

	XMFLOAT3 GetPosition() { return m_position; }
	void SetPosition(float x, float y, float z) { m_position.x = x;	m_position.y = y; m_position.z = z; }
	void SetPosition(XMFLOAT3 pos) { m_position = pos; }

	XMVECTOR GetForwardVector() { return m_forwardVec; }
	void SetForwardVector(float x, float y, float z) { m_forwardVec = XMVectorSet(x, y, z, 0); }
	void SetForwardVector(XMVECTOR forwardVec) { m_forwardVec = forwardVec; }

	void SetPositionVector(XMVECTOR positionVec) { m_positionVec = positionVec; }

	float GetSpeed() { return m_speed; }
	void SetSpeed(float speed) { m_speed = speed; }

	BulletClass* GetNext() { return m_Next; }
	void SetNext(BulletClass* next) { m_Next = next; }

	bool IsUse() { return m_isUse; }
	void SetUsage(bool usage) { m_isUse = usage; }
	bool IsJustDied() { return m_justDied; }
	void SetJustDied(bool justDied) { m_justDied = justDied; }

	SpriteClass* GetModel() { return m_Model; }

private:
	SpriteClass* m_Model;

	const WCHAR* m_textureName;
	int m_modelWidth;
	int m_modelHeight;

	XMFLOAT3 m_position;
	XMVECTOR m_forwardVec;
	XMVECTOR m_positionVec;

	float m_speed;
	float m_life;
	float m_lifeTime;
	bool m_isUse;
	bool m_justDied;

	BulletClass* m_Next;
};

#endif