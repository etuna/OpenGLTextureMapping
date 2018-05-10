//
//  Display a rotating cube with lighting - Gouraud shading
//
//  Light and material properties are sent to the shader as uniform
//    variables.  Vertex positions and normals are sent as vertex attributes
//

#include "Angel.h"
#include "mat.h"
#include <iostream>
#include <string>
#include <fstream>
#include "vec.h"
#include "MenuHandler.h"



#define v1 0
#define v2 1
#define v3 2
#define v4 3


using namespace std;

typedef Angel::vec4  color4;
typedef Angel::vec4  point4;
const int  TextureSize = 2048;
fstream file;
point4 *mvertices;
vec3  * mnormals;
vec3 *triangles;
GLuint texture[1];
GLubyte image[TextureSize][TextureSize][3];
GLuint *mprogram;
point4 *pts;
vec3 *nors;
int numvertices, numtriangles;
MenuHandler* menuHandler;
float scale = 1;
GLfloat w, h;

const int NumVertices = 36; //(6 faces)(2 triangles/face)(3 vertices/triangle)

point4 points[NumVertices];
vec3   normals[NumVertices];
vec3* loadOff(std::string filename);
color4 *tex_colors;
vec2 tex_coords[85617*3];
vec2 vt_cords[45149];

void populatePoints();
void setProjections();
vec3* readPPM();
// Vertices of a unit cube centered at origin, sides aligned with axes
point4 vertices[8] = {
    point4( -0.5, -0.5,  0.5, 1.0 ),
    point4( -0.5,  0.5,  0.5, 1.0 ),
    point4(  0.5,  0.5,  0.5, 1.0 ),
    point4(  0.5, -0.5,  0.5, 1.0 ),
    point4( -0.5, -0.5, -0.5, 1.0 ),
    point4( -0.5,  0.5, -0.5, 1.0 ),
    point4(  0.5,  0.5, -0.5, 1.0 ),
    point4(  0.5, -0.5, -0.5, 1.0 )
};

// Array of rotation angles (in degrees) for each coordinate axis
enum { Xaxis = 0, Yaxis = 1, Zaxis = 2, NumAxes = 3 };
int      Axis = Xaxis;
GLfloat  Theta[NumAxes] = { 0.0, 0.0, 0.0 };

// Model-view and projection matrices uniform location
GLuint  ModelView, Projection;

//----------------------------------------------------------------------------

// quad generates two triangles for each face and assigns colors
//    to the vertices

int Index = 0;

void
quad( int a, int b, int c, int d )
{
    // Initialize temporary vectors along the quad's edge to
    //   compute its face normal 
    vec4 u = vertices[b] - vertices[a];
    vec4 v = vertices[c] - vertices[b];

    vec3 normal = normalize( cross(u, v) );

    normals[Index] = normal; points[Index] = vertices[a]; Index++;
    normals[Index] = normal; points[Index] = vertices[b]; Index++;
    normals[Index] = normal; points[Index] = vertices[c]; Index++;
    normals[Index] = normal; points[Index] = vertices[a]; Index++;
    normals[Index] = normal; points[Index] = vertices[c]; Index++;
    normals[Index] = normal; points[Index] = vertices[d]; Index++;
    
    //Note that normals are fixed for a given face of the cube.
    //So the normal of a vertex is NOT computed based on neighboring faces.
    //which makes sense in this example since this is a cube with only 6 faces.
}

//----------------------------------------------------------------------------

// generate 12 triangles: 36 vertices and 36 colors
void
colorcube()
{
    quad( 1, 0, 3, 2 );
    quad( 2, 3, 7, 6 );
    quad( 3, 0, 4, 7 );
    quad( 6, 5, 1, 2 );
    quad( 4, 5, 6, 7 );
    quad( 5, 4, 0, 1 );
}

//----------------------------------------------------------------------------

