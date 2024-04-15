﻿#include "ComponentFactory.h"

#include "../components/DebugComponent.h"
#include "../components/RenderComponent.h"
#include "UserShare/components/TransformComponent.h"
#include "../components/RigidBodyComponent.h"
#include "../components/Camera/CameraComponent.h"
#include "windows.h"
#include "../BuildScript/UserScriptCompiler.h"
#include "../Projects/Project.h"

typedef yougine::components::Component* (*FUNC)(std::string);

yougine::components::Component* yougine::componentfactorys::ComponentFacotory::CreateComponent(
    std::string component_class_name)
{
    if (component_class_name == "yougine::components::DebugComponent")
    {
        return new components::DebugComponent();
    }
    else if (component_class_name == "yougine::components::TransformComponent")
    {
        return new components::TransformComponent(0, 0, 0);
    }
    else if (component_class_name == "yougine::components::RigidBodyComponent")
    {
        return new components::RigidBodyComponent();
    }
    else if (component_class_name == "yougine::components::RenderComponent")
    {
        return new components::RenderComponent();
    }
    else if (component_class_name == "yougine::components::camera::CameraComponent")
    {
        return new components::camera::CameraComponent();
    }
    //ここにユーザの作ったカスタムコンポーネントのelse if文も動的に入る予定
    HMODULE hModule = builders::UserScriptCompiler::GetModule();
    if (!hModule) {
        std::cerr << "DLLをロードできませんでした。" << std::endl;
        return nullptr;
    }
    FUNC generateUserScriptComponentFunc = (FUNC)GetProcAddress(hModule, "GenerateUserScriptComponent");
    auto component = generateUserScriptComponentFunc(component_class_name);
    return component;
}
