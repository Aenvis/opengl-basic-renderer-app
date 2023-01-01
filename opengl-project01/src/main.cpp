#include <iostream>
#include <vector>
#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>
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

int main();

void Init(unsigned int major, unsigned int minor);
void ProcessInput(GLFWwindow* window);
void mouseCallback(GLFWwindow* window, double xposIn, double yposIn);
void scrollCallback(GLFWwindow* window, double xpos, double ypos);

float deltaTime = 0.0f;
float lastFrame = 0.0f;

//-------------
// SCREEN
//-------------
const unsigned int SCREEN_WIDTH = 1280;
const unsigned int SCREEN_HEIGHT = 720;

//-------------
// MOUSE INPUT
//-------------
Camera camera(glm::vec3(1.0f, 1.0f, 5.0f));
float lastX = SCREEN_WIDTH / 2.0f;
float lastY = SCREEN_HEIGHT / 2.0f;
bool firstMouseInput = true;
bool isMouseFocused = true;

//-------------
// RENDER PARAMS
//-------------
int main()
{
	glm::vec3 lightSourcePos(1.0f, 2.0f, 2.0f);
	glm::vec3 lightSourceColor = { 1.0f, 1.0f, 1.0f };
	glm::vec3 ambient = { 0.0f, 0.0f, 0.0f };
	glm::vec3 diffuse = { 0.0f, 0.0f, 0.0f };
	glm::vec3 specular = { 0.0f, 0.0f, 0.0f };
	glm::vec3 lsAmbient = { 0.0f, 0.0f, 0.0f };
	glm::vec3 lsDiffuse = { 0.0f, 0.0f, 0.0f };
	glm::vec3 lsSpecular = { 0.0f, 0.0f, 0.0f };
	float shininess = 0.0f;
	float lampSpeedMultiplicator = 1.0f;
	float lampMoveRange = 1.0f;

	int renderedCubeCount = 0;
	
	Init(3, 3);

	MainWindow window(SCREEN_WIDTH, SCREEN_HEIGHT, "Wojtek");
	glfwSetInputMode(window.Get(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	glfwSetCursorPosCallback(window.Get(), mouseCallback);
	glfwSetScrollCallback(window.Get(), scrollCallback);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "ERROR::GLAD::INITIALIZATION_FAILED\n";
	}

	Shader cubeShader("src/shaders/vertex.glsl", "src/shaders/fragment.glsl");
	Shader lampShader("src/shaders/vertexLight.glsl", "src/shaders/fragmentLight.glsl");

	const float vertices[] = {
	-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
	 0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
	 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
	 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
	-0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
	-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,

	-0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
	 0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
	 0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
	 0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
	-0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
	-0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,

	-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
	-0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
	-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
	-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
	-0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
	-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,

	 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
	 0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
	 0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
	 0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
	 0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
	 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,

	-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
	 0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
	 0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
	 0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
	-0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
	-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,

	-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
	 0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
	 0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
	 0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
	-0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
	-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f
	};

	std::vector<glm::vec3> cubePositions{
	glm::vec3(0.0f, 0.0f, 0.0f),
	glm::vec3(2.0f, 0.0f, 0.0f),
	glm::vec3(4.0f, 0.0f, 0.0f),
	glm::vec3(0.0f, 2.0f, 0.0f),
	glm::vec3(2.0f, 2.0f, 0.0f),
	glm::vec3(4.0f, 2.0f, 0.0f),
	glm::vec3(0.0f, 4.0f, 0.0f),
	glm::vec3(2.0f, 4.0f, 0.0f),
	glm::vec3(4.0f, 4.0f, 0.0f),
	glm::vec3(0.0f, 0.0f, 2.0f),
	glm::vec3(2.0f, 0.0f, 2.0f),
	glm::vec3(4.0f, 0.0f, 2.0f),
	glm::vec3(0.0f, 2.0f, 2.0f),
	glm::vec3(2.0f, 2.0f, 2.0f),
	glm::vec3(4.0f, 2.0f, 2.0f),
	glm::vec3(0.0f, 4.0f, 2.0f),
	glm::vec3(2.0f, 4.0f, 2.0f),
	glm::vec3(4.0f, 4.0f, 2.0f),
	glm::vec3(0.0f, 0.0f, 4.0f),
	glm::vec3(2.0f, 0.0f, 4.0f),
	glm::vec3(4.0f, 0.0f, 4.0f),
	glm::vec3(0.0f, 2.0f, 4.0f),
	glm::vec3(2.0f, 2.0f, 4.0f),
	glm::vec3(4.0f, 2.0f, 4.0f),
	glm::vec3(0.0f, 4.0f, 4.0f),
	glm::vec3(2.0f, 4.0f, 4.0f),
	glm::vec3(4.0f, 4.0f, 4.0f),
	};

	VertexArray cubeVAO;
	VertexBuffer vBuffer(vertices, sizeof(vertices));

	//------------
	// cube attributes
	//------------
	cubeVAO.VertexAttribPtr(0, 3, 6 * sizeof(float), nullptr);
	cubeVAO.VertexAttribPtr(1, 3, 6 * sizeof(float), (void*)(3 * sizeof(float)));

	VertexArray lampVAO;
	vBuffer.Bind();
	lampVAO.VertexAttribPtr(0, 3, 6 * sizeof(float), nullptr);

	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	ImGui::StyleColorsDark();
	ImGui_ImplGlfw_InitForOpenGL(window.Get(), true);
	ImGui_ImplOpenGL3_Init("#version 330");


	//------------
	// z-buffer
	//------------
	glEnable(GL_DEPTH_TEST);

	//------------
	// debug info
	//------------
	int minFps = INT_MAX;
	int maxFps = INT_MIN;
	const float startTime = static_cast<float>(glfwGetTime());

	//------------
	// main loop
	//------------
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

		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();

		lightSourcePos.x = lampMoveRange * sin(static_cast<float>(glfwGetTime()) * lampSpeedMultiplicator);
		lightSourcePos.z = lampMoveRange * cos(static_cast<float>(glfwGetTime()) * lampSpeedMultiplicator);

		cubeShader.Use();
		cubeShader.SetVec3("material.ambient", ambient);
		cubeShader.SetVec3("material.diffuse", diffuse);
		cubeShader.SetVec3("material.specular", specular);
		cubeShader.SetFloat("material.shininess", glm::pow(2.0f, shininess));
		cubeShader.SetVec3("lightPos", lightSourcePos);
		cubeShader.SetVec3("lightSource.ambient", lsAmbient);
		cubeShader.SetVec3("lightSource.diffuse", lsDiffuse);
		cubeShader.SetVec3("lightSource.specular", lsSpecular);

		glm::mat4 projection = glm::perspective(glm::radians(camera.GetZoom()), (float)SCREEN_WIDTH / (float)SCREEN_HEIGHT, 0.1f, 100.0f);
		glm::mat4 view = camera.GetViewMatrix();

		if (renderedCubeCount > cubePositions.size()) renderedCubeCount = cubePositions.size();

		cubeVAO.Bind();
		for (size_t i = 0; i < renderedCubeCount; i++)
		{
			glm::mat4 model(1.0f);
			model = glm::translate(model, cubePositions[i]);
			cubeShader.SetMat4("model", model);
			glm::mat4 mvp;
			mvp = projection * view * model;
			cubeShader.Use();
			cubeShader.SetMat4("view", view);
			cubeShader.SetMat4("mvp", mvp);

			glDrawArrays(GL_TRIANGLES, 0, 36);
		}

		glm::mat4 model(1.0f);
		glm::mat4 mvp;
		lampShader.Use();
		lampShader.SetVec3("lampColor", lightSourceColor);
		model = glm::translate(model, lightSourcePos);
		model = glm::scale(model, glm::vec3(0.2f));

		mvp = projection * view * model;
		lampShader.SetMat4("mvp", mvp);

		lampVAO.Bind();
		glDrawArrays(GL_TRIANGLES, 0, 36);

		ImGui::Begin("Cube Material Params");
		ImGui::InputFloat3("Ambient", &ambient.x);
		ImGui::InputFloat3("Diffuse", &diffuse.x);
		ImGui::InputFloat3("Specular", &specular.x);
		ImGui::InputFloat("Shininess", &shininess);
		ImGui::InputInt("Cubes count: ", &renderedCubeCount);

		ImGui::End();

		ImGui::Begin("Lamp Params");
		ImGui::InputFloat3("Light Source ambient", &lsAmbient.x);
		ImGui::InputFloat3("Light Source diffuse", &lsDiffuse.x);
		ImGui::InputFloat3("Light Source specular", &lsSpecular.x);
		ImGui::InputFloat3("Light Source Position", &lightSourcePos.x);
		ImGui::End();

		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

		glfwSwapBuffers(window.Get());
		glfwPollEvents();
	}

	const float endTime = static_cast<float>(glfwGetTime());

	std::cout << "MIN FPS: " << minFps << std::endl << "MAX FPS: " << maxFps << std::endl << "APP WORK TIME: " << endTime - startTime << std::endl;

	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();

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
	{
		isMouseFocused = !isMouseFocused;
		if (isMouseFocused) glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
		else glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
	}

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
	if (!isMouseFocused) return;
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