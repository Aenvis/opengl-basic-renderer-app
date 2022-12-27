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

//-------------
// SCREEN
//-------------
const unsigned int SCREEN_WIDTH = 1280;
const unsigned int SCREEN_HEIGHT = 720;

float deltaTime = 0.0f;
float lastFrame = 0.0f;

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
glm::vec3 lightPos(1.0f, 2.0f, 2.0f);
float ambientStrength = 0.0f;
float specularStrength = 0.5f;
int specularCoeff = 4;
float lampSpeedMultiplicator = 1.0f;
float lampMoveRange = 1.0f;
int main()
{
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

	// position attribute
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


	// zbuffer
	glEnable(GL_DEPTH_TEST);

	//------------
	// debug info
	//------------
	int minFps = INT_MAX;
	int maxFps = INT_MIN;
	const float startTime = static_cast<float>(glfwGetTime());

	const char* specularCoefficients[] = { "1", "2", "4", "8", "16", "32", "64", "128", "256", "512", "1024" };

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

		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();

		lightPos.x = lampMoveRange * sin(static_cast<float>(glfwGetTime()) * lampSpeedMultiplicator);
		lightPos.z = lampMoveRange * cos(static_cast<float>(glfwGetTime()) * lampSpeedMultiplicator);

		cubeShader.Use();
		cubeShader.SetVec3("lightPos", lightPos);
		cubeShader.SetFloat("ambientStrength", ambientStrength);
		cubeShader.SetFloat("specularStrength", specularStrength);
		cubeShader.SetInt("specularCoefficient", glm::pow(2, specularCoeff));
		cubeShader.SetVec3("objectColor", 0.2f, 0.7f, 0.91f);
		cubeShader.SetVec3("lightColor", 1.0f, 1.0f, 1.0f);

		glm::mat4 projection = glm::perspective(glm::radians(camera.GetZoom()), (float)SCREEN_WIDTH / (float)SCREEN_HEIGHT, 0.1f, 100.0f);
		glm::mat4 view = camera.GetViewMatrix();

		cubeVAO.Bind();
		for (size_t i = 0; i < cubePositions.size(); i++)
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
		model = glm::translate(model, lightPos);
		model = glm::scale(model, glm::vec3(0.2f));

		mvp = projection * view * model;
		lampShader.SetMat4("mvp", mvp);

		lampVAO.Bind();
		glDrawArrays(GL_TRIANGLES, 0, 36);

		ImGui::Begin("Cube Material Params");
		ImGui::SliderFloat("ambient", &ambientStrength, 0.0f, 1.0f);
		ImGui::SliderFloat("specular", &specularStrength, 0.0f, 1.0f);
		ImGui::SliderFloat("specular", &specularStrength, 0.0f, 1.0f);
		ImGui::ListBox("Specular coeff", &specularCoeff, specularCoefficients, IM_ARRAYSIZE(specularCoefficients));
		ImGui::End();

		ImGui::Begin("Lamp Params");
		ImGui::SliderFloat("Move Speed", &lampSpeedMultiplicator, 0.0f, 3.0f);
		ImGui::SliderFloat("Move Range", &lampMoveRange, 0.0f, 8.0f);
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


