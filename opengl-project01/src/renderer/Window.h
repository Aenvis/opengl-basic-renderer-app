#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
namespace renderer
{
	class MainWindow
	{
	private:
		GLFWwindow* m_window;

	public:
		MainWindow(unsigned int screenWidth, unsigned int screenHeight, const char* windowName);
		~MainWindow();

		inline GLFWwindow* Get() const { return m_window; }
	};

	inline void FramebufferSizeCallback(GLFWwindow* window, int width, int height){ glViewport(0, 0, width, height); }

}
