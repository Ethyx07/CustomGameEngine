#include "AudioComponent.h"
#include "scene/GameObject.h"

namespace eng
{
	void AudioComponent::LoadProperties(const nlohmann::json& json)
	{
		if (json.contains("audio")) //Ensures component contains a audio json field
		{
			auto audioArray = json["audio"];
			for (auto& audioData : audioArray) //Loops through array of audios, gets path, name and volume. Adds them to the audio registery
			{
				std::string name = audioData.value("name", "noname");
				std::string path = audioData.value("path", "");
				float volume = audioData.value("volume", 1.0f);
				auto audio = Audio::Load(path);
				if (audio)
				{
					audio->SetVolume(volume);
					RegisterAudio(name, audio);
				}
			}
		}
	}

	void AudioComponent::Update(float deltaTime) //Every tick it sets the position of the clip to where the component is (only when playing)
	{
		auto pos = owner->GetWorldPosition();
		for (auto& clip : audioClips)
		{
			if (clip.second->IsPlaying())
			{
				clip.second->SetPosition(pos);
			}
		}
	}

	void AudioComponent::RegisterAudio(const std::string& name, const std::shared_ptr<Audio>& audio)
	{
		audioClips[name] = audio;
	}

	void AudioComponent::Play(const std::string& name, bool loop)
	{
		auto iterator = audioClips.find(name); //Checks if the name for the audio clip is within the registered list of clips, playing it and passing the loop bool to it
		if (iterator != audioClips.end())
		{
			if (iterator->second)
			{
				iterator->second->Play(loop);
			}
		}
	}

	void AudioComponent::Stop(const std::string& name)
	{
		auto iterator = audioClips.find(name); //Checks if the name for the audio clip is within the registered list of clips, stopping it
		if (iterator != audioClips.end())
		{
			if (iterator->second)
			{
				iterator->second->Stop();
			}
		}
	}

	bool AudioComponent::isPlaying(const std::string& name) const
	{
		auto iterator = audioClips.find(name);  //Searches for audioclips. If found checks if it is playing and returns that. If not returns false
		if (iterator != audioClips.end())
		{
			if (iterator->second)
			{
				return iterator->second->IsPlaying();
			}
		}
		return false;
	}
}