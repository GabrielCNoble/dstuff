#ifndef MATRIX_H
#define MATRIX_H

#include "vector.h"

// class mat3_t;

class mat3_t
{
    public:

        union
        {
            float comps[3][3];

            struct
            {
                float x0, y0, z0;
                float x1, y1, z1;
                float x2, y2, z2;
            };

            vec3_t rows[3];
        };

        mat3_t();

        mat3_t(const vec3_t &r0, const vec3_t &r1, const vec3_t &r2);

        mat3_t operator * (const mat3_t &mat) const;

        mat3_t operator *= (const mat3_t &mat);

        vec3_t &operator[] (int index);
};


/*
=====================================================================
=====================================================================
=====================================================================
*/

void mat3_t_identity(mat3_t* m);

void mat3_t_transpose(mat3_t* m);

void mat3_t_rotate_x(mat3_t* m, float angle);

void mat3_t_rotate_y(mat3_t* m, float angle);

void mat3_t_rotate_z(mat3_t* m, float angle);

void mat3_t_rotate(mat3_t* m, float angle);



class mat4_t
{
    public:

        union
        {
            float comps[4][4];

            struct
            {
                float x0, y0, z0, w0;
                float x1, y1, z1, w1;
                float x2, y2, z2, w2;
                float x3, y3, z3, w3;
            };

            vec4_t rows[4];
        };

        mat4_t();

        mat4_t(const vec4_t &r0, const vec4_t &r1, const vec4_t &r2, const vec4_t &r3);

        mat4_t(const mat3_t& r, const vec3_t& p);

        mat4_t operator * (const mat4_t &mat) const;

        mat4_t operator *= (const mat4_t &mat);

        vec4_t &operator[] (int index);
};

void mat4_t_identity(mat4_t* m);

void mat4_t_transpose(mat4_t* m);

void mat4_t_invvm(mat4_t *m);

void mat4_t_persp(mat4_t* m, float fov_y, float aspect, float z_near, float z_far);

void mat4_t_pitch(mat4_t* m, float pitch);

void mat4_t_yaw(mat4_t* m, float yaw);

#endif // MATRIX_H





