#include "SceneWindow.h"

#include <iostream>

editor::SceneWindow::SceneWindow(editor::EditorWindowsManager* editor_windows_manager,
    yougine::Scene*) : EditorWindow(editor_windows_manager,
        editor::EditorWindowName::SceneWindow)
{
    this->renderManager = new yougine::managers::RenderManager(100, 100);
}

void editor::SceneWindow::Draw()
{
    ImGui::Begin("Scene View window");

    auto size = ImGui::GetContentRegionAvail();
    std::cout << "contentregionAvail : " << size.x << " : " << size.y << std::endl;
    auto size2 = ImGui::GetWindowSize();
    std::cout << "windowsize : " << size2.x << " : " << size2.y << std::endl << std::endl;
    // renderManager->SetWindowSize(size);
    renderManager->RenderScene();
    ImGui::Image((void*)(intptr_t)renderManager->GetColorBuffer(), ImVec2(size.x, size.y), ImVec2(0, 1), ImVec2(1, 0));
    ImGui::End();
}
