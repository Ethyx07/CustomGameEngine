#pragma once
#include "scene/Component.h"

#include <glm/vec2.hpp>
#include <glm/vec4.hpp>
#include <memory>

namespace eng
{

	class Texture;

	class SpriteComponent : public Component
	{
		COMPONENT(SpriteComponent)
	public:
		void LoadProperties(const nlohmann::json& json) override;
		void Update(float deltaTime) override;

		void SetTexture(const std::shared_ptr<Texture>& newTexture);
		const std::shared_ptr<Texture>& GetTexture() const;

		void SetColour(const glm::vec4& newColour);
		const glm::vec4& GetColour() const;

		void SetSize(const glm::vec2& newSize);
		const glm::vec2& GetSize() const;

		void SetLowerLeftUV(const glm::vec2& newLowerLeft);
		const glm::vec2& GetLowerLeftUV() const;

		void SetUpperRightUV(const glm::vec2& newUpperRight);
		const glm::vec2& GetUpperRightUV() const;

		void SetUV(const glm::vec2& newLowerLeft, const glm::vec2& newUpperRight);

		void SetPivot(const glm::vec2& newPivot);
		const glm::vec2& GetPivot() const;

		void SetIsVisible(bool visible);
		bool GetIsVisible() const;

	private:
		std::shared_ptr<Texture> texture;
		glm::vec4 colour = glm::vec4(1.0f); //Vec4 as Sprite has opacity too
		glm::vec2 size = glm::vec2(100.0f); //Default sizing is 100x100
		glm::vec2 lowerLeftUV = glm::vec2(0.0f); //Bottom left corner coordinates on the UV (0,0)
		glm::vec2 upperRightUV = glm::vec2(1.0f); //Upper right corner coordinates on the UV (1,1)
		glm::vec2 pivot = glm::vec2(0.5f); //Pivot point is originally the centre of the sprite
		bool bIsVisible = true; //Defaults as being visible but can be hidden
	};
}