#pragma once
#include "scene/GameObject.h"
#include "nlohmann/json.hpp"
#include "Common.h"
#include <vector>
#include <string>
#include <memory>

namespace eng
{
	class Scene
	{
	public:
        static void RegisterTypes();
        void Update(float deltaTime);
        void Clear();

        static std::shared_ptr<Scene> Load(const std::string& path);
        GameObject* CreateObject(const std::string& name, GameObject* parent = nullptr);

        template<typename T, typename = typename std::enable_if_t<std::is_base_of_v<GameObject, T>>>
        T* CreateObject(const std::string& name, GameObject* parent = nullptr)
        {
            auto obj = new T();
            obj->SetName(name);
            SetParent(obj, parent);
            obj->scene = this;
            return obj;
        }

        bool SetParent(GameObject* obj, GameObject* parent);

        void SetMainCamera(GameObject* camera);
        GameObject* GetMainCamera();

        std::vector<LightData> CollectLights();

    private:
        void CollectLightsRecursive(GameObject* obj, std::vector<LightData>& out);
        void LoadObject(const nlohmann::json& objectJSON, GameObject* parent);

	private:
		std::vector<std::unique_ptr<GameObject>> objects;
        GameObject* mainCamera = nullptr;
	};
}