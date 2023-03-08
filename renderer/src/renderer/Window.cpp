#include "Window.h"
#include <iostream>

namespace renderer
{
	MainWindow::MainWindow(unsigned int screenWidth, unsigned int screenHeight, const char* windowName)
	{
		this->m_window = glfwCreateWindow(screenWidth, screenHeight, windowName, NULL, NULL);

		if (!m_window)
		{
			std::cout << "ERROR::GLFW::WINDOW::CREATION_FAILED\n";
			glfwTerminate();
		}

		glfwMakeContextCurrent(m_window);
		glfwSetFramebufferSizeCallback(m_window, FramebufferSizeCallback);
	}
	MainWindow::~MainWindow()
	{
		glfwTerminate();
	}
}