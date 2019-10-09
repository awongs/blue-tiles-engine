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

class ShaderManager;
class Shader;
class Scene;

struct Character {
	GLuint TextureID;   // ID handle of the glyph texture
	glm::ivec2 Size;    // Size of glyph
	glm::ivec2 Bearing;  // Offset from baseline to left/top of glyph
	GLuint Advance;    // Horizontal offset to advance to next glyph
};

// based on https://learnopengl.com/In-Practice/Text-Rendering
class TextRenderer
{
public:
	// Creates a text render and uses the width and height to create the correct perspective
	TextRenderer(int windowWidth, int windowHeight);

	// Deconstructor
	~TextRenderer();

	// Renders text to the screen at the x and y position 
	void RenderText(std::string text, GLfloat x, GLfloat y, GLfloat scale, glm::vec3& color);

	// Renders text to the screen at the x and y position
	void RenderText(std::string text, glm::vec3& pos, GLfloat scale, glm::vec3& color);

	// Renders text to the screen at the x and y position
	void RenderText(std::string text, glm::vec2& pos, GLfloat scale, glm::vec3& color);

	// Renders text with all of the TextBehaviours in the scene 
	void RenderScreenText(Scene& scene);

private:

	// Sets up the text shader
	void SetupTextShader();

	// Sets up the free text library stuff
	void SetupFreeText();

	// Pointed to a shader object
	std::shared_ptr<Shader> m_shader;

	// Width and height of the window
	int m_width, m_height;

	// ID to the VAO buffer
	GLuint m_vao;

	// ID to the VBO buffer
	GLuint m_vbo;

	// Map/Dictionary of character structures
	std::map<GLchar, Character> m_characters;

	// Shader manager used to compile the shaders
	ShaderManager* m_shaderManager;

};