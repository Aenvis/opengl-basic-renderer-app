#pragma once
#include <GLFW/glfw3.h>
#include <memory>
#include <string>

namespace renderer
{
	class MainWindow
	{
	private:
		std::shared_ptr<GLFWwindow> m_window;

	public:
		MainWindow(int screenWidth, int screenHeight, const std::string& windowName);

		GLFWwindow* GetWindow() const { return m_window.get(); }
	};
}
