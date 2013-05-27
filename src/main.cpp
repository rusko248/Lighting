// Simple OBJ.cpp : Defines the entry point for the console application.
//

//#include <Windows.h>
#include "stglew.h"
#include <GL/GL.h>
#include <GL/GLU.h>
#include <iostream>
#include <iomanip>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <math.h>

#include "Mesh.h"
#include "Material.h"
#include "DeferredLighting.h"

static int gWindowSizeX = 512;
static int gWindowSizeY = 512;

static const int FILE_LINE_LEN = 1024;
const float pi = 3.1415926535f;

//Mesh *orig_mesh = new Mesh();
Mesh *mesh = new Mesh();;
vector<Material> materials;
bool useTexture = false;
bool matHasTexture;
STTransform4 trans_Point;
STTransform4 trans_Normal;
bool smooth = false;
int currMat = 0;
bool rotateEye = false;

float imageWidth;
float imageHeight;
STVector3 imageCenter;
float fovy = 30.f;
STVector3 eye;
float zNear = .1f;
float zFar = 100.f;
float delta;

//used for lighting
DeferredLighting* dfe;
std::string shaderPath = "../../shaders";	// Path for DeferredLighting shaders
bool useDeferred = true;

bool debug = true;
bool debugOnce = true;

/* FUNCTIONS */

void PrintTransform(char *desc, STTransform4 &matrix);
void LoadMaterials();
int LookupMaterial(const string name);
std::vector<int> ParseObjFace(const std::string faceString);
void ParseObjFile(std::string objFilename);
void SetUp();
int SetMaterial(const int p_m);
void DrawFace(const int p_f);
void DrawScene();
void DrawDeferred();
void DrawPointLights();
void DrawLittleLights();
void DisplayCallback();
void ReshapeCallback(int w, int h);
void ChangeEyePosition(const float dx, const float dy, const float dz);
void ChangeSceneRotation(const float dx, const float dy, const float dz);
void SpecialKeyCallback(int key, int x, int y);
void KeyCallback(unsigned char key, int x, int y);

//
// PrintTransform - helper to print the contents of a STTransform4 matrix
//
void PrintTransform(char *desc, STTransform4 &matrix)
{
	printf("%s matrix:\n", desc);
	float *row;
	row = matrix[0];
	printf("     [0] = (%f, %f, %f, %f)\n", row[0], row[1], row[2], row[3]);
	row = matrix[1];
	printf("     [1] = (%f, %f, %f, %f)\n", row[0], row[1], row[2], row[3]);
	row = matrix[2];
	printf("     [2] = (%f, %f, %f, %f)\n", row[0], row[1], row[2], row[3]);
	row = matrix[3];
	printf("     [3] = (%f, %f, %f, %f)\n", row[0], row[1], row[2], row[3]);
}

