#include "SceneLoader.h";

#include <tinygltf/json.hpp>

namespace yougine
{
    SceneLoader::SceneLoader()
    {
        //json初期化とかする
    }

    void SceneLoader::InitializeScene(Scene* scene)
    {
        for (std::string gameobject_name : scene_json)
        {
            //GameObject* top_gameobject = scene->CreateGameObject(gameobject_name);

        }
    }

    void SceneLoader::RecursiveChildObjects(nlohmann::json j, GameObject* parent)
    {
    }


}
