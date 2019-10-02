#OBJS All c or cpp files
OBJS =  Main.cpp glad.c engine/GameEngine.cpp engine/graphics/ShaderManager.cpp engine/debugbt/DebugLog.cpp engine/input/Input.cpp  engine/graphics/Renderer.cpp util/FileManager.cpp engine/sound/Sound.cpp engine/MessageSystem.cpp engine/behaviours/Behaviour.cpp engine/behaviours/MeshRenderer.cpp engine/Scene.cpp engine/graphics/Camera.cpp engine/GameObject.cpp engine/graphics/Shader.cpp engine/sound/Music.cpp engine/sound/SoundManager.cpp engine/GameWindow.cpp engine/graphics/Texture.cpp engine/physics/Collider.cpp engine/physics/PhysicsEngine.cpp engine/physics/PhysicsObject.cpp engine/physics/SphereCollider.cpp util/MeshManager.cpp    

#CC Which Compiler to use.
CC = g++

#COMPILER_FLAGS Other include files
# -w suppresses all warnings
COMPILER_FLAGS = -I../libraries/includes -Llibraries/libs

#LINKER_FLAGS Specifies the libraries to link against
LINKER_FLAGS = -lSDL2 -lSDL2_mixer -lSDL2_image -lGL -ldl -lGL -lX11 -lpthread -lXrandr -lXi

#OBJ_NAME The name of the output file
OBJ_NAME = blue-engine

#The Make commands
all : $(OBJS)
	$(CC) $(OBJS) $(COMPILER_FLAGS) $(LINKER_FLAGS) -o $(OBJ_NAME)

clean:
	rm blue-engine
