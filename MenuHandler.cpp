

#include "MenuHandler.h"
#include "GL\freeglut.h"
#include "mat.h"
#include "vec.h"
#include "Angel.h"
GLuint program;
MenuHandler *menu;

using namespace std;
typedef Angel::vec4  point4;
void changeLightDirection(float dir);
void selectLights(int opt);
void MenuHandler::setGlobalMenuHandler(MenuHandler* m) {
	menu = m;
}

void MenuHandler::setProgram(GLuint p) {
	program = p;
}



void MenuHandler::setProjection(mat4 pr) {
	projection = pr;
}

void MenuHandler::setWH(GLfloat x, GLfloat y) {
	menu->w = x;
	menu->h = y;
}

void mainMenu(int opt) {

}



void dm_gouraudshading() {
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	menu->shading = 1.0;
	GLuint shading = glGetUniformLocation(program, "Shading");
	glUniform1f(shading , menu->shading);
	GLuint phgo = glGetUniformLocation(program, "ph_go");
	glUniform1f(phgo, 1.0);
	glutPostRedisplay();
}




void dm_phongshading() {
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	menu->shading = 1.0;
	GLuint shading = glGetUniformLocation(program, "Shading");
	glUniform1f(shading, menu->shading);
	GLuint phgo = glGetUniformLocation(program, "ph_go");
	glUniform1f(phgo,0.0);

	glutPostRedisplay();
}



void dm_wireframes() {
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	menu->shading = 0.0;
	GLuint shading = glGetUniformLocation(program, "Shading");
	glUniform1f(shading, menu->shading);
	glutPostRedisplay();
}

void drawCube() {
	//off_obj->load("cube.off");
	//off_obj->control = 0;
	//off_obj->reinit = 1;
	glutPostRedisplay();
}

void drawSphere() {
	//off_obj->load("shapeX.offx");
	//menu->shading = 1.0;
	//GLuint shading = glGetUniformLocation(program, "shading");
	//glUniform1f(shading , menu->shading);
	//std::cout << "SHADER : " << menu->shading << std::endl;
	//off_obj->control = 1;
	//off_obj->reinit = 1;
	
	glutPostRedisplay();

}


void drawCar() {
	//off_obj->load("car.off"); 
	//off_obj->control = 1;
	//off_obj->reinit = 1;
	glutPostRedisplay();
}

void drawTriangle() {
	//off_obj->load("triangle.off");
	//off_obj->control = 1;
	//off_obj->reinit = 1;
	glutPostRedisplay();
}


void drawCone() {
	//off_obj->load("cone.off");
	//off_obj->control = 1;
	//off_obj->reinit = 1;
	glutPostRedisplay();
}


void subMenu_obj(int opt) {

	switch (opt) {

	case 0:
		drawSphere();
		break;
	case 1:
		drawSphere();
		break;
	case 2:
		drawCar();
		break;
	case 3:
		drawTriangle();
		break;
	case 4:
		drawCone(); 
		break;
	}

}

void subMenu_drawing(int opt) {

	switch (opt) {
	case 0:
		dm_wireframes();
		break;
	case 1:
		dm_gouraudshading();
		break;
	case 2:
		dm_phongshading();
		break;
	case 3:
		menu->shading = 2.0;
		GLuint shading = glGetUniformLocation(program, "Shading");
		glUniform1f(shading, 2.0);
		GLuint phgo = glGetUniformLocation(program, "ph_go");
		glUniform1f(phgo, 1.0);
		glutPostRedisplay();
	}

}
void subMenu_color(int opt) {
	GLuint mcontrol;GLuint myColor;
	switch (opt) {
	case 0:
		
		//myColor = glGetUniformLocation(program, "myColor");
		//mcontrol = glGetUniformLocation(program, "control");
		//glUniform1f(mcontrol, 1.0);
		//glUniform4f(myColor, 1.0, 0.0, 0.0, 0.0);
		//glutPostRedisplay();
		break;
	case 1:
		
		//myColor = glGetUniformLocation(program, "myColor");
		
		//mcontrol = glGetUniformLocation(program, "control");
		//glUniform1f(mcontrol, 1.0);
		//glUniform4f(myColor, 1.0,1.0, 0.0, 1.0);
		//glutPostRedisplay();
		break;
	case 2:
		
		//myColor = glGetUniformLocation(program, "myColor");
		
		//mcontrol = glGetUniformLocation(program, "control");
		//glUniform1f(mcontrol, 1.0);
		//glUniform4f(myColor,0.0,1.0, 0.0, 1.0);
		glutPostRedisplay();
		break;
	case 3:/*
		color4(0.0, 0.0, 0.0, 1.0),  // black
			color4(1.0, 0.0, 0.0, 1.0),  // red
			color4(1.0, 1.0, 0.0, 1.0),  // yellow
			color4(0.0, 1.0, 0.0, 1.0),  // green
			color4(0.0, 0.0, 1.0, 1.0),  // blue
			color4(1.0, 0.0, 1.0, 1.0),  // magenta
			color4(1.0, 1.0, 1.0, 1.0),  // white
			color4(0.0, 1.0, 1.0, 1.0)   // cyan*/
		
		//myColor = glGetUniformLocation(program, "myColor");
		
		//mcontrol = glGetUniformLocation(program, "control");
		//glUniform1f(mcontrol, 1.0);
		//glUniform4f(myColor, 0.0, 0.0, 1.0, 1.0);
		//glutPostRedisplay();
		break;
	case 4:
		
		//myColor = glGetUniformLocation(program, "myColor");
		
		//mcontrol = glGetUniformLocation(program, "control");
		//glUniform1f(mcontrol, 1.0);
		//glUniform4f(myColor, 1.0, 0.0, 1.0, 1.0);
		//glutPostRedisplay();
		break;
	case 5:
		
		//myColor = glGetUniformLocation(program, "myColor");
		
		//mcontrol = glGetUniformLocation(program, "control");
		//glUniform1f(mcontrol, 1.0);
		//glUniform4f(myColor, 0.0, 0.0, 0.0, 1.0);
		glutPostRedisplay();
		break;
	}

}

