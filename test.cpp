#include "triangle.h"
#include <iostream>

int main()
{
	Vertex v1(0, 1);
	Vertex v2(1, 0);
	Vertex v3(0, -1);

	Vertex p(0.9, 0);
	Triangle t(v1, v2, v3);
	if (t.containsVertex(p))
	{
		std::cout << "true";
	}
	else
	{
		std::cout << "false";
	}
}