////////////////////////////////////////////////////////////////////////////////
// Filename: astarclass.cpp
////////////////////////////////////////////////////////////////////////////////
#include "astarclass.h"

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

vector<XMFLOAT3> AStarClass::findPath()
{
	priority_queue<Node*, vector<Node*>, Compare> openList;
	vector<NodeInfo> allNodesVector;  // unordered_map 대신 vector 사용

	Node* startNode = new Node(m_startPoint);
	openList.push(startNode);
	allNodesVector.push_back({ m_startPoint, *startNode });

	while (!openList.empty()) 
	{
		Node current = *openList.top();
		openList.pop();

		if (current.position.x == m_endPoint.x && current.position.z == m_endPoint.z)
		{
			vector<XMFLOAT3> path;
			while (current.parent)
			{
				path.push_back({ (current.position.x * 2) - m_map->col, 0.0f, current.position.z * 2 });
				current = *current.parent;
			}
			reverse(path.begin(), path.end());
			return path;
		}

		for (int x = -1; x <= 1; x++)
		{
			for (int z = -1; z <= 1; z++)
			{
				XMFLOAT3 newPosition;
				newPosition.x = current.position.x + x;
				newPosition.z = current.position.z + z;

				if (newPosition.x == current.position.x && newPosition.z == current.position.z) continue;
				if (newPosition.x < 0 || newPosition.z < 0 || newPosition.x >= m_map->col || newPosition.z >= m_map->row) continue;
				if (m_map->map[(int)newPosition.z][(int)newPosition.x] == 1) continue;

				Node* newNode = new Node(newPosition);
				float newCost = current.g + heuristic(current.position, newPosition);

				bool found = false;
				for (auto& node : allNodesVector) 
				{
					if (node.key.x == newPosition.x && node.key.y == newPosition.y && node.key.z == newPosition.z)
					{
						found = true;
						if (newCost < node.value.g)
						{
							node.value.g = newCost;
							node.value.h = heuristic(newPosition, m_endPoint);
							node.value.parent = &current;
						}
						break;
					}
				}

				if (!found) 
				{
					newNode->g = newCost;
					newNode->h = heuristic(newPosition, m_endPoint);
					newNode->parent = &current;
					allNodesVector.push_back({ newPosition, *newNode });
					openList.push(newNode);
				}
			}
		}
	}
	vector<XMFLOAT3> v;
	return v; // 경로를 찾을 수 없음
}

bool AStarClass::isValid(const int z, const int x)
{
	return m_map->map[z][x] ? false : true;
}
