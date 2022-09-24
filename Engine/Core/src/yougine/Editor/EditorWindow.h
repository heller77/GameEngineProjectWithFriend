﻿#pragma once
#include "EditorWindowsManager.h"
#include "../Scene.h"

namespace editor
{
    class EditorWindowsManager;
    enum class EditorWindowName;

    class EditorWindow
    {
    protected:
        EditorWindowsManager* editor_windows_manager;
        EditorWindowName window_name;

    public:
        EditorWindow(EditorWindowsManager*, EditorWindowName);
        virtual void Draw();

    protected:
        void Setup();
    };
}
