#pragma once
#include "vertex.h"


class Edge
{
public:
	Edge(const Vertex& v1, const Vertex& v2)
	{
		this->v1 = v1;
		this->v2 = v2;
		isBad = false;
	}

private:
	Vertex v1;
	Vertex v2;
	bool isBad;
};