#include "GameEngine.h"
#include "graphics/Renderer.h"
#include "debugbt/DebugLog.h"
#include "Scene.h"
#include "graphics/TextRenderer.h"
#include "MessageSystem.h"
#include "sound/SoundManager.h"
#include "input/Input.h"

GameEngine::GameEngine(SDL_Window* targetWindow)
	: m_window(targetWindow)
{
	DebugLog::Info("Engine initialization starting...");

#if __APPLE__
	const char* glsl_version = "#version 330";
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, SDL_GL_CONTEXT_FORWARD_COMPATIBLE_FLAG); // Always required on Mac
#else
	const char* glsl_version = "#version 330";
#endif
	// create context
	SDL_GLContext targetContext = SDL_GL_CreateContext(m_window);
	if (targetContext == NULL)
	{
		DebugLog::Error("Failed to create context");
	}
	else
	{
		DebugLog::Info("Created context");
	}

	// load OpenGL functions glad SDL
	gladLoadGLLoader(SDL_GL_GetProcAddress);

	GLint vpWidth, vpHeight;
	SDL_GL_GetDrawableSize(m_window, &vpWidth, &vpHeight);
	glViewport(0, 0, vpWidth, vpHeight);

	// text rendering (NOTE: must be created before any buffers!)
	textRenderer = new TextRenderer(vpWidth, vpHeight);

	// 3d rendering
	renderer = new Renderer(&targetContext, vpWidth, vpHeight);

	// Setup Dear ImGui context
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	m_io = ImGui::GetIO(); (void)m_io;
	//io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard; // Enable keyboard controls

	// Setup Dear ImGui style
	ImGui::StyleColorsDark();

	// Setup Platform/Renderer bindings
	ImGui_ImplSDL2_InitForOpenGL(m_window, targetContext);
	ImGui_ImplOpenGL3_Init(glsl_version);

	m_lastFrameTime = SDL_GetTicks();

	// Instantiate the physics engine.
	m_physEngine = std::make_unique<PhysicsEngine>();

	// Instantiate the sound manager
	SoundManager::getInstance();

	DebugLog::Info("Engine initialization completed!");
}

GameEngine::~GameEngine()
{
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplSDL2_Shutdown();
	ImGui::DestroyContext();

	delete renderer;
}

void GameEngine::HandleEvent(SDL_Event& _event)
{
	if (SDL_PollEvent(&_event))
	{
		Input::GetInstance().HandleInput(_event);
		ImGui_ImplSDL2_ProcessEvent(&_event);
	}
}

void GameEngine::Update()
{
	// Start the Dear ImGui frame
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplSDL2_NewFrame(m_window);
	ImGui::NewFrame();

	UpdateFPSCounter();

	// Update the message system before updating anything else.
	MessageSystem::ProcessAllMessages(m_currentScene.get());

	m_currentScene->Update(m_deltaTime);

	m_physEngine->Update();
}

void GameEngine::Draw()
{

	renderer->Render(*m_currentScene);

	textRenderer->RenderScreenText(*m_currentScene);

	// ImGui Rendering
	ImGui::Render();
	glViewport(0, 0, (int)m_io.DisplaySize.x, (int)m_io.DisplaySize.y);
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

	// swap buffer
	SDL_GL_SwapWindow(m_window);
}

void GameEngine::SetScene(Scene* scene)
{
	renderer->SetupLighting(*scene);
	m_currentScene = std::unique_ptr<Scene>(scene);
}

PhysicsEngine *GameEngine::GetPhysicsEngine() const
{
	if (m_physEngine != nullptr)
		return m_physEngine.get();

	return nullptr;
}

void GameEngine::UpdateFPSCounter()
{
	// delta time
	Uint32 deltaMS = SDL_GetTicks() - m_lastFrameTime;
	m_deltaTime = (float)deltaMS / 1000.0f;

	// update time tracker/counters
	m_lastFrameTime = SDL_GetTicks();
	m_frameRateUpdateCounter += deltaMS;
	m_frameCounter++;

	// update frame rate every couple millisec
	if (m_frameRateUpdateCounter > 1000)
	{
		// calculate frame rate
		m_frameRate = (int)((float)m_frameCounter * ((float)m_frameRateUpdateCounter / 1000.0f));
		m_frameRateUpdateCounter = 0;
		m_frameCounter = 0;

		// update window title with frame rate
		string newWindowTitle = "Blue Tiles Engine [FPS:" + std::to_string(m_frameRate) + "]";
		SDL_SetWindowTitle(m_window, newWindowTitle.c_str());
	}
}
