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
#include <list>

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
	class Node // 노드 클래스
	{
	public:
		Node* m_Parent;
		XMFLOAT3 m_position;
		XMFLOAT3 m_endPoint;
		float m_fValue;
		float m_gValue;
		float m_hValue;

	public:
		Node(XMFLOAT3, XMFLOAT3, Node*);
		Node();
		~Node();
	};

	class Map // 맵 클래스
	{
	public:
		int row, col;
		int** map;
	public:
		Map();
		~Map();
	};

public:
	AStarClass(XMFLOAT3 startPoint, XMFLOAT3 endPoint, int correction)
	{
		m_correction = correction;
		m_startPoint.x = (int)startPoint.x + m_correction;
		m_startPoint.z = (int)startPoint.z;
		m_endPoint.x = (int)endPoint.x + m_correction;
		m_endPoint.z = (int)endPoint.z;

		m_Navi = new Map();
	}
	~AStarClass()
	{ // path 동적할당 해제
		m_iter = m_path.begin();
		for (; m_iter != m_path.end(); m_iter++)
		{
			delete* m_iter;
		}
	}

private: // 내부 함수
	list<XMFLOAT3*> FindPath(Map*, XMFLOAT3, XMFLOAT3);

	// 노드리스트에서 x,y 좌표의 노드를 찾아서 주소를 반환. 없으면 end()반환
	list<Node*>::iterator FindNode(XMFLOAT3, list<Node*>*);

	// 오픈노드 중 F값이 제일 작은 노드 찾아서 반환
	list<Node*>::iterator FindNextNode(list<Node*>*); 

	// 8방향 노드를 탐색하고 열린 노드에 추가 및 부모 변경을 실행함
	void ExploreNode(Map* Navi, Node*, list<Node*>*, list<Node*>*, XMFLOAT3);

public:
	void FindPath();
	list<XMFLOAT3*> GetPath() { return m_path; } // 경로를 XMFLOAT3* 리스트로 통째로 받아옴

private:
	int m_row;
	int m_col;

	int m_correction;
	XMFLOAT3 m_startPoint; // 출발지점
	XMFLOAT3 m_endPoint; // 목표지점
	list<XMFLOAT3*> m_path; // 경로
	list<XMFLOAT3*>::iterator m_iter; // 경로 iterator

	Map* m_Navi; // 맵 생성
};

#endif


