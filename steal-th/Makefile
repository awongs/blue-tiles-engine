#OBJS All c or cpp files
OBJS =  Main.cpp Level.cpp ../blue-tiles-engine/glad.c scenes/LevelScene.cpp behaviours/PlayerMovement.cpp behaviours/FollowGameObject.cpp behaviours/Inventory.cpp behaviours/SimpleGuardMovementAIBehaviour.cpp behaviours/TileBehaviour.cpp behaviours/ObjectBehaviour.cpp behaviours/PlayerItemPickup.cpp behaviours/GuardDetection.cpp  

#CC Which Compiler to use.
CC = g++

#COMPILER_FLAGS Other include files
# -w suppresses all warnings
COMPILER_FLAGS = -I../libraries/includes -I../blue-tiles-engine/ -L../libraries/libs 

#LINKER_FLAGS Specifies the libraries to link against
LINKER_FLAGS = -lSDL2 -lSDL2_mixer -lSDL2_image -lGL -ldl -lGL -lfreetype -lOpenCL

#OBJ_NAME The name of the output file
OBJ_NAME = blue-engine

#The Make commands
all : $(OBJS)
	$(CC) $(OBJS) $(COMPILER_FLAGS)  blue-tiles-engine.a $(LINKER_FLAGS) -o $(OBJ_NAME)

clean:	
	rm blue-engine
