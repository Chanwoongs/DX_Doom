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

	if (m_Parent == NULL) // �θ� ���� ���
	{
		m_gValue = 0;
	}
	else if ( // ���� ������ ���
		(m_Parent->m_position.x == m_position.x - 1 && m_Parent->m_position.y == m_position.y) || // �θ� '��'���⿡ �ְų�
		(m_Parent->m_position.x == m_position.x + 1 && m_Parent->m_position.y == m_position.y) || // �θ� '��'���⿡ �ְų�
		(m_Parent->m_position.x == m_position.x && m_Parent->m_position.y == m_position.y - 1) || // �θ� '��'���⿡ �ְų�
		(m_Parent->m_position.x == m_position.x && m_Parent->m_position.y == m_position.y + 1)) // �θ� '��'���⿡ ������		
	{
		m_gValue = m_Parent->m_gValue + 10;
	}
	else if ( // �밢�� ������ ���
		(m_Parent->m_position.x - 1 && m_Parent->m_position.y - 1) || // �θ� '�»�'���⿡ �ְų�
		(m_Parent->m_position.x - 1 && m_Parent->m_position.y + 1) || // �θ� '���'���⿡ �ְų�
		(m_Parent->m_position.x + 1 && m_Parent->m_position.y - 1) || // �θ� '����'���⿡ �ְų�
		(m_Parent->m_position.x + 1 && m_Parent->m_position.y + 1)) // �θ� '����'���⿡ ������
	{
		m_gValue = m_Parent->m_gValue + 14;
	}
	else {
		m_fValue = -100000;
		m_gValue = -100000;
		m_hValue = -100000;
	}

	m_hValue = (abs(m_endPoint.x - m_endPoint.x) + abs(m_endPoint.y - m_endPoint.y)) * 10;

	m_fValue = m_gValue + m_hValue;
}

AStarClass::Node::~Node()
{

}


AStarClass::Map::Map(int sizeX, int sizeY)
{
	// �� ũ�� �Ҵ�
	map = new int* [sizeX]; // �� �Ҵ�
	for (int i = 0; i < sizeX; i++) // �� �Ҵ�
	{
		map[i] = new int[sizeY];
	}

	// ���⼭  �浹üũ�ؼ� ���ع����� �ƴ��� üũ�������
}

void AStarClass::Map::Copy(Map* _map)
{
	// �� ũ�� ����
	sizeX = _map->sizeX;
	sizeY = _map->sizeY;

	for (int i = 0; i < sizeX; i++)
	{
		for (int j = 0; j < sizeY; j++)
		{
			map[i][j] = _map->map[i][j];
		}
	}
}

AStarClass::Map::~Map()
{
	for (int i = 0; i < sizeX; i++)
	{
		delete[] map[i]; // ���� �� �����Ҵ� ����, ���� ���� ������ŭ �����Ҵ� �Ǿ����Ƿ� sizeX��ŭ �ݺ�
	}
	delete[] map; // ���� �� �����Ҵ� ����
}

void AStarClass::Map::PrintMap()
{
	for (int i = 0; i < sizeX; i++)
	{
		for (int j = 0; j < sizeY; j++)
		{
			printf("%d ", map[i][j]);
		}
		printf("\n");
	}
}

