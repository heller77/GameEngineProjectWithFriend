﻿#include "HierarchyWindow.h"
#include <iostream>

namespace editor
{
    HierarchyWindow::HierarchyWindow(EditorWindowsManager* editor_windows_manager, yougine::Scene* scene, yougine::InputManager* input_manager) :EditorWindow(editor_windows_manager, EditorWindowName::HierarchyWindow)
    {
        this->scene = scene;
        this->input_manager = input_manager;
        selection_info = SelectionInfo::GetInstance();

        InitializeMenuLists();
    }

    void HierarchyWindow::InitializeMenuLists()
    {
        MenuItem* item0 = new MenuItem();
        std::vector<MenuItem*> c_item0(0);
        item0->items.emplace_back(std::make_pair("GameObject", c_item0));

        // --- debug ---
        MenuItem* item1 = new MenuItem();
        std::vector<MenuItem*> c_item1;
        MenuItem* item10 = new MenuItem();
        MenuItem* item11 = new MenuItem();
        std::vector<MenuItem*> c_item10(0);
        item10->items.emplace_back(std::make_pair("Ghost", c_item10));
        std::vector<MenuItem*> c_item11(0);
        item11->items.emplace_back(std::make_pair("CC2", c_item11));
        c_item1.push_back(item10);
        c_item1.push_back(item11);
        item1->items.emplace_back(std::make_pair("GameObject_test", c_item1));
        // ------

        pulldown_menu_bar = new PullDownMenuBar();
        std::vector<MenuItem*> items;
        items.push_back(item0);
        items.push_back(item1);//
        pulldown_menu_bar->menu_items.emplace_back(std::make_pair("Create", items));
    }


    void HierarchyWindow::Draw()
    {
        InitializeFlameProperties();

        ImGuiWindowFlags flags = (ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_MenuBar);
        ImGui::Begin(editor_windows_manager->GetWindowName(window_name).c_str(), nullptr, flags);

        EditorWindow::RenderMenuBar();

        RenderObjectsTree();

        ImGui::End();
    }

    void HierarchyWindow::SelectedItemProcess(std::string item)
    {
        if (ImGui::MenuItem(item.c_str()))
        {
            if (item == "GameObject")
            {
                std::string o_name = selection_info->GetSelectObject() != nullptr ? selection_info->GetSelectObject()->GetName() + "_c" + std::to_string((selection_info->GetSelectObject()->GetChildObjects().size() + 1)) : "Obj" + std::to_string(scene->GetGameObjects().size() + 1);
                CreateGameObject(o_name, selection_info->GetSelectObject());
            }
        };
    }


    void HierarchyWindow::RenderObjectsTree()
    {
        RecursiveTree(scene->GetGameObjects());
    }

    void HierarchyWindow::RecursiveTree(std::list<yougine::GameObject*> game_objects)
    {
        for (yougine::GameObject* game_object : game_objects)
        {
            bool is_leaf = game_object->GetChildObjects().size() == 0;
            if (selection_info->GetSelectObject() == game_object)
                ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0., 1., 0., 1.));
            ImGuiTreeNodeFlags node_flag = (ImGuiTreeNodeFlags_OpenOnDoubleClick | ImGuiTreeNodeFlags_OpenOnArrow | (is_leaf ? ImGuiTreeNodeFlags_Leaf : 0));
            bool is_open_tree = ImGui::TreeNodeEx(game_object->GetName().c_str(), node_flag);
            if (selection_info->GetSelectObject() == game_object)
                ImGui::PopStyleColor();

            if (ImGui::IsItemClicked())
            {
                selection_info->SetSelectionInfo(game_object, true);
            }
            else if (flame_click_trigger && !selection_info->GetSelectedFlag())
            {
                if (ImGui::IsWindowHovered())
                {
                    selection_info->SetSelectionInfo(nullptr, true);
                }
            }

            if (is_open_tree)
            {
                RecursiveTree(game_object->GetChildObjects());
                ImGui::TreePop();
            }
        }
    }

    yougine::GameObject* HierarchyWindow::CreateGameObject(std::string name, yougine::GameObject* parent)
    {
        return scene->CreateGameObject(name, parent);
    }

    void HierarchyWindow::InitializeFlameProperties()
    {
        flame_click_trigger = ImGui::IsMouseClicked(0);
        selection_info->SetSelectedFlag(false);
    }

}