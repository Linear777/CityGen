#pragma once
#include "glm\glm.hpp"

class Camera
{
	private:
		glm::vec3 position;
		glm::vec3 up;
		glm::vec3 rotation;
		glm::vec3 direction;

		const float move_speed = 2.0;

	public:
		Camera():direction(glm::vec3(0,0,-1)),position(glm::vec3(0,0,3)),up(glm::vec3(0,1,0)){};
		Camera(const glm::vec3 pos) :position(pos), rotation(glm::vec3(0, 0, 0)){};

		glm::vec3 & getRotation()
		{
			return rotation;
		}

		glm::vec3 & getDirection()
		{
			return direction;
		}

		glm::vec3 & getUp()
		{
			return up;
		}
		glm::vec3 & getPosition()
		{
			return position;
		}
};