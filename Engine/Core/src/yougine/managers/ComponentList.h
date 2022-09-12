#pragma once
#include <iostream>
#include <map>
#include "ComponentName.h"
#include "../GameObject.h"

namespace yougine::managers
{
    class ComponentList
    {
    private:
        std::map<ComponentName, std::vector<GameObject*>> gameobjects_dictionary;

    public:
        std::map<ComponentName, std::vector<GameObject*>> GetObjectsDictionary();
        void AddObjectToDictionary(ComponentName, GameObject*);
        std::vector<GameObject*> GetReferObjectList(ComponentName);
        void RemoveObjectFromDictionary(managers::ComponentName component_name, components::Component* component);
    };
}