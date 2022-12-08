////////////////////////////////////////////////////////////////////////////////
// Filename: astarclass.cpp
////////////////////////////////////////////////////////////////////////////////
#include "astarclass.h"

AStarClass::Node::Node()
{

}

AStarClass::Node::Node(XMFLOAT3 position, XMFLOAT3 endPoint, Node* parent)
{
	m_position = position;

	m_Parent = parent;
	m_endPoint = endPoint;

	if (m_Parent == NULL) // 부모가 없는 경우
	{
		m_gValue = 0;
	}
	else if ( // 십자 방향인 경우
		(m_Parent->m_position.x == m_position.x - 1 && m_Parent->m_position.z == m_position.z) || // 부모가 '상'방향에 있거나
		(m_Parent->m_position.x == m_position.x + 1 && m_Parent->m_position.z == m_position.z) || // 부모가 '하'방향에 있거나
		(m_Parent->m_position.x == m_position.x && m_Parent->m_position.z == m_position.z - 1) || // 부모가 '좌'방향에 있거나
		(m_Parent->m_position.x == m_position.x && m_Parent->m_position.z == m_position.z + 1)) // 부모가 '우'방향에 있으면		
	{
		m_gValue = m_Parent->m_gValue + 10;
	}
	else if ( // 대각선 방향인 경우
		(m_Parent->m_position.x - 1 && m_Parent->m_position.z - 1) || // 부모가 '좌상'방향에 있거나
		(m_Parent->m_position.x - 1 && m_Parent->m_position.z + 1) || // 부모가 '우상'방향에 있거나
		(m_Parent->m_position.x + 1 && m_Parent->m_position.z - 1) || // 부모가 '좌하'방향에 있거나
		(m_Parent->m_position.x + 1 && m_Parent->m_position.z + 1)) // 부모가 '우하'방향에 있으면
	{
		m_gValue = m_Parent->m_gValue + 14;
	}
	else {
		m_fValue = -100000;
		m_gValue = -100000;
		m_hValue = -100000;
	}

	m_hValue = (abs(m_endPoint.x - m_position.x) + abs(m_endPoint.z - m_position.z)) * 10;

	m_fValue = m_gValue + m_hValue;
}

AStarClass::Node::~Node()
{

}


AStarClass::Map::Map()
{
	// 맵의 크기를 불러옴
	char crow[5], ccol[4];
	GetPrivateProfileStringA("MAP", "row", "", crow, sizeof(crow) - 1, "./data/map.txt");
	GetPrivateProfileStringA("MAP", "col", "", ccol, sizeof(ccol) - 1, "./data/map.txt");
	row = atoi(crow); 
	col = atoi(ccol);

	// 맵 크기 할당
	map = new int* [row]; 
	for (int i = 0; i < row; i++) 
	{
		map[i] = new int[col];
	}

	FILE* stream = fopen("./data/map.txt", "rt"); // 맵의 자료를 불러옴 0=지나갈 수 있는 곳 , 1=장애물
	for (int i = 0; i < row; i++)
	{
		for (int j = 0; j < col; j++)
		{
			fscanf(stream, "%d", &map[i][j]);
		}
	}
	fclose(stream);

}


AStarClass::Map::~Map()
{
	for (int i = 0; i < row; i++)
	{
		delete[] map[i]; // 맵의 열 동적할당 해제, 열은 행의 갯수만큼 동적할당 되었으므로 row만큼 반복
	}
	delete[] map; // 맵의 행 동적할당 해제
}


