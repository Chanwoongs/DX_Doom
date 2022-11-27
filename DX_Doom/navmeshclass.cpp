////////////////////////////////////////////////////////////////////////////////
// Filename: navmeshclass.cpp
////////////////////////////////////////////////////////////////////////////////
#include "navmeshclass.h"


NavmeshClass::NavmeshClass(int sizeX, int sizeY)
{
	m_sizeX = sizeX;
	m_sizeY = sizeY;
	m_size = sizeX * sizeY;

	m_nodePositions = new XMFLOAT3[m_size];
	SetNodePositions();
}


NavmeshClass::NavmeshClass(const NavmeshClass& other)
{
}


NavmeshClass::~NavmeshClass()
{
}


bool NavmeshClass::Initialize(ID3D11Device* device, HWND hwnd)
{
	bool result;

	for (int i = 0; i < m_size; i++)
	{
		auto node = new NodeClass(m_nodePositions[i]);
		if (!node)
		{
			return false;
		}

		m_Nodes.push_back(node);
	}

	vector<NodeClass*>::iterator iter;
	iter = m_Nodes.begin();
	for (; iter != m_Nodes.end(); iter++)
	{
		result = (*iter)->Initialize(device, L"./data/EM_Pyramid.obj", L"./data/ET_Seafloor.dds", hwnd);
		if (!result)
		{
			MessageBox(hwnd, L"Could not initialize the node object.", L"Error", MB_OK);
			return false;
		}
	}

	return true;
}


void NavmeshClass::Shutdown()
{
	for (int i = 0; i < m_size; i++)
	{
		m_Nodes.at(i)->Shutdown();
		delete m_Nodes.at(i);
	}

	delete[] m_nodePositions;
	
	return;
}


void NavmeshClass::Render(ID3D11DeviceContext* deviceContext)
{

	return;
}

void NavmeshClass::SetNodePositions()
{
	for (int i = 0; i < m_sizeY; i++)
	{
		for (int j = 0; j < m_sizeX; j++)
		{
			m_nodePositions[10 * i + j].x = -(m_sizeX / 2) + j;
			m_nodePositions[10 * i + j].y = i;
			m_nodePositions[10 * i + j].z = 0.0f;
		}
	}
}
