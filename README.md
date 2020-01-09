# Blue Tiles Game Engine
This repository contains code for a group project that was done during the Game Development bachelor program at BCIT. 

The purpose of this project is to demonstrate game development fundamentals by writing a game engine from scratch along with a simple game to showcase the engine's functionalities.

The project was developed by the following people during the 2019 Fall Term of the Game Development program:
* Jason C.
* Jackee M.
* Ricky M.
* Henry L.
* Davis P.
* Anthony W.

## Steal-th Game

Steal-th is a top-down stealth game, where the player navigates through puzzle-based levels while avoiding enemy guards. The game’s primary target platform is Windows.

The overall game is designed to support a set of levels. Within each level, the player’s objective
is to find and pick up the “technology item”. Once the player has picked up this objective item,
then the level’s exit is unlocked. Then, the level ends when the player reaches the exit. The
player’s score will be calculated based on how quickly the player completes the level. Finishing
the level more quickly will result in a higher score. Scores are calculated on a per-level basis.

If the player is detected by the guard, then the game instantly ends with a failure state. There is
no score given for failing to complete the level. In this case, the player must restart the level.