// OpenGL initialization
void
minit()
{
    colorcube();

    // Create a vertex array object
    GLuint vao;
    glGenVertexArrays( 1, &vao );
    glBindVertexArray( vao );

    // Create and initialize a buffer object
    GLuint buffer;
    glGenBuffers( 1, &buffer );
    glBindBuffer( GL_ARRAY_BUFFER, buffer );
    glBufferData( GL_ARRAY_BUFFER, sizeof(points) + sizeof(normals),
		  NULL, GL_STATIC_DRAW );
    glBufferSubData( GL_ARRAY_BUFFER, 0, sizeof(points), points );
    glBufferSubData( GL_ARRAY_BUFFER, sizeof(points),
		     sizeof(normals), normals );

    // Load shaders and use the resulting shader program
    GLuint program = InitShader( "vshader.glsl", "fshader.glsl" );
	
	*mprogram = program;
    glUseProgram( program );

    // set up vertex arrays
    GLuint vPosition = glGetAttribLocation( program, "vPosition" );
    glEnableVertexAttribArray( vPosition );
    glVertexAttribPointer( vPosition, 4, GL_FLOAT, GL_FALSE, 0,
			   BUFFER_OFFSET(0) );

    GLuint vNormal = glGetAttribLocation( program, "vNormal" ); 
    glEnableVertexAttribArray( vNormal );
    glVertexAttribPointer( vNormal, 3, GL_FLOAT, GL_FALSE, 0,
			   BUFFER_OFFSET(sizeof(points)) );

    // Initialize shader lighting parameters
    point4 light_position( -2.0, 0.0, 0.0, 1.0 );
	
    color4 light_ambient( 0.2, 0.2, 0.2, 1.0 ); // L_a
    color4 light_diffuse( 1.0, 1.0, 1.0, 1.0 ); // L_d
    color4 light_specular( 1.0, 1.0, 1.0, 1.0 ); // L_s

    color4 material_ambient( 1.0, 0.0, 1.0, 1.0 ); // k_a
    color4 material_diffuse( 1.0, 0.8, 0.0, 1.0 ); // k_d
    color4 material_specular( 1.0, 0.8, 0.0, 1.0 ); // k_s
    float  material_shininess = 100.0;

    color4 ambient_product = light_ambient * material_ambient; // k_a * L_a
    color4 diffuse_product = light_diffuse * material_diffuse; // k_d * L_d
    color4 specular_product = light_specular * material_specular; // k_s * L_s
    glUniform4fv( glGetUniformLocation(program, "AmbientProduct"),
		  1, ambient_product );
    glUniform4fv( glGetUniformLocation(program, "DiffuseProduct"),
		  1, diffuse_product );
    glUniform4fv( glGetUniformLocation(program, "SpecularProduct"),
		  1, specular_product );
	
    glUniform4fv( glGetUniformLocation(program, "LightPosition"),
		  1, light_position );

    glUniform1f( glGetUniformLocation(program, "Shininess"),
		 material_shininess );
		 
    // Retrieve transformation uniform variable locations
    ModelView = glGetUniformLocation( program, "ModelView" );
    Projection = glGetUniformLocation( program, "Projection" );

    glEnable( GL_DEPTH_TEST );

    glClearColor( 1.0, 1.0, 1.0, 1.0 ); 
}

//----------------------------------------------------------------------------










