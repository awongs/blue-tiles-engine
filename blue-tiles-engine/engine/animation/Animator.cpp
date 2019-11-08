#include "Animator.h"
#include "Animation.h"
#include "AnimatedMesh.h"
#include "JointTransform.h"
#include "Joint.h"

Animator::Animator(std::weak_ptr<AnimatedMesh> animatedMesh)
	: Behaviour(BehaviourType::Animator)
	, animatedMesh(animatedMesh)
	, animationTime(0.0f)
{
}

void Animator::PlayAnimation(std::shared_ptr<Animation> animation)
{
	if (animation == nullptr 
		|| (currentAnimation != nullptr && currentAnimation->name == animation->name))
	{
		return;
	}

	animationTime = 0.0f;
	currentAnimation = animation;
}

void Animator::PlayAnimation(std::string animationName)
{
	if (m_animations.count(animationName) == 0 
		|| (currentAnimation != nullptr && currentAnimation->name == animationName))
	{
		return;
	}

	animationTime = 0.0f;
	currentAnimation = m_animations.at(animationName);
}

void Animator::AddAnimation(std::shared_ptr<Animation> animation)
{
	// Don't add duplicate animations.
	if (animation == nullptr || m_animations.count(animation->name) != 0)
	{
		return;
	}

	m_animations[animation->name] = animation;
}

void Animator::StopAnimation()
{
	if (currentAnimation == nullptr)
	{
		return;
	}

	animationTime = 0.0f;
	currentAnimation = nullptr;
}

void Animator::Update(float deltaTime)
{
	// Only update if there is an animation.
	if (currentAnimation == nullptr)
	{
		return;
	}

	// Increase animation time, looping back when it exceeds the animation length.
	animationTime += deltaTime;
	while (animationTime > currentAnimation->length) 
	{
		animationTime -= currentAnimation->length;
	}

	// Calculate the current pose and apply it to all joints.
	std::unordered_map<std::string, glm::mat4> currentPose = calculateCurrentPose();
	glm::mat4 identity = glm::mat4(1);
	applyPoseToJoints(currentPose, *(animatedMesh.lock()->rootJoint), identity);
}

std::unordered_map<std::string, glm::mat4> Animator::calculateCurrentPose()
{
	// Get the previous and next keyframes.
	std::pair<KeyFrame, KeyFrame> keyFrames = getPreviousAndNextFrames();

	// Calculate and return an interpolated pose between the two keyframes.
	float progression = calculateProgression(keyFrames.first, keyFrames.second);
	return interpolatePoses(keyFrames.first, keyFrames.second, progression);
}

void Animator::applyPoseToJoints(std::unordered_map<std::string, glm::mat4>& currentPose, Joint& joint, glm::mat4& parentTransform)
{
	// Check if this joint is in the animation pose.
	if (currentPose.find(joint.name) == currentPose.end())
	{
		// Joint is not in animation pose, so just inherit from parent.
		joint.animatedTransform = parentTransform * joint.localBindTransform * joint.inverseBindTransform;
		return;
	}
	
	// Multiply current pose transform with parent and inverse to get the animated transform.
	glm::mat4& currentLocalTransform = currentPose.at(joint.name);
	glm::mat4 currentTransform = parentTransform * currentLocalTransform;
	joint.animatedTransform = currentTransform * joint.inverseBindTransform;

	// Recursively call this function on all children joints.
	for (std::shared_ptr<Joint>& childJoint : joint.children)
	{
		applyPoseToJoints(currentPose, *childJoint, currentTransform);
	}
}

std::pair<KeyFrame, KeyFrame> Animator::getPreviousAndNextFrames()
{
	std::vector<KeyFrame>& allFrames = currentAnimation->keyFrames;
	size_t frameIndex = animationTime / (currentAnimation->length / allFrames.size());

	return std::pair<KeyFrame, KeyFrame> { allFrames[frameIndex], allFrames[frameIndex + 1] };
}

float Animator::calculateProgression(KeyFrame& previousFrame, KeyFrame& nextFrame)
{
	
	float totalTime = nextFrame.timeStamp - previousFrame.timeStamp;
	float currentTime = animationTime - previousFrame.timeStamp;
	return currentTime / totalTime;
}

std::unordered_map<std::string, glm::mat4> Animator::interpolatePoses(KeyFrame& previousFrame, KeyFrame& nextFrame, float progression)
{
	std::unordered_map<std::string, glm::mat4> currentPose;

	for (auto it = previousFrame.pose.begin(); it != previousFrame.pose.end(); it++)
	{
		// TODO: Lazy copying
		JointTransform previousTransform = it->second;
		JointTransform nextTransform = nextFrame.pose.at(it->first);
		JointTransform currentTransform = JointTransform::Interpolate(previousTransform, nextTransform, progression);
		
		currentPose.insert(std::pair<std::string, glm::mat4>(it->first, currentTransform.GetLocalTransform()));
	}
	return currentPose;
}

void Animator::Draw(Shader& shader)
{
}

bool Animator::HandleMessage(unsigned int senderID, std::string& message)
{
	return false;
}

void Animator::OnCollisionStay(GLuint other)
{
}
