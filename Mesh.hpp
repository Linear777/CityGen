#pragma once
#include <vector>



namespace MESHGENERATOR
{

	class Mesh
	{
	private:
		GLuint vao;
		GLuint vbo;
		GLuint indices;

		std::vector<glm::vec3> vertexes;
		std::vector<GLuint> indexes;
		std::vector<glm::vec3> normals;
		std::vector<glm::vec4> colors;

	public:
		GLuint matrix_size;
		
		Mesh();
		~Mesh();
		

		void genCube();
		void genCylinder();
		void genSierpinsky();
		void genRandomShape(double h, double d_s, double s_d,double s_s, bool ran);

		void appendInstanced(GLuint instanced,GLuint matrix_size);

		void GPUSave();
		GLuint getVAO();
		unsigned int getSize();
	};

}