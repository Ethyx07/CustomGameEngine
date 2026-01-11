#include "audio/AudioManager.h"
#define MINIAUDIO_IMPLEMENTATION
#include <miniaudio.h>

namespace eng
{
	AudioManager::AudioManager()
	{
		engine = std::make_unique<ma_engine>(); //Sets engine pointer
	}

	AudioManager::~AudioManager()
	{
		if (engine)
		{
			ma_engine_uninit(engine.get()); //Un-initialises the engine
		}
	}

	bool AudioManager::Init()
	{
		auto result = ma_engine_init(nullptr, engine.get()); //Initialises the engine and returns true if result == MA_SUCCESS
		return result == MA_SUCCESS;
	}

	ma_engine* AudioManager::GetEngine()
	{
		return engine.get(); //Gets engine
	}

	void AudioManager::SetListenerPosition(const glm::vec3& pos)
	{
		if (engine) //If engine is valid it sets the listener position of it
		{
			ma_engine_listener_set_position(engine.get(), 0, pos.x, pos.y, pos.z);
		}
	}
}