#pragma once
#include <memory>
#include <string>
#include "UserShare/utilitys/YougineMath.h"
#include "UserShare/utilitys/Split.h"
#include "UserShare/utilitys/Quaternion.h"
#include <any>
#include "imgui/imgui.h"

namespace editor
{
    class PropertiesInputField
    {
    public:
        /**
         * \brief
         * @fn
         *
         * InputField��`�悷��
         * val�̐^�̌^�𔻕ʂ�InputField��`��
         *
         * \param val ���͂����l������|�C���^�ϐ�
         * \param label InputField�̃��x��
         * \param field_width InputField�̕� ��������Ȃ� or 0�ȉ��̓f�t�H���g���ɂȂ�
         */
        bool Draw(std::vector<std::any> propertie, float field_width = 0);

    private:
        bool Bool3View(utility::Bool3* value, const char* name);
        bool Vector3View(utility::Vector3* value, const char* name);
        bool IntView(int* value, const char* name);
        bool FloatView(float* value, const char* name);
        bool StringView(std::string* value, const char* name);
        bool BoolView(bool* value, const char* name);
        bool QuaternionView(std::shared_ptr<utility::Quaternion> value, const char* name);
    };
}