list<XMFLOAT3*> AStarClass::FindPath(Map* navi, XMFLOAT3 startPoint, XMFLOAT3 endPoint)
{
	// (��,��,��,��) 4���� �ð���� Ž�� �� ����� ���� (���,����,����,�»�) Ž��.	
	list<Node*> openNode; // �������
	list<Node*> closeNode; // �������
	Node* selectedNode; // ���õ� ���
	list<XMFLOAT3*> path;

	openNode.push_back(new Node(startPoint, endPoint, NULL)); // ���������� ������忡 �߰�

	list<Node*>::iterator iter;

	// ���� ��尡 ��ų�(��������� ����==��) �������� ����(������忡�� ���� �߰�)�� ��� ��������
	// �� ������ �ݴ�� '���� ��忡 ������ �ְų� �������� �� ã�� ���' �ݺ�
	while ((openNode.end() != openNode.begin()) && (openNode.end() == FindNode(endPoint, &openNode)))
	{
		iter = FindNextNode(&openNode); // ������� �� F���� ���� ���� ����� �ּҸ� ã�Ƽ� iter �� ����
		selectedNode = *iter; // ������� �� F���� ���� ���� ��带 SNode�� ����

		// ���õ� SNode �ֺ��� 8���� ��� Ž��, ���� ������ �� �ִ� ���� ���� ��� ���̹Ƿ� ���� ���� �ּҸ� ����.
		ExploreNode(navi, selectedNode, &openNode, &closeNode, endPoint);

		closeNode.push_back(selectedNode); // ���� Ž���� ��带 ���� ��忡 �߰�
		openNode.erase(iter); // ���� ��忡 �߰��� ��带 ���� ��忡�� ����
	}

	// ���� ã�� ���
	if ((openNode.end() != openNode.begin())) 
	{
		// �������� ��带 ã�Ƽ� iter�� ����
		iter = FindNode(endPoint, &openNode); 

		// �θ� NULL�� ������ path�� ��� ����
		for (selectedNode = *iter; selectedNode->m_Parent != NULL; selectedNode = selectedNode->m_Parent)  
		{
			path.push_back(new XMFLOAT3(selectedNode->m_position.x, selectedNode->m_position.y, 0.0f));
		}	

		// �θ� NULL�� ����� path���� ����(��� ����)
		path.push_back(new XMFLOAT3(selectedNode->m_position.x, selectedNode->m_position.y, 0.0f)); 

		// �������������� �������� �Է������Ƿ� �ٽ� �������� ������ ��������� ù ��°�� �ǵ��� ��.
		path.reverse(); 

		// ���� ã�� ��� �����Ҵ� ����
		iter = openNode.begin();
		for (; iter != openNode.end(); iter++)
		{
			delete* iter; // ���� ��� �����Ҵ� ����
		}
		iter = closeNode.begin();
		for (; iter != closeNode.end(); iter++)
		{
			delete* iter; // ���� ��� �����Ҵ� ����
		}

		return path; // ���� ã�� ��� ����
	}

	// ���� ã�� ���� ��� �����Ҵ� ����
	iter = closeNode.begin();
	for (; iter != closeNode.end(); iter++)
	{
		delete* iter; // ���� ��� �����Ҵ� ����
	}
	return path; // ���� ã�� ���� ��� ����
}

list<AStarClass::Node*>::iterator AStarClass::FindNode(XMFLOAT3 point, list<Node*>* nodeList)
{
	list<Node*>::iterator iter = nodeList->begin();

	for (int i = 1; iter != nodeList->end(); i++, iter++)
	{
		if ((*iter)->m_position.x == point.x && (*iter)->m_position.y == point.y)
		{
			return iter;
		}
	}

	return nodeList->end();
}

