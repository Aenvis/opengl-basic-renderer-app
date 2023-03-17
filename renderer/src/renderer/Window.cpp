#include "Window.h"
#include <iostream>

namespace renderer
{
	MainWindow::MainWindow(const int screenWidth, const int screenHeight, const std::string& windowName)
	{
		this->m_window = std::shared_ptr<GLFWwindow>(glfwCreateWindow(screenWidth, screenHeight, windowName.c_str(), nullptr, nullptr));

		if (!m_window)
		{
			std::cout << "ERROR::GLFW::WINDOW::CREATION_FAILED\n";
			glfwTerminate();
		}

		glfwMakeContextCurrent(m_window.get());
	}
}