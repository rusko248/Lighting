#ifndef _VERTEX_H
#define _VERTEX_H

#include <windows.h>
#include "st.h"
#include <vector>

using namespace std;

// --------------------------------------------------------------------------
// Structure to contain data pertinent to a specific vertex, as follows:
//    point            = coordinates of the vertex
//    normal           = calculated normal vector based on the normals of the
//                       faces it is part of.  As new faces are added to the
//                       adjoinFaces vector, their face normal is accumulated
//                       and the resulting vector is normalized when needed
//    normalNormalized = boolean to control a one-time normalization of the
//                       normal
//    adjoin           = indices of the vertices this vertex is adjacent to
//    adjoinFaces      = indices of the faces the vertex is part of
// --------------------------------------------------------------------------

struct Vertex
{
    STPoint3 point;
    STVector3 normal;
    bool normalNormalized;
	vector<int> adjoin;
	vector<int> adjoinFaces;

    Vertex(const float p_v1, const float p_v2, const float p_v3) {
		point = STPoint3(p_v1, p_v2, p_v3);
		normal = STVector3(0.0f, 0.0f, 0.0f);
		normalNormalized = false;
	}

	bool AddAdjoinVertex(const int p) {
		for (unsigned int i = 0; i < adjoin.size(); i++) {
			if (adjoin[i] == p) return false;	// Already set as adjoining
		}
		adjoin.push_back(p);
		return true;			// New adjoining vertex
	}

	void AddAdjoinFace(const int f, const STVector3 faceNormal) {
		adjoinFaces.push_back(f);
		normal += faceNormal;
	}
};

#endif //_VERTEX_H
