#include <iostream>
#include <glm\gtc\matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include<fstream>;
#include<vector>;
#include "Shader_loader.hpp";
#include "Camera.hpp"
#include "Mesh.hpp"
#include <string>

using namespace Core;
using namespace MESHGENERATOR;

#define SIZE 200
#define TYPES 50

GLuint program;
Camera camera;

glm::mat4 model;
glm::mat4 view;
glm::mat4 world;
glm::mat4 projection;

GLfloat time;
float v_time;
GLint MVP;
bool keys[1024];
int circls[2] = { 400, 300 };
float sensitivity = 1.0f;
std::vector<Mesh> VAOS;
char city[SIZE][SIZE];

GLuint createField(int r)
{
	//Calculate positions of the Sky-scrapers
	int index = 0;
	//glm::vec3 positions[1000];
	glm::mat4 models[1000];
	bool naslo = false;

	for (int k = 0; k < r; k++)
	{ 
		naslo = false;
		while (!naslo)
		{
			int x = rand() % SIZE;
			int z = rand() % SIZE;
			if (city[z][x] != 1)
			{
				float scalar = rand() % 5 + 1;
				glm::mat4 model;
				model = glm::scale(model, glm::vec3(1, 1, 1));

				model = glm::translate(model, glm::vec3((GLfloat)x * 3, 0, (GLfloat)z * 3));

				models[index++] = model;

				city[z][x] = 1;
				naslo = true;
			}
		}
	}
	

	GLuint instanced;
	glGenBuffers(1, &instanced);
	glBindBuffer(GL_ARRAY_BUFFER, instanced);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::mat4) * r, &models[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	return instanced;

}
void mouse(int x, int y)
{
	int offsetX = x - circls[0];
	int offsetY = circls[1] - y;

	circls[0] = x;
	circls[1] = y;

	offsetX *= sensitivity;
	offsetY *= sensitivity;

	camera.getRotation().x += offsetX;
	camera.getRotation().y += offsetY;

	if (camera.getRotation().y > 89.0f)
		camera.getRotation().y = 89.0f;
	else if (camera.getRotation().y < -89.0f)
		camera.getRotation().y = -89.0f;

	float xr = camera.getRotation().x;
	float yr = camera.getRotation().y;

	glm::vec3 front;
	front.x = cos(glm::radians(xr)) * cos(glm::radians(yr));
	front.y = sin(glm::radians(yr));
	front.z = sin(glm::radians(xr)) * cos(glm::radians(yr));
	camera.getDirection() = glm::normalize(front);
}

void movement()
{
	float speed = 0.5f;
	if (keys[033])
		glutLeaveMainLoop();
	if (keys['w'])
		camera.getPosition() += camera.getDirection() * speed;
	if (keys['a'])
		camera.getPosition() -= glm::normalize(glm::cross(camera.getDirection(), camera.getUp())) * speed;
	if (keys['s'])
		camera.getPosition() -= camera.getDirection() * speed;
	if (keys['d'])
		camera.getPosition() += glm::normalize(glm::cross(camera.getDirection(), camera.getUp())) * speed;
	if (keys[' '])
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	else
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}
void cameraPrepare()
{

	v_time += 0.001f;
	time = 1.0f;
	view = glm::lookAt			//Camera position , view vector and up vector 
		(
		camera.getPosition(),
		camera.getPosition() + camera.getDirection(),
		camera.getUp()
		);

	glUniform1f(glGetUniformLocation(program, "t"), time);

	glUniformMatrix4fv(glGetUniformLocation(program,"MVP"),1,GL_FALSE,&world[0][0]);

	world = projection * view * model;
}

void keyBoard_up(unsigned char key, int x, int y)
{
		keys[key] = false;
}
void keyBoard(unsigned char key, int x, int y)
{
		keys[key] = true;
}
void calculate_positions()
{
	
	
}
void renderScene(void)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(0.0, 0.0, 0.0, 1.0);//clear red

	cameraPrepare();
	movement();

	glUseProgram(program);
	
	for (std::vector<Mesh>::iterator k = VAOS.begin(); k != VAOS.end();k++)
	{
		glBindVertexArray(k->getVAO());
		glDrawElementsInstanced(GL_TRIANGLES, k->getSize(), GL_UNSIGNED_INT, (void*)0,k->matrix_size);
		glBindVertexArray(0);
	}
	glutSwapBuffers();

	glutPostRedisplay();
}


void Init()
{


	unsigned int occurence = SIZE*SIZE / TYPES;
	for (int kappa = 0; kappa < TYPES; kappa++)
	{
		Mesh a;

		char height = rand() % 5 + 1;
		char dividing = rand() % 4 + 2;
		char pieces = rand() % 3 + 2;
		double able = ((double)(rand()) / RAND_MAX)*50 + 1.5;
		char p = rand() % 40;

		if (p != 0) p = 0; else p = 1;
		if (able < 50) able = rand() % 3 + 2; else able = 10;
		

		a.genRandomShape((int)height, (int)dividing,(int)pieces,able, -p);
		a.GPUSave();
		a.appendInstanced(createField(occurence), occurence);

		VAOS.push_back(a);
	}




	

	//Camera options
	view = glm::lookAt			//Camera position , view vector and up vector 
		(
		camera.getPosition(),
		camera.getPosition() + camera.getDirection(),
		camera.getUp()
		);

	model = glm::mat4(1.0f);

	projection = glm::perspective(45.0f, 800.0f / 600.0f,0.1f,1000.0f);


	world = projection * view * model;

	//load and compile shaders
	Shader_Loader shaderLoader;
	program = shaderLoader.CreateProgram("Vertex_Shader.glsl","Fragment_Shader.glsl");

	
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_FRONT);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}


void closeCallback()
{
	std::cout << "Finished" << std::endl;
	glutLeaveMainLoop();
}
int main(int argc, char **argv)
{
	for (int x = 0; x < SIZE; x++) for (int y = 0; y < SIZE; y++) city[x][y] = 0;
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowPosition(100, 100);
	glutInitWindowSize(800, 600);
	glutCreateWindow("");
	glewInit();

	Init();
	// register callbacks
	glutDisplayFunc(renderScene);
	glutCloseFunc(closeCallback);
	glutKeyboardFunc(keyBoard);
	glutKeyboardUpFunc(keyBoard_up);
	glutPassiveMotionFunc(mouse);
	glutSetCursor(GLUT_CURSOR_NONE);

	glutMainLoop();
	glDeleteProgram(program);
	return 0;

}