list<XMFLOAT3*> AStarClass::FindPath(Map* navi, XMFLOAT3 startPoint, XMFLOAT3 endPoint)
{
	// (상,우,하,좌) 4방향 시계방향 탐색 후 결과에 따라 (우상,우하,좌하,좌상) 탐색.	
	list<Node*> openNode; // 열린노드
	list<Node*> closeNode; // 닫힌노드
	Node* selectedNode; // 선택된 노드
	list<XMFLOAT3*> path;

	if (navi->map[int(endPoint.z)][int(endPoint.x)] == 1)
	{
		return path;
	}

	openNode.push_back(new Node(startPoint, endPoint, NULL)); // 시작지점을 열린노드에 추가

	list<Node*>::iterator iter;

	// 열린 노드가 비거나(열린노드의 시작==끝) 목적지에 도착(열린노드에서 값이 발견)한 경우 끝내야함
	// 즉 조건은 반대로 '열린 노드에 내용이 있거나 목적지를 못 찾은 경우' 반복
	while ((openNode.end() != openNode.begin()) && (openNode.end() == FindNode(endPoint, &openNode)))
	{
		iter = FindNextNode(&openNode); // 열린노드 중 F값이 제일 작은 노드의 주소를 찾아서 iter 에 저장
		selectedNode = *iter; // 열린노드 중 F값이 제일 작은 노드를 SNode에 저장

		// 선택된 SNode 주변의 8방향 노드 탐색, 값이 수정될 수 있는 것은 열린 노드 뿐이므로 열린 노드는 주소를 전달.
		ExploreNode(navi, selectedNode, &openNode, &closeNode, endPoint);

		closeNode.push_back(selectedNode); // 현재 탐색한 노드를 닫힌 노드에 추가
		openNode.erase(iter); // 닫힌 노드에 추가한 노드를 열린 노드에서 제거
	}

	// 길을 찾은 경우
	if ((openNode.end() != openNode.begin())) 
	{
		// 목적지의 노드를 찾아서 iter에 저장
		iter = FindNode(endPoint, &openNode); 

		// 부모가 NULL일 때까지 path에 경로 저장
		for (selectedNode = *iter; selectedNode->m_Parent != NULL; selectedNode = selectedNode->m_Parent)  
		{
			path.push_back(new XMFLOAT3((selectedNode->m_position.x * 2) - m_correction, 0.0f, (selectedNode->m_position.z * 2)));
		}	

		// 부모가 NULL인 경우의 path까지 저장(출발 지점)
		path.push_back(new XMFLOAT3((selectedNode->m_position.x * 2) - m_correction, 0.0f, (selectedNode->m_position.z * 2)));

		// 목적지점으부터 역순으로 입력했으므로 다시 역순으로 뒤집어 출발지점이 첫 번째가 되도록 함.
		path.reverse(); 

		// 길을 찾은 경우 동적할당 해제
		iter = openNode.begin();
		for (; iter != openNode.end(); iter++)
		{
			delete* iter; // 열린 노드 동적할당 해제
		}
		iter = closeNode.begin();
		for (; iter != closeNode.end(); iter++)
		{
			delete* iter; // 닫힌 노드 동적할당 해제
		}

		return path; // 길을 찾은 경우 리턴
	}

	// 길을 찾지 못한 경우 동적할당 해제
	iter = closeNode.begin();
	for (; iter != closeNode.end(); iter++)
	{
		delete* iter; // 닫힌 노드 동적할당 해제
	}
	return path; // 길을 찾지 못한 경우 리턴
}

list<AStarClass::Node*>::iterator AStarClass::FindNode(XMFLOAT3 point, list<Node*>* nodeList)
{
	list<Node*>::iterator iter = nodeList->begin();

	for (int i = 1; iter != nodeList->end(); i++, iter++)
	{
		if ((*iter)->m_position.x == point.x && (*iter)->m_position.z == point.z)
		{
			return iter;
		}
	}

	return nodeList->end();
}

list<AStarClass::Node*>::iterator AStarClass::FindNextNode(list<Node*>* openNode)
{
	list<Node*>::iterator iter = (*openNode).begin();

	int minValue = 100000000; // 현재 제일 작은 값을 저장
	int index = 0; // 제일 작은 값이 몇 번째인지

	for (int i = 1; iter != (*openNode).end(); i++, iter++)
	{
		if ((*iter)->m_fValue <= minValue) // F값이 작거나 같은 경우를 발견하면(이렇게 구현 시 F값이 같은 경우 최근에 추가된 것이 우선)
		{
			minValue = (*iter)->m_fValue;
			index = i;
		}
	}

	iter = (*openNode).begin();
	for (int i = 1; i < index; i++)
	{
		iter++;
	}

	return iter;
}

