#pragma once

#include <glm/glm.hpp>
#include <vector>
#include <memory>
#include <unordered_map>

#include "../behaviours/Behaviour.h"
#include "KeyFrame.h"

class AnimatedMesh;
class Animation;
class Joint;

// Code referenced from: https://www.youtube.com/watch?v=f3Cr8Yx3GGA
class Animator : public Behaviour
{
public:
	// Constructor.
	Animator(std::weak_ptr<AnimatedMesh> animatedMesh);

	// Starts the animation.
	void StartAnimation(Animation* animation);

	// Overridden functions.
	void Update(float deltaTime) override;
	void Draw(Shader& shader) override;
	bool HandleMessage(unsigned int senderID, std::string& message) override;
	void OnCollisionStay(GLuint other) override;

	// Weak pointer to the animated mesh behaviour.
	std::weak_ptr<AnimatedMesh> animatedMesh;

	// The current animation to play.
	std::unique_ptr<Animation> currentAnimation;

	std::unordered_map<std::string, glm::mat4> currentPose;

	// The current time in the animation.
	float animationTime;

private:
	// Calculates and returns the current pose that the animated mesh should be in.
	// Interpolates between the previous and next keyframes.
	std::unordered_map<std::string, glm::mat4> calculateCurrentPose();
	void applyPoseToJoints(std::unordered_map<std::string, glm::mat4>& currentPose, Joint& joint, glm::mat4 parentTransform);
	std::vector<KeyFrame> getPreviousAndNextFrames();
	float calculateProgression(KeyFrame& previousFrame, KeyFrame& nextFrame);
	std::unordered_map<std::string, glm::mat4> interpolatePoses(KeyFrame& previousFrame, KeyFrame& nextFrame, float progression);

};