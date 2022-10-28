#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

void FramebufferSizeCallback(GLFWwindow* window, int width, int height);
void ProcessInput(GLFWwindow* window);

int main()
{
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow* window = glfwCreateWindow(1280, 720, "openGL", NULL, NULL);

	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window.\n";
		glfwTerminate();
		return -1;
	}

	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, FramebufferSizeCallback);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD\n";
		return -1;
	}

	glViewport(0, 0, 800, 600);
	GLfloat r = 0.f, g = 0.f, b = 0.f, a=1.f;
	while (!glfwWindowShouldClose(window))
	{
		ProcessInput(window);

		glClearColor(r, g, b, a);
		glClear(GL_COLOR_BUFFER_BIT);

		if (r < 1.f) r += .003f;
		else r = 0.f;
		if (g < 1.f) g += .003f;
		else g = 0.f;
		if (b < 1.f) b += .003f;
		else b = 0.f;

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwTerminate();
	return 0;
}

void FramebufferSizeCallback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

void ProcessInput(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, 1);
}