#include "scene/components/AnimationComponent.h"

namespace eng
{
	void AnimationComponent::Update(float deltaTime)
	{
		if (!currentClip || !bIsPlaying) //If no clip or nothing is playing then returns
		{
			return;
		}

		time += deltaTime; //Increments the time

		if (time > currentClip->duration) //Logic for ending the clip is here
		{
			if (bIsLooping) //If its looping it
			{
				time = std::fmod(time, currentClip->duration); //Keeps time within the duration. if time becomes 3.12 and duration is 3.0 time is set to 0.12 (prevents stutter)
			}
			else //Logic for ending the animation
			{
				time = 0.0f;
				bIsPlaying = false;
				return;
			}
		}

		for (auto& binding : bindings) //Loops through all bindings
		{
			GameObject* obj = binding.first;
			const auto& indices = binding.second->trackIndices;

			for (size_t idx : indices)
			{
				const TransformTrack& track = currentClip->tracks[idx];

				//Evaluation/break down of track information
				if (!track.positionKeys.empty())
				{
					glm::vec3 posUpdate = Interpolate(track.positionKeys, time);
					obj->SetPosition(posUpdate);
				}
				if (!track.rotationKeys.empty())
				{
					glm::quat rotUpdate = Interpolate(track.rotationKeys, time);
					obj->SetRotation(rotUpdate);
				}
				if (!track.scaleKeys.empty())
				{
					glm::vec3 scaleUpdate = Interpolate(track.scaleKeys, time);
					obj->SetScale(scaleUpdate);
				}
			}
		}
	}

	void AnimationComponent::SetClip(AnimationClip* clip)
	{

	}

	void AnimationComponent::RegisterClip(const std::string& name, const std::shared_ptr<AnimationClip>& clip)
	{

	}

	void AnimationComponent::Play(const std::string& name, bool loop)
	{

	}

	void AnimationComponent::BuildBindings()
	{

	}


	glm::vec3 AnimationComponent::Interpolate(const std::vector<KeyFrameVec3>& keys, float time) const
	{
		if (keys.empty())
		{
			return glm::vec3(0.0f); //If the keys are empty the default is returned
		}
		if (keys.size() == 1)
		{
			return keys[0].value; //If key only contains one key then returns that
		}

		if (time <= keys.front().time) //Less than or equal to the first time key, returns the first key value
		{
			return keys.front().value;
		}

		if (time >= keys.back().time) //Greater than the last time key, returns the last key value
		{
			return keys.back().value;
		}
		//If its within the range or keys times (greater than front, less than back)

		size_t prevKey = 0;
		size_t currKey = 0;

		for (size_t index = 0; index < keys.size(); index++) //Loops through all keys and gets their time value. If time is less than or equal to key time then that is the right frame index
		{
			if (time <= keys[index].time)
			{
				currKey = index;
				break;
			}
		}

		prevKey = currKey > 0 ? currKey - 1 : 0; //Previous key == currentKey - 1 if currentKey is greater than 0. Else previous key = 0

		if (time >= keys[prevKey].time && time <= keys[currKey].time) //Time must be within this range
		{
			float deltaTime = keys[currKey].time - keys[prevKey].time;
			float lerp = (time - keys[prevKey].time) / deltaTime; //Gets the lerp value/the progress between previous and next in a 0.0-1.0 ratio (eg. value could be 0.3 which means that time is 30% of the progression between prevKey and currKey)
			
			return glm::mix(keys[prevKey].value, keys[currKey].value, lerp); //Lerps from prev to curr using the lerp value
		}

	}

	glm::quat AnimationComponent::Interpolate(const std::vector<KeyFrameQuat>& keys, float time) const
	{
		if (keys.empty())
		{
			return glm::vec3(0.0f);
		}
	}
}