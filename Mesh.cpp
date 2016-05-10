#include "glm\glm.hpp"
#include "Dependencies\glew\glew.h"
#include "Mesh.hpp"
#include <iostream>
#include <ctime>

#define PI 3.14159
#define TAU 2*PI

using namespace MESHGENERATOR;
using namespace std;


void Mesh::genCube()
{
	
}

void Mesh::genRandomShape(double h, double d_s, double s_d,double s_s, bool ran)
{
	unsigned __int8 p_h = 0;
	
	unsigned int _prev = 0;
	for (double s = 0; s < s_d; s++)
	{
		unsigned int _vex = 0;
		srand(time(0));
		double theta = 0;

		glm::vec3 second_n;
		glm::vec3 begin(1/(s+1), h, 0);

		vertexes.push_back(begin);
		begin.y = p_h;
		vertexes.push_back(begin);
		while (true)
		{
			
		    theta += (double)(PI / s_s);

			if (theta > TAU) break;
			second_n.x = cos(theta)/(s+ 1);
			second_n.y = h;
			second_n.z = sin(theta) / (s + 1);

			vertexes.push_back(second_n);
			second_n.y = p_h;
			vertexes.push_back(second_n);
			_vex++;
		}

		begin.y = h;
		vertexes.push_back(begin);
		begin.y = p_h;
		vertexes.push_back(begin);
		
		if (ran && (s == s_d-1))
		{
			vertexes.push_back(glm::vec3(0, h + 3, 0));
		}
		else
			vertexes.push_back(glm::vec3(0, h, 0));


		for (int i = 0; i < _vex+1; i++)
		{

			indexes.push_back(2 * i + (_prev));
			indexes.push_back(2 * i + 2 + (_prev));
			indexes.push_back(vertexes.size() - 1);

			indexes.push_back(2 * i + (_prev));
			indexes.push_back(2 * i + 1 + (_prev));
			indexes.push_back(2 * i + 3 + (_prev));

			indexes.push_back(2 * i + (_prev));
			indexes.push_back(2 * i + 3 + (_prev));
			indexes.push_back(2 * i + 2 + (_prev));
		}
		_prev = vertexes.size();
	
		p_h = h;
		h += h / (pow(d_s,s+1));
	}
}

unsigned int Mesh::getSize()
{
	return indexes.size();
}
void Mesh::GPUSave()
{

	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	glGenBuffers(1, &vbo); //Vertexes
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER,sizeof(glm::vec3) * vertexes.size(),&vertexes[0],GL_STATIC_DRAW);

	glGenBuffers(1, &indices);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indices);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint) * indexes.size(), &indexes[0], GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,sizeof(GLfloat)*3,(void*)0);

	glBindVertexArray(0);
}

GLuint Mesh::getVAO()
{
	return vao;
}

void Mesh::appendInstanced(GLuint instanced,GLuint matrix_size)
{
	glBindVertexArray(vao);

	GLuint vec4Size = sizeof(glm::vec4);
	glEnableVertexAttribArray(2);
	glBindBuffer(GL_ARRAY_BUFFER, instanced);
	glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, 4 * vec4Size, (GLvoid*)0);
	glEnableVertexAttribArray(3);
	glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, 4 * vec4Size, (GLvoid*)(vec4Size));
	glEnableVertexAttribArray(4);
	glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, 4 * vec4Size, (GLvoid*)(2 * vec4Size));
	glEnableVertexAttribArray(5);
	glVertexAttribPointer(5, 4, GL_FLOAT, GL_FALSE, 4 * vec4Size, (GLvoid*)(3 * vec4Size));
	glBindBuffer(GL_ARRAY_BUFFER,0);

	glVertexAttribDivisor(2, 1);
	glVertexAttribDivisor(3, 1);
	glVertexAttribDivisor(4, 1);
	glVertexAttribDivisor(5, 1);

	glBindVertexArray(0);
	this->matrix_size = matrix_size;
}
Mesh::Mesh()
{

}

Mesh::~Mesh()
{

}