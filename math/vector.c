#include "vector.h"
#include "matrix.h"
#include <math.h>




vec2_t::vec2_t()
{

}

vec2_t::vec2_t(float v)
{
    this->x = v;
    this->y = v;
}

vec2_t::vec2_t(float x, float y)
{
    this->x = x;
    this->y = y;
}

vec2_t vec2_t::operator + (const vec2_t &vec)
{
    return vec2_t(this->x + vec.x, this->y + vec.y);
}

vec2_t vec2_t::operator += (const vec2_t &vec)
{
    this->x += vec.x;
    this->y += vec.y;
    return *this;
}

vec2_t vec2_t::operator - (const vec2_t &vec)
{
    return vec2_t(this->x - vec.x, this->y - vec.y);
}

vec2_t vec2_t::operator - ()
{
    return vec2_t(-this->x, -this->y);
}

vec2_t vec2_t::operator -= (const vec2_t &vec)
{
    this->x -= vec.x;
    this->y -= vec.y;
    return *this;
}

vec2_t vec2_t::operator * (float s) const
{
    return vec2_t(this->x * s, this->y * s);
}

vec2_t operator * (float s, const vec2_t &vec)
{
    return vec2_t(vec.x * s, vec.y * s);
}

vec2_t vec2_t::operator *= (float s)
{
    this->x *= s;
    this->y *= s;
    return *this;
}

vec2_t vec2_t::operator / (float s)
{
    return vec2_t(this->x / s, this->y / s);
}

vec2_t operator / (float s, const vec2_t &vec)
{
    return vec2_t(vec.x / s, vec.y / s);
}

vec2_t vec2_t::operator /= (float s)
{
    this->x /= s;
    this->y /= s;
    return *this;
}

float & vec2_t::operator [] (int index)
{
    return this->comps[index];
}

vec2_t vec2_t_normalize(vec2_t* vec)
{
    float len = vec2_t_length(vec);
    if(len)
    {
        return vec2_t(vec->x / len, vec->y / len);
    }
    
    return vec2_t(0.0);
}

float vec2_t_length(vec2_t* vec)
{
    return sqrt(vec->x * vec->x + vec->y * vec->y);
}

float vec2_t_dot(vec2_t* a, vec2_t* b)
{
    return a->x * b->x + a->y * b->y;
}

/*
=====================================================================
=====================================================================
=====================================================================
*/

vec3_t::vec3_t()
{

}

vec3_t::vec3_t(float v)
{
    this->x = v;
    this->y = v;
    this->z = v;
}

vec3_t::vec3_t(float x, float y, float z)
{
    this->x = x;
    this->y = y;
    this->z = z;
}

vec3_t vec3_t::operator + (const vec3_t &vec) const
{
    return vec3_t(this->x + vec.x, this->y + vec.y, this->z + vec.z);
}

vec3_t vec3_t::operator += (const vec3_t &vec)
{
    this->x += vec.x;
    this->y += vec.y;
    this->z += vec.z;
    return *this;
}

vec3_t vec3_t::operator - (const vec3_t &vec) const
{
    return vec3_t(this->x - vec.x, this->y - vec.y, this->z - vec.z);
}

vec3_t vec3_t::operator - ()
{
    return vec3_t(-this->x, -this->y, -this->z);
}

vec3_t vec3_t::operator -= (const vec3_t &vec)
{
    this->x -= vec.x;
    this->y -= vec.y;
    this->z -= vec.z;
    return *this;
}

vec3_t vec3_t::operator * (float s) const
{
    return vec3_t(this->x * s, this->y * s, this->z * s);
}

vec3_t vec3_t::operator * (const mat3_t &mat) const
{
    return vec3_t(this->x * mat.rows[0] + this->y * mat.rows[1] + this->z * mat.rows[2]);
}

vec3_t operator * (float s, const vec3_t &vec)
{
    return vec3_t(vec.x * s, vec.y * s, vec.z * s);
}

vec3_t vec3_t::operator *= (float s)
{
    this->x *= s;
    this->y *= s;
    this->z *= s;
    return *this;
}

vec3_t vec3_t::operator / (float s)
{
    return vec3_t(this->x / s, this->y / s, this->z / s);
}

vec3_t operator / (float s, const vec3_t &vec)
{
    return vec3_t(vec.x / s, vec.y / s, vec.z / s);
}

vec3_t vec3_t::operator /= (float s)
{
    this->x /= s;
    this->y /= s;
    this->z /= s;
    return *this;
}

