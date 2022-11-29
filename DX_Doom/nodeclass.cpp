////////////////////////////////////////////////////////////////////////////////
// Filename: nodeclass.cpp
////////////////////////////////////////////////////////////////////////////////
#include "nodeclass.h"


NodeClass::NodeClass(XMFLOAT3 position)
{
	m_position = position;

}

NodeClass::NodeClass(const NodeClass& other)
{
}


NodeClass::~NodeClass()
{
}


bool NodeClass::Initialize()
{
	bool result;

	XMVECTOR positionVec = XMLoadFloat3(&m_position);
	XMVECTOR posToZeroVec = XMVectorZero() - positionVec;

	XMFLOAT3 tempPos = XMFLOAT3(m_position.x, m_position.y, m_position.z + 1);
	XMVECTOR tempPosVec = XMLoadFloat3(&tempPos);
	XMVECTOR posToTempVec = tempPosVec - positionVec;

	XMVECTOR upVec = XMVector3Cross(XMVector3Normalize(tempPosVec), XMVector3Normalize(posToZeroVec));

	m_upVec = XMVector3Normalize(upVec);

	return true;
}


void NodeClass::Shutdown()
{

	return;
}



