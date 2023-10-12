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

using namespace DirectX;
using namespace std;

///////////////////////
// MY CLASS INCLUDES //
///////////////////////

////////////////////////////////////////////////////////////////////////////////
// Class name: AStarClass
////////////////////////////////////////////////////////////////////////////////
class AStarClass
{
public:
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
			int** map;
		public:
			Map();
			~Map();
	};

public:
	AStarClass(XMFLOAT3 start, XMFLOAT3 end) : m_startPoint(start), m_endPoint(end)
	{
		m_map = new Map();
		m_startPoint.x = ((int)m_startPoint.x + m_map->col) / 2;
		m_startPoint.y = 0.0f;
		m_startPoint.z = (int)(m_startPoint.z / 2);
		m_endPoint.x = ((int)m_endPoint.x + m_map->col) / 2;
		m_endPoint.y = 0.0f;
		m_endPoint.z = (int)(m_endPoint.z / 2);
	}
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

private:
	XMFLOAT3 m_startPoint;
	XMFLOAT3 m_endPoint;
	Map* m_map;
};

#endif