#include "MeshManager.h"
#include "../engine/debugbt/DebugLog.h"
#include "FileManager.h"
#include <sstream>
#include <cstdio>
#include <map>

namespace meshmanager
{
	std::map<std::string, Mesh> cachedObjs;

	/*
	Code used from:
	https://gamedev.stackexchange.com/a/104074
	Lucidelve
	*/
	bool LoadObj(const std::string filePath, std::vector<Vertex>& out_vertices, std::vector<GLuint>& out_indices)
	{
		std::string file = filemanager::LoadFile(filePath);
		if (file.empty()) return false;

		if (cachedObjs.find(filePath) != cachedObjs.end())
		{
			std::ostringstream oss;
			oss << filePath << " is cached, reloading.";
			DebugLog::Info(oss.str());
			out_vertices = std::vector<Vertex>(cachedObjs[filePath].vertices);
			out_indices = std::vector<GLuint>(cachedObjs[filePath].indices);
			return true;
		}
		else
		{
			std::ostringstream oss;
			oss << filePath << " is uncached, caching it..";
			DebugLog::Info(oss.str());
		}

		std::vector<GLuint> vertexIndices, uvIndices, normalIndices;
		std::vector<glm::vec3> temp_positions;
		std::vector<glm::vec2> temp_uvs;
		std::vector<glm::vec3> temp_normals;

		std::istringstream iss(file);

		std::string currentLine;
		int lineNumber = 0;
		while (std::getline(iss, currentLine))
		{
			lineNumber++;
			std::string directive;

			std::istringstream lineSS(currentLine);

			lineSS >> directive;
			if (directive.compare("#") == 0)
			{
				// Ignore comments
				continue;
			}
			else if (directive.compare("v") == 0)
			{
				glm::vec3 vertex;
				lineSS >> vertex.x >> vertex.y >> vertex.z;
				temp_positions.push_back(vertex);
			}
			else if (directive.compare("vt") == 0)
			{
				glm::vec2 uv;
				lineSS >> uv.x >> uv.y;
				// OpenGL flips the UV coordinate, so we reverse it
				uv.y = 1 - uv.y;
				temp_uvs.push_back(uv);
			}
			else if (directive.compare("vn") == 0)
			{
				glm::vec3 normal;
				lineSS >> normal.x >> normal.y >> normal.z;
				temp_normals.push_back(normal);
			}
			else if (directive.compare("f") == 0)
			{
				GLuint vertexIndex[3], uvIndex[3], normalIndex[3];
				lineSS >> std::skipws >> vertexIndex[0];
				if (lineSS.fail())
				{
					std::ostringstream oss;
					oss << "File can't be read by our parser. Line: " << lineNumber << " v0";
					DebugLog::Error(oss.str());
					return false;
				}
				if (lineSS.peek() == '/')
					lineSS.ignore();
				lineSS >> uvIndex[0];
				if (lineSS.fail())
				{
					std::ostringstream oss;
					oss << "File can't be read by our parser. Line: " << lineNumber << " uv0";
					DebugLog::Error(oss.str());
					return false;
				}
				if (lineSS.peek() == '/')
					lineSS.ignore();
				lineSS >> normalIndex[0];
				if (lineSS.fail())
				{
					std::ostringstream oss;
					oss << "File can't be read by our parser. Line: " << lineNumber << " n0";
					DebugLog::Error(oss.str());
					return false;
				}
				if (lineSS.peek() == '/')
					lineSS.ignore();
				lineSS >> vertexIndex[1];
				if (lineSS.fail())
				{
					std::ostringstream oss;
					oss << "File can't be read by our parser. Line: " << lineNumber << " v1";
					DebugLog::Error(oss.str());
					return false;
				}
				if (lineSS.peek() == '/')
					lineSS.ignore();
				lineSS >> uvIndex[1];
				if (lineSS.fail())
				{
					std::ostringstream oss;
					oss << "File can't be read by our parser. Line: " << lineNumber << " uv1";
					DebugLog::Error(oss.str());
					return false;
				}
				if (lineSS.peek() == '/')
					lineSS.ignore();
				lineSS >> normalIndex[1];
				if (lineSS.fail())
				{
					std::ostringstream oss;
					oss << "File can't be read by our parser. Line: " << lineNumber << " n1";
					DebugLog::Error(oss.str());
					return false;
				}
				if (lineSS.peek() == '/')
					lineSS.ignore();
				lineSS >> vertexIndex[2];
				if (lineSS.fail())
				{
					std::ostringstream oss;
					oss << "File can't be read by our parser. Line: " << lineNumber << " v2";
					DebugLog::Error(oss.str());
					return false;
				}
				if (lineSS.peek() == '/')
					lineSS.ignore();
				lineSS >> uvIndex[2];
				if (lineSS.fail())
				{
					std::ostringstream oss;
					oss << "File can't be read by our parser. Line: " << lineNumber << " uv2";
					DebugLog::Error(oss.str());
					return false;
				}
				if (lineSS.peek() == '/')
					lineSS.ignore();
				lineSS >> normalIndex[2];
				if (lineSS.fail())
				{
					std::ostringstream oss;
					oss << "File can't be read by our parser. Line: " << lineNumber << " n2";
					DebugLog::Error(oss.str());
					return false;
				}
				vertexIndices.push_back(vertexIndex[0]);
				vertexIndices.push_back(vertexIndex[1]);
				vertexIndices.push_back(vertexIndex[2]);
				uvIndices.push_back(uvIndex[0]);
				uvIndices.push_back(uvIndex[1]);
				uvIndices.push_back(uvIndex[2]);
				normalIndices.push_back(normalIndex[0]);
				normalIndices.push_back(normalIndex[1]);
				normalIndices.push_back(normalIndex[2]);
			}
		}

		for (GLuint i = 0; i < vertexIndices.size(); i++)
		{
			unsigned int vertexIndex = vertexIndices[i];
			unsigned int uvIndex = uvIndices[i];
			unsigned int normalIndex = normalIndices[i];

			Vertex vertex;
			vertex.position = temp_positions[vertexIndex - 1];
			vertex.uv = temp_uvs[uvIndex - 1];
			vertex.normal = temp_normals[normalIndex - 1];

			GLuint hv = -1, ht = -1, hn = -1;

			/*
			// This is an optimization to prevent the re-use of vertices in the same position, TAKES FOREVER
			for (std::vector<Vertex>::iterator hvit = out_vertices.begin(); hvit != out_vertices.end(); ++hvit)
			{
				if ((*hvit).position.x == vertex.position.x &&
					(*hvit).position.y == vertex.position.y &&
					(*hvit).position.z == vertex.position.z)
				{
					hv = hvit - out_vertices.begin();
				}

				if ((*hvit).uv.x == vertex.uv.x &&
					(*hvit).uv.y == vertex.uv.y)
				{
					ht = hvit - out_vertices.begin();
				}

				if ((*hvit).normal.x == vertex.normal.x &&
					(*hvit).normal.y == vertex.normal.y &&
					(*hvit).normal.z == vertex.normal.z)
				{
					hn = hvit - out_vertices.begin();
				}
			}
			*/
			if (ht != -1 && hv == ht && hn == ht)
			{
				out_indices.push_back(hv);
			}
			else
			{
				out_vertices.push_back(vertex);
				out_indices.push_back(static_cast<GLuint>(out_vertices.size()) - 1);
			}
		}

		cachedObjs[filePath].vertices = std::vector<Vertex>(out_vertices);
		cachedObjs[filePath].indices = std::vector<GLuint>(out_indices);
		
		return true;
	}
}