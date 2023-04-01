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

        MenuItem* item021 = new MenuItem();
        std::vector<MenuItem*> c_item021(0);
        item021->items.emplace_back(std::make_pair("Float", c_item021));

        c_item02.push_back(item020);
        c_item02.push_back(item021);

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
                AddCode(t_node);//���C���m�[�h�Ȃ̂�
            }
            if (item == "Vector3")
            {
                CreateNode(new ShaderGraphVector3Node(), item);
            }
            if (item == "Float")
            {
                CreateNode(new ShaderGraphFloatNode(), item);
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
        int id = nodes.empty() ? 1 : nodes.back()->GetOutputInfos().back()->attr + 1;

        AddNode(node, id, node->GetInputInfos().size(), node->GetOutputInfos().size(), node_name);
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
        std::cout << "Add Node ID" + std::to_string(id) + " : " + name << std::endl;

        for (int inputID = node->id + 1; inputID < num_inputs + node->id + 1; inputID++)
        {
            node->SetInputInfoAttr(index++, inputID);
        }

        index = 0;
        for (int outputID = node->GetInputInfos().back()->attr + 1; outputID < num_outputs + node->GetInputInfos().back()->attr + 1; outputID++)
        {
            node->SetOutputInfoAttr(index++, outputID);
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

        for (std::shared_ptr<InputInfo> input_info : node->GetInputInfos())
        {
            ImNodes::BeginInputAttribute(input_info->attr);
            if (shader_graph_input_field_viewer->DrawView(input_info->val, input_info->label.c_str(), !(input_info->is_linked), input_info->field_width))
            {
                node->UpdateOutputVal();
                if (node->GetOutputInfos()[0]->linked_input_attr > -1) UpdateNodeValue(node, std::make_pair(node->GetOutputInfos()[0]->linked_input_attr, node->GetOutputInfos()[0]->attr));
            };
            ImNodes::EndInputAttribute();
        }

        for (std::shared_ptr<OutputInfo> output : node->GetOutputInfos())
        {
            ImNodes::BeginOutputAttribute(output->attr);
            ImGui::Indent(node->GetInputFieldWidth() + indent_space_arrange_width);
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
                for (std::shared_ptr<InputInfo> input : node->GetInputInfos())
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
                for (std::shared_ptr<OutputInfo> output : node->GetOutputInfos())
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

        sub_nodes.first->DisLinkNode(link_pair);

        sub_nodes.second->InitOutputInfoLinkedInputAttr();
        UpdateNodeValue(sub_nodes.second, link_pair);

        //RemoveCode(sub_nodes.second);
    }

    /*
     * pair<int, int> first ... input_attr(�e��), second ... output_attr(�q��)
     */
    void ShaderGraphWindow::UpdateLinkedNode(ShaderGraphNode* parent_node, ShaderGraphNode* child_node, std::pair<int, int> attr_pair)
    {
        child_node->SetParentNode(parent_node, attr_pair); //����Add����

        for (std::shared_ptr<InputInfo> input_info : parent_node->GetInputInfos())
        {
            if (input_info->attr == attr_pair.first)
            {
                child_node->GetOutputInfos()[0]->linked_input_attr = input_info->attr;
                input_info->linked_output_attr = child_node->GetOutputInfos()[0]->attr;
                parent_node->SetChildNode(input_info, child_node);
            }
        }

        UpdateNodeValue(child_node, attr_pair);
    }

    /*
     * child_node�̐e�m�[�h�̎q���֌W��������, �q���S�Ă̒l���X�V����
     */
    void ShaderGraphWindow::UpdateNodeValue(ShaderGraphNode* child_node, std::pair<int, int> attr_pair)
    {
        std::cout << std::to_string(attr_pair.first) + ", " + std::to_string(attr_pair.second) << std::endl;
        /*
         *�e�m�[�h�����݂���ΐe�m�[�h�̒l���X�V����
         */
        if ((child_node->UpdateParentNodeValue(attr_pair)))
        {
            AddCode(child_node);
            /*
             * �����N���Ă���y�A����q���֌W��T����attr��������
             */
            for (Link link : links)
            {
                //�q�m�[�h��output_attr�������N�y�A��input_attr�ɑ��݂���Ƃ�, �e�m�[�h���q�Ƃ����e�q�֌W�����݂���
                if (link.start_attr == child_node->GetParentNode()->GetOutputInfos()[0]->attr)
                {
                    int input_attr = link.end_attr;
                    int output_attr = link.start_attr;
                    UpdateNodeValue(child_node->GetParentNode(), std::make_pair(input_attr, output_attr));
                }
            }
        }
    }

    /**
     * \brief
     * \param node
     */
    void ShaderGraphWindow::AddCode(ShaderGraphNode* node)
    {
        if (node->GetCodeType() == CodeType::kVariable)
        {
            ResisterSortedLinkedCodeList(&stack_defined_variable_shadercode, node);
            DebugCode(stack_defined_variable_shadercode);
        }
        if (node->GetCodeType() == CodeType::kFunction)
        {
            int stackeds = stack_defined_function_shadercode.size();
            if (node->GetOutputInfos()[0]->stack_index == -1)
            {
                stack_defined_function_shadercode.emplace_back(node->GetOutputInfos()[0]->val);
                node->GetOutputInfos()[0]->stack_index = stack_defined_function_shadercode.size() - 1;
                DebugCode(stack_defined_function_shadercode);
            }
        }
        if (node->GetCodeType() == CodeType::kMain)
        {
            ResisterSortedLinkedCodeList(&stack_main_shadercode, node);
            DebugCode(stack_main_shadercode);
        }
    }

    /**
     * \brief
     * \param stack_code_list �ǉ�������code_list������ (�� : CodeType::kVariable�̏ꍇ, variable_shadercode�̃|�C���^��n��
     * \param p_node �A�������������X�g�̈�ԏ�K�w�̐e�m�[�h������. ��{�I�ɂ͎g�p���node���̂܂܂�ok
     *
     * stack_code_list��p_node���e�ƂȂ�K�w�\������������
     *
     * 1. MakeupLinkNodeCodeList�Ō������������X�g��p�ӂ���
     * 2. stack_code_list�ɑ}������index���`
     * 3. stack_code_list��1.)�̃��X�g��}��
     * 4. p_node�̎q�m�[�h��OutputInfo->stack_index���������index�ɍX�V
     */
    void ShaderGraphWindow::ResisterSortedLinkedCodeList(std::vector<std::string*>* stack_code_list, ShaderGraphNode* p_node)
    {
        std::vector < std::string > t_link_list;
        MakeupLinkNodeCodeList(&t_link_list, p_node);
        int insert_index = p_node->GetParentNode()->GetOutputInfos()[0]->stack_index;
        //error
        (*stack_code_list).insert(stack_defined_variable_shadercode.begin() + insert_index, t_link_list.begin(), t_link_list.end());
        UpdateStackIndex(p_node, insert_index);
    }

    /**
     * \brief
     * \param link_list �������郊�X�g
     * \param node ���X�g�ɒǉ��������m�[�h
     *
     * �ċA�֐�
     *
     * list�̊i�[��
     * ��)
     * p_node
     *  - node1
     *    - node11
     *    - node12
     *  - node2
     *
     *  list = [p_node, node1, node11, node12, node2]
     *
     *  p_node��node1, node2��output��m���Ă���, node1��node11, node12��output��m���Ă���K�v������
     */
    void ShaderGraphWindow::MakeupLinkNodeCodeList(std::vector<std::string>* link_list, ShaderGraphNode* node)
    {
        node->GetOutputInfos()[0]->stack_index = node->GetParentNode()->GetOutputInfos()[0]->stack_index + link_list->size();
        link_list->emplace_back(*(node->GetOutputInfos()[0]->val));
        for (std::shared_ptr<InputInfo> input_info : node->GetInputInfos())
        {
            if (input_info->child_node != nullptr)
            {
                MakeupLinkNodeCodeList(link_list, input_info->child_node);
            }
        }
    }

    void ShaderGraphWindow::SearchLeafIndexNodeAndResetStackIndex(ShaderGraphNode* node, ShaderGraphNode* leaf_node, int p_node_stack_index)
    {
        node->GetOutputInfos()[0]->stack_index -= p_node_stack_index;
        for (std::shared_ptr<InputInfo> input_info : node->GetInputInfos())
        {
            if (input_info->child_node != nullptr)
            {
                SearchLeafIndexNodeAndResetStackIndex(input_info->child_node, leaf_node, p_node_stack_index);
            }
            else
            {
                if (leaf_node->GetOutputInfos()[0]->stack_index < node->GetOutputInfos()[0]->stack_index)
                {
                    leaf_node = node;
                }
            }
        }
    }

    void ShaderGraphWindow::RemoveCode(ShaderGraphNode* node)
    {
        if (node->GetCodeType() == CodeType::kVariable)
        {
            UnResisterSortedLinkedCodeList(&stack_defined_variable_shadercode, node);
        }
        if (node->GetCodeType() == CodeType::kFunction)
        {
            stack_defined_function_shadercode.erase(stack_defined_function_shadercode.begin() + node->GetOutputInfos()[0]->stack_index);
        }
        if (node->GetCodeType() == CodeType::kMain)
        {
            UnResisterSortedLinkedCodeList(&stack_main_shadercode, node);
        }
    }

    void ShaderGraphWindow::UnResisterSortedLinkedCodeList(std::vector<std::string*>* stack_code_list, ShaderGraphNode* p_node)
    {
        ShaderGraphNode* leaf_node = p_node;
        SearchLeafIndexNodeAndResetStackIndex(p_node, leaf_node, p_node->GetOutputInfos()[0]->stack_index);
        std::pair<int, int> remove_range = std::make_pair(p_node->GetOutputInfos()[0]->stack_index, leaf_node->GetOutputInfos()[0]->stack_index);
        //error
        (*stack_code_list).erase((*stack_code_list).begin() + remove_range.first, (*stack_code_list).begin() + remove_range.second);
        UpdateStackIndex(p_node, remove_range.second);
    }

    /**
     * \brief
     * \param node index���X�V����Ώۃm�[�h
     * \param start_index ���X�g�̌�����index
     *
     * �ċA�֐�
     *
     * �����Ώۂ̃��X�g�Ɋi�[����Ă���m�[�h��stack_index���X�V����
     * �estack_index�ɂ͌����Ostack���X�g�̗v�f����(start_index)+�����
     *  �ċA��start_index��+1���čX�V
     */
    void ShaderGraphWindow::UpdateStackIndex(ShaderGraphNode* node, int start_index)
    {
        int index = start_index + 1;
        node->GetOutputInfos()[0]->stack_index = index;
        for (std::shared_ptr<InputInfo> input_info : node->GetInputInfos())
        {
            if (input_info->child_node != nullptr)
            {
                UpdateStackIndex(node->GetChildNode(input_info), index);
            }
        }
    }

    void ShaderGraphWindow::DebugCode(std::vector<std::string*> code_list)
    {
        std::cout << "" << std::endl;
        std::cout << "- Code List Log -" << std::endl;
        for (std::string* code : code_list)
        {
            std::cout << code << std::endl;
        }
        std::cout << "- This Log End -" << std::endl;
        std::cout << "" << std::endl;
    }

}
