Lighting
========

For lighting:

include:

	textfile.cpp
	textfile.h

	DeferredLighting.cpp
	DeferredLighting.h

	Shader.cpp
	Shader.h

------------------------------------------------------------------------------------------------

Shaders needed:
	
	dLight.frag
	pLight.frag
	pLight.vert
	deferred_direct_target.frag
	deferred_gather.frag
	deferred_gather.vert

------------------------------------------------------------------------------------------------
------------------------------------------------------------------------------------------------
************************************************************************************************
************************************************************************************************
------------------------------------------------------------------------------------------------
------------------------------------------------------------------------------------------------

To inlcude this shader in Graphics.cpp under Paul's program structure:
this is the code:

@ top:

	DeferredLighting* dfe;
	std::string shaderPath = "../../shaders"; //this is the directory the shaders live in

	float zNear = .1f;
	float zFar = 100.f;

------------------------------------------------------------------------------------------------
************************************************************************************************
------------------------------------------------------------------------------------------------

in void GraphicsInit:
------------------------------------------------------------------------------------------------

	void GraphicsInit(int argc, char** argv) {

	..........................................
	..........................................
	..........................................	

	glClearColor(1,1,1,1);

	glClearDepth(1.f); //added

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);
	glShadeModel(GL_SMOOTH);

	// Initialize camera
	cam.pos = STVector3(0.f, 0.f, 4.f);
	cam.up = STVector3(0.f, 1.f, 0.f);
	cam.pan = STVector3(0.f, 0.f, 0.f);

	// Enable global lights
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	float light0Position[4] = {cam.pos.x, cam.pos.y, cam.pos.z, 1.f};
	glLightfv(GL_LIGHT0, GL_POSITION, light0Position);

	// Enable texture
	glEnable(GL_TEXTURE_2D);

	dfe = new DeferredLighting(windowWidth, windowHeight); //added
	dfe->Init(shaderPath); //added
} 

-------------------------------------------------------------------------------------------------
*************************************************************************************************
-------------------------------------------------------------------------------------------------

in display function:

--------------------------------------------------------------------------------------

	void display() {
		glClearColor(1,1,1,1);
		glClearDepth(1.f); //added
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glEnable(GL_DEPTH_TEST); //added (I'm not sure if this is actually needed here but that is what I have)

		dfe->PreDrawScene();//added
		
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
		gluLookAt(cam.pos.x+cam.pan.x, cam.pos.y+cam.pan.y, cam.pos.z+cam.pan.z, cam.pan.x, cam.pan.y, cam.pan.z, cam.up.x, cam.up.y, cam.up.z);

		gameLogic();

		// Draw
		for(unsigned i = 0; i<renderList.size(); i++)
		renderList[i]->render();

		renderList.clear();

		dfe->PostDrawScene();//added
		
		//rendering part
		dfe->DrawDirectionalAndAmbient(windowWidth, windowHeight); //added
		dfe->PreDrawPOintLights(windowWidth, windowHeight, zNear, zFar);
		DrawPointLights(); //added (see below)
		dfe->PostDrawPointLights(); //added

		DrawLittleLights();//added (see below) (this will go away once the fire is in, it is only to see the lights in real space)

		glutSwapBuffers();
	} 

-----------------------------------------------------------------------------------------------------------
***********************************************************************************************************
-----------------------------------------------------------------------------------------------------------

//code below here is referenced above but only draws a static point light in space
//it is not going to stay this way but it will help as a place holder

//***********gluLookAt may have to be changed*************


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