#include "matrix.h"
#include <math.h>

mat3_t::mat3_t()
{

}

mat3_t::mat3_t(const vec3_t &r0, const vec3_t &r1, const vec3_t &r2)
{
    this->rows[0] = r0;
    this->rows[1] = r1;
    this->rows[2] = r2;
}

mat3_t mat3_t::operator * (const mat3_t &mat) const
{
    mat3_t result;

    result.rows[0] = mat.rows[0] * this->rows[0].x +
                     mat.rows[1] * this->rows[0].y +
                     mat.rows[2] * this->rows[0].z;

    result.rows[1] = mat.rows[0] * this->rows[1].x +
                     mat.rows[1] * this->rows[1].y +
                     mat.rows[2] * this->rows[1].z;

    result.rows[2] = mat.rows[0] * this->rows[2].x +
                     mat.rows[1] * this->rows[2].y +
                     mat.rows[2] * this->rows[2].z;

    return result;
}

mat3_t mat3_t::operator *= (const mat3_t &mat)
{
    mat3_t result;

    this->rows[0] = mat.rows[0] * this->rows[0].x +
                    mat.rows[1] * this->rows[0].y +
                    mat.rows[2] * this->rows[0].z;

    this->rows[1] = mat.rows[0] * this->rows[1].x +
                    mat.rows[1] * this->rows[1].y +
                    mat.rows[2] * this->rows[1].z;

    this->rows[2] = mat.rows[0] * this->rows[2].x +
                    mat.rows[1] * this->rows[2].y +
                    mat.rows[2] * this->rows[2].z;

    return *this;
}

vec3_t &mat3_t::operator[] (int index)
{
    return this->rows[index];
}

void mat3_t_identity(mat3_t* m)
{
   m->rows[0] = vec3_t(1.0, 0.0, 0.0);
   m->rows[1] = vec3_t(0.0, 1.0, 0.0);
   m->rows[2] = vec3_t(0.0, 0.0, 1.0);
}

void mat3_t_transpose(mat3_t* m)
{
    float temp;

    temp = m->comps[0][1];
    m->comps[0][1] = m->comps[1][0];
    m->comps[1][0] = temp;

    temp = m->comps[0][2];
    m->comps[0][2] = m->comps[2][0];
    m->comps[2][0] = temp;

    temp = m->comps[2][1];
    m->comps[2][1] = m->comps[1][2];
    m->comps[1][2] = temp;
}

void mat3_t_rotate_x(mat3_t* m, float angle)
{
    float s;
    float c;

    s = sin(angle * M_PI);
    c = cos(angle * M_PI);

    *m *= mat3_t(vec3_t(1.0, 0.0, 0.0),
                  vec3_t(0.0,   c,   s),
                  vec3_t(0.0,  -s,   c));
}

void mat3_t_rotate_y(mat3_t* m, float angle)
{
    float s;
    float c;

    s = sin(angle * M_PI);
    c = cos(angle * M_PI);

    *m *= mat3_t(vec3_t(c  , 0.0,  -s),
                  vec3_t(0.0, 1.0, 0.0),
                  vec3_t(s  , 0.0,   c));
}

void mat3_t_rotate_z(mat3_t* m, float angle)
{
    float s;
    float c;

    s = sin(angle * M_PI);
    c = cos(angle * M_PI);

    *m *= mat3_t(vec3_t(c  ,   s, 0.0),
                  vec3_t(-s ,   c, 0.0),
                  vec3_t(0.0, 0.0, 1.0));
}


mat3_t mat3_t_rotate(vec3_t* axis, float angle)
{
	float s, c;
	float c_m_one;
	mat3_t result;

	s = sin(3.14159265 * angle);
	c = cos(3.14159265 * angle);

	c_m_one = 1.0 - c;

    result[0][0] = axis->x * axis->x * (c_m_one) + c;
    result[0][1] = axis->x * axis->y * (c_m_one) + axis->z * s;
    result[0][2] = axis->x * axis->z * (c_m_one) - axis->y * s;

    result[1][0] = axis->x * axis->y * (c_m_one) - axis->y * s;
    result[1][1] = axis->y * axis->y * (c_m_one) + c;
    result[1][2] = axis->y * axis->z * (c_m_one) + axis->x * s;

    result[2][0] = axis->x * axis->z * (c_m_one) + axis->y * s;
    result[2][1] = axis->y * axis->z * (c_m_one) - axis->x * s;
    result[2][2] = axis->z * axis->z * (c_m_one) + c;

	return result;
}

/*
=====================================================================
=====================================================================
=====================================================================
*/

mat4_t::mat4_t()
{

}

mat4_t::mat4_t(const vec4_t &r0, const vec4_t &r1, const vec4_t &r2, const vec4_t &r3)
{
    this->rows[0] = r0;
    this->rows[1] = r1;
    this->rows[2] = r2;
    this->rows[3] = r3;
}

mat4_t::mat4_t(const mat3_t& r, const vec3_t& t)
{
    this->rows[0] = vec4_t(r.rows[0], 0.0);
    this->rows[1] = vec4_t(r.rows[1], 0.0);
    this->rows[2] = vec4_t(r.rows[2], 0.0);
    this->rows[3] = vec4_t(t, 1.0);
}