//
// LoadMaterials - load the materials vector with the various named materials
//                 that are referenced in the OBJ file
//
void LoadMaterials()
{
	materials.push_back(Material("_default"));
	materials[0].Ns = 30.0f;
	materials[0].Ni = 1.0f;
	materials[0].d = 1.0f;
	materials[0].Tf = STColor3f(1.0f, 1.0f, 1.0f);
	materials[0].illum = 2;
	materials[0].Ka = STColor3f(1.0f, 0.5f, 0.5f);
	materials[0].Kd = STColor3f(1.0f, 0.5f, 0.5f);
//	materials[0].Ks = STColor3f(1.0f, 0.5f, 0.5f);
	materials[0].Ks = STColor3f(0.5f, 0.25f, 0.25f);

	materials.push_back(Material("red"));
	materials[1].Ns = 10.0000f;
	materials[1].Ni = 1.5000f;
	materials[1].d = 1.0000f;
	materials[1].Tf = STColor3f(1.0000f, 1.0000f, 1.0000f);
	materials[1].illum = 2;
	materials[1].Ka = STColor3f(1.0000f, 0.0000f, 0.0000f);
	materials[1].Kd = STColor3f(1.0000f, 0.0000f, 0.0000f);
	materials[1].Ks = STColor3f(0.7000f, 0.0000f, 0.0000f);

	materials.push_back(Material("green"));
	materials[2].Ns = 10.0000f;
	materials[2].Ni = 1.5000f;
	materials[2].d = 1.0000f;
	materials[2].Tf = STColor3f(1.0000f, 1.0000f, 1.0000f);
	materials[2].illum = 2;
	materials[2].Ka = STColor3f(0.0000f, 1.0000f, 0.0000f);
	materials[2].Kd = STColor3f(0.0000f, 1.0000f, 0.0000f);
	materials[2].Ks = STColor3f(0.0000f, 0.0000f, 0.0000f);

	materials.push_back(Material("blue"));
	materials[3].Ns = 10.0000f;
	materials[3].Ni = 1.5000f;
	materials[3].d = 1.0000f;
	materials[3].Tf = STColor3f(1.0000f, 1.0000f, 1.0000f);
	materials[3].illum = 2;
	materials[3].Ka = STColor3f(0.0000f, 0.0000f, 1.0000f);
	materials[3].Kd = STColor3f(0.0000f, 0.0000f, 1.0000f);
	materials[3].Ks = STColor3f(0.0000f, 0.0000f, 0.0000f);

	materials.push_back(Material("yellow"));
	materials[4].Ns = 10.0000f;
	materials[4].Ni = 1.5000f;
	materials[4].d = 1.0000f;
	materials[4].Tf = STColor3f(1.0000f, 1.0000f, 1.0000f);
	materials[4].illum = 2;
	materials[4].Ka = STColor3f(1.0000f, 1.0000f, 0.0000f);
	materials[4].Kd = STColor3f(1.0000f, 1.0000f, 0.0000f);
	materials[4].Ks = STColor3f(0.0000f, 0.0000f, 0.0000f);

	materials.push_back(Material("cyan"));
	materials[5].Ns = 10.0000f;
	materials[5].Ni = 1.5000f;
	materials[5].d = 1.0000f;
	materials[5].Tf = STColor3f(1.0000f, 1.0000f, 1.0000f);
	materials[5].illum = 2;
	materials[5].Ka = STColor3f(0.0000f, 1.0000f, 1.0000f);
	materials[5].Kd = STColor3f(0.0000f, 1.0000f, 1.0000f);
	materials[5].Ks = STColor3f(0.0000f, 0.0000f, 0.0000f);

	materials.push_back(Material("magenta"));
	materials[6].Ns = 10.0000f;
	materials[6].Ni = 1.5000f;
	materials[6].d = 1.0000f;
	materials[6].Tf = STColor3f(1.0000f, 1.0000f, 1.0000f);
	materials[6].illum = 2;
	materials[6].Ka = STColor3f(1.0000f, 0.0000f, 1.0000f);
	materials[6].Kd = STColor3f(1.0000f, 0.0000f, 1.0000f);
	materials[6].Ks = STColor3f(0.0000f, 0.0000f, 0.0000f);

	materials.push_back(Material("black"));
	materials[7].Ns = 10.0000f;
	materials[7].Ni = 1.5000f;
	materials[7].d = 1.0000f;
	materials[7].Tf = STColor3f(1.0000f, 1.0000f, 1.0000f);
	materials[7].illum = 2;
	materials[7].Ka = STColor3f(0.0000f, 0.0000f, 0.0000f);
	materials[7].Kd = STColor3f(0.0000f, 0.0000f, 0.0000f);
	materials[7].Ks = STColor3f(0.0000f, 0.0000f, 0.0000f);
}

//
// LookupMaterial = look up the material by name and return its material
//                  vector index
//
int LookupMaterial(const string name)
{
	for (unsigned int m = 0; m < materials.size(); m++) {
		if (name.compare(materials[m].name) == 0) return m;
	}
	return 0;
}

//
// ParseObjFace - parse the string containing the parameters for a single vertex string
//
std::vector<int> ParseObjFace(const std::string faceString)
{
	std::vector<int> faceVals;
	std::string cvtString;
	int i, e, l;
	unsigned int s;
	for (i = 0; i < 3; i++) faceVals.push_back(-1);
	i = 0;
	s = 0;
	do {
		e = faceString.find("/", s);
		if (e == std::string::npos) e = faceString.length();
		l = e - s;
		if (l > 0) {
			cvtString = faceString.substr(s, l);
			faceVals[i] = std::stoi(cvtString) - 1;
		}
		i++;
		s = e + 1;
	} while (i < 3 && s < faceString.length());
	return faceVals;
}

