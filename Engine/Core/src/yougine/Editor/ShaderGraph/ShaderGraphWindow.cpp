#include "ShaderGraphWindow.h"

namespace editor::shadergraph
{
    ShaderGraphWindow::ShaderGraphWindow(EditorWindowsManager* editor_windows_manager) : EditorWindow(editor_windows_manager, editor::EditorWindowName::ShaderGraphWindow)
    {
        InitializeMenuLists();

        shaderfile_overwriter = new ShaderfileOverwriter("F:/GameEngineProject/Yougine/Shaders/", "test.frag");
        shader_graph_input_field_viewer = std::make_shared<ShaderGraphInputFieldViewer>();
    }

    void ShaderGraphWindow::InitializeMenuLists()
    {
        MenuItem* item00 = new MenuItem();
        std::vector<MenuItem*> c_item00;
        MenuItem* item000 = new MenuItem();
        std::vector<MenuItem*> c_item000(0);
        item000->items.emplace_back(std::make_pair("Sample Node", c_item000));
        c_item00.push_back(item000);
        item00->items.emplace_back(std::make_pair("Debug", c_item00));

        MenuItem* item01 = new MenuItem();
        std::vector<MenuItem*> c_item01;
        MenuItem* item010 = new MenuItem();
        std::vector<MenuItem*> c_item010(0);
        item010->items.emplace_back(std::make_pair("Unlit", c_item010));
        c_item01.push_back(item010);
        item01->items.emplace_back(std::make_pair("Main", c_item01));

        MenuItem* item02 = new MenuItem();
        std::vector<MenuItem*> c_item02;
        MenuItem* item020 = new MenuItem();
        std::vector<MenuItem*> c_item020(0);
        item020->items.emplace_back(std::make_pair("Vector3", c_item020));
        c_item02.push_back(item020);
        item02->items.emplace_back(std::make_pair("Input", c_item02));

        pulldown_menu_bar = new PullDownMenuBar();
        std::vector<MenuItem*> items0;
        items0.push_back(item00);
        items0.push_back(item01);
        items0.push_back(item02);
        pulldown_menu_bar->menu_items.emplace_back(std::make_pair("Add Node", items0));


        MenuItem* item10 = new MenuItem();
        std::vector<MenuItem*> c_item10;
        MenuItem* item100 = new MenuItem();
        std::vector<MenuItem*> c_item100(0);
        item100->items.emplace_back(std::make_pair("Update", c_item100));
        c_item10.push_back(item100);
        item10->items.emplace_back(std::make_pair("Ex", c_item10));

        std::vector<MenuItem*> items1;
        items1.push_back(item10);
        pulldown_menu_bar->menu_items.emplace_back(std::make_pair("ShaderOverwriter", items1));
    }


    void ShaderGraphWindow::Draw()
    {
        ImGuiWindowFlags flags = (ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_MenuBar);
        ImGui::Begin("simple node editor", nullptr, flags);

        //���j���[�o�[
        EditorWindow::RenderMenuBar();

        ImNodes::BeginNodeEditor();

        //�m�[�h��`��
        PhaseNode();

        //�m�[�h���m���q���郊���N����`��
        PhaseLink();

        ImNodes::EndNodeEditor();

        PhaseAddLink();
        PhaseDisLink();

        ImGui::End();
    }

    void ShaderGraphWindow::SelectedItemProcess(std::string item)
    {
        if (ImGui::MenuItem(item.c_str()))
        {
            if (item == "Unlit")
            {
                UnlitShaderGraphNode* t_node = new UnlitShaderGraphNode();
                main_node = t_node;
                CreateNode(t_node, item);
            }
            if (item == "Vector3")
            {
                CreateNode(new ShaderGraphVector3Node(), item);
            }
            if (item == "Update")
            {
                if (main_node != nullptr)
                {
                    shaderfile_overwriter->UpdateFile(main_node->GetShaderCodeByOutputVal());
                }
                else
                {
                    std::cout << "Main�m�[�h�����݂��܂���" << std::endl;
                }
            }
        }
    }


    void ShaderGraphWindow::PhaseNode()
    {
        for (ShaderGraphNode* node : nodes)
        {
            DrawNode(node);
        }
    }

    void ShaderGraphWindow::CreateNode(ShaderGraphNode* node, std::string node_name)
    {
        int id = nodes.empty() ? 1 : nodes.back()->output_infos.back()->attr + 1;

        AddNode(node, id, node->input_infos.size(), node->output_infos.size(), node_name);
    }


