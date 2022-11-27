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


bool NodeClass::Initialize(ID3D11Device* device, const WCHAR* modelFilename, const WCHAR* textureFilename, HWND hwnd)
{
	bool result;

	m_Model = new ModelClass(&m_position, 1);
	if (!m_Model)
	{
		return false;
	}

	// Initialize the model object.
	result = m_Model->Initialize(device, L"./data/EM_Cube.obj", L"./data/ET_Seafloor.dds");
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the model object.", L"Error", MB_OK);

		return false;
	}

	return true;
}


void NodeClass::Shutdown()
{
	delete m_Model;

	return;
}



