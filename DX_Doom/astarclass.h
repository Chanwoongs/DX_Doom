////////////////////////////////////////////////////////////////////////////////
// Filename: astarclass.h
////////////////////////////////////////////////////////////////////////////////
#ifndef _ASTARCLASS_H_
#define _ASTARCLASS_H_

#define _CRT_SECURE_NO_WARNINGS

//////////////
// INCLUDES //
//////////////
#include <d3d11.h>
#include <directxmath.h>
#include <vector>
#include <queue>
#include <unordered_map>
#include <iostream>

using namespace DirectX;
using namespace std;

///////////////////////
// MY CLASS INCLUDES //
///////////////////////

////////////////////////////////////////////////////////////////////////////////
// Class name: AStarClass
////////////////////////////////////////////////////////////////////////////////

class Node
{
public:
	XMFLOAT3 position;

	Node() : position({ 0, 0, 0 }), g(0.0f), h(0.0f), parent(nullptr) {}
	Node(XMFLOAT3& pos) : position(pos), g(0.0f), h(0.0f), parent(nullptr) {}

	float totalCost() const { return g + h; }

	Node* parent;
	float g; // start에서 현재 위치까지의 비용
	float h; // heuristic 
};

class Map
{
public:
	int row, col;
	vector<vector<int>> map;
public:
	Map();
	~Map();
};

class AStarClass
{
private:
	AStarClass() {}

public:
	void setInfo(XMFLOAT3 start, XMFLOAT3 end)
	{
		if (m_map == nullptr) createMap();

		m_startPoint.x = ((int)start.x + m_map->col) / 2;
		m_startPoint.y = 0.0f;
		m_startPoint.z = (int)(start.z / 2);
		m_endPoint.x = ((int)end.x + m_map->col) / 2;
		m_endPoint.y = 0.0f;
		m_endPoint.z = (int)(end.z / 2);
	}

	void createMap() { m_map = new Map(); }

	vector<XMFLOAT3> findPath();

	int hash(XMFLOAT3 point) {
		return (int)point.x * 73856093 ^ (int)point.z * 19349663;
	}

	float heuristic(const XMFLOAT3& a, const XMFLOAT3& b)
	{
		return abs(a.x - b.x) + abs(a.z - b.z);
	}

	struct Compare 
	{
		bool operator() (const Node* a, const Node* b) 
		{
			return a->totalCost() > b->totalCost();
		}
	};

	bool isValid(const int z, const int x);

	static AStarClass* GetInstance()
	{
		if (Instance == nullptr)
		{
			Instance = new AStarClass();
		}
		return Instance;
	}

private:
	static AStarClass* Instance;

	XMFLOAT3 m_startPoint;
	XMFLOAT3 m_endPoint;
	Map* m_map;
};

#endif