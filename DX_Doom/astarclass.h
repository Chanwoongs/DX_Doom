////////////////////////////////////////////////////////////////////////////////
// Filename: astarclass.h
////////////////////////////////////////////////////////////////////////////////
#ifndef _ASTARCLASS_H_
#define _ASTARCLASS_H_


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
	class Node // ��� Ŭ����
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

	class Map // �� Ŭ����
	{
	public:
		int sizeX, sizeY;
		int** map;
	public:
		void Copy(Map*); // �Ű������� Map ���� �޾ƿ��� �Ű��������� �������簡 �Ͼ.
		void PrintMap();
	public:
		Map(int, int);
		~Map();
	};

public:
	AStarClass(XMFLOAT3 startPoint, XMFLOAT3 endPoint, int sizeX, int sizeY)
	{
		m_startPoint.x = startPoint.x;
		m_startPoint.y = startPoint.y;
		m_endPoint.x = endPoint.x;
		m_endPoint.y = endPoint.y;

		m_Navi = new Map(sizeX, sizeY);
	}
	~AStarClass()
	{ // path �����Ҵ� ����
		m_iter = m_path.begin();
		for (; m_iter != m_path.end(); m_iter++)
		{
			delete* m_iter;
		}
	}

private: // ���� �Լ�
	list<XMFLOAT3*> FindPath(Map*, XMFLOAT3, XMFLOAT3);

	// ��帮��Ʈ���� x,y ��ǥ�� ��带 ã�Ƽ� �ּҸ� ��ȯ. ������ end()��ȯ
	list<Node*>::iterator FindNode(XMFLOAT3, list<Node*>*);

	// ���³�� �� F���� ���� ���� ��� ã�Ƽ� ��ȯ
	list<Node*>::iterator FindNextNode(list<Node*>*); 

	// 8���� ��带 Ž���ϰ� ���� ��忡 �߰� �� �θ� ������ ������
	void ExploreNode(Map* Navi, Node*, list<Node*>*, list<Node*>*, XMFLOAT3);

public:
	void FindPath();
	list<XMFLOAT3*> GetPath() { return m_path; } // ��θ� XMFLOAT3* ����Ʈ�� ��°�� �޾ƿ�

private:
	int m_sizeX;
	int m_sizeY;

	XMFLOAT3 m_startPoint; // �������
	XMFLOAT3 m_endPoint; // ��ǥ����
	list<XMFLOAT3*> m_path; // ���
	list<XMFLOAT3*>::iterator m_iter; // ��� iterator

	Map* m_Navi; // �� ����
};

#endif