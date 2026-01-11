#include "AudioListenerComponent.h"
#include "scene/GameObject.h"
#include "Engine.h"

namespace eng
{
	void AudioListenerComponent::Update(float deltaTime)
	{
		auto pos = owner->GetPosition();
		Engine::GetInstance().GetAudioManager().SetListenerPosition(pos);
	}
}