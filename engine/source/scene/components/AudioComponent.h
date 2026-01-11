#pragma once
#include "scene/Component.h"
#include "audio/Audio.h"
#include <unordered_map>

namespace eng
{
	class AudioComponent : public Component
	{
		COMPONENT(AudioComponent)
	public:
		void LoadProperties(const nlohmann::json& json) override;
		void Update(float deltaTime) override;

		void RegisterAudio(const std::string& name, const std::shared_ptr<Audio>& audio);
		void Play(const std::string& name, bool loop = false);
		void Stop(const std::string& name);
		bool isPlaying(const std::string& name) const;

	private:
		std::unordered_map<std::string, std::shared_ptr<Audio>> audioClips;
	};
}
