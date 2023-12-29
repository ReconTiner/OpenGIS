#pragma once

#include <cmath>

// 顶点数据
struct Vertex
{
	double x, y, z;		// 坐标
	int num;			// 索引
	bool flag;			// 标志
};

// 顶点链表
struct vertexList
{
	Vertex* vertex;
	vertexList* next;
};

// 三角形边
struct Edge
{
	Vertex* v1;		// 边的端点1
	Vertex* v2;		// 边的端点2
	Vertex* v3;		// 边所对的顶点
	bool flag;
};

// 三角形边链表
struct edgeList
{
	Edge* edge;
	edgeList* next;
};

// 三角形链表
struct Triangle
{
	Edge* e1;
	Edge* e2;
	Edge* e3;
	Triangle* next;
};

// 构建第一个三角形
void BuildFirstTriangle(const vertexList* list, Triangle*& tglNode, edgeList*& lineHead, edgeList*& lineLast, Edge*& e1, Edge*& e2, Edge*& e3, Vertex*& v1, Vertex*& v2, Vertex*& v3)
{
	v1 = list->vertex;		//数据链中的首点
	//找离pt1最近点pt2
	v2 = list->next->vertex;//数据链表中的第二点

	vertexList* node;
	node = list->next->next;
	while (node != nullptr)
	{
		if (JudgeDis(v1, v2, node->vertex))
			v2 = node->vertex;
		node = node->next;
	}

	//找第三个点
	//重新初始化node为链表第一点的next
	node = list->next;
	getPoint3(node, v3, v1, v2);

	//根据三点构建三角形
	edgeList* edgeNode;
	//构建边ln1
	edgeNode = new edgeList();
	BuildLine(pt1, pt2, pt3, linenode);
	ln1 = linenode->line;

	//设置边链表的头尾指针
	linehead = linelast = linenode;

	//构建边ln2
	linenode = new Linelist();
	BuildLine(pt2, pt3, pt1, linenode);
	ln2 = linenode->line;
	//设置边链表的尾指针
	AddLine(linelast, linenode);

	//构建边ln3
	linenode = new Linelist();
	BuildLine(pt3, pt1, pt2, linenode);
	ln3 = linenode->line;
	//设置边链表的尾指针
	AddLine(linelast, linenode);

	//first Triangle

	//构建三角形
	tglnode = new Triangle;
	tglnode->line1 = ln1;
	tglnode->line2 = ln2;
	tglnode->line3 = ln3;
	tglnode->next = NULL;
}

//比较链表中node与pt2到pt1的距离
bool JudgeDis(Vertex* pt1, Vertex* pt2, Vertex* node) 
{
	return (pt1->x - node->x) * (pt1->x - node->x) + (pt1->y - node->y) * (pt1->y - node->y) < (pt1->x - pt2->x) * (pt1->x - pt2->x) + (pt1->y - pt2->y) * (pt1->y - pt2->y);
}


//根据已知pt1 pt2 从node链表中寻找最佳pt3
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
			//比较当前结点pixel与pt3谁更合适--选取夹角最大的点
			if (JudgePoint(pt1, pt2, pt3, node->vertex))
				pt3 = node->vertex;
		}
		node = node->next;
	}
}

//比较链表中node pt3与pt1 pt2所成角的大小
bool JudgePoint(Vertex* pt1, Vertex* pt2, Vertex* pt3, Vertex* node) 
{
	double dist11 = sqrt((pt1->x - node->x) * (pt1->x - node->x) + (pt1->y - node->y) * (pt1->y - node->y));
	double dist12 = sqrt((pt2->x - node->x) * (pt2->x - node->x) + (pt2->y - node->y) * (pt2->y - node->y));
	double dist12_3 = sqrt((pt1->x - pt2->x) * (pt1->x - pt2->x) + (pt1->y - pt2->y) * (pt1->y - pt2->y));
	double dist21 = sqrt((pt1->x - pt3->x) * (pt1->x - pt3->x) + (pt1->y - pt3->y) * (pt1->y - pt3->y));
	double dist22 = sqrt((pt3->x - pt2->x) * (pt3->x - pt2->x) + (pt3->y - pt2->y) * (pt3->y - pt2->y));
	return ((pow(dist11, 2) + pow(dist12, 2) - pow(dist12_3, 2)) / (2 * dist11 * dist12) < (pow(dist21, 2) + pow(dist22, 2) - pow(dist12_3, 2)) / (2 * dist21 * dist22)); //余弦判断角度
}


//将当前边加入linenode边链表中
void BuildLine(Vertex* pt1, Vertex* pt2, Vertex* pt3, edgeList*& linenode) 
{
	linenode->edge->v1 = pt1;
	linenode->edge->v2 = pt2;
	linenode->edge->v3 = pt3;
	linenode->edge->flag = false;
	linenode->next = NULL;
}
