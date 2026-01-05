#pragma once
#include "scene/Component.h"
#include "scene/GameObject.h"
#include <glm/vec3.hpp>
#include <glm/gtc/quaternion.hpp>
#include <string>
#include <vector>
#include <memory>
#include <unordered_map>

namespace eng
{
	struct KeyFrameVec3 //Used for Vec3 animations (scale and position)
	{
		float time = 0.0f;
		glm::vec3 value = glm::vec3(0.0f);
	};

	struct KeyFrameQuat //Used for quaternion animations (rotation)
	{
		float time = 0.0f;
		glm::quat value = glm::quat(1, 0, 0, 0); //w,x,y,z
	};

	struct TransformTrack //Stores an object name and collection of keyframe structs. This is like a track in other animation systems
	{
		std::string targetName; //Name of the gameobject
		std::vector<KeyFrameVec3> positionKeys; //position keyframes
		std::vector<KeyFrameQuat> rotationKeys; //Rotation keyframes
		std::vector<KeyFrameVec3> scaleKeys; //Scale keyframes
	};

	struct AnimationClip //A collection of transform tracks. Allows for an animation where multiple gameobjects are animated at once. Also allows looping and stores animation duration
	{
		std::string name; //Name of the animation
		float duration = 0.0f; //Duration of animation
		bool bLooping = true; //If the animation clips loops until asked to stop
		std::vector<TransformTrack> tracks; //A collection of transformation tracks that make up the whole animation sequence
	};

	struct ObjectBinding
	{
		GameObject* object = nullptr; //The object to animate 
		std::vector<size_t> trackIndices; //Indices into AnimationClip::tracks
	};

	class AnimationComponent : public Component
	{
		COMPONENT(AnimationComponent)

	public:
		void Update(float deltaTime) override;
		void SetClip(AnimationClip* clip);
		void RegisterClip(const std::string& name, const std::shared_ptr<AnimationClip>& clip);
		void Play(const std::string& name, bool loop = false); //Defaults looping to false

		bool IsPlaying() const;

	private:
		//Current playback state
		AnimationClip* currentClip = nullptr;
		float time = 0.0f;
		bool bIsLooping = false;
		bool bIsPlaying = false;

		//All clips loaded from the objects Gltf
		std::unordered_map<std::string, std::shared_ptr<AnimationClip>> clips;

		//Target-object bindings for current clip
		std::unordered_map<GameObject*, std::unique_ptr<ObjectBinding>> bindings;

		//Helpers to rebuild binders from clips
		void BuildBindings();

		//Interpolation Helpers (Helps with making smoother motion between frames/animation keys)
		glm::vec3 Interpolate(const std::vector<KeyFrameVec3>& keys, float time) const;
		glm::quat Interpolate(const std::vector<KeyFrameQuat>& keys, float time) const;

		


	};
}