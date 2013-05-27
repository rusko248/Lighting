#ifndef _FACE_H
#define _FACE_H

#include <windows.h>
#include "st.h"
#include <vector>
#include "Vertex.h"

using namespace std;

// --------------------------------------------------------------------------
// Structure to contain data for each face in a mesh.  Most of the values in
// this structure are indexes of the vectors storing parameter values.  For
// this reason the supporting vectors need to be loaded first.  This should
// be no problem given the structure of an OBJ file.  The face values are as
// follows:
//    v1, v2, v3    = the indexes of the three vertices of the face triangle
//                    in the vertices vector for the mesh.  The vertices
//                    should be specified the correct sequence for
//                    calculation of the normal for the face.
//    n1, n2, n3    = the indexes of the normals for the corresponding
//                    vertices if specified.  These values correspond to the
//                    normals vector for the mesh.
//    v1, v2, v3    = the texture map coordinates for the corresponding
//                    vertices if specified.  These values correspond to the 
//                    texcoords vector for the mesh.
//    oe1, oe2, oe3 = the indexes of the edges opposite the corresponding
//                    vertices.  These are indexes for the edges vector and
//                    are maintained by the Mesh PutEdge function.  The are
//                    used in the Loop subdivision function.
//    normal        = the normal vector for the face based on the three
//                    vertices and normally calculated by the constructor.
//    mat           = available to store a materials index for the face.
//                    At present, these materials are not part of the Mesh
//                    itself, but are the responsibility of the calling
//                    program.
// --------------------------------------------------------------------------

struct Face
{
    int v1, v2, v3;
    int n1, n2, n3;
    int t1, t2, t3;
	int oe1, oe2, oe3;	// Opposite labeled by opposing vertex
	STVector3 normal;
	int mat;
    Face(const int p_v1, const int p_v2, const int p_v3, const int p_mat, vector<Vertex> &p_vertices) {
		v1 = p_v1;
		v2 = p_v2;
		v3 = p_v3;
		n1 = -1;
		n2 = -1;
		n3 = -1;
		t1 = -1;
		t2 = -1;
		t3 = -1;
		oe1 = -1;
		oe2 = -1;
		oe3 = -1;
		mat = p_mat;
		normal = STVector3::Cross(STVector3(p_vertices[v1].point) - STVector3(p_vertices[v2].point), STVector3(p_vertices[v2].point) - STVector3(p_vertices[v3].point));
		normal.Normalize();
	}
};

#endif //_FACE_H
