////////////////////////////////////////////////////////////////////////////////
// Filename: navmesh.h
////////////////////////////////////////////////////////////////////////////////
#ifndef _NAVMESHCLASS_H_
#define _NAVMESHCLASS_H_


//////////////
// INCLUDES //
//////////////
#include <d3d11.h>
#include <directxmath.h>
#include <vector>

using namespace DirectX;

///////////////////////
// MY CLASS INCLUDES //
///////////////////////
#include "textureclass.h"
#include "nodeclass.h"

using namespace std;

////////////////////////////////////////////////////////////////////////////////
// Class name: NavmeshClass
////////////////////////////////////////////////////////////////////////////////
class NavmeshClass
{
public:
	NavmeshClass(int, int);
	NavmeshClass(const NavmeshClass&);
	~NavmeshClass();

	bool Initialize(ID3D11Device*, HWND);
	void Shutdown();
	void Render(ID3D11DeviceContext*);

	vector<NodeClass*>& GetNodes() { return m_Nodes; }

	void SetNodePositions();

private:
	vector<NodeClass*> m_Nodes;
	XMFLOAT3* m_nodePositions;

	int m_sizeX;
	int m_sizeY;

	int m_size;

};

#endif