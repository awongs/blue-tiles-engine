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

void Animator::StartAnimation(Animation* animation)
{
	animationTime = 0.0f;
	currentAnimation = std::unique_ptr<Animation>(animation);
}

void Animator::Update(float deltaTime)
{
	// Only update if there is an animation.
	if (currentAnimation == nullptr)
	{
		return;
	}

	// Increase animation time.
	animationTime += deltaTime;
	// TODO: probably make this while loop instead
	if (animationTime > currentAnimation->length) 
	{
		animationTime -= currentAnimation->length;
	}

	std::unordered_map<std::string, glm::mat4> currentPose = calculateCurrentPose();
	applyPoseToJoints(currentPose, animatedMesh.lock()->rootJoint, glm::mat4(1));
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

std::unordered_map<std::string, glm::mat4> Animator::calculateCurrentPose()
{
	std::vector<KeyFrame> keyFrames = getPreviousAndNextFrames();
	float progression = calculateProgression(keyFrames[0], keyFrames[1]);
	return interpolatePoses(keyFrames[0], keyFrames[1], progression);
}

void Animator::applyPoseToJoints(std::unordered_map<std::string, glm::mat4> currentPose, Joint& joint, glm::mat4 parentTransform)
{
	glm::mat4 currentLocalTransform = currentPose.at(joint.name);
	glm::mat4 currentTransform = parentTransform * currentLocalTransform;
	for (std::shared_ptr<Joint>& childJoint : joint.children)
	{
		applyPoseToJoints(currentPose, *childJoint, currentTransform);
	}
	currentTransform = currentTransform * joint.inverseBindTransform;
}

std::vector<KeyFrame> Animator::getPreviousAndNextFrames()
{
	std::vector<KeyFrame>& allFrames = currentAnimation->keyFrames;
	KeyFrame& previousFrame = allFrames[0];
	KeyFrame& nextFrame = allFrames[0];

	// TODO: Keep track of current frame instead of looping like this.
	for (int i = 1; i < allFrames.size(); i++)
	{
		nextFrame = allFrames[i];
		if (nextFrame.timeStamp > animationTime)
		{
			break;
		}
		previousFrame = allFrames[i];
	}

	// TODO: Change this function to return a pair instead.
	return std::vector<KeyFrame> { previousFrame, nextFrame };
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
