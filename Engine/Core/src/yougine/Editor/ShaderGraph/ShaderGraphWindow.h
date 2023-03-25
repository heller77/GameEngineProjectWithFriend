#pragma once
#include "imnodes/imnodes.h"
#include "imgui/imgui.h"
#include "../EditorWindow.h"
#include "Nodes/ShaderGraphNode.h"
#include "Nodes/ShaderGraphInputNodes.h"
#include "Nodes/UnlitShaderGraphNode.h"
#include "../../utilitys/Split.cpp"
#include "ShaderfileOverwriter.h"

namespace editor::shadergraph
{
    /*
     * �����N�\����
     */
    struct Link
    {
        int id; //�����N�ŗLID
        int start_attr, end_attr; //�n�_ID, �I�_ID 
    };

    class ShaderGraphWindow : public EditorWindow
    {
    private:
        std::vector<ShaderGraphNode*> nodes;
        std::vector<Link> links; //���̃��X�g�Ɋi�[����Ă��郊���N�\���̂̃����N��`��
        int currentLinks = 0; //Link.id�̎w�W
        ShaderfileOverwriter* shaderfile_overwriter;

    private:
        void SearchSelectingNodeID();
        void PhaseLink();
        void PhaseAddLink();
        void PhaseDisLink();
        void PhaseNode();
        void AddNode(ShaderGraphNode* node, int id, int num_inputs, int num_outputs, std::string name);
        void DrawNode(ShaderGraphNode* node);
        void UpdateLinkedNode(ShaderGraphNode* parent_node, ShaderGraphNode* child_node, std::pair<int, int> attr_pair);
        void UpdateNodeValue(ShaderGraphNode* child_node, std::pair<int, int> attr_pair);
        void DisLinkNodes(std::pair<int, int> link_pair);
        std::pair<ShaderGraphNode*, ShaderGraphNode*> FindSubNodesByLinkAttr(std::pair<int, int> attr_pair);

    public:
        ShaderGraphWindow(EditorWindowsManager* editor_windows_manager);
        void Draw() override;

    protected:
        void SelectedItemProcess(std::string item) override;
        void InitializeMenuLists() override;
    };
}