//
// ParseObjFile - parse the contents of the requested OBJ file and load the appropriate Mesh
//                values
//
void ParseObjFile(std::string objFilename)
{
	std::ifstream objFile(objFilename.c_str());
	
	// Die if we couldn't find the file
	if (objFile.fail())
	{
		printf("ParseObjFile - Could not find input OBJ file '%s'\n", objFilename.c_str());
		delete mesh;
		exit(1);
	}

	char line[1024];
	while (!objFile.eof())
	{
		objFile.getline(line, 1023);
		std::stringstream ss;
		ss.str(line);
		std::string command;
		ss >> command;

		if ((command == "v") || (command == "vn") || (command == "vt"))
		{
			float x, y, z;
			ss >> x >> y >> z;
			if (command == "v")			mesh->AddVertex(x, y, z);
			else if (command == "vn")	mesh->AddNormal(x, y, z);
			else						mesh->AddTexcoord(x, y, z);
		}
		else
		if (command == "f")
		{
			std::string fs1, fs2, fs3;
			std::vector<int> faceData;
			int v1, v2, v3;
			int t1, t2, t3;
			int n1, n2, n3;
			ss >> fs1 >> fs2 >> fs3;
			faceData = ParseObjFace(fs1);
			v1 = faceData[0];
			t1 = faceData[1];
			n1 = faceData[2];
			faceData = ParseObjFace(fs2);
			v2 = faceData[0];
			t2 = faceData[1];
			n2 = faceData[2];
			faceData = ParseObjFace(fs3);
			v3 = faceData[0];
			t3 = faceData[1];
			n3 = faceData[2];
			int f = mesh->AddFace(v1, v2, v3, currMat);
			if (n1 >= 0) mesh->AddFaceNormals(f, n1, n2, n3);
			if (t1 >= 0) mesh->AddFaceTexcoords(f, t1, t2, t3);
		}
		else
		if (command == "usemtl")
		{
			string matName;
			ss >> matName;
			currMat = LookupMaterial(matName);
		}
	}
	objFile.close();
}

//
// SetUp - calculate various viewing parameters and initialize for initial display
//
void SetUp()
{
	STVector3 maxCoords = mesh->GetMaxCoords();
	STVector3 minCoords = mesh->GetMinCoords();
	imageCenter = maxCoords - minCoords;
//	float imageWidth = imageCenter.x;	changed to globals
//	float imageHeight = imageCenter.y;
	imageWidth = imageCenter.x;
	imageHeight = imageCenter.y;
	imageCenter /= 2.f;
	imageCenter += minCoords;
	eye.x = imageCenter.x;
	eye.y = imageCenter.y;
	if (imageWidth > imageHeight)	eye.z = imageWidth;
	else							eye.z = imageHeight;
	float zClip = eye.z;
	eye.z /= (2.0f * tan((fovy * pi / 180.f) / 2.0f));
	eye.z += maxCoords.z;
	if (zClip < imageCenter.z) zClip = imageCenter.z;
	if ((eye.z - zClip) < zNear)	zNear = eye.z - zClip;
	if ((eye.z + zClip) > zFar)		zFar = eye.z + zClip;
	delta = 2.0f * (pi / 180.0f);	// set rotation values to 2 degrees

	trans_Point = STTransform4::Identity();
//	trans_Point *= STTransform4::Translation(-imageCenter.x, -imageCenter.y, -imageCenter.z);

	trans_Normal = trans_Point.Inverse();
	trans_Normal = trans_Normal.Transpose();

	if (debug) {
		printf("SetUp: Finished\n");
		printf("minCoords   = (%f, %f, %f)\n", minCoords.x, minCoords.y, minCoords.z);
		printf("maxCoords   = (%f, %f, %f)\n", maxCoords.x, maxCoords.y, maxCoords.z);
		printf("imageCenter = (%f, %f, %f)\n", imageCenter.x, imageCenter.y, imageCenter.z);
		printf("eye         = (%f, %f, %f)\n", eye.x, eye.y, eye.z);
		printf("zNear = %f               zFar = %f\n", zNear, zFar);
		printf("imageWidth = %f   imageHeight = %f\n", imageWidth, imageHeight);
		PrintTransform("trans_Point", trans_Point);
	}

//	orig_mesh = mesh;
}

