#pragma once
#include <glm/glm.hpp>
#include <cmath>

class Vertex
{
public:
	/* ���캯�� */
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
	/* ��Ա���� */
	// ��������֮��ľ���
	double distance(const Vertex& v) const
	{
		return sqrt(hypotf(x - v.x, y - v.y));
	}

	bool operator== (const Vertex& v)
	{
		return (x == v.x) && (y == v.y);
	}
private:
	// ����
	double x;
	double y;
};