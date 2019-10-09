#include <ft2build.h>
#include FT_FREETYPE_H

#include "TextRenderer.h"
#include "ShaderManager.h"
#include "Shader.h"
#include "../../util/FileManager.h"
#include "../debugbt/DebugLog.h"
#include "../Scene.h"
#include "../GameObject.h"
#include "../behaviours/TextBehaviour.h"

TextRenderer::TextRenderer(int windowWidth, int windowHeight)
	: m_width(windowWidth)
	, m_height(windowHeight)
{
	DebugLog::Info("Setting up TextRenderer...");

	m_shaderManager = new ShaderManager();

	SetupTextShader();

	SetupFreeText();
}

TextRenderer::~TextRenderer()
{
	delete m_shaderManager;
}

void TextRenderer::RenderText(std::string text, GLfloat x, GLfloat y, GLfloat scale, glm::vec3& color)
{
	glEnable(GL_CULL_FACE);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	// Activate corresponding render state	
	m_shaderManager->UseShaderProgram(m_shader->GetProgramHandle());
	m_shader->SetUniform3f("textColor", color);

	glActiveTexture(GL_TEXTURE0);
	glBindVertexArray(m_vao);

	// Iterate through all characters
	std::string::const_iterator c;
	for (c = text.begin(); c != text.end(); c++)
	{
		Character ch = m_characters[*c];

		GLfloat xpos = x + ch.Bearing.x * scale;
		GLfloat ypos = y - (ch.Size.y - ch.Bearing.y) * scale;

		GLfloat w = ch.Size.x * scale;
		GLfloat h = ch.Size.y * scale;
		// Update VBO for each character
		GLfloat vertices[6][4] = {
			{ xpos,     ypos + h,   0.0, 0.0 },
			{ xpos,     ypos,       0.0, 1.0 },
			{ xpos + w, ypos,       1.0, 1.0 },

			{ xpos,     ypos + h,   0.0, 0.0 },
			{ xpos + w, ypos,       1.0, 1.0 },
			{ xpos + w, ypos + h,   1.0, 0.0 }
		};
		// Render glyph texture over quad
		glBindTexture(GL_TEXTURE_2D, ch.TextureID);

		// Update content of VBO memory
		glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
		glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices); // Be sure to use glBufferSubData and not glBufferData

		glBindBuffer(GL_ARRAY_BUFFER, 0);
		
		// Render quad
		glDrawArrays(GL_TRIANGLES, 0, 6);
		
		// Now advance cursors for next glyph (note that advance is number of 1/64 pixels)
		x += (ch.Advance >> 6) * scale; // Bitshift by 6 to get value in pixels (2^6 = 64 (divide amount of 1/64th pixels by 64 to get amount of pixels))
	}
	glBindVertexArray(0);
	glBindTexture(GL_TEXTURE_2D, 0);

	glDisable(GL_CULL_FACE);
	glDisable(GL_BLEND);
}

void TextRenderer::RenderText(std::string text, glm::vec3& pos, GLfloat scale, glm::vec3& color)
{
	RenderText(text, pos.x, pos.y, scale, color);
}

void TextRenderer::RenderText(std::string text, glm::vec2& pos, GLfloat scale, glm::vec3& color)
{
	RenderText(text, pos.x, pos.y, scale, color);
}

void TextRenderer::RenderScreenText(Scene& scene)
{
	for (const std::unique_ptr<GameObject>& sceneObject : scene.getScreenGameObject())
	{
		TextBehaviour* behav = static_cast<TextBehaviour*>(sceneObject->GetBehaviour(BehaviourType::TextBehaviour));
		
		if (behav == nullptr) continue;

		behav->DrawTextWithRenderer(this);
	}
}

void TextRenderer::SetupTextShader()
{
	DebugLog::Info("Compiling shader program for TextRenderer...");

	// id of shader programs
	GLuint vertexShader;
	GLuint fragmentShader;

	// file
	std::string vertex = filemanager::LoadFile("../Assets/shaders/TextShader.vsh");
	std::string fragment = filemanager::LoadFile("../Assets/shaders/TextShader.fsh");

	// shader compiling
	vertexShader = m_shaderManager->CompileShader(GL_VERTEX_SHADER, vertex.c_str());
	fragmentShader = m_shaderManager->CompileShader(GL_FRAGMENT_SHADER, fragment.c_str());

	// shader program
	m_shader = m_shaderManager->CreateShaderProgram(vertexShader, fragmentShader);

	// set projeection for rendering
	glm::mat4 projection = glm::ortho(0.0f, static_cast<GLfloat>(m_width), 0.0f, static_cast<GLfloat>(m_height));
	m_shader->SetUniformMatrix4fv("projection", projection);
	
}

void TextRenderer::SetupFreeText()
{	
	// FreeType
	FT_Library ft;
	// All functions return a value different than 0 whenever an error occurred
	if (FT_Init_FreeType(&ft))
	{
		DebugLog::Info("ERROR::FREETYPE: Could not init FreeType Library");
	}

	// Load font as face
	FT_Face face;
	if (FT_New_Face(ft, "../Assets/fonts/OpenSans-Regular.ttf", 0, &face))
	{
		DebugLog::Info("ERROR::FREETYPE: Failed to load font");
	}

	// Set size to load glyphs as
	FT_Set_Pixel_Sizes(face, 0, 48);

	// Disable byte-alignment restriction
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

	DebugLog::Info("Generating 128 ascii characters for TextRenderer...");

	// Load first 128 characters of ASCII set
	for (GLubyte c = 0; c < 128; c++)
	{
		// Load character glyph 
		if (FT_Load_Char(face, c, FT_LOAD_RENDER))
		{
			DebugLog::Info("ERROR::FREETYTPE: Failed to load Glyph " + std::to_string(c));
			continue;
		}

		// Generate texture
		GLuint texture;
		glGenTextures(1, &texture);
		glBindTexture(GL_TEXTURE_2D, texture);
		glTexImage2D(
			GL_TEXTURE_2D,
			0,
			GL_RED,
			face->glyph->bitmap.width,
			face->glyph->bitmap.rows,
			0,
			GL_RED,
			GL_UNSIGNED_BYTE,
			face->glyph->bitmap.buffer
		);

		// Set texture options
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		// Now store character for later use
		Character character = {
			texture,
			glm::ivec2(face->glyph->bitmap.width, face->glyph->bitmap.rows),
			glm::ivec2(face->glyph->bitmap_left, face->glyph->bitmap_top),
			face->glyph->advance.x
		};
		m_characters.insert(std::pair<GLchar, Character>(c, character));
	}
	glBindTexture(GL_TEXTURE_2D, 0);

	// Destroy FreeType once we're finished
	FT_Done_Face(face);
	FT_Done_FreeType(ft);

	DebugLog::Info("Generating buffers for TextRenderer...");

	// Configure VAO/VBO for texture quads
	glGenVertexArrays(1, &m_vao);
	glGenBuffers(1, &m_vbo);
	glBindVertexArray(m_vbo);
	glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 6 * 4, NULL, GL_DYNAMIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), 0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}