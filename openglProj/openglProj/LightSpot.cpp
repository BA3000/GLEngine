#include "LightSpot.h"



LightSpot::LightSpot(glm::vec3 _position, glm::vec3 _angles, glm::vec3 _color) :
	position(_position),
	angles(_angles),
	color(_color)
{
	constant = 1.0f;
	linear = 0.09f;
	quadratic = 0.032f;
	UpdatelightDir();
}


LightSpot::~LightSpot()
{
}

void LightSpot::UpdatelightDir()
{
	lightDir = glm::vec3(0, 0, 1.0f); // poiting to z (forward)
	lightDir = glm::rotateZ(lightDir, angles.z);
	lightDir = glm::rotateX(lightDir, angles.x);
	lightDir = glm::rotateY(lightDir, angles.y);
	lightDir = -1.0f * lightDir;
}
