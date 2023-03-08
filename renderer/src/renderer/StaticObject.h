#pragma once
#include "../shaders/Shader.h"
#include <memory>
namespace renderer
{
	class StaticObject
	{
	private:
		std::shared_ptr<Shader> m_shader;

	};
}


