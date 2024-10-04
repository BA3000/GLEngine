#pragma region include and define
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#define STB_IMAGE_IMPLEMENTATION
//#include "stb_image.h"
#include "Camera.h"
#include "Shader.h"
#include "Material.h"
#include "LightDirectional.h"
#include "LightPoint.h"
#include "LightSpot.h"
#include "Mesh.h"
#include "Model.h"

#pragma endregion

#pragma region function names
using namespace std;

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow *window);
unsigned int LoadImageToGPU(const char* filename, GLint internalFormat, GLenum format, int textureSlot);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
#pragma endregion

#pragma region settings
// settings
const unsigned int SCR_WIDTH = 1920;
const unsigned int SCR_HEIGHT = 1080;

// mouse location
float lastX = 0.0f;
float lastY = 0.0f;
float fov = 45.0f;
bool firstMouse = true;
#pragma endregion

#pragma region Camera Declare
// Instantiate Camera class
//Camera camera(glm::vec3(0, 0, 3.0f), glm::vec3(0, -1.0f, 0), glm::vec3(0, 1.0f, 0));
Camera camera(glm::vec3(0, 0, 3.0f), glm::radians(0.0f), glm::radians(180.0f), glm::vec3(0, 1.0f, 0));
#pragma endregion

#pragma region Light Declare
LightDirectional lightD(glm::vec3(1.0f, 1.0f, -1.0f), glm::vec3(glm::radians(-70.0f), glm::radians(180.0f), glm::radians(0.0f)), glm::vec3(10.0f, 10.0f, 10.0f));

LightPoint lightP0(glm::vec3(1.0f, 1.0f, -1.0f), glm::vec3(glm::radians(45.0f), 0, 0), glm::vec3(1.0f, 0.0f, 0.0f));
LightPoint lightP1(glm::vec3(1.0f, 0.0f, -1.0f), glm::vec3(glm::radians(45.0f), 0, 0), glm::vec3(0.0f, 1.0f, 0.0f));
LightPoint lightP2(glm::vec3(0.0f, 1.0f, -1.0f), glm::vec3(glm::radians(45.0f), 0, 0), glm::vec3(0.0f, 0.0f, 1.0f));
LightPoint lightP3(glm::vec3(0.0f, 0.0f, -1.0f), glm::vec3(glm::radians(45.0f), 0, 0), glm::vec3(1.0f, 1.0f, 1.0f));

LightSpot lightS(glm::vec3(0.0f, 5.0f, 0.0f), glm::vec3(glm::radians(90.0f), glm::radians(0.0f), 0), glm::vec3(10.0f, 10.0f, 10.0f));
#pragma endregion