//
// SetMaterial - set the various OpenGL values for the specified material
//
int SetMaterial(const int p_m)
{
	int m = 0;
	if ((p_m >= 0) && (p_m < (int) materials.size())) m = p_m;

	GLfloat mat_ambient[] = {materials[m].Ka.r, materials[m].Ka.g, materials[m].Ka.b, materials[m].d};
	GLfloat mat_diffuse[] = {materials[m].Kd.r, materials[m].Kd.g, materials[m].Kd.b, materials[m].d};
	GLfloat mat_specular[] = {materials[m].Ks.r, materials[m].Ks.g, materials[m].Ks.b, materials[m].d};

	glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
	glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
	glMaterialf(GL_FRONT, GL_SHININESS, materials[m].Ns);

	if (useTexture && materials[m].hasTexture) {
		glBindTexture(GL_TEXTURE_2D, materials[m].texture);
		glEnable(GL_TEXTURE_2D);
		matHasTexture = true;
	} else {
		matHasTexture = false;
	}

	return p_m;
}

void DrawFace(const int p_f)
{
	STPoint3 p1, p2, p3;
	STVector3 n1, n2, n3, t1, t2, t3;

	int m = mesh->GetFaceMaterial(p_f);
	if (m != currMat) currMat = SetMaterial(m);

	if (smooth) mesh->GetFaceVectors(p_f, p1, p2, p3, n1, n2, n3);
	else		mesh->GetFaceVectors(p_f, p1, p2, p3, n1);

	if (matHasTexture) mesh->GetFaceTexcoords(p_f, t1, t2, t3);

/*	if (debug && (p_f == 0)) {
		printf("DrawFace face %d  original p1 = (%f, %f, %f)\n", p_f, p1.x, p1.y, p1.z);
		printf("                          p2 = (%f, %f, %f)\n", p2.x, p2.y, p2.z);
		printf("                          p3 = (%f, %f, %f)\n", p3.x, p3.y, p3.z);
		printf("                      normal = (%f, %f, %f)\n", n1.x, n1.y, n1.z);
	} */
	p1 = trans_Point * p1;
	p2 = trans_Point * p2;
	p3 = trans_Point * p3;
	n1 = trans_Normal * n1;
	n1.Normalize();
	if (smooth) {
		n2 = trans_Normal * n2;
		n2.Normalize();
		n3 = trans_Normal * n3;
		n3.Normalize();
	}
/*	if (debug && (p_f == 0)) {
		printf("                   xlated p1 = (%f, %f, %f)\n", p1.x, p1.y, p1.z);
		printf("                          p2 = (%f, %f, %f)\n", p2.x, p2.y, p2.z);
		printf("                          p3 = (%f, %f, %f)\n", p3.x, p3.y, p3.z);
		printf("                      normal = (%f, %f, %f)\n", n1.x, n1.y, n1.z);
	} */
//	glBegin(GL_LINE_LOOP);
	glBegin(GL_TRIANGLES);
		glNormal3f(n1.x, n1.y, n1.z);
		if (matHasTexture) glTexCoord2f(t1.x, t1.y);
		glVertex3f(p1.x, p1.y, p1.z);
		if (smooth) glNormal3f(n2.x, n2.y, n2.z);
		if (matHasTexture) glTexCoord2f(t2.x, t2.y);
		glVertex3f(p2.x, p2.y, p2.z);
		if (smooth) glNormal3f(n3.x, n3.y, n3.z);
		if (matHasTexture) glTexCoord2f(t3.x, t3.y);
		glVertex3f(p3.x, p3.y, p3.z);
	glEnd();
}

