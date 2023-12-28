#pragma once
#include "vertex.h"


class Edge
{
public:
	Vertex v1;
	Vertex v2;
	bool isBad;

	Edge()
	{
		this->isBad = false;
	}
	Edge(const Vertex& v1, const Vertex& v2)
	{
		this->v1 = v1;
		this->v2 = v2;
		isBad = false;
	}
	Edge(const Edge& e)
	{
		this->v1 = e.v1;
		this->v2 = e.v2;
		this->isBad = e.isBad;
	}
	bool operator==(const Edge& e)
	{
		return (v1 == e.v1 && v2 == e.v2) || (v1 == e.v2 && v2 == e.v1);
	}
	
};