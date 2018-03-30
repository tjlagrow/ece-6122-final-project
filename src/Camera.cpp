//
// Created by rory on 3/29/18.
//

#include "Camera.h"

Camera::Camera(glm::vec3 position, glm::vec3 lookat, glm::vec3 up)
	: m_position(position), m_lookat(lookat), m_up(up)
{

}

glm::mat4 Camera::get_view() const
{
	return glm::lookAt(m_position, m_position + m_lookat, m_up);
}

void Camera::update_mouse_pos(double &xpos, double &ypos)
{
	glm::vec2 delta = glm::vec2(xpos - m_mouse_x, ypos - m_mouse_y);
	m_lookat = glm::mat3(glm::rotate(delta.x, m_up)) * m_lookat;

	m_mouse_x = xpos;
	m_mouse_y = ypos;
}
