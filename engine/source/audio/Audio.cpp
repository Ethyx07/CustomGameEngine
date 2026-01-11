#include "audio/Audio.h"
#include "Engine.h"
#include <miniaudio.h>

namespace eng
{
	Audio::~Audio()
	{
		if (sound)
		{
			ma_sound_uninit(sound.get());
		}
		if (decoder)
		{
			ma_decoder_uninit(decoder.get());
		}
	}

	void Audio::SetPosition(const glm::vec3& pos)
	{
		if (sound)
		{
			ma_sound_set_position(sound.get(), pos.x, pos.y, pos.z);
		}
	}

	void Audio::Play(bool loop)
	{
		if (sound) //Plays sound. Depending on loop MA sound is set as MA_TRUE or MA_FALSE
		{
			ma_sound_start(sound.get());
			auto looping = loop ? MA_TRUE : MA_FALSE;
			ma_sound_set_looping(sound.get(), looping);
		}
	}

	void Audio::Stop()
	{
		if (sound) //Stops sound and rewinds to frame 0
		{
			ma_sound_stop(sound.get());
			ma_sound_seek_to_pcm_frame(sound.get(), 0);
		}
	}

	bool Audio::IsPlaying() const
	{
		if (sound)
		{
			return ma_sound_is_playing(sound.get()); //Returns whether or not the sound is playing
		}
		return false; //Returns false if sound is nullptr
	}

	void Audio::SetVolume(float volume)
	{
		if (sound) //Sets volume. Clamps between 0 and 1 (0% and 100% volume)
		{
			ma_sound_set_volume(sound.get(), std::clamp(volume, 0.0f, 1.0f));
		}
	}

	float Audio::GetVolume() const
	{
		if (sound) //Returns current sound of the set sound. If nullptr, returns 0.0f
		{
			return ma_sound_get_volume(sound.get());
		}
		return 0.0f;
	}

	std::shared_ptr<Audio> Audio::Load(const std::string& path)
	{
		auto buffer = Engine::GetInstance().GetFileSystem().LoadAssetFile(path); //Gets buffer object
		auto engine = Engine::GetInstance().GetAudioManager().GetEngine(); //Gets AudioManager engine

		auto audio = std::make_shared<Audio>(); //Creates new audio and sets its values
		audio->sound = std::make_unique<ma_sound>();
		audio->buffer = buffer;
		audio->decoder = std::make_unique<ma_decoder>();

		//Initialises decoder
		auto result = ma_decoder_init_memory(audio->buffer.data(), audio->buffer.size(), nullptr, audio->decoder.get());
		if (result != MA_SUCCESS)
		{
			return nullptr;
		}
		//Initialises sound, returns nullptr if failed
		result = ma_sound_init_from_data_source(engine, audio->decoder.get(), 0, NULL, audio->sound.get());
		if (result != MA_SUCCESS)
		{
			return nullptr;
		}
		//Sets spatialization
		ma_sound_set_spatialization_enabled(audio->sound.get(), MA_TRUE);

		return audio;
	}
}