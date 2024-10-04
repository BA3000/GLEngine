#include "LightDirectional.h"



LightDirectional::LightDirectional(glm::vec3 _position, glm::vec3 _angles, glm::vec3 _color) :
	position(_position),
	angles(_angles),
	color(_color)
{
	UpdateDirection();
}


LightDirectional::~LightDirectional()
{
}

void LightDirectional::UpdateDirection() {
	lightDir = glm::vec3(0, 0, 1.0f); // poiting to z (forward)
	lightDir = glm::rotateZ(lightDir, angles.z);
	lightDir = glm::rotateY(lightDir, angles.y);
	lightDir = glm::rotateX(lightDir, angles.x);
	lightDir = -1.0f * lightDir;
}