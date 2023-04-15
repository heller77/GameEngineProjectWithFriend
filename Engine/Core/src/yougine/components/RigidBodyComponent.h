﻿#pragma once
#include "Component.h"
#include "../utilitys/YougineMath.h"

namespace yougine::components
{
    class RigidBodyComponent : public yougine::components::Component
    {
    private:
        utility::Vector3* velocity; // 速度
        // Vector3* angularVelocity; // 回転速度
        utility::Vector3* acceleration; // 加速度
        // utility::Vector3* angularAcceleration; // 回転加速度
        // utility::Vector3* angularForce; // 回転運動エネルギー
        float mass; // 質量
        float drag; // 抵抗
        float angular_drag; // トルク抵抗
        bool attraction; // 引力の有無

        utility::Bool3* freeze_position; // 各軸の移動を固定する
        utility::Bool3* freeze_rotation; // 各回転軸の移動を固定する

    public:
        RigidBodyComponent();

        utility::Vector3 GetVelocity();
        void SetVelocity(utility::Vector3 velocity);

        utility::Vector3 GetAcceleration();
        void SetAcceleration(utility::Vector3 acceleration);

        float GetMass();
        void SetMass(float mass);

        float GetDrag();
        void SetDrag(float drag);

        float GetAngularDrag();
        void SetAngularDrag(float angular_drag);

        bool GetAttraction();
        void SetAttraction(bool attraction);

        utility::Bool3 GetFreezePosition();
        void SetFreezePosition(utility::Bool3 freeze_position);

        utility::Bool3 GetFreezeRotation();
        void SetFreezeRotation(utility::Bool3 freeze_rotation);

    };
}