int main(int argc, char* argv[])
{
	// path of current exe file
	std::string exePath = argv[0];
	// model path
	auto exeDir = exePath.substr(0, exePath.find_last_of('\\'));
	std::string projPath = exePath.substr(0, exeDir.find_last_of('\\')) + "\\..\\..\\..\\";
	std::string modelPath = projPath + "Resources\\models\\nanosuit.obj";
#ifdef DEBUG
	std::cout << modelPath << endl;
#endif

	// glfw: initialize and configure, opengl version: 3.3
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif // __APPLE__

	// glfw window creation
	GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "GLEngine", NULL, NULL);
	if (window == NULL) {
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	// capture cursor
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetScrollCallback(window, scroll_callback);

	// glad: load all OpenGL function pointers
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	// depth test enable
	glEnable(GL_DEPTH_TEST);
#pragma region Init Shader Program
	auto vertPath = projPath + "src\\vertexSource.vert";
	auto fragPath = projPath + "src\\fragmentSource.frag";
	Shader* testShader = new Shader(vertPath, fragPath);
#pragma endregion

#pragma region Init Material
	std::string containerTexPath = projPath + "Resources\\textures\\container2.png";
	std::string containerSpecularTexPath = projPath + "Resources\\textures\\container2_specular.png";
#ifdef DEBUG
	std::cout << containerTexPath << std::endl;
#endif
	Material * myMaterial = new Material(testShader,
		LoadImageToGPU(containerTexPath.c_str(), GL_RGBA, GL_RGBA, Shader::DIFFUSE),
		LoadImageToGPU(containerSpecularTexPath.c_str(), GL_RGBA, GL_RGBA, Shader::SPECULAR),
		glm::vec3(1.0f, 1.0f, 1.0f),
		32.0f);
#pragma endregion

	Model model(modelPath);

	glm::mat4 modelMat;
	glm::mat4 modelMat2;
	//modelMat = glm::rotate(modelMat, glm::radians(-45.0f), glm::vec3(1.0f, 0, 0));
	glm::mat4 viewMat;
	glm::mat4 projMat;
	

	while (!glfwWindowShouldClose(window))
	{
		processInput(window);

		// set background color
		glClearColor(0.2f, 0.3f, 0.5f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		viewMat = camera.GetViewMatrix();

		// set models' positions
		modelMat2 = glm::translate(modelMat2, glm::vec3(0.0f, 0.0f, 0.0f));
		projMat = glm::perspective(fov, float(SCR_WIDTH) / float(SCR_HEIGHT), 0.1f, 100.0f);

		testShader->use();
		glUniformMatrix4fv(glGetUniformLocation(testShader->ID, "modelMat"), 1, GL_FALSE, glm::value_ptr(modelMat2));
		glUniformMatrix4fv(glGetUniformLocation(testShader->ID, "viewMat"), 1, GL_FALSE, glm::value_ptr(viewMat));
		glUniformMatrix4fv(glGetUniformLocation(testShader->ID, "projMat"), 1, GL_FALSE, glm::value_ptr(projMat));
		glUniform3f(glGetUniformLocation(testShader->ID, "objColor"), 1.0f, 1.0f, 1.0f);
		glUniform3f(glGetUniformLocation(testShader->ID, "ambientColor"), 0.1f, 0.1f, 0.7f);
		glUniform3f(glGetUniformLocation(testShader->ID, "cameraPos"), camera.Position.x, camera.Position.y, camera.Position.z);

#pragma region setting light sources
		glUniform3f(glGetUniformLocation(testShader->ID, "lightD.pos"), lightD.position.x, lightD.position.y, lightD.position.z);	// light position
		glUniform3f(glGetUniformLocation(testShader->ID, "lightD.dirToLight"), lightD.lightDir.x, lightD.lightDir.y, lightD.lightDir.z);
		glUniform3f(glGetUniformLocation(testShader->ID, "lightD.color"), lightD.color.x, lightD.color.y, lightD.color.z);

		//glUniform3f(glGetUniformLocation(testShader->ID, "lightP0.pos"), lightP0.position.x, lightP0.position.y, lightP0.position.z);	// light position
		//glUniform3f(glGetUniformLocation(testShader->ID, "lightP0.dirToLight"), lightP0.lightDir.x, lightP0.lightDir.y, lightP0.lightDir.z);
		//glUniform3f(glGetUniformLocation(testShader->ID, "lightP0.color"), lightP0.color.x, lightP0.color.y, lightP0.color.z);
		//glUniform1f(glGetUniformLocation(testShader->ID, "lightP0.constant"), lightP0.constant);
		//glUniform1f(glGetUniformLocation(testShader->ID, "lightP0.linear"), lightP0.linear);
		//glUniform1f(glGetUniformLocation(testShader->ID, "lightP0.quadratic"), lightP0.quadratic);

		//glUniform3f(glGetUniformLocation(testShader->ID, "lightP1.pos"), lightP1.position.x, lightP1.position.y, lightP1.position.z);	// light position
		//glUniform3f(glGetUniformLocation(testShader->ID, "lightP1.dirToLight"), lightP1.lightDir.x, lightP1.lightDir.y, lightP1.lightDir.z);
		//glUniform3f(glGetUniformLocation(testShader->ID, "lightP1.color"), lightP1.color.x, lightP1.color.y, lightP1.color.z);
		//glUniform1f(glGetUniformLocation(testShader->ID, "lightP1.constant"), lightP1.constant);
		//glUniform1f(glGetUniformLocation(testShader->ID, "lightP1.linear"), lightP1.linear);
		//glUniform1f(glGetUniformLocation(testShader->ID, "lightP1.quadratic"), lightP1.quadratic);

		//glUniform3f(glGetUniformLocation(testShader->ID, "lightP2.pos"), lightP2.position.x, lightP2.position.y, lightP2.position.z);	// light position
		//glUniform3f(glGetUniformLocation(testShader->ID, "lightP2.dirToLight"), lightP2.lightDir.x, lightP2.lightDir.y, lightP2.lightDir.z);
		//glUniform3f(glGetUniformLocation(testShader->ID, "lightP2.color"), lightP2.color.x, lightP2.color.y, lightP2.color.z);
		//glUniform1f(glGetUniformLocation(testShader->ID, "lightP2.constant"), lightP2.constant);
		//glUniform1f(glGetUniformLocation(testShader->ID, "lightP2.linear"), lightP2.linear);
		//glUniform1f(glGetUniformLocation(testShader->ID, "lightP2.quadratic"), lightP2.quadratic);

		//glUniform3f(glGetUniformLocation(testShader->ID, "lightP3.pos"), lightP3.position.x, lightP3.position.y, lightP3.position.z);	// light position
		//glUniform3f(glGetUniformLocation(testShader->ID, "lightP3.dirToLight"), lightP3.lightDir.x, lightP3.lightDir.y, lightP3.lightDir.z);
		//glUniform3f(glGetUniformLocation(testShader->ID, "lightP3.color"), lightP3.color.x, lightP3.color.y, lightP3.color.z);
		//glUniform1f(glGetUniformLocation(testShader->ID, "lightP3.constant"), lightP3.constant);
		//glUniform1f(glGetUniformLocation(testShader->ID, "lightP3.linear"), lightP3.linear);
		//glUniform1f(glGetUniformLocation(testShader->ID, "lightP3.quadratic"), lightP3.quadratic);

		glUniform3f(glGetUniformLocation(testShader->ID, "lightS.pos"), lightS.position.x, lightS.position.y, lightS.position.z);	// light position
		glUniform3f(glGetUniformLocation(testShader->ID, "lightS.dirToLight"), lightS.lightDir.x, lightS.lightDir.y, lightS.lightDir.z);
		glUniform3f(glGetUniformLocation(testShader->ID, "lightS.color"), lightS.color.x, lightS.color.y, lightS.color.z);
		glUniform1f(glGetUniformLocation(testShader->ID, "lightS.constant"), lightS.constant);
		glUniform1f(glGetUniformLocation(testShader->ID, "lightS.linear"), lightS.linear);
		glUniform1f(glGetUniformLocation(testShader->ID, "lightS.quadratic"), lightS.quadratic);
		glUniform1f(glGetUniformLocation(testShader->ID, "lightS.CosPhyInner"), lightS.CosPhyInner);
		glUniform1f(glGetUniformLocation(testShader->ID, "lightS.CosPhyOutter"), lightS.CosPhyOutter);
#pragma endregion

		myMaterial->shader->SetUniform3f("material.ambient", myMaterial->ambient);
		myMaterial->shader->SetUniform1i("material.diffuse", Shader::DIFFUSE);
		myMaterial->shader->SetUniform1i("material.specular", Shader::SPECULAR);
		//myMaterial->shader->SetUniform3f("material.specular", myMaterial->specular);
		myMaterial->shader->SetUniform1f("material.shininess", myMaterial->shininess);

		glDrawArrays(GL_TRIANGLES, 0, 36);

		model.Draw(myMaterial->shader);

		// glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
		glfwSwapBuffers(window);
		glfwPollEvents();
		camera.UpdateCameraPos();
	}

	// glfw: terminate, clearing all previously allocated GLFW resources.
	glfwTerminate();
	return 0;
}


void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

// process Input and move view camera
void processInput(GLFWwindow *window)
{
	// escape
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
	// forward and backward
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
		camera.speedZ = 1.0f;
	}
	else if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
		camera.speedZ = -1.0f;
	}
	else
	{
		camera.speedZ = 0;
	}
	// left and right
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		camera.speedX = 1.0f;
	else if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		camera.speedX = -1.0f;
	else {
		camera.speedX = 0;
	}
	// up and down
	if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
		camera.speedY = -1.0f;
	else if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
		camera.speedY = 1.0f;
	else {
		camera.speedY = 0;
	}
}

