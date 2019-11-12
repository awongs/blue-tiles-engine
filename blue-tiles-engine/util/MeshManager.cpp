#include <pugixml/pugixml.hpp>
#include <iostream>
#include <sstream>
#include <cstdio>
#include <algorithm>
#include <map>

#include "MeshManager.h"
#include "../engine/debugbt/DebugLog.h"
#include "FileManager.h"


namespace meshmanager
{
	std::map<std::string, Mesh> cachedObjs;
	std::map<std::pair<std::string, std::string>, AnimatedMesh> cachedObjsWithSkeleton;

	constexpr int MAX_JOINTS_PER_VERTEX = 3;

	/*
	Code used from:
	https://gamedev.stackexchange.com/a/104074
	Lucidelve
	*/
	bool LoadObj(const std::string filePath, std::vector<Vertex>& out_vertices, std::vector<GLuint>& out_indices)
	{
		std::string file = FileManager::LoadFile(filePath);
		if (file.empty()) return false;

		if (cachedObjs.find(filePath) != cachedObjs.end())
		{
			std::ostringstream oss;
			//oss << filePath << " is cached, reloading.";
			//DebugLog::Info(oss.str());
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

		if (!ParseObjData(file, temp_positions, temp_uvs, temp_normals, vertexIndices, uvIndices, normalIndices))
		{
			return false;
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
	bool LoadObjWithSkeleton(const std::string objPath, const std::string skeletonPath, std::vector<AnimatedVertex>& out_vertices, std::vector<GLuint>& out_indices)
	{
		// Check cache first.
		std::pair<std::string, std::string> pathPair = std::make_pair(objPath, skeletonPath);
		if (cachedObjsWithSkeleton.find(pathPair) != cachedObjsWithSkeleton.end())
		{
			std::ostringstream oss;
			out_vertices = std::vector<AnimatedVertex>(cachedObjsWithSkeleton[pathPair].vertices);
			out_indices = std::vector<GLuint>(cachedObjsWithSkeleton[pathPair].indices);
			return true;
		}
		else
		{
			std::ostringstream oss;
			oss << objPath << " with skeleton from " << skeletonPath << " is uncached, caching it..";
			DebugLog::Info(oss.str());
		}

		std::string objFile = FileManager::LoadFile(objPath);
		pugi::xml_document skeFile;

		// Make sure file paths are valid
		if (objFile.empty() || !skeFile.load_file(skeletonPath.c_str()))
		{
			return false;
		}

		// Parse .obj data first
		std::vector<GLuint> vertexIndices, uvIndices, normalIndices;
		std::vector<glm::vec3> temp_positions, temp_normals;
		std::vector<glm::vec2> temp_uvs;

		if (!ParseObjData(objFile, temp_positions, temp_uvs, temp_normals, vertexIndices, uvIndices, normalIndices))
		{
			return false;
		}

		// Parse .dae data next
		std::vector<glm::ivec3> temp_jointIds;
		std::vector<glm::vec3> temp_jointWeights;

		// Get controller name.
		std::string controllerName = skeFile.child("COLLADA")
			.child("library_controllers")
			.child("controller")
			.attribute("id").as_string();
		std::string controllerWeightsStr = controllerName.append("-Weights");

		// Grab joint weights from file.
		std::vector<float> weights;
		std::string weightsStr = skeFile.child("COLLADA")
			.child("library_controllers")
			.child("controller")
			.child("skin")
			.find_child_by_attribute("id", controllerWeightsStr.c_str())
			.child("float_array").text().as_string();

		// Convert weights from strings to floats.
		std::istringstream iss(weightsStr);
		while (iss)
		{
			std::string weight;
			iss >> weight;
			if (weight.length() == 0)
			{
				continue;
			}

			weights.push_back(stof(weight));
		}

		// Grab vertex counts from file.
		std::vector<int> vCounts;
		std::string vCountsStr = skeFile.child("COLLADA")
			.child("library_controllers")
			.child("controller")
			.child("skin")
			.child("vertex_weights")
			.child("vcount").text().as_string();

		// Convert vertex counts from strings to ints.
		iss = std::istringstream(vCountsStr);
		while (iss)
		{
			std::string vCount;
			iss >> vCount;
			if (vCount.length() == 0)
			{
				continue;
			}

			vCounts.push_back(stoi(vCount));
		}

		// Grab vertex weight indices from file.
		std::vector<int> vIndices;
		std::string vIndicesStr = skeFile.child("COLLADA")
			.child("library_controllers")
			.child("controller")
			.child("skin")
			.child("vertex_weights")
			.child("v").text().as_string();

		// Convert vertex weights from strings to ints.
		iss = std::istringstream(vIndicesStr);
		while (iss)
		{
			std::string vWeight;
			iss >> vWeight;
			if (vWeight.length() == 0)
			{
				continue;
			}

			vIndices.push_back(stoi(vWeight));
		}

		int vCount = 0;

		// Calculate joints and weights for each vertex.
		for (auto it = vIndices.begin(); it != vIndices.end();)
		{
			size_t numJointsAffecting = vCounts[vCount];
			glm::ivec3 jointIds;
			glm::vec3 jointWeights;

			// Get joint ids and weights.
			std::vector<std::pair<int, float>> weightList;
			for (size_t i = 0; i < numJointsAffecting; i++)
			{
				weightList.push_back(std::pair<int, float>(*(it + i * 2), weights[*(it + i * 2 + 1)]));
			}

			// Sort joint ids by weight.
			std::sort(weightList.begin(), weightList.end(),
				[](const std::pair<int, float>& a, const std::pair<int, float>& b) -> bool
				{
					return a.second > b.second;
				}
			);
			
			for (size_t i = 0; i < numJointsAffecting && i < MAX_JOINTS_PER_VERTEX; i++)
			{
				jointIds[i] = weightList[i].first;
				jointWeights[i] = weightList[i].second;
			}

			// Fill remaining weights with zeros if numJoints is less than the maximum.
			for (int i = numJointsAffecting; i < MAX_JOINTS_PER_VERTEX; i++)
			{
				jointIds[i] = 0;
				jointWeights[i] = 0.0f;
			}

			// Normalize the weights.
			float totalWeight = jointWeights.x + jointWeights.y + jointWeights.z;
			jointWeights.x /= totalWeight;
			jointWeights.y /= totalWeight;
			jointWeights.z /= totalWeight;

			// TODO: Unnecessary copying
			temp_jointIds.push_back(jointIds);
			temp_jointWeights.push_back(jointWeights);

			it += numJointsAffecting * 2;
			vCount++;
		}

		// Construct vertices and indices.
		for (GLuint i = 0; i < vertexIndices.size(); i++)
		{
			unsigned int vertexIndex = vertexIndices[i];
			unsigned int uvIndex = uvIndices[i];
			unsigned int normalIndex = normalIndices[i];
			unsigned int jointIdIndex = vertexIndex;
			unsigned int jointWeightIndex = vertexIndex;

			AnimatedVertex vertex;
			vertex.position = temp_positions[vertexIndex - 1];
			vertex.uv = temp_uvs[uvIndex - 1];
			vertex.normal = temp_normals[normalIndex - 1];
			vertex.jointId = temp_jointIds[jointIdIndex - 1];
			vertex.jointWeight = temp_jointWeights[jointWeightIndex - 1];

			out_vertices.push_back(vertex);
			out_indices.push_back(static_cast<GLuint>(out_vertices.size()) - 1);
		}

		cachedObjsWithSkeleton[pathPair].vertices = std::vector<AnimatedVertex>(out_vertices);
		cachedObjsWithSkeleton[pathPair].indices = std::vector<GLuint>(out_indices);

		return true;
	}

	bool ParseObjData(std::string file, std::vector<glm::vec3>& temp_positions, std::vector<glm::vec2>& temp_uvs, std::vector<glm::vec3>& temp_normals, 
		std::vector<GLuint>& vertexIndices, std::vector<GLuint>& uvIndices, std::vector<GLuint>& normalIndices)
	{
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

		return true;
	}
}