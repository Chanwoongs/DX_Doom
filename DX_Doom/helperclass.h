////////////////////////////////////////////////////////////////////////////////
// Filename: helperclass.h
////////////////////////////////////////////////////////////////////////////////
#ifndef _HELPERCLASS_H_
#define _HELPERCLASS_H_

//////////////
// INCLUDES //
//////////////
#include <DirectXMath.h>
#include <utility>

///////////////////////
// MY CLASS INCLUDES //
///////////////////////

using namespace DirectX;
using namespace std;

////////////////////////////////////////////////////////////////////////////////
// Class name: helperclass
////////////////////////////////////////////////////////////////////////////////

XMVECTOR SubVector(XMFLOAT3* source, XMFLOAT3* dest);

float Distance(XMFLOAT3* source, XMFLOAT3* dest);
float Distance(XMFLOAT3* source, XMFLOAT3&& dest);
float Distance(XMFLOAT3&& source, XMFLOAT3* dest);
float Distance(XMFLOAT3&& source, XMFLOAT3&& dest);

#endif