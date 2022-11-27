//////////////////////////////////////////////////////////////////////////////////
//// Filename: nodeclass.cpp
//////////////////////////////////////////////////////////////////////////////////
//#include "nodeclass.h"
//
//
//NodeClass::NodeClass(XMFLOAT3 position, XMFLOAT3 endPoint, NodeClass* parent)
//{
//	m_position = position;
//
//	m_Parent = parent;
//	m_endPoint = endPoint;
//
//	if (m_Parent == NULL) // 부모가 없는 경우
//	{
//		m_gValue = 0;
//	}
//	else if ( // 십자 방향인 경우
//		(m_Parent->m_position.x == m_position.x - 1 && m_Parent->m_position.y == m_position.y) || // 부모가 '상'방향에 있거나
//		(m_Parent->m_position.x == m_position.x + 1 && m_Parent->m_position.y == m_position.y) || // 부모가 '하'방향에 있거나
//		(m_Parent->m_position.x == m_position.x && m_Parent->m_position.y == m_position.y - 1) || // 부모가 '좌'방향에 있거나
//		(m_Parent->m_position.x == m_position.x && m_Parent->m_position.y == m_position.y + 1)) // 부모가 '우'방향에 있으면		
//	{
//		m_gValue = m_Parent->m_gValue + 10;
//	}
//	else if ( // 대각선 방향인 경우
//		(m_Parent->m_position.x - 1 && m_Parent->m_position.y - 1) || // 부모가 '좌상'방향에 있거나
//		(m_Parent->m_position.x - 1 && m_Parent->m_position.y + 1) || // 부모가 '우상'방향에 있거나
//		(m_Parent->m_position.x + 1 && m_Parent->m_position.y - 1) || // 부모가 '좌하'방향에 있거나
//		(m_Parent->m_position.x + 1 && m_Parent->m_position.y + 1)) // 부모가 '우하'방향에 있으면
//	{
//		m_gValue = m_Parent->m_gValue + 14;
//	}
//	else {
//		m_fValue = -100000;
//		m_gValue = -100000;
//		m_hValue = -100000;
//	}
//
//	m_hValue = (abs(m_endPoint.x - m_endPoint.x) + abs(m_endPoint.y - m_endPoint.y)) * 10;
//
//	m_fValue = m_gValue + m_hValue;
//}
//
//
//NodeClass::NodeClass(const NodeClass& other)
//{
//}
//
//
//NodeClass::~NodeClass()
//{
//}
//
//
//bool NodeClass::Initialize(ID3D11Device* device, const WCHAR* modelFilename, const WCHAR* textureFilename, HWND hwnd)
//{
//	bool result;
//
//	m_Model = new ModelClass(m_position, 1);
//	if (!m_Model)
//	{
//		return false;
//	}
//
//	// Initialize the model object.
//	result = m_Model->Initialize(device, L"./data/EM_Pyramid.obj", L"./data/ET_Seafloor.dds");
//	if (!result)
//	{
//		MessageBox(hwnd, L"Could not initialize the model object.", L"Error", MB_OK);
//
//		return false;
//	}
//
//	return true;
//}
//
//
//void NodeClass::Shutdown()
//{
//	delete m_Model;
//
//	return;
//}
//
//
//