void
init()
{
	loadOff("shapeX.offx");
	populatePoints();
	
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	// Create a vertex array object
	GLuint vao;
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	readPPM();

	
	
	// Create and initialize a buffer object
	GLuint buffer;
	glGenBuffers(1, &buffer);
	glBindBuffer(GL_ARRAY_BUFFER, buffer);
	glBufferData(GL_ARRAY_BUFFER, numtriangles*3*16 + numtriangles*3*12 + sizeof(tex_coords),
		NULL, GL_STATIC_DRAW);
	cout << numtriangles*3*16 << numvertices << numtriangles << endl;
	

	glBufferSubData(GL_ARRAY_BUFFER, 0, numtriangles * 3 * 16, pts); 
	glBufferSubData(GL_ARRAY_BUFFER, numtriangles*3*16,
		numtriangles*3*12, nors);

	glBufferSubData(GL_ARRAY_BUFFER, numtriangles * 3 * 16+
		numtriangles * 3 * 12, sizeof(tex_coords),tex_coords);


	//glBufferSubData(GL_ARRAY_BUFFER, numtriangles * 3 * 16 +
	//	numtriangles * 3 * 12 +2048 * 2048 * 16, numtriangles*3*8,tex_coords);
	
	// Load shaders and use the resulting shader program
	GLuint program = InitShader("vshader.glsl", "fshader.glsl");
	glUseProgram(program);

	//getchar();


	/*



	int k, n, m;
	char c, b[100];
	FILE *fd = fopen("texture.ppm", "r");
	fscanf(fd, "%[^\n] ", b);
	fscanf(fd, "%c", &c);
	while (c == '#') {
		fscanf(fd, "%[^\n] ", b);
		fscanf(fd, "%c", &c);
	}
	ungetc(c, fd);
	fscanf(fd, "%d %d %d", &n, &m, &k);
	GLubyte *imagem = (GLubyte *)malloc(3 * sizeof(GLubyte) * n * m);
	int red, green, blue;
	for (unsigned int i = n * m; i > 0; i--) {
		fscanf(fd, "%d %d %d", &red, &green, &blue);
		imagem[3 * n * m - 3 * i] = red;
		imagem[3 * n * m - 3 * i + 1] = green;
		imagem[3 * n * m - 3 * i + 2] = blue;
	}
	GLuint textures[1];
	glGenTextures(1, textures);
	glBindTexture(GL_TEXTURE_2D, textures[0]);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, n, m, 0, GL_RGB, GL_UNSIGNED_BYTE, imagem);



	*/









	glGenTextures(1, texture);

	glBindTexture(GL_TEXTURE_2D, texture[0]);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, TextureSize, TextureSize, 0,
		GL_RGB, GL_UNSIGNED_BYTE, image);
	glGenerateMipmap(GL_TEXTURE_2D);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST_MIPMAP_LINEAR); //try here different alternatives
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR); //try here different alternatives

	

//getchar();




	

	menuHandler = new MenuHandler();
	menuHandler->setGlobalMenuHandler(menuHandler);
	menuHandler->setProgram(program);
	setProjections();
	// set up vertex arrays
	GLuint vPosition = glGetAttribLocation(program, "vPosition");
	glEnableVertexAttribArray(vPosition);
	glVertexAttribPointer(vPosition, 4, GL_FLOAT, GL_FALSE, 0,
		BUFFER_OFFSET(0));

	GLuint vNormal = glGetAttribLocation(program, "vNormal");
	glEnableVertexAttribArray(vNormal);
	glVertexAttribPointer(vNormal, 3, GL_FLOAT, GL_FALSE, 0,
		BUFFER_OFFSET(numtriangles*16*3));



	//GLuint vColor = glGetAttribLocation(program, "vColor");
	//glEnableVertexAttribArray(vColor);
	//glVertexAttribPointer(vColor, 4, GL_FLOAT, GL_FALSE, 0,
	//	BUFFER_OFFSET(numtriangles*16*3+numtriangles*3*12));

	GLuint vTexCoord = glGetAttribLocation(program, "vTexCoord");
	glEnableVertexAttribArray(vTexCoord);
	glVertexAttribPointer(vTexCoord, 2, GL_FLOAT, GL_FALSE, 0,
		BUFFER_OFFSET(numtriangles*16*3+numtriangles*3*12));




	// Initialize shader lighting parameters
	point4 light_position(-2.0, 0.0, 0.0, 1.0);
	point4 light2_position(0.0, 0.0, 2.0, 1.0);
	color4 light_ambient(0.2, 0.2, 0.2, 1.0); // L_a
	color4 light_diffuse(1.0, 1.0, 1.0, 1.0); // L_d
	color4 light_specular(1.0, 1.0, 1.0, 1.0); // L_s

	color4 material_ambient(1.0, 0.0, 1.0, 1.0); // k_a
	color4 material_diffuse(1.0, 0.8, 0.0, 1.0); // k_d
	color4 material_specular(1.0, 0.8, 0.0, 1.0); // k_s
	
	float  material_shininess = 100.0;

	color4 ambient_product = light_ambient * material_ambient; // k_a * L_a
	color4 diffuse_product = light_diffuse * material_diffuse; // k_d * L_d
	color4 specular_product = light_specular * material_specular; // k_s * L_s
	glUniform4fv(glGetUniformLocation(program, "AmbientProduct"),
		1, ambient_product);
	glUniform4fv(glGetUniformLocation(program, "DiffuseProduct"),
		1, diffuse_product);
	glUniform4fv(glGetUniformLocation(program, "SpecularProduct"),
		1, specular_product);

	glUniform4fv(glGetUniformLocation(program, "LightPosition"),
		1, light_position);

	glUniform1f(glGetUniformLocation(program, "Shininess"),
		material_shininess);

	// Retrieve transformation uniform variable locations
	ModelView = glGetUniformLocation(program, "ModelView");
	Projection = glGetUniformLocation(program, "Projection");

	glEnable(GL_DEPTH_TEST);

	glClearColor(0.0, 0.0, 0.0, 1.0);
}