void DrawScene()
{
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	gluLookAt(eye.x, eye.y, eye.z, imageCenter.x, imageCenter.y, imageCenter.z, 0.f, 1.f, 0.f);

	glColor3f (0.f, 0.f, 0.f);

	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	GLfloat ambient[] = {0.0, 0.0, 0.0, 1.0};
	GLfloat diffuse[] = {0.5, 0.5, 0.5, 1.0};
	GLfloat specular[] = {0.0, 0.0, 0.0, 1.0};
	GLfloat spot[] = {1.0, 1.0, 1.0, 1.0};
	GLfloat spotPosition[] = {-2.0f, -2.0f, 6.0f, 1.0};
	glLightfv(GL_LIGHT0, GL_AMBIENT, ambient);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuse);
	glLightfv(GL_LIGHT0, GL_SPECULAR, specular);

	glLightfv(GL_LIGHT0, GL_POSITION, spotPosition);
	//Ambient light
	GLfloat globalAmbient[] = {0.3, 0.3, 0.3, 0.0};
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, globalAmbient);

	if (debug && (debugOnce)) {
		printf("DisplayCallback: spotPosition  = (%f, %f, %f, %f)\n", spotPosition[0], spotPosition[1], spotPosition[2], spotPosition[3]);
		debugOnce = false;
	}

	STTransform4 saveTrans = trans_Point;
	int numFaces = mesh->GetFaceCount();
	
	currMat = -1;

	if (smooth) glShadeModel(GL_SMOOTH);
	else		glShadeModel(GL_FLAT);

	trans_Point = STTransform4::Identity();

	for (int i = 0; i < numFaces; i++) {
		DrawFace(i);
		if (i == 1) trans_Point = saveTrans;
	}

	trans_Point = saveTrans;

	glPushMatrix();
	glTranslatef(-2.0f, -2.0f, 6.0f);
	glutWireSphere(0.5f, 5, 5);
	glPopMatrix();
}

/*--------------------------------------
Lighting stuff.
This performs the rendering aspect by 
setting up the scene and the lights.
Finishes with the actual computations.
--------------------------------------*/

//used in lighting
void DrawDeferred()
{
	//BUILD G-BUFFER
	dfe->PreDrawScene();
	DrawScene();
	dfe->PostDrawScene();

	//RENDER SCENE USING G-BUFFER
	dfe->DrawDirectionalAndAmbient(gWindowSizeX, gWindowSizeX);
	dfe->PreDrawPointLights(gWindowSizeX, gWindowSizeX, zNear, zFar);
	DrawPointLights();
	dfe->PostDrawPointLights();

	DrawLittleLights();

}

/*------------------------------------------
Sets the point light. Creates a sphere for
visualization of the light in space.
------------------------------------------*/

void DrawPointLights()
{
	glDisable(GL_LIGHTING);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	//view();
	gluLookAt(eye.x, eye.y, eye.z, imageCenter.x, imageCenter.y, imageCenter.z, 0.f, 1.f, 0.f);

	dfe->SetPointLightColor(1.0, 1.0, 1.0);
	glPushMatrix();
	glTranslatef(-2.0f, -2.0f, 6.0f);
	glutSolidSphere(0.5f, 5, 5);
	glPopMatrix();

}

/*---------------------------------------
Draws the actual body of light source.
This is only for visualization purposes.
---------------------------------------*/

void DrawLittleLights()
{
	glDisable(GL_LIGHTING);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(eye.x, eye.y, eye.z, imageCenter.x, imageCenter.y, imageCenter.z, 0.f, 1.f, 0.f);

	glColor3f(1.0, 1.0, 1.0);
	glPushMatrix();
	glTranslatef(-2.0f, -2.0f, 6.0f);
	glutSolidSphere(0.5f, 5, 5);
	glPopMatrix();
}