float & vec3_t::operator [] (int index)
{
    return this->comps[index];
}

vec3_t vec3_t_normalize(const vec3_t& vec)
{
    float len = vec3_t_length(vec);

    if(len)
    {
        return vec3_t(vec.x / len, vec.y / len, vec.z / len);
    }

    return vec3_t(0.0);
}

float vec3_t_length(const vec3_t& vec)
{
    return sqrt(vec.x * vec.x + vec.y * vec.y + vec.z * vec.z);
}

float vec3_t_dot(const vec3_t& a, const vec3_t& b)
{
    return a.x * b.x + a.y * b.y + a.z * b.z;
}

vec3_t vec3_t_cross(const vec3_t& a, const vec3_t& b)
{
    return vec3_t(a.z * b.y - a.y * b.z, a.x * b.z - a.z * b.x, a.y * b.x - a.x * b.y);
}

vec3_t vfabs(const vec3_t& vec)
{
    return vec3_t(fabs(vec.x), fabs(vec.y), fabs(vec.z));
}

/*
=====================================================================
=====================================================================
=====================================================================
*/



vec4_t::vec4_t()
{

}

vec4_t::vec4_t(float v)
{
    this->x = v;
    this->y = v;
    this->z = v;
    this->w = v;
}

vec4_t::vec4_t(float x, float y, float z, float w)
{
    this->x = x;
    this->y = y;
    this->z = z;
    this->w = w;
}

vec4_t::vec4_t(const vec3_t& vec3, float w)
{
    this->x = vec3.x;
    this->y = vec3.y;
    this->z = vec3.z;
    this->w = w;
}



vec4_t vec4_t::operator+(const vec4_t &vec)
{
    return vec4_t(this->x + vec.x, this->y + vec.y, this->z + vec.z, this->w + vec.w);
}

vec4_t vec4_t::operator += (const vec4_t &vec)
{
    this->x += vec.x;
    this->y += vec.y;
    this->z += vec.z;
    this->w += vec.w;
    return *this;
}



vec4_t vec4_t::operator - (const vec4_t &vec)
{
    return vec4_t(this->x - vec.x, this->y - vec.y, this->z - vec.z, this->w - vec.w);
}

vec4_t vec4_t::operator - ()
{
    return vec4_t(-this->x, -this->y, -this->z, -this->w);
}

vec4_t vec4_t::operator -= (const vec4_t &vec)
{
    this->x -= vec.x;
    this->y -= vec.y;
    this->z -= vec.z;
    this->w -= vec.w;
    return *this;
}



vec4_t vec4_t::operator * (float s) const
{
    return vec4_t(this->x * s, this->y * s, this->z * s, this->w * s);
}

vec4_t vec4_t::operator * (const mat4_t &mat) const
{
    return vec4_t(this->x * mat.rows[0] + this->y * mat.rows[1] + this->z * mat.rows[2] + this->w * mat.rows[3]);
}

vec4_t operator * (float s, const vec4_t &vec)
{
    return vec4_t(vec.x * s, vec.y * s, vec.z * s, vec.w * s);
}

vec4_t vec4_t::operator *= (float s)
{
    this->x *= s;
    this->y *= s;
    this->z *= s;
    this->w *= s;
    return *this;
}

vec4_t vec4_t::operator / (float s)
{
    return vec4_t(this->x / s, this->y / s, this->z / s, this->w / s);
}

vec4_t operator / (float s, const vec4_t &vec)
{
    return vec4_t(vec.x / s, vec.y / s, vec.z / s, vec.w / s);
}

vec4_t vec4_t::operator /= (float s)
{
    this->x /= s;
    this->y /= s;
    this->z /= s;
    this->w /= s;
    return *this;
}

float & vec4_t::operator [] (int index)
{
    return this->comps[index];
}

vec4_t vec4_t_normalize(const vec4_t& vec)
{
    float len = vec4_t_length(vec);
    if(len)
    {
        return vec4_t(vec.x / len, vec.y / len, vec.z / len, vec.w / len);
    }
    return vec4_t(0.0);
}

float vec4_t_length(const vec4_t& vec)
{
    return sqrt(vec.x * vec.x + vec.y * vec.y + vec.z * vec.z + vec.w * vec.w);
}

float vec4_t_dot(const vec4_t& a, const vec4_t& b)
{
    return a.x * b.x + a.y * b.y + a.z * b.z + a.w * b.w;
}