void MenuHandler::setRotHB(int i) {
	rotation_hearthbeat = i;
}

void subMenu_motion(int opt) {
	switch (opt) {
	case 0:
		menu->rotation_hearthbeat = 0;
		break;
	case 1:
		menu->rotation_hearthbeat = 1;
		break;
	}
}


void subMenu_plasticmetal(int opt) {
	GLuint metal_plastic;
	switch (opt) {
	case 0:
		metal_plastic = glGetUniformLocation(program, "metal_plastic");
		glUniform1f(metal_plastic, 0.01);
		glutPostRedisplay();
		break;
	case 1:
		metal_plastic = glGetUniformLocation(program, "metal_plastic");
		glUniform1f(metal_plastic, 5);
		glutPostRedisplay();
		break;
	}
}




void subMenu_bgcolor(int opt) {
	switch (opt) {
	case 0:
		glClearColor(1.0, 1.0, 1.0, 1.0);
		glutPostRedisplay();
		break;
	case 1:
		glClearColor(0, 0, 0, 1);
		glutPostRedisplay();
		break;
	case 2:
		glClearColor(0.5, 0.5, 0.5, 1.0);
		glutPostRedisplay();
		break;
	}
}

void subMenu_proj(int opt) {
	switch (opt) {
		int ModelView;
		int Projection;
	case 0:
		Projection = glGetUniformLocation(program, "Projection");
		//ModelView = glGetUniformLocation(program, "ModelView");
		//glUniformMatrix4fv(ModelView, 1, GL_TRUE, Translate(vec3(0.0, 0.0, -3.0))*Scale(0.2, 0.2, 0.2));
		menu->projectionortho = Ortho(-1.0 , 1.0 , -1.0, 1.0, -1.0, 1.0)*Scale(0.8, 0.8, 0.8)*Translate(vec3(0.0, 0.0, 3.0));
		glUniformMatrix4fv(Projection, 1, GL_TRUE, menu->projectionortho);
		glutPostRedisplay();
		break;
	case 1:
		//ModelView = glGetUniformLocation(program, "ModelView");
		//glUniformMatrix4fv(ModelView, 1, GL_TRUE, menu->defaultModelView);
		Projection = glGetUniformLocation(program, "Projection");
		glUniformMatrix4fv(Projection, 1, GL_TRUE, menu->projectionpers);
		glutPostRedisplay();
		break;
	
	}
}


void subMenu_lights(int opt) {
	switch (opt) {
	case 0:
		selectLights(0);
		glutPostRedisplay();
		break;
	case 1:
		selectLights(1);
		glutPostRedisplay();
		break;
	
	case 2:
		selectLights(2);
		glutPostRedisplay();
		break;
}
}