void DisplayCallback()
{
	glClearColor(1.f, 1.f, 1.f, 0.f);
    glClearDepth(1.f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glEnable(GL_DEPTH_TEST);

	if (useDeferred)	DrawDeferred();
	else				DrawScene();

	glutSwapBuffers();
}

//resizes the window when a change occurs
void ReshapeCallback(int w, int h)
{
	gWindowSizeX = w;
    gWindowSizeY = h;

    glMatrixMode( GL_PROJECTION );
    glLoadIdentity();
	// Set up a perspective projection
    float aspectRatio = (float) gWindowSizeX / (float) gWindowSizeY;
	gluPerspective(fovy, aspectRatio, zNear, zFar);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
	
	glViewport(0, 0, gWindowSizeX, gWindowSizeY);
}

void ChangeEyePosition(const float dx, const float dy, const float dz)
{
	eye.x += dx;
	eye.y += dy;
	eye.z += dz;
    glutPostRedisplay();
}

void ChangeSceneRotation(const float dx, const float dy, const float dz)
{
	STTransform4 rotMat = STTransform4::Rotation(dx, dy, dz);
	trans_Point *= rotMat;
	trans_Normal = trans_Point.Inverse();
	trans_Normal = trans_Normal.Transpose();
    glutPostRedisplay();
}

void SpecialKeyCallback(int key, int x, int y)
{
    switch(key) {
        case GLUT_KEY_LEFT:
			if (rotateEye)	ChangeEyePosition(-delta, 0.0f, 0.0f);
			else			ChangeSceneRotation(-delta, 0.0f, 0.0f);
            break;
        case GLUT_KEY_RIGHT:
			if (rotateEye)	ChangeEyePosition(delta, 0.0f, 0.0f);
			else			ChangeSceneRotation(delta, 0.0f, 0.0f);
            break;
        case GLUT_KEY_DOWN:
			if (rotateEye)	ChangeEyePosition(0.0f, -delta, 0.0f);
			else			ChangeSceneRotation(0.0f, -delta, 0.0f);
            break;
        case GLUT_KEY_UP:
			if (rotateEye)	ChangeEyePosition(0.0f, delta, 0.0f);
			else			ChangeSceneRotation(0.0f, delta, 0.0f);
            break;
        default:
            break;
    }

//    glutPostRedisplay();
}


void KeyCallback(unsigned char key, int x, int y)
{
    switch(key) {

	case 27: //escape key
		delete mesh;
		exit(0);
		break;
	case 'D':
	case 'd': //toggle deferred
		if (useDeferred) {
			useDeferred = false;
			printf("Now using OpenGL Lighting\n");
		} else {
			useDeferred = true;
			printf("Now using Deferred Lighting\n");
		}
		break;
	case 'E':
	case 'e': //toggle eye/object rotation
		if (rotateEye) {
			rotateEye = false;
			printf("Now rotating scene\n");
		} else {
			rotateEye = true;
			printf("Now rotating eye position\n");
		}
		break;
	case 'S':
	case 's': //toggle eye/object rotation
		if (smooth) {
			smooth = false;
			printf("Now using flat shading\n");
		} else {
			smooth = true;
			printf("Now using smooth shading\n");
		}
		break;
	case 'F':
	case 'f': //zoom in
		if (rotateEye)	ChangeEyePosition(0.0f, 0.0f, delta);
		else			ChangeSceneRotation(0.0f, 0.0f, delta);
		break;
	case 'V':
	case 'v': //zoom out
		if (rotateEye)	ChangeEyePosition(0.0f, 0.0f, -delta);
		else			ChangeSceneRotation(0.0f, 0.0f, -delta);
		break;
	default: break;
	}
}

int main(int argc, char** argv) {

    //
    // Initialize GLUT.
    //
    glutInit(&argc, argv);
//    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
    glutInitDisplayMode( GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH );
    glutInitWindowPosition(20, 20);
	glutInitWindowSize(gWindowSizeX, gWindowSizeY);
    glutCreateWindow("CS248 Assignment 1");

    glutDisplayFunc(DisplayCallback);
    glutReshapeFunc(ReshapeCallback);
    glutKeyboardFunc(KeyCallback); 
	glutSpecialFunc(SpecialKeyCallback);

	std::string OBJfile = "../../obj/testbox.obj";	// Will allow for testbox to load on default
	if(argc > 1) OBJfile = argv[1];					// If there is something else specified by command

	LoadMaterials();
	ParseObjFile(OBJfile);

	SetUp();

	// Error check
	GLenum err = glewInit();
	if (err != GLEW_OK || !glewIsSupported("GL_VERSION_2_0")) {
		printf("OpenGL 2.0 not supported. No shaders!\n");
		printf("%s\n", glewGetErrorString(err));
		printf("%s\n", (char*)glGetString( GL_VERSION ) );
		exit(-1);
	}
	printf("OpenGL 2.0 supported. Using OpenGL %s \n\n",(char*)glGetString( GL_VERSION ));
	printf("Status: Using GLEW %s\n", glewGetString(GLEW_VERSION));

	/*---------------------
	Set up light shaders.
	---------------------*/

	// engine
	dfe = new DeferredLighting(gWindowSizeX, gWindowSizeY);
	dfe->Init(shaderPath);

    glutMainLoop();

    return 0;
}