void AStarClass::ExploreNode(Map* navi, Node* selectedNode, list<Node*>* openNode, list<Node*>* closeNode, XMFLOAT3 endPoint)
{
	// 이 결과에 따라 대각선 방향 탐색 여부를 결정. true == 장애물 있음, false == 없음
	bool up = true, right = true, down = true, left = true; 

	list<Node*>::iterator iter;
	XMFLOAT3 point;

	// '상' 방향 탐색
	point.x = selectedNode->m_position.x;	
	if (selectedNode->m_position.z < m_row - 1)
	{
		point.z = selectedNode->m_position.z + 1;
	}
	else if (int(selectedNode->m_position.z) == m_row - 1)
	{
		point.z = selectedNode->m_position.z;
	}
	if (selectedNode->m_position.z > 0 && navi->map[int(point.z)][int(point.x)] == 0) // '상' 방향에 맵이 존재하고 장애물이 없을 경우
	{
		// 장애물이 없는 경우에 해당하므로 장애물 false 세팅
		up = false;

		// 이미 열린 노드에 있는 경우
		if (openNode->end() != FindNode(point, openNode))
		{
			iter = FindNode(point, openNode);

			// 원래 부모를 통해서 갔을 때의 비용보다 현재 노드를 통해서 갔을 때 비용이 더 낮아질 경우 
			if ((*iter)->m_gValue > (selectedNode->m_gValue + 10))
			{
				(*iter)->m_Parent = selectedNode; // 현재 노드를 부모로 바꿈
			}
		}

		// 닫힌 노드에 있는 경우
		else if (closeNode->end() != FindNode(point, closeNode))
		{
		}

		// 상방향에 장애물이 없고 열린 노드 및 닫힌 노드에 추가되어있지 않은 경우
		// 상방향 노드를 열린 노드에 추가, 부모는 현재 탐색 노드로 지정.
		else
		{
			openNode->push_back(new Node(point, endPoint, selectedNode));
		}
	}
	// '우' 방향 탐색
	point.x = selectedNode->m_position.x + 1;	point.z = selectedNode->m_position.z;
	if (selectedNode->m_position.x < (navi->col - 1) && navi->map[int(point.z)][int(point.x)] == 0) // '우' 방향에 맵이 존재하고 장애물이 없을 경우
	{
		// 장애물이 없는 경우에 해당하므로 장애물 false 세팅
		right = false;

		// 이미 열린 노드에 있는 경우
		if (openNode->end() != FindNode(point, openNode))
		{
			iter = FindNode(point, openNode);
			if ((*iter)->m_gValue > (selectedNode->m_gValue + 10)) // 원래 부모를 통해서 갔을 때의 비용보다 현재 노드를 통해서 갔을 때 비용이 더 낮아질 경우 
			{
				(*iter)->m_Parent = selectedNode; // 현재 노드를 부모로 바꿈
			}
		}

		// 닫힌 노드에 있는 경우
		else if (closeNode->end() != FindNode(point, closeNode))
		{
		}

		// 상방향에 장애물이 없고 열린 노드 및 닫힌 노드에 추가되어있지 않은 경우
		// 상방향 노드를 열린 노드에 추가, 부모는 현재 탐색 노드로 지정.
		else
		{
			openNode->push_back(new Node(point, endPoint, selectedNode));
		}
	}
	// '하' 방향 탐색
	point.x = selectedNode->m_position.x;	
	if (selectedNode->m_position.z >= 1)
	{
		point.z = selectedNode->m_position.z - 1;
	}
	if (selectedNode->m_position.z < (navi->row - 1) && navi->map[int(point.z)][int(point.x)] == 0) // '하' 방향에 맵이 존재하고 장애물이 없을 경우
	{
		// 장애물이 없는 경우에 해당하므로 장애물 false 세팅
		down = false;

		// 이미 열린 노드에 있는 경우
		if (openNode->end() != FindNode(point, openNode))
		{
			iter = FindNode(point, openNode);
			if ((*iter)->m_gValue > (selectedNode->m_gValue + 10)) // 원래 부모를 통해서 갔을 때의 비용보다 현재 노드를 통해서 갔을 때 비용이 더 낮아질 경우 
			{
				(*iter)->m_Parent = selectedNode; // 현재 노드를 부모로 바꿈
			}
		}

		// 닫힌 노드에 있는 경우
		else if (closeNode->end() != FindNode(point, closeNode))
		{
		}

		// 상방향에 장애물이 없고 열린 노드 및 닫힌 노드에 추가되어있지 않은 경우
		// 상방향 노드를 열린 노드에 추가, 부모는 현재 탐색 노드로 지정.
		else
		{
			openNode->push_back(new Node(point, endPoint, selectedNode));
		}
	}
	// '좌' 방향 탐색
	point.x = selectedNode->m_position.x - 1;	point.z = selectedNode->m_position.z;
	if (selectedNode->m_position.x > 0 && navi->map[int(point.z)][int(point.x)] == 0) // '좌' 방향에 맵이 존재하고 장애물이 없을 경우
	{
		// 장애물이 없는 경우에 해당하므로 장애물 false 세팅
		left = false;

		// 이미 열린 노드에 있는 경우
		if (openNode->end() != FindNode(point, openNode))
		{
			iter = FindNode(point, openNode);
			if ((*iter)->m_gValue > (selectedNode->m_gValue + 10)) // 원래 부모를 통해서 갔을 때의 비용보다 현재 노드를 통해서 갔을 때 비용이 더 낮아질 경우 
			{
				(*iter)->m_Parent = selectedNode; // 현재 노드를 부모로 바꿈
			}
		}

		// 닫힌 노드에 있는 경우
		else if (closeNode->end() != FindNode(point, closeNode))
		{
		}

		// 상방향에 장애물이 없고 열린 노드 및 닫힌 노드에 추가되어있지 않은 경우
		// 상방향 노드를 열린 노드에 추가, 부모는 현재 탐색 노드로 지정.
		else
		{
			openNode->push_back(new Node(point, endPoint, selectedNode));
		}
	}

	// '우상' 방향 탐색
	point.x = selectedNode->m_position.x + 1;	
	if (selectedNode->m_position.z < m_row - 1)
	{
		point.z = selectedNode->m_position.z + 1;
	}
	else if (int(selectedNode->m_position.z) == m_row - 1)
	{
		point.z = selectedNode->m_position.z;
	}
	if (selectedNode->m_position.z > 0 && selectedNode->m_position.x < (navi->col - 1) && navi->map[int(point.z)][int(point.x)] == 0 &&
		up == false && right == false) // '우상' 방향에 맵이 존재하고 장애물이 없으며, 우방향과 상방향에도 장애물이 없을 경우
	{
		// 이미 열린 노드에 있는 경우
		if (openNode->end() != FindNode(point, openNode))
		{
			iter = FindNode(point, openNode);
			if ((*iter)->m_gValue > (selectedNode->m_gValue + 14)) // 원래 부모를 통해서 갔을 때의 비용보다 현재 노드를 통해서 갔을 때 비용이 더 낮아질 경우 
			{
				(*iter)->m_Parent = selectedNode; // 현재 노드를 부모로 바꿈
			}
		}

		// 닫힌 노드에 있는 경우
		else if (closeNode->end() != FindNode(point, closeNode))
		{
		}

		// 상방향에 장애물이 없고 열린 노드 및 닫힌 노드에 추가되어있지 않은 경우
		// 상방향 노드를 열린 노드에 추가, 부모는 현재 탐색 노드로 지정.
		else
		{
			openNode->push_back(new Node(point, endPoint, selectedNode));
		}
	}
	// '우하' 방향 탐색
	point.x = selectedNode->m_position.x + 1;	
	if (selectedNode->m_position.z >= 1)
	{
		point.z = selectedNode->m_position.z - 1;
	}
	if (selectedNode->m_position.z < (navi->row - 1) && selectedNode->m_position.x < (navi->col - 1) &&
		navi->map[int(point.z)][int(point.x)] == 0 && right == false && down == false)
		// '우하' 방향에 맵이 존재하고 장애물이 없으며, 우방향과 하방향에도 장애물이 없을 경우
	{
		// 이미 열린 노드에 있는 경우
		if (openNode->end() != FindNode(point, openNode))
		{
			iter = FindNode(point, openNode);
			if ((*iter)->m_gValue > (selectedNode->m_gValue + 14)) // 원래 부모를 통해서 갔을 때의 비용보다 현재 노드를 통해서 갔을 때 비용이 더 낮아질 경우 
			{
				(*iter)->m_Parent = selectedNode; // 현재 노드를 부모로 바꿈
			}
		}

		// 닫힌 노드에 있는 경우
		else if (closeNode->end() != FindNode(point, closeNode))
		{
		}

		// 상방향에 장애물이 없고 열린 노드 및 닫힌 노드에 추가되어있지 않은 경우
		// 상방향 노드를 열린 노드에 추가, 부모는 현재 탐색 노드로 지정.
		else
		{
			openNode->push_back(new Node(point, endPoint, selectedNode));
		}
	}
	// '좌하' 방향 탐색
	point.x = selectedNode->m_position.x - 1;
	if (selectedNode->m_position.z >= 1)
	{
		point.z = selectedNode->m_position.z - 1;
	}
	if (selectedNode->m_position.z < (navi->row - 1) && selectedNode->m_position.x > 0 && 
		navi->map[int(point.z)][int(point.x)] == 0 &&
		left == false && down == false) // '좌하' 방향에 맵이 존재하고 장애물이 없으며, 좌방향과 하방향에도 장애물이 없을 경우
	{
		// 이미 열린 노드에 있는 경우
		if (openNode->end() != FindNode(point, openNode))
		{
			iter = FindNode(point, openNode);
			if ((*iter)->m_gValue > (selectedNode->m_gValue + 14)) // 원래 부모를 통해서 갔을 때의 비용보다 현재 노드를 통해서 갔을 때 비용이 더 낮아질 경우 
			{
				(*iter)->m_Parent = selectedNode; // 현재 노드를 부모로 바꿈
			}
		}

		// 닫힌 노드에 있는 경우
		else if (closeNode->end() != FindNode(point, closeNode))
		{
		}

		// 상방향에 장애물이 없고 열린 노드 및 닫힌 노드에 추가되어있지 않은 경우
		// 상방향 노드를 열린 노드에 추가, 부모는 현재 탐색 노드로 지정.
		else
		{
			openNode->push_back(new Node(point, endPoint, selectedNode));
		}
	}
	// '좌상' 방향 탐색
	point.x = selectedNode->m_position.x - 1;
	if (selectedNode->m_position.z < m_row - 1)
	{
		point.z = selectedNode->m_position.z + 1;
	}
	else if (int(selectedNode->m_position.z) == m_row - 1)
	{
		point.z = selectedNode->m_position.z;
	}
	if (selectedNode->m_position.z > 0 && selectedNode->m_position.x > 0 && navi->map[int(point.z)][int(point.x)] == 0 &&
		left == false && up == false) // '좌상' 방향에 맵이 존재하고 장애물이 없으며, 좌방향과 상방향에도 장애물이 없을 경우
	{
		// 이미 열린 노드에 있는 경우
		if (openNode->end() != FindNode(point, openNode))
		{
			iter = FindNode(point, openNode);
			if ((*iter)->m_gValue > (selectedNode->m_gValue + 14)) // 원래 부모를 통해서 갔을 때의 비용보다 현재 노드를 통해서 갔을 때 비용이 더 낮아질 경우 
			{
				(*iter)->m_Parent = selectedNode; // 현재 노드를 부모로 바꿈
			}
		}

		// 닫힌 노드에 있는 경우
		else if (closeNode->end() != FindNode(point, closeNode))
		{
		}

		// 상방향에 장애물이 없고 열린 노드 및 닫힌 노드에 추가되어있지 않은 경우
		// 상방향 노드를 열린 노드에 추가, 부모는 현재 탐색 노드로 지정.
		else
		{
			openNode->push_back(new Node(point, endPoint, selectedNode));
		}
	}
}

void AStarClass::FindPath()
{
	m_row = m_Navi->row;
	m_col = m_Navi->col;
	m_path = FindPath(m_Navi, m_startPoint, m_endPoint);
	m_iter = m_path.begin(); // iter 값 원래대로 돌려주기
}



