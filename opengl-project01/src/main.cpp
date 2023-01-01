#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/matrix.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "shaders/Shader.h"
#include "stb_image.cpp"
#include "renderer/VertexBuffer.h"
#include "renderer/VertexArray.h"
#include "renderer/Window.h"
#include "renderer/Texture.h"
#include "renderer/Camera.h"

using namespace renderer;

void Init(unsigned int major, unsigned int minor);
void ProcessInput(GLFWwindow* window);
void mouseCallback(GLFWwindow* window, double xposIn, double yposIn);
void scrollCallback(GLFWwindow* window, double xpos, double ypos);

//settings
const unsigned int SCREEN_WIDTH = 1280;
const unsigned int SCREEN_HEIGHT = 720;

float deltaTime = 0.0f;
float lastFrame = 0.0f;

//mouse input
Camera camera(glm::vec3(0.0f, 1.0f, -3.0f));
float lastX = SCREEN_WIDTH / 2;
float lastY = SCREEN_HEIGHT / 2;
bool firstMouseInput = true;

glm::vec3 lightPos(1.2f, 1.0f, 2.0f);

int main()
{
	Init(4, 6);

	MainWindow window(SCREEN_WIDTH, SCREEN_HEIGHT, "Wojtek");
	glfwSetInputMode(window.Get(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	glfwSetCursorPosCallback(window.Get(), mouseCallback);
	glfwSetScrollCallback(window.Get(), scrollCallback);
	Shader cubeShader("src/shaders/vertex.glsl", "src/shaders/fragment.glsl");
	Shader lampShader("src/shaders/vertexLight.glsl", "src/shaders/fragmentLight.glsl");

    float vertices[] = {
        -0.5f, -0.5f, -0.5f,
         0.5f, -0.5f, -0.5f,
         0.5f,  0.5f, -0.5f,
         0.5f,  0.5f, -0.5f,
        -0.5f,  0.5f, -0.5f,
        -0.5f, -0.5f, -0.5f,

        -0.5f, -0.5f,  0.5f,
         0.5f, -0.5f,  0.5f,
         0.5f,  0.5f,  0.5f,
         0.5f,  0.5f,  0.5f,
        -0.5f,  0.5f,  0.5f,
        -0.5f, -0.5f,  0.5f,

        -0.5f,  0.5f,  0.5f,
        -0.5f,  0.5f, -0.5f,
        -0.5f, -0.5f, -0.5f,
        -0.5f, -0.5f, -0.5f,
        -0.5f, -0.5f,  0.5f,
        -0.5f,  0.5f,  0.5f,

         0.5f,  0.5f,  0.5f,
         0.5f,  0.5f, -0.5f,
         0.5f, -0.5f, -0.5f,
         0.5f, -0.5f, -0.5f,
         0.5f, -0.5f,  0.5f,
         0.5f,  0.5f,  0.5f,

        -0.5f, -0.5f, -0.5f,
         0.5f, -0.5f, -0.5f,
         0.5f, -0.5f,  0.5f,
         0.5f, -0.5f,  0.5f,
        -0.5f, -0.5f,  0.5f,
        -0.5f, -0.5f, -0.5f,

        -0.5f,  0.5f, -0.5f,
         0.5f,  0.5f, -0.5f,
         0.5f,  0.5f,  0.5f,
         0.5f,  0.5f,  0.5f,
        -0.5f,  0.5f,  0.5f,
        -0.5f,  0.5f, -0.5f,
    };

	VertexArray cubeVAO;
	VertexArray lampVAO;
	VertexBuffer vBuffer(vertices, sizeof(vertices));

    // position attribute
	cubeVAO.VertexAttribPtr(0, 3, 3 * sizeof(float), (void*)0);

	vBuffer.Bind();
	lampVAO.VertexAttribPtr(0, 3, 3 * sizeof(float), (void*)0);

	// zbuffer
	glEnable(GL_DEPTH_TEST);

	//------------
	// debug info
	//------------
	int minFps = INT_MAX, maxFps = INT_MIN;
	float startTime = static_cast<float>(glfwGetTime());

	//main loop
	while (!glfwWindowShouldClose(window.Get()))
    {
        float currentFrame = static_cast<float>(glfwGetTime());
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;
		int fps = static_cast<int>(1.0f / deltaTime);
		
		if (fps < minFps) minFps = fps;
		else if (fps > maxFps) maxFps = fps;

		std::cout.precision(2);
		std::cout << fps << " fps :: " << deltaTime << " ms\n";

		ProcessInput(window.Get());

        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		cubeShader.Use();
		cubeShader.SetVec3("objectColor", 1.0f, 0.5f, 0.31f); //coral color
		cubeShader.SetVec3("lightColor", 1.0f, 1.0f, 1.0f);

        glm::mat4 projection = glm::perspective(glm::radians(camera.GetZoom()), (float)SCREEN_WIDTH / (float)SCREEN_HEIGHT, 0.1f, 100.0f);
        glm::mat4 view = camera.GetViewMatrix();
		glm::mat4 model(1.0f);
		model = glm::translate(model, lightPos);
		model = glm::scale(model, glm::vec3(1.0f));
		glm::mat4 mvp;
		mvp = projection * view * model;
		cubeShader.Use();
		cubeShader.SetMat4("mvp", mvp);

		cubeVAO.Bind();
        glDrawArrays(GL_TRIANGLES, 0, 36);

		lampShader.Use();
        model = glm::mat4(1.0f);
        model = glm::translate(model, lightPos);
		model = glm::scale(model, glm::vec3(0.2f));

		mvp = projection * view * model;
		lampShader.SetMat4("mvp", mvp);

		lampVAO.Bind();
		glDrawArrays(GL_TRIANGLES, 0, 36);

		glfwSwapBuffers(window.Get());
        glfwPollEvents();
    }

	float endTime = static_cast<float>(glfwGetTime());

	std::cout << "MIN FPS: " << minFps << std::endl << "MAX FPS: " << maxFps << std::endl << "APP WORK TIME: " << endTime - startTime << std::endl;

    return 0;
}

void Init(unsigned int major, unsigned int minor)
{
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, major);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, minor);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
}
 
void ProcessInput(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, 1);

	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		camera.ProcessKeyboard(FORWARD, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		camera.ProcessKeyboard(BACKWARD, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		camera.ProcessKeyboard(LEFT, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		camera.ProcessKeyboard(RIGHT, deltaTime);
}

void mouseCallback(GLFWwindow* window, double xposIn, double yposIn)
{
	float xpos = static_cast<float>(xposIn);
	float ypos = static_cast<float>(yposIn);

	if (firstMouseInput)
	{
		lastX = xpos;
		lastY = ypos;
		firstMouseInput = false;
	}

	float xoffset = xpos - lastX;
	float yoffset = lastY - ypos;
	lastX = xpos;
	lastY = ypos;

	camera.ProcessMouseMovement(xoffset, yoffset, true);
}

void scrollCallback(GLFWwindow* window, double xoffset, double yoffset)
{
	camera.ProcessMouseScroll(static_cast<float>(yoffset));
}


