﻿#pragma once
#include "../Scene.h"
#include "../managers/ComponentName.h"

namespace yougine
{
    class GameObject;
}

namespace yougine::components
{
    class Component
    {
    private:
        GameObject* parent_gameobject;
        managers::ComponentName component_name;
        // bool isRegisterdToComponentList;
        /**
         * \brief 登録しているComponentList
         */
        managers::ComponentList* register_component_list;

    private:
        void InitializeProperties();

    public:
        Component(managers::ComponentName componentname);
        ~Component();
        virtual void Excute();
        managers::ComponentName GetComponentName();
        void InitializeOnPlayBack();
        bool operator==(const Component& rhs) const;
        GameObject* GetGameObject();
        void SetParentGameObject(GameObject* parent_gameobject);
        bool RegisterThisComponentToComponentList(Scene* scene);
        void UnregisterThisComponentFromComponentList();
        bool isAlradyRegisterComponentList();
    };
}
