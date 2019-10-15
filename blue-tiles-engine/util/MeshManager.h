#pragma once

#include <glad/glad.h>
#include <string>
#include <glm/glm.hpp>
#include <vector>
#include <map>

namespace meshmanager
{
	struct Vertex {
		glm::vec3 position;
		glm::vec2 uv;
		glm::vec3 normal;
	};
	struct Mesh {
		std::vector<Vertex> vertices;
		std::vector<GLuint> indices;
	};
	extern std::map<std::string, Mesh> cachedObjs;
	bool LoadObj(const std::string filePath, std::vector<Vertex>& out_vertices, std::vector<GLuint>& out_indices);
}