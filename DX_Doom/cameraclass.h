////////////////////////////////////////////////////////////////////////////////
// Filename: cameraclass.h
////////////////////////////////////////////////////////////////////////////////
#ifndef _CAMERACLASS_H_
#define _CAMERACLASS_H_


//////////////
// INCLUDES //
//////////////
#include <directxmath.h>

#include "AlignedAllocationPolicy.h"

using namespace DirectX;

////////////////////////////////////////////////////////////////////////////////
// Class name: CameraClass
////////////////////////////////////////////////////////////////////////////////
class CameraClass : public AlignedAllocationPolicy<16>
{
public:
	CameraClass();
	CameraClass(const CameraClass&);
	~CameraClass();

	void SetPosition(float, float, float);
	void SetRotation(float, float, float);
	void MoveLeft(float);
	void MoveRight(float);
	void MoveBack(float);
	void MoveForward(float);
	void MoveDown(float);
	void MoveUp(float);
	void StartHeadbob(float);
	void EndHeadbob(float);

	XMFLOAT3 GetPosition();
	XMFLOAT3 GetRotation();

	void Render();
	void GetViewMatrix(XMMATRIX&);
	XMVECTOR GetTargetVector() { return m_target; }
	XMVECTOR GetRightVector() { return m_camRight; }
	XMVECTOR GetForwardVector() { return m_camForward; }
	void RotateCamera(float, float, float);
	void Move(float);
	void StopMove() { m_isMoving = false; }

private:
	XMFLOAT3 m_position;
	XMFLOAT3 m_rotation;
	XMMATRIX m_viewMatrix;
	XMMATRIX camRotationMatrix;

	float m_moveLeftRight;
	float m_moveBackForward;
	float m_moveUpDown;

	XMVECTOR m_up, m_target;
	XMVECTOR m_camRight, m_camForward;
	XMVECTOR m_defaultRight, m_defaultForward;
	float m_yaw, m_pitch, m_roll;

	float m_defaultYPos;
	bool m_isMoving;
};

#endif