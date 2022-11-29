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
	int m_row;
	int m_col;

	int m_correction;
	XMFLOAT3 m_startPoint; // �������
	XMFLOAT3 m_endPoint; // ��ǥ����
	list<XMFLOAT3*> m_path; // ���
	list<XMFLOAT3*>::iterator m_iter; // ��� iterator

	Map* m_Navi; // �� ����
};

#endif