    /*
     * �`�悷��m�[�h�𑝂₵�����Ƃ��ɌĂяo��
     *
     * id ... �O�m�[�h��output_attr.back()+1�̒l��n��
     * num_inputs ... input_attr�̐�
     * num_outputs ... ouput_attr�̐�
     */
    void ShaderGraphWindow::AddNode(ShaderGraphNode* node, int id, int num_inputs, int num_outputs, std::string name)
    {
        node->id = id;

        int index = 0;
        for (int inputID = node->id + 1; inputID < num_inputs + node->id + 1; inputID++)
        {
            std::shared_ptr<InputInfo> input_info = std::make_shared<InputInfo>();
            //input_info->type = ???;
            //input_info->label = ???;
            input_info->attr = inputID;
            input_info->init_val = node->GetInitInputVals()[index++];
            input_info->val = input_info->init_val;
            node->input_infos.emplace_back(input_info);
        }

        index = 0;
        for (int outputID = node->input_infos.back()->attr + 1; outputID < num_outputs + node->input_infos.back()->attr + 1; outputID++)
        {
            std::shared_ptr<OutputInfo> output_info = std::make_shared<OutputInfo>();
            //output_info->type = ???;
            //output_info->label = ???;
            output_info->attr = outputID;
            output_info->init_val = node->GetInitOutputVals()[index++];
            output_info->val = output_info->init_val;
            node->output_infos.emplace_back(output_info);
        }
        node->name = name;

        nodes.push_back(node);
    }

    /*
     * nodes�z��Ɋi�[����Ă���Node�\���̂�Node�Ƃ��ĕ`�悷��
     */
    void ShaderGraphWindow::DrawNode(ShaderGraphNode* node)
    {
        ImNodes::BeginNode(node->id);

        ImNodes::BeginNodeTitleBar();
        ImGui::TextUnformatted(node->name.c_str());
        ImNodes::EndNodeTitleBar();

        for (std::shared_ptr<InputInfo> input_info : node->input_infos)
        {
            ImNodes::BeginInputAttribute(input_info->attr);
            shader_graph_input_field_viewer->DrawView(input_info->val, input_info->label.c_str());
            ImNodes::EndInputAttribute();
        }

        for (std::shared_ptr<OutputInfo> output : node->output_infos)
        {
            ImNodes::BeginOutputAttribute(output->attr);
            ImGui::Indent(40);
            ImGui::Text("output");
            ImNodes::EndOutputAttribute();
        }

        ImNodes::EndNode();
    }


    /*
     * Node���m�������N����
     * links���X�g�Ɋi�[����Ă��郊���N�\���̑S�Ă������N��ԂƂ��ĕ`�悷��
     */
    void ShaderGraphWindow::PhaseLink()
    {
        for (Link& link : links)
        {
            ImNodes::Link(link.id, link.start_attr, link.end_attr);
        }
    }

    /*
     * �����N�\���̂�ǉ�����
     * �m�[�h1����m�[�h2�̃s���Ƀ����N�����킹���炻���������N���Ă���Ƃ݂Ȃ��A�����N�\���̂𐶐�����links���X�g�ɒǉ�����
     */
    void ShaderGraphWindow::PhaseAddLink()
    {
        Link link;
        if (ImNodes::IsLinkCreated(&link.start_attr, &link.end_attr))
        {
            std::pair<ShaderGraphNode*, ShaderGraphNode*> sub_nodes; //0��input(�e), 1��output(�q)
            int input_attr = link.end_attr, output_attr = link.start_attr;
            std::cout << "input_attr : " + std::to_string(input_attr) + ", output_attr : " + std::to_string(output_attr) << std::endl;

            //�����N����m�[�h��T��
            sub_nodes = FindSubNodesByLinkAttr(std::make_pair(input_attr, output_attr));

            std::cout << "LinkCreated:" + std::to_string(link.start_attr) + "to" + std::to_string(link.end_attr) << std::endl;
            link.id = ++currentLinks;
            links.push_back(link);

            UpdateLinkedNode(sub_nodes.first, sub_nodes.second, std::make_pair(input_attr, output_attr));
        }
    }

