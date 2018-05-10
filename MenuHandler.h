#pragma once
#include "mat.h"
#include "mat.h"
#include "vec.h"
#include "Angel.h"

class MenuHandler {


public: 
	
	int rotation_hearthbeat;
	mat4 projection;
	
	float drawingMode;
	GLfloat w, h;
	GLfloat scl;
	mat4 modelView;
	int orth_pers;
	float shading;
	mat4 defaultModelView;

	mat4  projectionpers;
	mat4 projectionortho;



	//functions
	void setWH(GLfloat, GLfloat);
	MenuHandler(void);
	~MenuHandler(void);
	void setProjection(mat4);
	//void setOffObj(OffModel*);
	void setProgram(GLuint);
	void setRotHB(int);
	void setGlobalMenuHandler(MenuHandler*);

};