void selectLights(int opt) {
	if (opt == 0) {
		GLuint lights = glGetUniformLocation(program, "lights");
		glUniform1f(lights, 0.0);
	}else if (opt == 1) {
		//we have already one
		point4 light_position(-2.0, 0.0, 0.0, 1.0);
		glUniform4fv(glGetUniformLocation(program, "LightPosition"),
			1, light_position);
		GLuint lights = glGetUniformLocation(program, "lights");
		glUniform1f(lights, 1.0);
	}
	else {
		//add one more
		point4 light_position(-2.0, 0.0, 0.0, 1.0);
		glUniform4fv(glGetUniformLocation(program, "LightPosition"),
			1, light_position);
		GLuint lights = glGetUniformLocation(program, "lights");
		glUniform1f(lights, 1.0);


		point4 light2_position(2.0, 0.0, 0.0, 1.0);
		glUniform4fv(glGetUniformLocation(program, "Light2Position"),
			1, light2_position);
		
		 lights = glGetUniformLocation(program, "lights");
		glUniform1f(lights, 2.0);
	}
}





void subMenu_lightDir(int opt) {
	switch (opt) {
	case 0:
		changeLightDirection(0.0);
		glutPostRedisplay();
		break;
	case 1:
		changeLightDirection(1.0);
		glutPostRedisplay();
		break;
	case 2:
		changeLightDirection(5.0);
		glutPostRedisplay();
		break;
	}
}

void changeLightDirection(float dir) {
	if (dir == 0.0) {
		point4 light_position(-2.0, 0.0, 0.0, 1.0);
		glUniform4fv(glGetUniformLocation(program, "LightPosition"),
			1, light_position);
		glutPostRedisplay();
		
	}
	else if (dir == 1.0) {
		point4 light_position(0.0, 0.0, 1.0, 1.0);
		glUniform4fv(glGetUniformLocation(program, "LightPosition"),
			1, light_position);
		glutPostRedisplay();
	}
	else {
		point4 light_position(2.0, 0.0, 1.0, 1.0);
		glUniform4fv(glGetUniformLocation(program, "LightPosition"),
			1, light_position);
		glutPostRedisplay();
	}
}




MenuHandler::MenuHandler() {

	//menu->drawingMode = 0.0;

	rotation_hearthbeat = 0;
	scl = 0.4;
	orth_pers = 0;

	int proj_type = glutCreateMenu(subMenu_proj);
	glutAddMenuEntry("Ortho", 0);
	glutAddMenuEntry("Perspective", 1);


	int bgcolor = glutCreateMenu(subMenu_bgcolor);
	glutAddMenuEntry("white", 0);
	glutAddMenuEntry("black", 1);
	glutAddMenuEntry("gray", 2);


	int obj_type = glutCreateMenu(subMenu_obj);
	glutAddMenuEntry("Cube", 0);
	glutAddMenuEntry("Sphere", 1);
	glutAddMenuEntry("Car", 2);
	glutAddMenuEntry("Triangle", 3);
	glutAddMenuEntry("Cone", 4);

	int motion_menu = glutCreateMenu(subMenu_motion);
	glutAddMenuEntry("Rotate", 0);
	glutAddMenuEntry("Hearth Beat", 1);


	int drawing_mode = glutCreateMenu(subMenu_drawing);
	glutAddMenuEntry("wireframe", 0);
	glutAddMenuEntry("Gouraud Shading", 1);
	glutAddMenuEntry("Phong Shading", 2);
	glutAddMenuEntry("Texture", 3);

	int plastic_metal = glutCreateMenu(subMenu_plasticmetal);
	glutAddMenuEntry("plastic", 0);
	glutAddMenuEntry("metal", 1);
 
	int color = glutCreateMenu(subMenu_color);
	glutAddMenuEntry("red", 0);
	glutAddMenuEntry("yellow", 1);
	glutAddMenuEntry("green", 2);
	glutAddMenuEntry("blue", 3);
	glutAddMenuEntry("magenta", 4);
	glutAddMenuEntry("black", 5);

	int lightdir = glutCreateMenu(subMenu_lightDir);
	glutAddMenuEntry("left", 0);
	glutAddMenuEntry("center", 1);
	glutAddMenuEntry("right", 2);

	int lights = glutCreateMenu(subMenu_lights);
	glutAddMenuEntry("zero", 0);
	glutAddMenuEntry("one", 1);
	glutAddMenuEntry("two", 2);

	glutCreateMenu(mainMenu);
	glutAddSubMenu("BACKGROUND", bgcolor);
	glutAddSubMenu("PROJECTION", proj_type);
	//glutAddSubMenu("OBJECT TYPE", obj_type);
	//glutAddSubMenu("MOTION TYPE", motion_menu);
	glutAddSubMenu("DRAWING MODE", drawing_mode);
	glutAddSubMenu("MATERIAL", plastic_metal);
	glutAddSubMenu("LIGHT DIRECTION", lightdir);
	glutAddSubMenu("NUMBER OF LIGHTS", lights);
	//glutAddSubMenu("COLOR", color);

	glutAttachMenu(GLUT_RIGHT_BUTTON);



}

MenuHandler::~MenuHandler() {

}