unsigned int LoadImageToGPU(const char* filename, GLint internalFormat, GLenum format, int textureSlot) {
	unsigned int TexBuffer;
	glGenTextures(1, &TexBuffer);
	glActiveTexture(GL_TEXTURE0 + textureSlot);
	glBindTexture(GL_TEXTURE_2D, TexBuffer);

	int width, height, nrChannel;
	stbi_set_flip_vertically_on_load(true);
	unsigned char *data = stbi_load(filename, &width, &height, &nrChannel, 0);
	if (data)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, width, height, 0, format, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "load image failed. filename: " << filename << std::endl;
	}
	stbi_image_free(data);
	return TexBuffer;
}

// using mouse to control direction of view camera
void mouse_callback(GLFWwindow* window, double xpos, double ypos) {
	if (firstMouse == true)
	{
		lastX = static_cast<float>(xpos);
		lastY = static_cast<float>(ypos);
		firstMouse = false;
	}
	float deltaX;
	float deltaY;
	deltaX = static_cast<float>(xpos) - lastX;
	deltaY = static_cast<float>(ypos) - lastY;

	lastX = static_cast<float>(xpos);
	lastY = static_cast<float>(ypos);

	camera.ProcessMouseMovement(deltaX, deltaY);
	//printf("%f\n", deltaX);
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset) {
	if (fov >= glm::radians(20.0f) && fov <= glm::radians(45.0f)) {
		fov -= static_cast<float>(glm::radians(yoffset));
	}
	if (fov <= glm::radians(20.0f)) {
		fov = static_cast<float>(glm::radians(20.0f));
	}
	if (fov >= glm::radians(45.0f)) {
		fov = static_cast<float>(glm::radians(45.0f));
	}
}