//----------------------------------------------------------------------------










void
display( void )
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    //  Generate tha model-view matrixn

    const vec3 viewer_pos( 0.0, 0.0, 3.0 );
    mat4  model_view = (Translate( -viewer_pos ) *
			 Scale( scale )*
			 Scale( scale )*
			 Scale( scale )*
		RotateX(Theta[Xaxis])*
		RotateY(Theta[Yaxis])*
		RotateZ(Theta[Zaxis]));

	menuHandler->defaultModelView = model_view;
    
    glUniformMatrix4fv( ModelView, 1, GL_TRUE, model_view );

    glDrawArrays( GL_TRIANGLES, 0, numtriangles*3 );
    glutSwapBuffers();
}

//----------------------------------------------------------------------------

void
mouse( int button, int state, int x, int y )
{
    if ( state == GLUT_DOWN ) {
	switch( button ) {
	    case GLUT_LEFT_BUTTON:    Axis = Xaxis;  break;
	    case GLUT_MIDDLE_BUTTON:  Axis = Yaxis;  break;
	    //case GLUT_RIGHT_BUTTON:   Axis = Zaxis;  break;
	}
    }
}

//----------------------------------------------------------------------------

void
idle( void )
{
    Theta[Axis] += 0.01;

    if ( Theta[Axis] > 360.0 ) {
	Theta[Axis] -= 360.0;
    }
    
    glutPostRedisplay();
}

//----------------------------------------------------------------------------

void
keyboard( unsigned char key, int x, int y )
{
	GLuint mshading;
    switch( key ) {
	case 033: // Escape Key
	case 'q': case 'Q':
	    exit( EXIT_SUCCESS );
	    break;
	case 'z':
		scale += 0.1;
		glutPostRedisplay();
		break;
	case 'i':
		//menuHandler->shading = 0.0;
		//mshading = glGetUniformLocation(*mprogram, "Shading");
		//glUniform1f(mshading,0.0);
		glutPostRedisplay();
		break;
	case 'Z':
		scale -= 0.1;
		glutPostRedisplay();
		break;
	case 'h':
		std::cout << "Welcome" << endl;
		std::cout << "------HELP------" << endl;
		std::cout << "i : You can restart by typing i" << endl;
		std::cout << "h : You can type h to access help" << endl;
		std::cout << "q : You can exit by typing q" << endl;
		std::cout << "You can rotate by arrow keys" << endl;
		std::cout << "You can zoom in and out by z and Z" << endl;
		std::cout << "You can use menu by clicking right button of the mouse" << endl;
		std::cout << "You can also select corresponding menu items and display it differently" << endl;
		break;

    }
}



void keyboardplusplus(int key, int x, int y) {
	switch (key) {
	case GLUT_KEY_UP:

		Theta[Xaxis] -= 10;
		glutPostRedisplay();
		//speed++;
		//rot_speed += 0.1;
		//scale_rh += 0.001;
		break;
	case GLUT_KEY_DOWN:
		Theta[Xaxis] += 10;

		glutPostRedisplay();

		//if (speed != 0) {
		//rot_speed -= 0.1;
		//speed--;
		//}
		break;
	case GLUT_KEY_RIGHT:

		Theta[Yaxis] += 2;

		glutPostRedisplay();

		//if (speed != 0) {
		//rot_speed -= 0.1;
		//speed--;
		//}
		break;
	case GLUT_KEY_LEFT:
		Theta[Yaxis] -= 2;


		glutPostRedisplay();
		//if (speed != 0) {
		//rot_speed -= 0.1;
		//speed--;
		//}
		break;
	}
}


