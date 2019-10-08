#pragma once

// std includes
#include <map>
#include <string>
#include <memory>

// sdl
#include <sdl2/SDL.h>

// opengl
#include <glad/glad.h>

// GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

// FreeType
struct Character;

class ShaderManager;
class Shader;

struct Character {
	GLuint TextureID;   // ID handle of the glyph texture
	glm::ivec2 Size;    // Size of glyph
	glm::ivec2 Bearing;  // Offset from baseline to left/top of glyph
	GLuint Advance;    // Horizontal offset to advance to next glyph
};

class TextRenderer
{
public:
	TextRenderer(int windowWidth, int windowHeight);

	~TextRenderer();

	void RenderText(std::string text, GLfloat x, GLfloat y, GLfloat scale, glm::vec3 color);

private:

	void SetupTextShader();

	void SetupFreeText();

	std::shared_ptr<Shader> m_shader;

	int m_width, m_height;

	GLuint m_vao, m_vbo;

	std::map<GLchar, Character> m_characters;

	ShaderManager* m_shaderManager;

};