    /*
     * �����N����������
     * �����N���Ă���m�[�h�̃s�����h���b�O�A���h�h���b�v�����炻�̃����N����������
     */
    void ShaderGraphWindow::PhaseDisLink()
    {
        int pin;
        //�m�[�h�̃s���Ƀ}�E�X�z�o�[����Ă����Ԃō��N���b�N
        if (ImNodes::IsPinHovered(&pin) && ImGui::IsMouseClicked(ImGuiMouseButton_Left))
        {
            std::cout << "PinClicked:" + std::to_string(pin) << std::endl;
            //�I������Ă��郊���N������
            int l_id;
            if (ImNodes::IsLinkHovered(&l_id))
            {
                std::cout << "LinkDestroyed : " + std::to_string(l_id) << std::endl;
                //ImNodes::ClearLinkSelection();

                //�������ꂽ�����N��������links���X�g�ɍX�V
                std::vector<Link> newLinks;
                for (Link ll : links)
                {
                    if (ll.id != l_id) newLinks.push_back(ll);
                    else
                    {
                        //ll.end_attr...input��, ll.start_attr...output��
                        DisLinkNodes(std::make_pair(ll.end_attr, ll.start_attr));
                    }
                }
                links = newLinks;
            }
        }
    }

    /*
     * return linked nodes
     *
     * Return
     * first ... input����鑤�̃m�[�h(�e�m�[�h)
     * second ... output���鑤�̃m�[�h(�q�m�[�h)
     */
    std::pair<ShaderGraphNode*, ShaderGraphNode*> ShaderGraphWindow::FindSubNodesByLinkAttr(std::pair<int, int> attr_pair)
    {
        int input_attr = attr_pair.first, output_attr = attr_pair.second;
        std::pair<ShaderGraphNode*, ShaderGraphNode*> sub_nodes;

        bool is_detected_input_node = false, is_detected_output_node = false;
        //int first_attr_id = input_attr < output_attr ? 0 : 1;
        for (ShaderGraphNode* node : nodes)
        {
            if (!is_detected_input_node)
            {
                for (std::shared_ptr<InputInfo> input : node->input_infos)
                {
                    int i_attr = input->attr;
                    is_detected_input_node = (i_attr == input_attr);
                    if (is_detected_input_node)
                    {
                        sub_nodes.first = node;
                        break;
                    }
                }
            }
            if (!is_detected_output_node)
            {
                for (std::shared_ptr<OutputInfo> output : node->output_infos)
                {
                    int o_attr = output->attr;
                    is_detected_output_node = (o_attr == output_attr);
                    if (is_detected_output_node)
                    {
                        sub_nodes.second = node;
                        break;
                    }
                }
            }
        }

        return sub_nodes;
    }


    void ShaderGraphWindow::DisLinkNodes(std::pair<int, int> link_pair)
    {
        std::pair<ShaderGraphNode*, ShaderGraphNode*> sub_nodes;
        sub_nodes = FindSubNodesByLinkAttr(link_pair);
        ShaderGraphNode* parent = sub_nodes.first;

        parent->DisLinkNode(link_pair);
        UpdateNodeValue(sub_nodes.second, link_pair);
    }

    /*
     * pair<int, int> first ... input_attr(�e��), second ... output_attr(�q��)
     */
    void ShaderGraphWindow::UpdateLinkedNode(ShaderGraphNode* parent_node, ShaderGraphNode* child_node, std::pair<int, int> attr_pair)
    {
        child_node->SetParentNode(parent_node, attr_pair); //����Add����

        UpdateNodeValue(child_node, attr_pair);
    }

    /*
     * child_node�̐e�m�[�h�̎q���֌W��������, �q���S�Ă̒l���X�V����
     */
    void ShaderGraphWindow::UpdateNodeValue(ShaderGraphNode* child_node, std::pair<int, int> attr_pair)
    {
        /*
         *�e�m�[�h�����݂���ΐe�m�[�h�̒l���X�V����
         */
        if ((child_node->UpdateParentNodeValue(attr_pair)))
        {
            /*
             * �����N���Ă���y�A����q���֌W��T����attr��������
             */
            for (Link link : links)
            {
                //�q�m�[�h��output_attr�������N�y�A��input_attr�ɑ��݂���Ƃ�, �e�m�[�h���q�Ƃ����e�q�֌W�����݂���
                if (link.start_attr == child_node->GetParentNode()->output_infos[0]->attr)
                {
                    int input_attr = link.end_attr;
                    int output_attr = link.start_attr;
                    UpdateNodeValue(child_node->GetParentNode(), std::make_pair(input_attr, output_attr));
                }
            }
        }
    }
}