mat4_t mat4_t::operator * (const mat4_t &mat) const
{
    mat4_t result;

    result.rows[0] = mat.rows[0] * this->rows[0].x +
                     mat.rows[1] * this->rows[0].y +
                     mat.rows[2] * this->rows[0].z +
                     mat.rows[3] * this->rows[0].w;

    result.rows[1] = mat.rows[0] * this->rows[1].x +
                     mat.rows[1] * this->rows[1].y +
                     mat.rows[2] * this->rows[1].z +
                     mat.rows[3] * this->rows[1].w;

    result.rows[2] = mat.rows[0] * this->rows[2].x +
                     mat.rows[1] * this->rows[2].y +
                     mat.rows[2] * this->rows[2].z +
                     mat.rows[3] * this->rows[2].w;

    result.rows[3] = mat.rows[0] * this->rows[3].x +
                     mat.rows[1] * this->rows[3].y +
                     mat.rows[2] * this->rows[3].z +
                     mat.rows[3] * this->rows[3].w;

    return result;
}

mat4_t mat4_t::operator *= (const mat4_t &mat)
{
    this->rows[0] = mat.rows[0] * this->rows[0].x +
                    mat.rows[1] * this->rows[0].y +
                    mat.rows[2] * this->rows[0].z +
                    mat.rows[3] * this->rows[0].w;

    this->rows[1] = mat.rows[0] * this->rows[1].x +
                    mat.rows[1] * this->rows[1].y +
                    mat.rows[2] * this->rows[1].z +
                    mat.rows[3] * this->rows[1].w;

    this->rows[2] = mat.rows[0] * this->rows[2].x +
                    mat.rows[1] * this->rows[2].y +
                    mat.rows[2] * this->rows[2].z +
                    mat.rows[3] * this->rows[2].w;

    this->rows[3] = mat.rows[0] * this->rows[3].x +
                    mat.rows[1] * this->rows[3].y +
                    mat.rows[2] * this->rows[3].z +
                    mat.rows[3] * this->rows[3].w;

    return *this;
}

vec4_t& mat4_t::operator[] (int index)
{
    return this->rows[index];
}

void mat4_t_identity(mat4_t* m)
{
    m->rows[0] = vec4_t(1.0, 0.0, 0.0, 0.0);
    m->rows[1] = vec4_t(0.0, 1.0, 0.0, 0.0);
    m->rows[2] = vec4_t(0.0, 0.0, 1.0, 0.0);
    m->rows[3] = vec4_t(0.0, 0.0, 0.0, 1.0);
}

void mat4_t_transpose(mat4_t* m)
{
    float t;

    t = m->comps[1][0];
    m->comps[1][0] = m->comps[0][1];
    m->comps[0][1] = t;

    t = m->comps[2][0];
    m->comps[2][0] = m->comps[0][2];
    m->comps[0][2] = t;

    t = m->comps[3][0];
    m->comps[3][0] = m->comps[0][3];
    m->comps[0][3] = t;

    t = m->comps[3][1];
    m->comps[3][1] = m->comps[1][3];
    m->comps[1][3] = t;

    t = m->comps[3][2];
    m->comps[3][2] = m->comps[2][3];
    m->comps[2][3] = t;

    t = m->comps[2][1];
    m->comps[2][1] = m->comps[1][2];
    m->comps[1][2] = t;
}

void mat4_t_invvm(mat4_t *m)
{
    /* TODO: SSE */
    mat4_t_transpose(m);

    m->rows[3].comps[0] = -m->comps[0][3] * m->comps[0][0]
                            -m->comps[1][3] * m->comps[1][0]
                            -m->comps[2][3] * m->comps[2][0];
    
    m->rows[3].comps[1] = -m->comps[0][3] * m->comps[0][1]
                            -m->comps[1][3] * m->comps[1][1]
                            -m->comps[2][3] * m->comps[2][1];

    m->rows[3].comps[2] = -m->comps[0][3] * m->comps[0][2]
                            -m->comps[1][3] * m->comps[1][2]
                            -m->comps[2][3] * m->comps[2][2];

    m->comps[0][3] = 0.0;
    m->comps[1][3] = 0.0;
    m->comps[2][3] = 0.0;
}

void mat4_t_persp(mat4_t* m, float fov_y, float aspect, float z_near, float z_far)
{
    float t = tanf(fov_y) * z_near;
    float r = t * aspect;

    mat4_t_identity(m);

    m->comps[0][0] = z_near / r;
    m->comps[1][1] = z_near / t;
    m->comps[2][2] = -1.0;
    m->comps[2][3] = -1.0;
    m->comps[3][2] = -(2.0 * z_near * z_far) / (z_far - z_near);
    m->comps[3][3] = 0.0;
}

void mat4_t_pitch(mat4_t* m, float pitch)
{
    mat4_t_identity(m);
    float s = sinf(pitch * 3.14159265);
    float c = cosf(pitch * 3.14159265);

    m->rows[1].comps[1] = c;
    m->rows[1].comps[2] = s;
    m->rows[2].comps[1] = -s;
    m->rows[2].comps[2] = c;
}

void mat4_t_yaw(mat4_t* m, float yaw)
{
    mat4_t_identity(m);

    float s = sinf(yaw * 3.14159265);
    float c = cosf(yaw * 3.14159265);

    m->rows[0].comps[0] = c;
    m->rows[0].comps[2] = -s;
    m->rows[2].comps[0] = s;
    m->rows[2].comps[2] = c;
}







