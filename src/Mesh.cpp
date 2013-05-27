#include "Mesh.h"

const float oneEighth = 0.125f;
const float threeEighths = 0.375f;
const float threeSixteenths = 0.1875f;
const bool debug = true;

Mesh::Mesh(void)
{
}


Mesh::~Mesh(void)
{
}

void Mesh::AddVertex(const float p_x, const float p_y, const float p_z)
{
	int i = vertices.size();

	vertices.push_back(Vertex(p_x, p_y, p_z));

	if (i == 0) {
		minCoords = STVector3(vertices[i].point);
		maxCoords = STVector3(vertices[i].point);
	} else {
		minCoords = STVector3::ComponentMin(minCoords, STVector3(vertices[i].point));
		maxCoords = STVector3::ComponentMax(maxCoords, STVector3(vertices[i].point));
	}
}

void Mesh::AddNormal(const float p_x, const float p_y, const float p_z)
{
	normals.push_back(STVector3(p_x, p_y, p_z));
}

void Mesh::AddTexcoord(const float p_x, const float p_y, const float p_z)
{
	texcoords.push_back(STVector3(p_x, p_y, p_z));
}

int Mesh::AddFace(const int p_v1, const int p_v2, const int p_v3, const int p_mat)
{
	int f = faces.size();
	faces.push_back(Face(p_v1, p_v2, p_v3, p_mat, vertices));		// Add face
	vertices[p_v1].AddAdjoinFace(f, faces[f].normal);				// Add as adjoining face for vector
	vertices[p_v2].AddAdjoinFace(f, faces[f].normal);				//  and update normal
	vertices[p_v3].AddAdjoinFace(f, faces[f].normal);
	return f;
}

void Mesh::AddFaceNormals(const int f, const int p_n1, const int p_n2, const int p_n3)
{
	faces[f].n1 = p_n1;
	faces[f].n2 = p_n2;
	faces[f].n3 = p_n3;
}

void Mesh::AddFaceTexcoords(const int f, const int p_t1, const int p_t2, const int p_t3)
{
	faces[f].t1 = p_t1;
	faces[f].t2 = p_t2;
	faces[f].t3 = p_t3;
}

void Mesh::GetFaceVectors(const int f, STPoint3 &p_p1, STPoint3 &p_p2, STPoint3 &p_p3, STVector3 &p_nf)
{
	p_p1 = vertices[faces[f].v1].point;
	p_p2 = vertices[faces[f].v2].point;
	p_p3 = vertices[faces[f].v3].point;
	p_nf = faces[f].normal;
}

void Mesh::GetFaceVectors(const int f, STPoint3 &p_p1, STPoint3 &p_p2, STPoint3 &p_p3, STVector3 &p_n1, STVector3 &p_n2, STVector3 &p_n3)
{
	p_p1 = vertices[faces[f].v1].point;
	p_p2 = vertices[faces[f].v2].point;
	p_p3 = vertices[faces[f].v3].point;
	p_n1 = GetVertexNormal(faces[f].v1);
	p_n2 = GetVertexNormal(faces[f].v2);
	p_n3 = GetVertexNormal(faces[f].v3);
}

void Mesh::GetFaceTexcoords(const int f, STVector3 &p_t1, STVector3 &p_t2, STVector3 &p_t3)
{
	p_t1 = texcoords[faces[f].t1];
	p_t2 = texcoords[faces[f].t2];
	p_t3 = texcoords[faces[f].t3];
}

int Mesh::GetFaceMaterial(const int f)
{
	return faces[f].mat;
}

