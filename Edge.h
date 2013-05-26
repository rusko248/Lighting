#ifndef _EDGE_H
#define _EDGE_H

#include <windows.h>
#include "st.h"
#include <vector>
#include "Vertex.h"

using namespace std;

// --------------------------------------------------------------------------
// Structure to contain an edge in the mesh defined by two endpoints.  These
//  endpoints are identified by their vertices vector indexes.
// Edges are assumed to be part of at most two triangles.  The opposing
//  vertices are stored in op1 and op2 as their vertices vector indexes.
// --------------------------------------------------------------------------

struct Edge
{
    int p1, p2, op1, op2;
	int midPoint;
    Edge(const int v1, const int v2, const int v3) {
		p1 = v1;
		p2 = v2;
		op1 = v3;
		op2 = -1;
		midPoint = -1;
	}
    void AddOpp(const int v1) {
		op2 = v1;
	}
};

#endif //_EDGE_H
