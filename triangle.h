#pragma once
#include "edge.h"


class Triangle
{
public:
	// 三个顶点
	Vertex v1;
	Vertex v2;
	Vertex v3;
	// 三条边
	Edge e1;
	Edge e2;
	Edge e3;

	bool isBad;

	// 构造函数
	Triangle(const Vertex& v1, const Vertex& v2, const Vertex& v3)
	{
		this->v1 = v1;
		this->v2 = v2;
		this->v3 = v3;

		e1 = Edge(v1, v2);
		e2 = Edge(v2, v3);
		e3 = Edge(v3, v1);

		this->isBad = false;
	}

	/* 成员函数 */
	// 判断一点是否在三角形内部
	bool containsVertex(const Vertex& v) const
	{
		double AB = (v2.x - v1.x) * (v.y - v1.y) - (v2.y - v1.y) * (v.x - v1.x);
		double BC = (v3.x - v2.x) * (v.y - v2.y) - (v3.y - v2.y) * (v.x - v2.x);
		double CA = (v1.x - v3.x) * (v.y - v3.y) - (v1.y - v3.y) * (v.x - v3.x);
		if (((AB > 0 && BC > 0 && CA > 0) || (AB < 0 && BC < 0 && CA < 0)))
			return true;
		else
			return false;
	}
	// 判断一点是否在外接圆内（包括外接圆上）
	bool circumCircleContains(const Vertex& v) const
	{

	}
};