#pragma once
#include "Common.h"
#include <memory>
#include <string>
#include <vector>

struct ma_sound;
struct ma_decoder;


namespace eng
{
	class Audio
	{
	public:
		~Audio();

		void SetPosition(const glm::vec3& pos);
		void Play(bool loop = false);
		void Stop();
		bool IsPlaying() const;

		void SetVolume(float volume);
		float GetVolume() const;

		static std::shared_ptr<Audio> Load(const std::string& path);

	private:
		std::unique_ptr<ma_sound> sound;
		std::unique_ptr<ma_decoder> decoder;
		std::vector<char> buffer;
	};
}