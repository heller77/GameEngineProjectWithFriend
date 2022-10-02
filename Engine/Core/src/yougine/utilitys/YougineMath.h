#pragma once
namespace utility
{
    struct Vector2
    {
    public:
        float x, y, z;

        Vector2(float x, float y)
            : x(x),
            y(y)
        {
        }

        /**
         * \brief ������Z�q
         * \param v �������l��
         * \return ������ꂽ�Q��
         */
        Vector2& operator=(const Vector2& v)
        {
            this->x = v.x;
            this->y = v.y;
        }
    };

    struct Vector3
    {
    public:
        float x, y, z;

        Vector3(float x, float y, float z)
            : x(x),
            y(y),
            z(z)
        {
        }

        /**
         * \brief ������Z�q
         * \param v �������l��
         * \return ������ꂽ�Q��
         */
        Vector3& operator=(const Vector3& v)
        {
            this->x = v.x;
            this->y = v.y;
            this->z = v.z;
        }
    };

    struct Vector4
    {
    public:
        float x, y, z, w;

        Vector4(float x, float y, float z, float w)
            : x(x),
            y(y),
            z(z),
            w(w)
        {
        }

        /**
         * \brief ������Z�q
         * \param v �������l��
         * \return ������ꂽ�Q��
         */
        Vector4& operator=(const Vector4& v)
        {
            this->x = v.x;
            this->y = v.y;
            this->z = v.z;
            this->w = v.w;
        }
    };
}