Mesh* Mesh::LoopSubdivide() {

	Mesh* rtnMesh = new Mesh();

	STVector3 newVertex;
	float beta;

	for (unsigned int i = 0; i < vertices.size(); i++) {
		newVertex = STVector3();
		unsigned int fMax = vertices[i].adjoinFaces.size();
		for (unsigned int f = 0; f < fMax; f++) {
			int v1 = faces[vertices[i].adjoinFaces[f]].v1;
			int v2 = faces[vertices[i].adjoinFaces[f]].v2;
			int v3 = faces[vertices[i].adjoinFaces[f]].v3;
			if (v1 == i) {
				faces[vertices[i].adjoinFaces[f]].oe1 = PutEdge(v2, v3, v1);
				if (vertices[i].AddAdjoinVertex(v2)) newVertex += STVector3(vertices[v2].point);
				if (vertices[i].AddAdjoinVertex(v3)) newVertex += STVector3(vertices[v3].point);
			} else if (v2 == i) {
				faces[vertices[i].adjoinFaces[f]].oe2 = PutEdge(v1, v3, v2);
				if (vertices[i].AddAdjoinVertex(v1)) newVertex += STVector3(vertices[v1].point);
				if (vertices[i].AddAdjoinVertex(v3)) newVertex += STVector3(vertices[v3].point);
			} else {
				faces[vertices[i].adjoinFaces[f]].oe3 = PutEdge(v1, v2, v3);
				if (vertices[i].AddAdjoinVertex(v1)) newVertex += STVector3(vertices[v1].point);
				if (vertices[i].AddAdjoinVertex(v2)) newVertex += STVector3(vertices[v2].point);
			}
		}
		if (vertices[i].adjoin.size() < 3) newVertex = STVector3(vertices[i].point);
		else {
			if (vertices[i].adjoin.size() == 3)	beta = threeSixteenths;
			else								beta = threeEighths / vertices[i].adjoin.size();
			newVertex *= beta;
			newVertex += STVector3(vertices[i].point) * (1.f - beta * vertices[i].adjoin.size());
		}
		rtnMesh->AddVertex(newVertex.x, newVertex.y, newVertex.z);
		if (debug && (i == 0)) {
			printf("Mesh::LoopSubdivide: added vertices[%d] = (%f, %f, %f)\n", rtnMesh->GetVertexCount() - 1, newVertex.x, newVertex.y, newVertex.z);
		}
	}

	for (unsigned int i = 0; i < faces.size(); i++) {
		int oe1mp = GetMidpoint(faces[i].oe1, rtnMesh);
		int oe2mp = GetMidpoint(faces[i].oe2, rtnMesh);
		int oe3mp = GetMidpoint(faces[i].oe3, rtnMesh);
		int mat = faces[i].mat;
		rtnMesh->AddFace(faces[i].v1, oe3mp, oe2mp, mat);		// add faces anchored by
		if (debug && (i == 0)) {
			printf("\nMesh::LoopSubdivide: added face[%d] vertices = (%d, %d, %d)\n", rtnMesh->GetFaceCount() - 1, faces[i].v1, oe3mp, oe2mp);
			rtnMesh->DisplayLastFace();
		}
		rtnMesh->AddFace(faces[i].v2, oe1mp, oe3mp, mat);		//  current vertices
		if (debug && (i == 0)) {
			printf("\nMesh::LoopSubdivide: added face[%d] vertices = (%d, %d, %d)\n", rtnMesh->GetFaceCount() - 1, faces[i].v2, oe1mp, oe3mp);
			rtnMesh->DisplayLastFace();
		}
		rtnMesh->AddFace(faces[i].v3, oe2mp, oe1mp, mat);		
		if (debug && (i == 0)) {
			printf("\nMesh::LoopSubdivide: added face[%d] vertices = (%d, %d, %d)\n", rtnMesh->GetFaceCount() - 1, faces[i].v3, oe2mp, oe1mp);
			rtnMesh->DisplayLastFace();
		}
		rtnMesh->AddFace(oe3mp, oe1mp, oe2mp, mat);				// add new face from midpoints
		if (debug && (i == 0)) {
			printf("\nMesh::LoopSubdivide: added face[%d] vertices = (%d, %d, %d)\n", rtnMesh->GetFaceCount() - 1, oe3mp, oe1mp, oe2mp);
			rtnMesh->DisplayLastFace();
		}
	}

	return rtnMesh;
}

//
// PutEdge - Checks for the existence of an edge with the same endpoints
//             If none exists, adds the edge with the third parameter as a vertex opposing the edge.
//             If found, adds the third parameter as a second vertex opposing the edge.
//             (e.g. for triangle v1,v2,v3 an edge of v1,v2 has an opposing vertex of v3)
//
int Mesh::PutEdge(const int p_p1, const int p_p2, const int p_p3) {
	int p1, p2;
	unsigned int i;

	if  (p_p1 > p_p2) {
		p1 = p_p2;
		p2 = p_p1;
	} else {
		p1 = p_p1;
		p2 = p_p2;
	}

	for (i = 0; i < edges.size(); i++) {
		if (edges[i].p1 == p1 && edges[i].p2 == p2) {		// Update with second opposing vertex
			edges[i].AddOpp(p_p3);
			return i;
		}
	}

	i = edges.size();
	edges.push_back(Edge(p1, p2, p_p3));					// Add with first opposing vertex
	return i;
}

STVector3 Mesh::GetVertexNormal(const int p_v)
{
	if (!vertices[p_v].normalNormalized) {
		vertices[p_v].normal.Normalize();
		vertices[p_v].normalNormalized = true;
	}

	return vertices[p_v].normal;
}

int Mesh::GetMidpoint(const int e, Mesh *nMesh) {
	STVector3 midVector, workVector;

	if (edges[e].midPoint != -1) return edges[e].midPoint;

	if ((edges[e].p1 < 0) || (edges[e].p2 < 0) || (edges[e].op1 < 0) || (edges[e].op2 < 0)) {
		printf("Mesh::GetMidpoint: edge[%d]  p1 = %d   p2 = %d   op1 = %d   op2 = %d\n", e, edges[e].p1, edges[e].p2, edges[e].op1, edges[e].op2);
		return edges[e].midPoint;
	}

	midVector  = STVector3(vertices[edges[e].p1].point);		// This looks cumbersome but it is done to optimize STVector3 operations
	midVector += STVector3(vertices[edges[e].p2].point);
	midVector *= threeEighths;
	workVector  = STVector3(vertices[edges[e].op1].point);
	workVector += STVector3(vertices[edges[e].op2].point);
	workVector *= oneEighth;
	midVector += workVector;

	edges[e].midPoint = nMesh->GetVertexCount();
	nMesh->AddVertex(midVector.x, midVector.y, midVector.z);

	if (debug && (nMesh->GetVertexCount() >= 27) && (nMesh->GetVertexCount() <= 29)) {
		printf("Mesh::LoopSubdivide: added vertices[%d] = (%f, %f, %f)\n", nMesh->GetVertexCount() - 1, midVector.x, midVector.y, midVector.z);
	}

	return edges[e].midPoint;
}

void Mesh::DisplayLastFace(void) {
	STPoint3 p1, p2, p3;
	STVector3 pn;
	int newface = this->GetFaceCount() - 1;
	this->GetFaceVectors(newface, p1, p2, p3, pn);
	printf("Mesh::DisplayLastFace: faces[%d]  v1 = %d   (%f, %f, %f)\n", newface, this->faces[newface].v1, p1.x, p1.y, p1.z);
	printf("                                 v2 = %d   (%f, %f, %f)\n", this->faces[newface].v2, p2.x, p2.y, p2.z);
	printf("                                 v3 = %d   (%f, %f, %f)\n", this->faces[newface].v3, p3.x, p3.y, p3.z);
}