////////////////////////////////////////////////////////////////////////////////
// Filename: nodeclass.h
////////////////////////////////////////////////////////////////////////////////
#ifndef _NODECLASS_H_
#define _NODECLASS_H_


//////////////
// INCLUDES //
//////////////
#include <d3d11.h>
#include <directxmath.h>

using namespace DirectX;

///////////////////////
// MY CLASS INCLUDES //
///////////////////////
#include "textureclass.h"
#include "modelclass.h"
#include "astarclass.h"

#include <fstream>
using namespace std;

////////////////////////////////////////////////////////////////////////////////
// Class name: NodeClass
////////////////////////////////////////////////////////////////////////////////
class NodeClass
{
public:
	NodeClass(XMFLOAT3, XMFLOAT3, NodeClass*);
	NodeClass(const NodeClass&);
	~NodeClass();
	
	friend class AStarClass;

	bool Initialize(ID3D11Device*, const WCHAR*, const WCHAR*, HWND);
	void Shutdown();

private:
	ModelClass* m_Model;

	NodeClass* m_Parent;
	XMFLOAT3 m_position;
	XMFLOAT3 m_endPoint;
	float m_fValue;
	float m_gValue;
	float m_hValue;
	bool m_isVaild;
};

#endif