#pragma once

#include <glad/glad.h>
#include <string>
#include <glm/glm.hpp>
#include <vector>

namespace meshmanager
{
	struct Vertex {
		glm::vec3 position;
		glm::vec2 uv;
		glm::vec3 normal;
	};
	bool LoadObj(const std::string filePath, std::vector<glm::vec3>& out_vertices, std::vector<glm::vec2>& out_uvs, std::vector<glm::vec3>& out_normals, std::vector<GLuint>& out_indices);
}