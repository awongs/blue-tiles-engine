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

	struct AnimatedVertex
	{
		glm::vec3 position;
		glm::vec2 uv;
		glm::vec3 normal;
		glm::ivec3 jointId;
		glm::vec3 jointWeight;
	};

	struct Mesh {
		std::vector<Vertex> vertices;
		std::vector<GLuint> indices;
	};

	struct AnimatedMesh
	{
		std::vector<AnimatedVertex> vertices;
		std::vector<GLuint> indices;
	};

	extern std::map<std::string, Mesh> cachedObjs;
	extern std::map<std::pair<std::string, std::string>, AnimatedMesh> cachedObjsWithSkeleton;

	bool LoadObj(const std::string filePath, std::vector<Vertex>& out_vertices, std::vector<GLuint>& out_indices);

	bool LoadObjWithSkeleton(const std::string objPath, const std::string skeletonPath, std::vector<AnimatedVertex>& out_vertices, std::vector<GLuint>& out_indices);

	bool ParseObjData(std::string file, std::vector<glm::vec3>& temp_positions, std::vector<glm::vec2>& temp_uvs, std::vector<glm::vec3>& temp_normals,
		std::vector<GLuint>& vertexIndices, std::vector<GLuint>& uvIndices, std::vector<GLuint>& normalIndices);
}