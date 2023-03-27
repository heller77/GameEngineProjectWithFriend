#include "UnlitShaderGraphNode.h"

#include <iostream>

namespace editor::shadergraph
{
    UnlitShaderGraphNode::UnlitShaderGraphNode() : MainShaderGraphNode()
    {
        std::vector<std::any> input_vals;
        input_vals.emplace_back(&albedo);

        std::vector<std::any> output_vals;
        color = albedo;
        output_vals.emplace_back(&color);

        Initialize(input_vals, output_vals);

        shaderCodeListByOutputVal.push_back(qualifier_dictionary[ShaderQualifier::kOut] + " " + type_dictionary[ShaderPropertyType::kVec3] + " " + stage_dictionary[ShaderStage::kFragment]);
        shaderCodeListByOutputVal.push_back(type_dictionary[ShaderPropertyType::kVec3] + " " + "color");
        shaderCodeListByOutputVal.push_back(codeMainFunction);
        shaderCodeListByOutputVal.push_back("{");
        shaderCodeListByOutputVal.push_back(stage_dictionary[ShaderStage::kFragment] + " = " + "glm::" + type_dictionary[ShaderPropertyType::kVec4] + "(" + CastValueToString(init_input_vals[0]) + ", 1.0);");
        shaderCodeListByOutputVal.push_back("}");
    }

    void UnlitShaderGraphNode::UpdateOutputVal()
    {
        std::cout << "?" << std::endl;
        output_infos[0]->val = input_infos[0]->val;
        shaderCodeListByOutputVal[5] = stage_dictionary[ShaderStage::kFragment] + " = " + "glm::" + type_dictionary[ShaderPropertyType::kVec4] + "(" + CastValueToString(output_infos[0]->val) + ", 1.0);";
    }

}