//----------------------------------------------------------------------------

void
reshape( int width, int height )
{
    glViewport( 0, 0, width, height );
	w = width;
	h = height;
	setProjections();
    GLfloat aspect = GLfloat(width)/height;
    mat4  projection = Perspective( 45.0, aspect, 0.5, 6.0 );

    glUniformMatrix4fv( Projection, 1, GL_TRUE, projection );
}

//----------------------------------------------------------------------------

int
main( int argc, char **argv )
{
    glutInit( &argc, argv );
    glutInitDisplayMode( GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH );
    glutInitWindowSize( 800, 800 );
    glutCreateWindow( "etuna - HW3" );

	glewInit();
	glewExperimental = GL_TRUE;

    init();

    glutDisplayFunc( display );
    glutKeyboardFunc( keyboard );
    glutReshapeFunc( reshape );
    glutMouseFunc( mouse );
    //glutIdleFunc( idle );

	glutSpecialFunc(keyboardplusplus);

    glutMainLoop();
    return 0;
}


void setupLoad(std::string filename) {

}

vec3* loadOff(std::string filename) {

	int i, num_vertices, num_triangles;
	string offx_word;
	


	file = fstream(filename);
	if (!file.is_open()) {
		return false;
	}

	file >> offx_word;
	cout << offx_word << endl;
	file >> num_vertices >> num_triangles >> i;


	
	
	const int mnumVertices = num_triangles * 3;
	numvertices = num_vertices;
	numtriangles = num_triangles;


	mvertices = new point4[numvertices];
	mnormals = new vec3[numvertices];
	triangles = new vec3[numtriangles];

	
	for (i = 0; i < num_vertices; i++) {
		GLfloat temp1, temp2, temp3;
		file >> mvertices[i][v1] >> mvertices[i][v2] >>mvertices[i][v3];
		//cout << mpoints[i][v1] << mpoints[i][v2] << mpoints[i][v3] << endl;
	}

	for (i = 0; i < num_triangles; i++)
	{
		GLfloat tempa, temp2, temp3, temp4;
		file >> tempa >> triangles[i][v1] >> triangles[i][v2] >> triangles[i][v3];
		//cout << tempa << endl;
	}

	for (i = 0; i < num_vertices; i++)
	{
		string tempa;
		GLfloat temp2, temp3, temp4;//vt
		file >> tempa >> temp2 >> temp3;
		vt_cords[i] = vec2(temp2, 1- temp3);
		cout << temp2 << " "<< temp3 << endl;
	}

	

	for (i = 0; i < num_vertices; i++)
	{
		string tempa;//vn
		file >> tempa >> mnormals[i][v1] >> mnormals[i][v2] >> mnormals[i][v3];
		//cout << mnormals[i][v1] << mnormals[i][v2] << mnormals[i][v3] << endl;
		//cout << tempa << endl;
	}
	
	file.close();
	return mnormals;
	

}



void setProjections() {
	GLfloat aspect = GLfloat(w) / h;
	mat4  projectionpers = Perspective(45.0, aspect, 0.5, 6.0);

	mat4 projectionortho = Ortho(-1.0 * h / w, 1.0 * h / w, -1.0, 1.0, -1.0, 1.0);

	menuHandler->projectionpers = projectionpers;
	menuHandler->projectionortho = projectionortho;

}








