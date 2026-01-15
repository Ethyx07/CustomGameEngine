#include "scene/components/SpriteComponent.h"
#include "graphics/Texture.h"


namespace eng
{
	void SpriteComponent::LoadProperties(const nlohmann::json& json) 
	{
		//Texture Loading
		const std::string texturePath = json.value("texture", "");
		if (auto texture = Texture::Load(texturePath))
		{
			SetTexture(texture);
		}

		//Colour Loading
		if (json.contains("colour"))
		{
			auto& colourObj = json["colour"]; //Gets rgba values. If key isnt found, default value is 1.0f
			glm::vec4 colour;
			colour.r = colourObj.value("r", 1.0f);
			colour.g = colourObj.value("g", 1.0f);
			colour.b = colourObj.value("b", 1.0f);
			colour.a = colourObj.value("a", 1.0f);
		}

		//Size Loading
		if (json.contains("size"))
		{
			auto& sizeObj = json["size"];
			glm::vec2 size;
			size.x = sizeObj.value("x", 100.0f);
			size.y = sizeObj.value("y", 100.0f);
		}
		//Lower left UV Loading
		if (json.contains("lowerLeftUV"))
		{
			auto& lowerLeftObj = json["lowerLeftUV"];
			glm::vec2 lowerLeftUV;
			lowerLeftUV.x = lowerLeftObj.value("u", 0.0f);
			lowerLeftUV.y = lowerLeftObj.value("v", 0.0f);
			SetLowerLeftUV(lowerLeftUV);
		}

		//Upper Right UV Loading
		if (json.contains("upperRightUV"))
		{
			auto& upperRightObj = json["upperRightUV"];
			glm::vec2 upperRightUV;
			upperRightUV.x = upperRightObj.value("u", 1.0f);
			upperRightUV.y = upperRightObj.value("v", 1.0f);
			SetUpperRightUV(upperRightUV);
		}
		//Pivot Loading
		if (json.contains("pivot"))
		{
			auto& pivotObj = json["pivot"];
			glm::vec2 pivot;
			pivot.x = pivotObj.value("x", 0.5f);
			pivot.y = pivotObj.value("y", 0.5f);
			SetPivot(pivot);
		}

	}
	void SpriteComponent::Update(float deltaTime)
	{
		if (!texture || !bIsVisible) //Do nothing if no texture or sprite is set to invisible
		{
			return;
		}
	}

	void SpriteComponent::SetTexture(const std::shared_ptr<Texture>& newTexture)
	{
		texture = newTexture;
	}

	const std::shared_ptr<Texture>& SpriteComponent::GetTexture() const
	{
		return texture;
	}

	void SpriteComponent::SetColour(const glm::vec4& newColour)
	{
		colour = newColour;
	}

	const glm::vec4& SpriteComponent::GetColour() const
	{
		return colour;
	}

	void SpriteComponent::SetSize(const glm::vec2& newSize)
	{
		size = newSize;
	}

	const glm::vec2& SpriteComponent::GetSize() const
	{
		return size;
	}

	void SpriteComponent::SetLowerLeftUV(const glm::vec2& newLowerLeft)
	{
		lowerLeftUV = newLowerLeft;
	}

	const glm::vec2& SpriteComponent::GetLowerLeftUV() const
	{
		return lowerLeftUV;
	}

	void SpriteComponent::SetUpperRightUV(const glm::vec2& newUpperRight)
	{
		upperRightUV = newUpperRight;
	}

	const glm::vec2& SpriteComponent::GetUpperRightUV() const
	{
		return upperRightUV;
	}

	void SpriteComponent::SetUV(const glm::vec2& newLowerLeft, const glm::vec2& newUpperRight)
	{
		lowerLeftUV = newLowerLeft;
		upperRightUV = newUpperRight;
	}

	void SpriteComponent::SetPivot(const glm::vec2& newPivot)
	{
		pivot = newPivot;
	}

	const glm::vec2& SpriteComponent::GetPivot() const
	{
		return pivot;
	}

	void SpriteComponent::SetIsVisible(bool visible)
	{
		bIsVisible = visible;
	}

	bool SpriteComponent::GetIsVisible() const
	{
		return bIsVisible;
	}
}
