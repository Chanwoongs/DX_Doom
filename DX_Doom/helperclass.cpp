////////////////////////////////////////////////////////////////////////////////
// Filename: helperclass.cpp
////////////////////////////////////////////////////////////////////////////////
#include "helperclass.h"

// 리팩토링을 위한 클래스

XMVECTOR SubVector(XMFLOAT3* source, XMFLOAT3* dest)
{
	return XMLoadFloat3(dest) - XMLoadFloat3(source);
}

// Distance()
float Distance(XMFLOAT3* source, XMFLOAT3* dest)
{
	return XMVectorGetX(XMLoadFloat3(dest) - XMLoadFloat3(source));
}
float Distance(XMFLOAT3* source, XMFLOAT3&& dest)
{
	XMVECTOR s = XMLoadFloat3(source);
	XMVECTOR d = XMLoadFloat3(&dest);

	return XMVectorGetX(d - s);
}
float Distance(XMFLOAT3&& source, XMFLOAT3* dest)
{
	XMVECTOR s = XMLoadFloat3(&source);
	XMVECTOR d = XMLoadFloat3(dest);

	return XMVectorGetX(d - s);
}
float Distance(XMFLOAT3&& source, XMFLOAT3&& dest)
{
	XMVECTOR s = XMLoadFloat3(&source);
	XMVECTOR d = XMLoadFloat3(&dest);

	return XMVectorGetX(d - s);
}