void
populatePoints() {
	int i,index = 0;
	point4 temp1,temp2,temp3,temp4;
	vec3 nor1,nor2,nor3;
	vec2 cord1, cord2, cord3;
	pts = new point4[numtriangles * 3];
	nors = new vec3[numtriangles * 3];
	for (i = 0; i < numtriangles; i++) {
		temp1.x = mvertices[unsigned int(triangles[i][v1])].x;
		temp1.y = mvertices[unsigned int(triangles[i][v1])].y;
		temp1.z = mvertices[unsigned int(triangles[i][v1])].z;
		temp1.w = 1.0;

		temp2.x = mvertices[unsigned int(triangles[i][v2])].x;
		temp2.y = mvertices[unsigned int(triangles[i][v2])].y;
		temp2.z = mvertices[unsigned int(triangles[i][v2])].z;
		temp2.w = 1.0;

		temp3.x = mvertices[unsigned int(triangles[i][v3])].x;
		temp3.y = mvertices[unsigned int(triangles[i][v3])].y;
		temp3.z = mvertices[unsigned int(triangles[i][v3])].z;
		temp3.w = 1.0;

		pts[3 * i] = normalize(temp1);
		pts[(3 * i)+1] = normalize(temp2);
		pts[(3 * i)+2] = normalize(temp3);


		cord1 = vt_cords[unsigned int(triangles[i][v1])];
		cord2 = vt_cords[unsigned int(triangles[i][v2])];
		cord3 = vt_cords[unsigned int(triangles[i][v3])];

		tex_coords[3 * i] = cord1;
		tex_coords[(3 * i) + 1] = cord2;
		tex_coords[(3 * i) + 2] = cord3;




		nor1.x = mnormals[unsigned int(triangles[i][v1])].x;
		nor1.y = mnormals[unsigned int(triangles[i][v1])].y;
		nor1.z = mnormals[unsigned int(triangles[i][v1])].z;
		

		nor2.x = mnormals[unsigned int(triangles[i][v2])].x;
		nor2.y = mnormals[unsigned int(triangles[i][v2])].y;
		nor2.z = mnormals[unsigned int(triangles[i][v2])].z;
		

		nor3.x = mnormals[unsigned int(triangles[i][v3])].x;
		nor3.y = mnormals[unsigned int(triangles[i][v3])].y;
		nor3.z = mnormals[unsigned int(triangles[i][v3])].z;
		

		nors[3 * i] = normalize(nor1);
		nors[(3 * i) + 1] = normalize(nor2);
		nors[(3 * i) + 2] = normalize(nor3);





		}	
	}



vec3 *readPPM(){
		string p3_word;
		int width, height,max_level;

		file = fstream("texture.ppm");
		if (!file.is_open()) {
			return false;
		}
	
		file >> p3_word;
		cout << p3_word << endl;

		char comment[400];
		string temp0, temp1, temp2, temp3, temp4, temp5, temp6;
		//file.getline(comment, 400);
		file >> temp0 >> temp1 >> temp2 >> temp3 >> temp4 >> temp5 >> temp6;
		cout << temp0 << temp1 << temp6 << endl;

		file >> width >> height >> max_level;

		//cout << width << " " << height << " " << max_level << endl;


		GLuint nm = width * height;
		//GLuint image[2048*2048][4];

		int red, green, blue;

		int i = 0;
		int k = 0;

		for (i = 0; i < width; i++) {
			for (k = 0; k < height; k++) {

			file >> red >> green >> blue;
			image[i][k][0] = red;
			image[i][k][1] = green;
			image[i][k][2] = blue;
			//cout << "ilk forda" << image[i][k][0] << image[i][k][1] << image[i][k][2] << i << k << endl;
			}

			//getchar();

		}

		//tex_colors = new color4[2048*2048];

		for (i = 0; i < width; i++) {
			for (k = 0; k < height; k++) {
				//cout << "geldi" << endl;
				//getchar();
				//color4 mvec(GLfloat (image[i][k][0])/256.0, GLfloat(image[i][k][1])/256.0, GLfloat(image[i][k][2])/256.0, 1.0);
				//getchar();
				//tex_colors[i*2048+k] = mvec;

			}
			//getchar();
			//cout << "ikinci forda" << endl;
		}

		//tex_coords = new vec2[2048*2048];
		for (i = 0; i < width; i++) {

			for (k = 0; k < height; k++) {
				//vec2 vec(i, k);
				//tex_coords[2048 * i + k] = vec;
			}
			
			//cout << "ucuncu forda" << endl;
		}

	

	
	//cout << "geldi" << endl;
	file.close();

}
