#pragma once
#include <memory>
#include <string>
#include "../utilitys/YougineMath.h"
#include "../utilitys/Split.cpp"
#include "../utilitys/Quaternion.h"
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
        void Draw(std::any val, const char* label, float field_width = 0);

    private:
        void Bool3View(utility::Bool3* value, const char* name);
        void Vector3View(utility::Vector3* value, const char* name);
        void IntView(int* value, const char* name);
        void FloatView(float* value, const char* name);
        void StringView(std::string* value, const char* name);
        void BoolView(bool* value, const char* name);
        void QuaternionView(std::shared_ptr<utility::Quaternion> value, const char* name);
    };
}
