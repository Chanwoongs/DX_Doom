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
		(m_Parent->m_position.x == m_position.x - 1 && m_Parent->m_position.z == m_position.z) || // �θ� '��'���⿡ �ְų�
		(m_Parent->m_position.x == m_position.x + 1 && m_Parent->m_position.z == m_position.z) || // �θ� '��'���⿡ �ְų�
		(m_Parent->m_position.x == m_position.x && m_Parent->m_position.z == m_position.z - 1) || // �θ� '��'���⿡ �ְų�
		(m_Parent->m_position.x == m_position.x && m_Parent->m_position.z == m_position.z + 1)) // �θ� '��'���⿡ ������		
	{
		m_gValue = m_Parent->m_gValue + 10;
	}
	else if ( // �밢�� ������ ���
		(m_Parent->m_position.x - 1 && m_Parent->m_position.z - 1) || // �θ� '�»�'���⿡ �ְų�
		(m_Parent->m_position.x - 1 && m_Parent->m_position.z + 1) || // �θ� '���'���⿡ �ְų�
		(m_Parent->m_position.x + 1 && m_Parent->m_position.z - 1) || // �θ� '����'���⿡ �ְų�
		(m_Parent->m_position.x + 1 && m_Parent->m_position.z + 1)) // �θ� '����'���⿡ ������
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
	// ���� ũ�⸦ �ҷ���
	char crow[5], ccol[4];
	GetPrivateProfileStringA("MAP", "row", "", crow, sizeof(crow) - 1, "./data/map.txt");
	GetPrivateProfileStringA("MAP", "col", "", ccol, sizeof(ccol) - 1, "./data/map.txt");
	row = atoi(crow); 
	col = atoi(ccol);

	// �� ũ�� �Ҵ�
	map = new int* [row]; 
	for (int i = 0; i < row; i++) 
	{
		map[i] = new int[col];
	}

	FILE* stream = fopen("./data/map.txt", "rt"); // ���� �ڷḦ �ҷ��� 0=������ �� �ִ� �� , 1=��ֹ�
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
		delete[] map[i]; // ���� �� �����Ҵ� ����, ���� ���� ������ŭ �����Ҵ� �Ǿ����Ƿ� row��ŭ �ݺ�
	}
	delete[] map; // ���� �� �����Ҵ� ����
}


list<XMFLOAT3*> AStarClass::FindPath(Map* navi, XMFLOAT3 startPoint, XMFLOAT3 endPoint)
{
	// (��,��,��,��) 4���� �ð���� Ž�� �� ����� ���� (���,����,����,�»�) Ž��.	
	list<Node*> openNode; // �������
	list<Node*> closeNode; // �������
	Node* selectedNode; // ���õ� ���
	list<XMFLOAT3*> path;

	if (navi->map[int(endPoint.z)][int(endPoint.x)] == 1)
	{
		return path;
	}

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
			path.push_back(new XMFLOAT3((selectedNode->m_position.x * 2) - m_correction, 0.0f, (selectedNode->m_position.z * 2)));
		}	

		// �θ� NULL�� ����� path���� ����(��� ����)
		path.push_back(new XMFLOAT3((selectedNode->m_position.x * 2) - m_correction, 0.0f, (selectedNode->m_position.z * 2)));

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
	point.x = selectedNode->m_position.x;	
	if (selectedNode->m_position.z < m_row - 1)
	{
		point.z = selectedNode->m_position.z + 1;
	}
	else if (int(selectedNode->m_position.z) == m_row - 1)
	{
		point.z = selectedNode->m_position.z;
	}
	if (selectedNode->m_position.z > 0 && navi->map[int(point.z)][int(point.x)] == 0) // '��' ���⿡ ���� �����ϰ� ��ֹ��� ���� ���
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
	point.x = selectedNode->m_position.x + 1;	point.z = selectedNode->m_position.z;
	if (selectedNode->m_position.x < (navi->col - 1) && navi->map[int(point.z)][int(point.x)] == 0) // '��' ���⿡ ���� �����ϰ� ��ֹ��� ���� ���
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
	point.x = selectedNode->m_position.x;	
	if (selectedNode->m_position.z >= 1)
	{
		point.z = selectedNode->m_position.z - 1;
	}
	if (selectedNode->m_position.z < (navi->row - 1) && navi->map[int(point.z)][int(point.x)] == 0) // '��' ���⿡ ���� �����ϰ� ��ֹ��� ���� ���
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
	point.x = selectedNode->m_position.x - 1;	point.z = selectedNode->m_position.z;
	if (selectedNode->m_position.x > 0 && navi->map[int(point.z)][int(point.x)] == 0) // '��' ���⿡ ���� �����ϰ� ��ֹ��� ���� ���
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
	point.x = selectedNode->m_position.x + 1;	
	if (selectedNode->m_position.z >= 1)
	{
		point.z = selectedNode->m_position.z - 1;
	}
	if (selectedNode->m_position.z < (navi->row - 1) && selectedNode->m_position.x < (navi->col - 1) &&
		navi->map[int(point.z)][int(point.x)] == 0 && right == false && down == false)
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
	point.x = selectedNode->m_position.x - 1;
	if (selectedNode->m_position.z >= 1)
	{
		point.z = selectedNode->m_position.z - 1;
	}
	if (selectedNode->m_position.z < (navi->row - 1) && selectedNode->m_position.x > 0 && 
		navi->map[int(point.z)][int(point.x)] == 0 &&
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
	m_row = m_Navi->row;
	m_col = m_Navi->col;
	m_path = FindPath(m_Navi, m_startPoint, m_endPoint);
	m_iter = m_path.begin(); // iter �� ������� �����ֱ�
}



