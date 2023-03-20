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

void glfwInitializationSequence(int major, int minor);
void ProcessInput(GLFWwindow* window);
void FramebufferSizeCallback(GLFWwindow*, int width, int height);
void mouseCallback(GLFWwindow* window, double xposIn, double yposIn);
void scrollCallback(GLFWwindow* window, double xpos, double ypos);

float deltaTime = 0.0f;
float lastFrame = 0.0f;

constexpr unsigned int SCREEN_WIDTH = 1280;
constexpr unsigned int SCREEN_HEIGHT = 720;

Camera camera(glm::vec3(1.0f, 1.0f, 5.0f));

float lastX = SCREEN_WIDTH / 2.0f;
float lastY = SCREEN_HEIGHT / 2.0f;
bool firstMouseInput = true;
bool isMouseFocused = true;
bool showLampParam = false;

int main()
{
	glfwInitializationSequence(3, 3);

	MainWindow window(SCREEN_WIDTH, SCREEN_HEIGHT, "Wojtek");

	glfwSetInputMode(window.GetWindow(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	glfwSetFramebufferSizeCallback(window.GetWindow(), FramebufferSizeCallback);
	glfwSetCursorPosCallback(window.GetWindow(), mouseCallback);
	glfwSetScrollCallback(window.GetWindow(), scrollCallback);

	if (!gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress)))
	{
		std::cout << "ERROR::GLAD::INITIALIZATION_FAILED\n";
	}

	Texture diffuseMap("assets/container.png", true);
	Texture specularMap("assets/container_specular.png", true);
	Texture emissionMap("assets/container_emissive.png", true);

	glm::vec3 lightSourcePos(0.2f, 1.0f, 0.3f);
	constexpr glm::vec3 lightSourceColor = { 1.0f, 1.0f, 1.0f }; //only defines color of the lamp cube
	glm::vec3 lightSourceAmbient = { 0.2f, 0.2f, 0.2f };
	glm::vec3 lightSourceDiffuse = { 0.5f, 0.5f, 0.5f };
	glm::vec3 lightSourceSpecular = { 1.0f, 1.0f, 1.0f };

	glm::vec3 specular = { 0.0f, 0.0f, 0.0f };

	float lightSourceConstant = 1.0f;
	float lightSourceLinear = 0.045f;
	float lightSourceQuadratic = 0.0075f;
	float shininess = 0.0f;

	int renderedCubeCount = 3;

	Shader cubeShader("src/shaders/vertex.glsl", "src/shaders/fragment.glsl");
	Shader lampShader("src/shaders/vertexLight.glsl", "src/shaders/fragmentLight.glsl");

	constexpr float vertices[] = {
		// positions          // normals           // texture coords
		-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,
		 0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 0.0f,
		 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
		 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
		-0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,

		-0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 0.0f,
		 0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 0.0f,
		 0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 1.0f,
		 0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 1.0f,
		-0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 1.0f,
		-0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 0.0f,

		-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
		-0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
		-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
		-0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
		-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,

		 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
		 0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
		 0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
		 0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
		 0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
		 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,

		-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,
		 0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 1.0f,
		 0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f,
		 0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 0.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,

		-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f,
		 0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 1.0f,
		 0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
		 0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
		-0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 0.0f,
		-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f
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

	cubeVAO.VertexAttribPtr(0, 3, 8 * sizeof(float), nullptr);
	cubeVAO.VertexAttribPtr(1, 3, 8 * sizeof(float), reinterpret_cast<void*>(3 * sizeof(float)));
	cubeVAO.VertexAttribPtr(2, 2, 8 * sizeof(float), reinterpret_cast<void*>(6 * sizeof(float)));

	VertexArray lampVAO;
	vBuffer.Bind();
	lampVAO.VertexAttribPtr(0, 3, 8 * sizeof(float), nullptr);

	cubeShader.Use();
	cubeShader.SetInt("material.diffuse", 0);
	cubeShader.SetInt("material.specular", 1);
	cubeShader.SetInt("material.emission", 2);

	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	ImGui::StyleColorsDark();
	ImGui_ImplGlfw_InitForOpenGL(window.GetWindow(), true);
	ImGui_ImplOpenGL3_Init("#version 330");

	glEnable(GL_DEPTH_TEST);

	while (!glfwWindowShouldClose(window.GetWindow()))
	{
		float currentFrame = static_cast<float>(glfwGetTime());
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		ProcessInput(window.GetWindow());

		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();

		//lightSourcePos.x = lampMoveRange * sin(static_cast<float>(glfwGetTime()) * lampSpeedMultiplicator);
		//lightSourcePos.z = lampMoveRange * cos(static_cast<float>(glfwGetTime()) * lampSpeedMultiplicator);

		cubeShader.Use();
		cubeShader.SetVec3("material.specular", specular);
		cubeShader.SetFloat("material.shininess", glm::pow(2.0f, shininess));
		cubeShader.SetVec3("spotlight.position", camera.GetPosition());
		cubeShader.SetVec3("spotlight.position", camera.GetForward());
		cubeShader.SetVec3("spotlight.ambient", lightSourceAmbient);
		cubeShader.SetVec3("spotlight.diffuse", lightSourceDiffuse);
		cubeShader.SetVec3("spotlight.specular", lightSourceSpecular);
		cubeShader.SetFloat("spotlight.cutoff", glm::cos(glm::radians(12.5f)));
		cubeShader.SetFloat("spotlight.constant", lightSourceConstant);
		cubeShader.SetFloat("spotlight.linear", lightSourceLinear);
		cubeShader.SetFloat("spotlight.quadratic", lightSourceQuadratic);

		glm::mat4 projection = glm::perspective(glm::radians(camera.GetZoom()), static_cast<float>(SCREEN_WIDTH) / static_cast<float>(SCREEN_HEIGHT), 0.1f, 100.0f);
		glm::mat4 view = camera.GetViewMatrix();

		if (renderedCubeCount > cubePositions.size()) renderedCubeCount = cubePositions.size();

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, diffuseMap.GetID());
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, specularMap.GetID());
		glActiveTexture(GL_TEXTURE2);
		glBindTexture(GL_TEXTURE_2D, emissionMap.GetID());
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

		//TODO: LOAD MAPS THROUGH IMGUI 
		ImGui::Begin("Cube Material Params");
		ImGui::InputFloat3("Specular", &specular.x);
		ImGui::InputFloat("Shininess", &shininess);
		ImGui::InputInt("Cubes count: ", &renderedCubeCount);
		ImGui::End();

		if (showLampParam)
		{
			ImGui::Begin("Lamp Params");
			ImGui::InputFloat3("Light Source Position", &lightSourcePos.x);
			ImGui::InputFloat3("Light Source ambient", &lightSourceAmbient.x);
			ImGui::InputFloat3("Light Source diffuse", &lightSourceDiffuse.x);
			ImGui::InputFloat3("Light Source specular", &lightSourceSpecular.x);
			ImGui::InputFloat("Light Source constant", &lightSourceConstant);
			ImGui::InputFloat("Light Source linear", &lightSourceLinear);
			ImGui::InputFloat("Light Source quadratic", &lightSourceQuadratic);
			ImGui::End();
		}

		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

		glfwSwapBuffers(window.GetWindow());
		glfwPollEvents();
		glfwPollEvents();
	}

	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();
	return 0;
}

void glfwInitializationSequence(int major, int minor)
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
	if (glfwGetKey(window, GLFW_KEY_L) == GLFW_PRESS)
		showLampParam = !showLampParam;	
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		camera.ProcessKeyboard(FORWARD, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		camera.ProcessKeyboard(BACKWARD, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		camera.ProcessKeyboard(LEFT, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		camera.ProcessKeyboard(RIGHT, deltaTime);
}

void FramebufferSizeCallback(GLFWwindow*, int width, int height)
{
	glViewport(0, 0, width, height);
}

void mouseCallback(GLFWwindow* window, double xposIn, double yposIn)
{
	if (!isMouseFocused) return;
	auto xpos = static_cast<float>(xposIn);
	auto ypos = static_cast<float>(yposIn);

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