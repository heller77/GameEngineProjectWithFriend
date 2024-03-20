﻿#include "InspectorWindow.h"

#include <memory>

#include "../component_factory/ComponentFactory.h"
#include "imgui/stblib/imgui_stdlib.h"
#include <string>

#include "./../componentclassnames/ComponentClassNames.h"

namespace editor
{
    InspectorWindow::InspectorWindow(EditorWindowsManager* editor_windows_manager, yougine::Scene* scene,
        yougine::InputManager* input_manager) : EditorWindow(
            editor_windows_manager, EditorWindowName::InspectorWindow)
    {
        this->scene = scene;
        this->input_manager = input_manager;
        selection_info = SelectionInfo::GetInstance();
        layer_manager = yougine::LayerManager::GetInstance();
        this->componentfactory = new yougine::componentfactorys::ComponentFacotory();

        SelectionInfo::GetInstance()->AddSelectChangeEvent([=]()
            {
                this->ViewReGenerate();
            });
    }

    void InspectorWindow::ViewReGenerate()
    {
        auto recentSelectTarget = SelectionInfo::GetInstance()->GetRecentClickTarget();
        switch (recentSelectTarget)
        {
            //Hierarchyのものが選択されていたら
        case SelectTarget::HierarchyWindow:
            if (selection_info->GetSelectObject() != nullptr)
            {
                //HierarchyWindowで選択したものを表示するクラスが実装されたらここでフィールドに入れるとかするとよいかも
            }
            break;
        case SelectTarget::Projectwindow:
            if (SelectionInfo::GetInstance()->GetSelectElementInProjectWindow() != nullptr)
            {
                auto select = SelectionInfo::GetInstance()->GetSelectElementInProjectWindow();
                auto asset = select->GetAsset();
                this->asset_view = std::make_shared<AssetView::AssetView>(asset);
            }
            break;
        default:
            break;
        }
    }

    void InspectorWindow::Draw()
    {
        ImGuiWindowFlags_ flag = (ImGuiWindowFlags_NoCollapse);
        ImGui::Begin(editor_windows_manager->GetWindowName(window_name).c_str(), nullptr, flag);

        //最も最近選択されたtargetを取得
        auto recentSelectTarget = SelectionInfo::GetInstance()->GetRecentClickTarget();
        switch (recentSelectTarget)
        {
            //Hierarchyのものが選択されていたら
        case SelectTarget::HierarchyWindow:
            if (selection_info->GetSelectObject() != nullptr)
            {
                ShowGameObjectData();
                ShowAddComponentMenu();
            }
            break;
        case SelectTarget::Projectwindow:
            if (SelectionInfo::GetInstance()->GetSelectElementInProjectWindow() != nullptr)
            {
                if (this->asset_view != nullptr)
                {
                    asset_view->DrawAssetParameter();
                }
            }
            break;
        default:
            break;
        }

        ImGui::End();
    }

    void InspectorWindow::ShowGameObjectData()
    {
        /*
         * GameObjectの名前のInputTextBoxの実装
         */
        const int size_str = sizeof(selection_info->GetSelectObject()->GetName());
        char temp_s[size_str];
        strcpy_s(temp_s, size_str, selection_info->GetSelectObject()->GetName().c_str());

        char* temp_c = temp_s;
        bool is_input = ImGui::InputText("name", temp_c, MAX_SIZE_OBJ_NAME, ImGuiInputTextFlags_EnterReturnsTrue);
        if (is_input)
        {
            selection_info->GetSelectObject()->SetName(temp_c);
        }

        //ImGui::SameLine();

        /*
         * Layer情報 未実装
         */
        const char* items[2] = { "Default", "GameObject" };
        static int item_current = 0;
        ImGui::Combo("Layer", &item_current, items, IM_ARRAYSIZE(items));

        /*
         * Componentのビュー表示
         *
         * ComponentName1
         *  (componentviewer::drawviews
         *  - Prop1s
         *  - Prop2
         *
         * ComponentName2
         *  - Prop1
         *  - Prop2
         *  - Prop3
         */
        for (ComponentViewer* c_viewer : selection_info->GetComponentViewers())
        {
            bool c_tree = ImGui::CollapsingHeader(c_viewer->GetComponentName().c_str());
            if (c_tree)
            {
                c_viewer->DrawViews();
            }
        }
    }

    void InspectorWindow::ShowAddComponentMenu()
    {
        int selected = -1;
        // const char* componentNames[] = {
        //     "yougine::components::DebugComponent", "yougine::components::TransformComponent",
        //     "yougine::components::RigidBodyComponent", "yougine::components::RenderComponent",
        //     "yougine::components::camera::CameraComponent"
        //
        // };
        ComponentClassNames::ComponentClassNames::GetInstance()->AddUserScriptNames();
        auto componentNames = ComponentClassNames::ComponentClassNames::GetInstance()->GetComponentClassNames();
        ImGui::Spacing();
        if (ImGui::Button("Add Component"))
            ImGui::OpenPopup("add_component_popup");
        if (ImGui::BeginPopup("add_component_popup"))
        {
            ImGui::Text("Component List");
            for (int i = 0; i < componentNames.size(); i++)
                if (ImGui::Selectable(componentNames[i].data()))
                {
                    selected = i;
                    selection_info->GetInstance()->GetSelectObject()->AddComponent(
                        componentfactory->CreateComponent(componentNames[selected]));
                }
            ImGui::EndPopup();
        }
    }


}
