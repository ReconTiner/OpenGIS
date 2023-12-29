#pragma once

#include <cmath>

// ��������
struct Vertex
{
	double x, y, z;		// ����
	int num;			// ����
	bool flag;			// ��־
};

// ��������
struct vertexList
{
	Vertex* vertex;
	vertexList* next;
};

// �����α�
struct Edge
{
	Vertex* v1;		// �ߵĶ˵�1
	Vertex* v2;		// �ߵĶ˵�2
	Vertex* v3;		// �����ԵĶ���
	bool flag;
};

// �����α�����
struct edgeList
{
	Edge* edge;
	edgeList* next;
};

// ����������
struct Triangle
{
	Edge* e1;
	Edge* e2;
	Edge* e3;
	Triangle* next;
};

// ������һ��������
void BuildFirstTriangle(const vertexList* list, Triangle*& tglNode, edgeList*& lineHead, edgeList*& lineLast, Edge*& e1, Edge*& e2, Edge*& e3, Vertex*& v1, Vertex*& v2, Vertex*& v3)
{
	v1 = list->vertex;		//�������е��׵�
	//����pt1�����pt2
	v2 = list->next->vertex;//���������еĵڶ���

	vertexList* node;
	node = list->next->next;
	while (node != nullptr)
	{
		if (JudgeDis(v1, v2, node->vertex))
			v2 = node->vertex;
		node = node->next;
	}

	//�ҵ�������
	//���³�ʼ��nodeΪ�����һ���next
	node = list->next;
	getPoint3(node, v3, v1, v2);

	//�������㹹��������
	edgeList* edgeNode;
	//������ln1
	edgeNode = new edgeList();
	BuildLine(pt1, pt2, pt3, linenode);
	ln1 = linenode->line;

	//���ñ������ͷβָ��
	linehead = linelast = linenode;

	//������ln2
	linenode = new Linelist();
	BuildLine(pt2, pt3, pt1, linenode);
	ln2 = linenode->line;
	//���ñ������βָ��
	AddLine(linelast, linenode);

	//������ln3
	linenode = new Linelist();
	BuildLine(pt3, pt1, pt2, linenode);
	ln3 = linenode->line;
	//���ñ������βָ��
	AddLine(linelast, linenode);

	//first Triangle

	//����������
	tglnode = new Triangle;
	tglnode->line1 = ln1;
	tglnode->line2 = ln2;
	tglnode->line3 = ln3;
	tglnode->next = NULL;
}

//�Ƚ�������node��pt2��pt1�ľ���
bool JudgeDis(Vertex* pt1, Vertex* pt2, Vertex* node) 
{
	return (pt1->x - node->x) * (pt1->x - node->x) + (pt1->y - node->y) * (pt1->y - node->y) < (pt1->x - pt2->x) * (pt1->x - pt2->x) + (pt1->y - pt2->y) * (pt1->y - pt2->y);
}


//������֪pt1 pt2 ��node������Ѱ�����pt3
void getPoint3(vertexList*& node, Vertex*& pt3, Vertex* pt1, Vertex* pt2) 
{
	pt3 = nullptr;
	while (node != nullptr)
	{
		if (node->vertex == pt1 || node->vertex == pt2)
		{
			node = node->next;
			continue;
		}

		if (pt3 == nullptr)
		{
			pt3 = node->vertex;
		}
		else
		{
			//�Ƚϵ�ǰ���pixel��pt3˭������--ѡȡ�н����ĵ�
			if (JudgePoint(pt1, pt2, pt3, node->vertex))
				pt3 = node->vertex;
		}
		node = node->next;
	}
}

//�Ƚ�������node pt3��pt1 pt2���ɽǵĴ�С
bool JudgePoint(Vertex* pt1, Vertex* pt2, Vertex* pt3, Vertex* node) 
{
	double dist11 = sqrt((pt1->x - node->x) * (pt1->x - node->x) + (pt1->y - node->y) * (pt1->y - node->y));
	double dist12 = sqrt((pt2->x - node->x) * (pt2->x - node->x) + (pt2->y - node->y) * (pt2->y - node->y));
	double dist12_3 = sqrt((pt1->x - pt2->x) * (pt1->x - pt2->x) + (pt1->y - pt2->y) * (pt1->y - pt2->y));
	double dist21 = sqrt((pt1->x - pt3->x) * (pt1->x - pt3->x) + (pt1->y - pt3->y) * (pt1->y - pt3->y));
	double dist22 = sqrt((pt3->x - pt2->x) * (pt3->x - pt2->x) + (pt3->y - pt2->y) * (pt3->y - pt2->y));
	return ((pow(dist11, 2) + pow(dist12, 2) - pow(dist12_3, 2)) / (2 * dist11 * dist12) < (pow(dist21, 2) + pow(dist22, 2) - pow(dist12_3, 2)) / (2 * dist21 * dist22)); //�����жϽǶ�
}


//����ǰ�߼���linenode��������
void BuildLine(Vertex* pt1, Vertex* pt2, Vertex* pt3, edgeList*& linenode) 
{
	linenode->edge->v1 = pt1;
	linenode->edge->v2 = pt2;
	linenode->edge->v3 = pt3;
	linenode->edge->flag = false;
	linenode->next = NULL;
}
