//
// Created by rory on 3/29/18.
//

#ifndef ECE_6122_FINAL_PROJECT_CAMERA_H
#define ECE_6122_FINAL_PROJECT_CAMERA_H


#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/matrix_transform.hpp>

class Camera
{
public:
	Camera(glm::vec3 position, glm::vec3 lookat, glm::vec3 up);

	glm::mat4 get_view() const;
	void update_mouse_pos(double &xpos, double &ypos);

private:
	glm::vec3 m_position;
	glm::vec3 m_lookat;
	glm::vec3 m_up;

	double m_mouse_x, m_mouse_y;

};


#endif //ECE_6122_FINAL_PROJECT_CAMERA_H
