#pragma once
#include <glm/glm.hpp>
#include <cmath>

class Vertex
{
public:
	/* 构造函数 */
	Vertex() : x(0.0), y(0.0) {};
	Vertex(double x, double y)
	{
		this->x = x;
		this->y = y;
	}
	Vertex(const Vertex& v)
	{
		this->x = v.x;
		this->y = v.y;
	}
	/* 成员函数 */
	// 计算两点之间的距离
	double distance(const Vertex& v) const
	{
		return sqrt(hypotf(x - v.x, y - v.y));
	}

	bool operator== (const Vertex& v)
	{
		return (x == v.x) && (y == v.y);
	}
private:
	// 坐标
	double x;
	double y;
};