list<AStarClass::Node*>::iterator AStarClass::FindNextNode(list<Node*>* openNode)
{
	list<Node*>::iterator iter = (*openNode).begin();

	int minValue = 100000000; // ���� ���� ���� ���� ����
	int index = 0; // ���� ���� ���� �� ��°����

	for (int i = 1; iter != (*openNode).end(); i++, iter++)
	{
		if ((*iter)->m_fValue <= minValue) // F���� �۰ų� ���� ��츦 �߰��ϸ�(�̷��� ���� �� F���� ���� ��� �ֱٿ� �߰��� ���� �켱)
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
	// �� ����� ���� �밢�� ���� Ž�� ���θ� ����. true == ��ֹ� ����, false == ����
	bool up = true, right = true, down = true, left = true; 

	list<Node*>::iterator iter;
	XMFLOAT3 point;

	// '��' ���� Ž��
	point.x = selectedNode->m_position.x - 1;	point.y = selectedNode->m_position.y;
	if (selectedNode->m_position.x > 0 && navi->map[int(point.x)][int(point.y)] == 0) // '��' ���⿡ ���� �����ϰ� ��ֹ��� ���� ���
	{
		// ��ֹ��� ���� ��쿡 �ش��ϹǷ� ��ֹ� false ����
		up = false;

		// �̹� ���� ��忡 �ִ� ���
		if (openNode->end() != FindNode(point, openNode))
		{
			iter = FindNode(point, openNode);

			// ���� �θ� ���ؼ� ���� ���� ��뺸�� ���� ��带 ���ؼ� ���� �� ����� �� ������ ��� 
			if ((*iter)->m_gValue > (selectedNode->m_gValue + 10))
			{
				(*iter)->m_Parent = selectedNode; // ���� ��带 �θ�� �ٲ�
			}
		}

		// ���� ��忡 �ִ� ���
		else if (closeNode->end() != FindNode(point, closeNode))
		{
		}

		// ����⿡ ��ֹ��� ���� ���� ��� �� ���� ��忡 �߰��Ǿ����� ���� ���
		// ����� ��带 ���� ��忡 �߰�, �θ�� ���� Ž�� ���� ����.
		else
		{
			openNode->push_back(new Node(point, endPoint, selectedNode));
		}
	}
	// '��' ���� Ž��
	point.x = selectedNode->m_position.x;	point.y = selectedNode->m_position.y + 1;
	if (selectedNode->m_position.y < (navi->sizeY - 1) && navi->map[int(point.x)][int(point.y)] == 0) // '��' ���⿡ ���� �����ϰ� ��ֹ��� ���� ���
	{
		// ��ֹ��� ���� ��쿡 �ش��ϹǷ� ��ֹ� false ����
		right = false;

		// �̹� ���� ��忡 �ִ� ���
		if (openNode->end() != FindNode(point, openNode))
		{
			iter = FindNode(point, openNode);
			if ((*iter)->m_gValue > (selectedNode->m_gValue + 10)) // ���� �θ� ���ؼ� ���� ���� ��뺸�� ���� ��带 ���ؼ� ���� �� ����� �� ������ ��� 
			{
				(*iter)->m_Parent = selectedNode; // ���� ��带 �θ�� �ٲ�
			}
		}

		// ���� ��忡 �ִ� ���
		else if (closeNode->end() != FindNode(point, closeNode))
		{
		}

		// ����⿡ ��ֹ��� ���� ���� ��� �� ���� ��忡 �߰��Ǿ����� ���� ���
		// ����� ��带 ���� ��忡 �߰�, �θ�� ���� Ž�� ���� ����.
		else
		{
			openNode->push_back(new Node(point, endPoint, selectedNode));
		}
	}
	// '��' ���� Ž��
	point.x = selectedNode->m_position.x + 1;	point.y = selectedNode->m_position.y;
	if (selectedNode->m_position.x < (navi->sizeX - 1) && navi->map[int(point.x)][int(point.y)] == 0) // '��' ���⿡ ���� �����ϰ� ��ֹ��� ���� ���
	{
		// ��ֹ��� ���� ��쿡 �ش��ϹǷ� ��ֹ� false ����
		down = false;

		// �̹� ���� ��忡 �ִ� ���
		if (openNode->end() != FindNode(point, openNode))
		{
			iter = FindNode(point, openNode);
			if ((*iter)->m_gValue > (selectedNode->m_gValue + 10)) // ���� �θ� ���ؼ� ���� ���� ��뺸�� ���� ��带 ���ؼ� ���� �� ����� �� ������ ��� 
			{
				(*iter)->m_Parent = selectedNode; // ���� ��带 �θ�� �ٲ�
			}
		}

		// ���� ��忡 �ִ� ���
		else if (closeNode->end() != FindNode(point, closeNode))
		{
		}

		// ����⿡ ��ֹ��� ���� ���� ��� �� ���� ��忡 �߰��Ǿ����� ���� ���
		// ����� ��带 ���� ��忡 �߰�, �θ�� ���� Ž�� ���� ����.
		else
		{
			openNode->push_back(new Node(point, endPoint, selectedNode));
		}
	}
	// '��' ���� Ž��
	point.x = selectedNode->m_position.x;	point.y = selectedNode->m_position.y - 1;
	if (selectedNode->m_position.y > 0 && navi->map[int(point.x)][int(point.y)] == 0) // '��' ���⿡ ���� �����ϰ� ��ֹ��� ���� ���
	{
		// ��ֹ��� ���� ��쿡 �ش��ϹǷ� ��ֹ� false ����
		left = false;

		// �̹� ���� ��忡 �ִ� ���
		if (openNode->end() != FindNode(point, openNode))
		{
			iter = FindNode(point, openNode);
			if ((*iter)->m_gValue > (selectedNode->m_gValue + 10)) // ���� �θ� ���ؼ� ���� ���� ��뺸�� ���� ��带 ���ؼ� ���� �� ����� �� ������ ��� 
			{
				(*iter)->m_Parent = selectedNode; // ���� ��带 �θ�� �ٲ�
			}
		}

		// ���� ��忡 �ִ� ���
		else if (closeNode->end() != FindNode(point, closeNode))
		{
		}

		// ����⿡ ��ֹ��� ���� ���� ��� �� ���� ��忡 �߰��Ǿ����� ���� ���
		// ����� ��带 ���� ��忡 �߰�, �θ�� ���� Ž�� ���� ����.
		else
		{
			openNode->push_back(new Node(point, endPoint, selectedNode));
		}
	}

	// '���' ���� Ž��
	point.x = selectedNode->m_position.x - 1;	point.y = selectedNode->m_position.y + 1;
	if (selectedNode->m_position.x > 0 && selectedNode->m_position.y < (navi->sizeY - 1) && navi->map[int(point.x)][int(point.y)] == 0 &&
		up == false && right == false) // '���' ���⿡ ���� �����ϰ� ��ֹ��� ������, ������ ����⿡�� ��ֹ��� ���� ���
	{
		// �̹� ���� ��忡 �ִ� ���
		if (openNode->end() != FindNode(point, openNode))
		{
			iter = FindNode(point, openNode);
			if ((*iter)->m_gValue > (selectedNode->m_gValue + 14)) // ���� �θ� ���ؼ� ���� ���� ��뺸�� ���� ��带 ���ؼ� ���� �� ����� �� ������ ��� 
			{
				(*iter)->m_Parent = selectedNode; // ���� ��带 �θ�� �ٲ�
			}
		}

		// ���� ��忡 �ִ� ���
		else if (closeNode->end() != FindNode(point, closeNode))
		{
		}

		// ����⿡ ��ֹ��� ���� ���� ��� �� ���� ��忡 �߰��Ǿ����� ���� ���
		// ����� ��带 ���� ��忡 �߰�, �θ�� ���� Ž�� ���� ����.
		else
		{
			openNode->push_back(new Node(point, endPoint, selectedNode));
		}
	}
	// '����' ���� Ž��
	point.x = selectedNode->m_position.x + 1;	point.y = selectedNode->m_position.y + 1;
	if (selectedNode->m_position.x < (navi->sizeX - 1) && selectedNode->m_position.y < (navi->sizeY - 1) &&
		navi->map[int(point.x)][int(point.y)] == 0 && right == false && down == false)
		// '����' ���⿡ ���� �����ϰ� ��ֹ��� ������, ������ �Ϲ��⿡�� ��ֹ��� ���� ���
	{
		// �̹� ���� ��忡 �ִ� ���
		if (openNode->end() != FindNode(point, openNode))
		{
			iter = FindNode(point, openNode);
			if ((*iter)->m_gValue > (selectedNode->m_gValue + 14)) // ���� �θ� ���ؼ� ���� ���� ��뺸�� ���� ��带 ���ؼ� ���� �� ����� �� ������ ��� 
			{
				(*iter)->m_Parent = selectedNode; // ���� ��带 �θ�� �ٲ�
			}
		}

		// ���� ��忡 �ִ� ���
		else if (closeNode->end() != FindNode(point, closeNode))
		{
		}

		// ����⿡ ��ֹ��� ���� ���� ��� �� ���� ��忡 �߰��Ǿ����� ���� ���
		// ����� ��带 ���� ��忡 �߰�, �θ�� ���� Ž�� ���� ����.
		else
		{
			openNode->push_back(new Node(point, endPoint, selectedNode));
		}
	}
	// '����' ���� Ž��
	point.x = selectedNode->m_position.x + 1;	point.y = selectedNode->m_position.y - 1;
	if (selectedNode->m_position.x < (navi->sizeX - 1) && selectedNode->m_position.y > 0 && navi->map[int(point.x)][int(point.y)] == 0 &&
		left == false && down == false) // '����' ���⿡ ���� �����ϰ� ��ֹ��� ������, �¹���� �Ϲ��⿡�� ��ֹ��� ���� ���
	{
		// �̹� ���� ��忡 �ִ� ���
		if (openNode->end() != FindNode(point, openNode))
		{
			iter = FindNode(point, openNode);
			if ((*iter)->m_gValue > (selectedNode->m_gValue + 14)) // ���� �θ� ���ؼ� ���� ���� ��뺸�� ���� ��带 ���ؼ� ���� �� ����� �� ������ ��� 
			{
				(*iter)->m_Parent = selectedNode; // ���� ��带 �θ�� �ٲ�
			}
		}

		// ���� ��忡 �ִ� ���
		else if (closeNode->end() != FindNode(point, closeNode))
		{
		}

		// ����⿡ ��ֹ��� ���� ���� ��� �� ���� ��忡 �߰��Ǿ����� ���� ���
		// ����� ��带 ���� ��忡 �߰�, �θ�� ���� Ž�� ���� ����.
		else
		{
			openNode->push_back(new Node(point, endPoint, selectedNode));
		}
	}
	// '�»�' ���� Ž��
	point.x = selectedNode->m_position.x - 1;	point.y = selectedNode->m_position.y - 1;
	if (selectedNode->m_position.x > 0 && selectedNode->m_position.y > 0 && navi->map[int(point.x)][int(point.y)] == 0 &&
		left == false && up == false) // '�»�' ���⿡ ���� �����ϰ� ��ֹ��� ������, �¹���� ����⿡�� ��ֹ��� ���� ���
	{
		// �̹� ���� ��忡 �ִ� ���
		if (openNode->end() != FindNode(point, openNode))
		{
			iter = FindNode(point, openNode);
			if ((*iter)->m_gValue > (selectedNode->m_gValue + 14)) // ���� �θ� ���ؼ� ���� ���� ��뺸�� ���� ��带 ���ؼ� ���� �� ����� �� ������ ��� 
			{
				(*iter)->m_Parent = selectedNode; // ���� ��带 �θ�� �ٲ�
			}
		}

		// ���� ��忡 �ִ� ���
		else if (closeNode->end() != FindNode(point, closeNode))
		{
		}

		// ����⿡ ��ֹ��� ���� ���� ��� �� ���� ��忡 �߰��Ǿ����� ���� ���
		// ����� ��带 ���� ��忡 �߰�, �θ�� ���� Ž�� ���� ����.
		else
		{
			openNode->push_back(new Node(point, endPoint, selectedNode));
		}
	}
}

void AStarClass::FindPath()
{
	m_path = FindPath(m_Navi, m_startPoint, m_endPoint);
	m_iter = m_path.begin(); // iter �� ������� �����ֱ�
}



