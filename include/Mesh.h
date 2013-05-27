#ifndef _MESH_H
#define _MESH_H

#include <windows.h>
#include "st.h"
#include <vector>
#include "Face.h"
#include "Vertex.h"
#include "Edge.h"

using namespace std;

class Mesh
{
public:
	Mesh(void);
	~Mesh(void);
	void AddVertex(const float p_x, const float p_y, const float p_z);
	void AddNormal(const float p_x, const float p_y, const float p_z);
	void AddTexcoord(const float p_x, const float p_y, const float p_z);
	int AddFace(const int p_v1, const int p_v2, const int p_v3, const int p_mat);
	void AddFaceNormals(const int f, const int p_n1, const int p_n2, const int p_n3);
	void AddFaceTexcoords(const int f, const int p_t1, const int p_t2, const int p_t3);
	void GetFaceVectors(const int f, STPoint3 &p_p1, STPoint3 &p_p2, STPoint3 &p_p3, STVector3 &p_nf);
	void GetFaceVectors(const int f, STPoint3 &p_p1, STPoint3 &p_p2, STPoint3 &p_p3, STVector3 &p_n1, STVector3 &p_n2, STVector3 &p_n3);
	void GetFaceTexcoords(const int f, STVector3 &p_t1, STVector3 &p_t2, STVector3 &p_t3);
	int GetFaceMaterial(const int f);
	Mesh* LoopSubdivide();
	int GetVertexCount() { return vertices.size(); }
	int GetNormalCount() { return normals.size(); }
	int GetTexcoordCount() { return texcoords.size(); }
	int GetFaceCount() { return faces.size(); }
	int GetEdgeCount() { return edges.size(); }
	STVector3 GetMinCoords() { return minCoords; }
	STVector3 GetMaxCoords() { return maxCoords; }
	void DisplayLastFace(void);					// for debugging

private:
	vector<Vertex> vertices;
	vector<STVector3> normals;
	vector<STVector3> texcoords;
	vector<Face> faces;
	vector<Edge> edges;
	STVector3 minCoords;
	STVector3 maxCoords;

	int PutEdge(const int p_p1, const int p_p2, const int p_p3);
	STVector3 GetVertexNormal(const int p_v);
	int GetMidpoint(const int e, Mesh *nMesh);
};

#endif //_MESH_H


