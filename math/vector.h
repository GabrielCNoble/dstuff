#ifndef VECTOR_H
#define VECTOR_H


class vec2_t
{
    public:

        union
        {
            float comps[2];

            struct
            {
                float x;
                float y;
            };
        };

        vec2_t();
        vec2_t(float v);
        vec2_t(float x, float y);

        vec2_t operator + (const vec2_t &vec);
        vec2_t operator += (const vec2_t &vec);

        vec2_t operator - (const vec2_t &vec);
        vec2_t operator - ();
        vec2_t operator -= (const vec2_t &vec);

        vec2_t operator * (float s) const;
        vec2_t operator *= (float s);

        vec2_t operator / (float s);
        vec2_t operator /= (float s);

        float& operator [] (int index);
        
};

vec2_t operator * (float s, const vec2_t &vec);

vec2_t operator / (float s, const vec2_t &vec);

vec2_t vec2_t_normalize(vec2_t* vec);

float vec2_t_length(vec2_t* vec);

float vec2_t_dot(vec2_t* a, vec2_t* b);

/*
=====================================================================
=====================================================================
=====================================================================
*/

class mat3_t;

class vec3_t
{
    public:

        union
        {
            float comps[3];

            struct
            {
                float x;
                float y;
                float z;
            };
        };

        vec3_t();
        vec3_t(float v);
        vec3_t(float x, float y, float z);

        vec3_t operator + (const vec3_t &vec) const;
        vec3_t operator += (const vec3_t &vec);

        vec3_t operator - (const vec3_t &vec) const;
        vec3_t operator - ();
        vec3_t operator -= (const vec3_t &vec);

        vec3_t operator * (float s) const;
        vec3_t operator * (const mat3_t &mat) const;
        vec3_t operator *= (float s);

        vec3_t operator / (float s);
        vec3_t operator /= (float s);

        float& operator [] (int index);
};

vec3_t operator * (float s, const vec3_t &vec);

vec3_t operator / (float s, const vec3_t &vec);

vec3_t vec3_t_normalize(const vec3_t& vec);

float vec3_t_length(const vec3_t& vec);

float vec3_t_dot(const vec3_t& a, const vec3_t& b);

vec3_t vec3_t_cross(const vec3_t& a, const vec3_t& b);

vec3_t vfabs(const vec3_t& vec);


/*
=====================================================================
=====================================================================
=====================================================================
*/


class mat4_t;
class vec4_t
{
    public:

        union
        {
            float comps[4];

            struct
            {
                float x;
                float y;
                float z;
                float w;
            };
        };

        vec4_t();
        vec4_t(float v);
        vec4_t(float x, float y, float z, float w);
        vec4_t(const vec3_t& vec3, float w);

        vec4_t operator + (const vec4_t &vec);
        vec4_t operator += (const vec4_t &vec);

        vec4_t operator - (const vec4_t &vec);
        vec4_t operator - ();
        vec4_t operator -= (const vec4_t &vec);

        vec4_t operator * (float s) const;
        vec4_t operator * (const mat4_t &mat) const;
        vec4_t operator *= (float s);

        vec4_t operator / (float s);
        vec4_t operator /= (float s);

        float& operator [] (int index);

        // float length() const;
};

vec4_t operator * (float s, const vec4_t &vec);

vec4_t operator / (float s, const vec4_t &vec);

vec4_t vec4_t_normalize(const vec4_t& vec);

float vec4_t_length(const vec4_t& vec);

float vec4_t_dot(const vec4_t& a, vec4_t& b);




#endif // VECTOR_H










