#pragma once

#include <glm/glm.hpp>
#include <vector>
#include <memory>
#include <utility>
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
	void PlayAnimation(std::shared_ptr<Animation> animation);
	void PlayAnimation(std::string animationName);

	// Stops the currently playing animation.
	void StopAnimation();

	// Adds an animation to the animation map.
	void AddAnimation(std::shared_ptr<Animation> animation);

	// Weak pointer to the animated mesh behaviour.
	std::weak_ptr<AnimatedMesh> animatedMesh;

	// The current animation to play.
	std::shared_ptr<Animation> currentAnimation;

	// The current time in the animation.
	float animationTime;

	// Overridden functions.
	void Update(float deltaTime) override;
	void Draw(Shader& shader) override;
	bool HandleMessage(unsigned int senderID, std::string& message) override;
	void OnCollisionStay(GLuint other) override;

private:
	std::unordered_map<std::string, std::shared_ptr<Animation>> m_animations;

	// Calculates and returns the current pose that the animated mesh should be in.
	// Interpolates between the previous and next keyframes.
	std::unordered_map<std::string, glm::mat4> calculateCurrentPose();

	// Recursively calculates and sets the animation matrices for each joint.
	// I believe this is a Forward Kinematics implementation.
	void applyPoseToJoints(std::unordered_map<std::string, glm::mat4>& currentPose, Joint& joint, glm::mat4& parentTransform);

	// Gets the previous and next keyframes in the animation.
	std::pair<KeyFrame, KeyFrame> getPreviousAndNextFrames();
	
	// Calculates how far between two frames the animation is on currently.
	// Returns a value between 0 and 1.
	float calculateProgression(KeyFrame& previousFrame, KeyFrame& nextFrame);

	// Calculates and returns an interpolated pose between the previous and next keyframe.
	std::unordered_map<std::string, glm::mat4> interpolatePoses(KeyFrame& previousFrame, KeyFrame& nextFrame, float progression);
};