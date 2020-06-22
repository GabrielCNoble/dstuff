#ifndef VECTOR_H
#define VECTOR_H

#include <math.h>

typedef struct vec2_t
{
    union
    {
        float comps[2];

        struct
        {
            float x;
            float y;
        };
    };

}vec2_t;

#define vec2_t_c(x, y) (vec2_t){{{x, y}}}

void vec2_t_add(vec2_t *r, vec2_t *a, vec2_t *b);

void vec2_t_sub(vec2_t *r, vec2_t *a, vec2_t *b);

float vec2_t_dot(vec2_t *a, vec2_t *b);

void vec2_t_mul(vec2_t *r, vec2_t *a, float s);

void vec2_t_normalize(vec2_t *r, vec2_t *v);

float vec2_t_length(vec2_t *v);

// vec2_t operator * (float s, const vec2_t &vec);

// vec2_t operator / (float s, const vec2_t &vec);

// vec2_t vec2_t_normalize(vec2_t* vec);

// float vec2_t_length(vec2_t* vec);

// float vec2_t_dot(vec2_t* a, vec2_t* b);

/*
=====================================================================
=====================================================================
=====================================================================
*/

typedef struct vec3_t
{
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
}vec3_t;

#define vec3_t_c(x, y, z) (vec3_t){{{x, y, z}}}

void vec3_t_add(vec3_t *r, vec3_t *a, vec3_t *b);

void vec3_t_sub(vec3_t *r, vec3_t *a, vec3_t *b);

void vec3_t_mul(vec3_t *r, vec3_t *v, float s);

void vec3_t_div(vec3_t *r, vec3_t *v, float s);

float vec3_t_length(vec3_t *v);

void vec3_t_normalize(vec3_t *r, vec3_t *v);

float vec3_t_dot(vec3_t *a, vec3_t *b);

void vec3_t_cross(vec3_t *r, vec3_t *a, vec3_t *b);

void vec3_t_fabs(vec3_t *r, vec3_t *v);

void vec3_t_fmadd(vec3_t *r, vec3_t *a, vec3_t *b, float s);

/*
=====================================================================
=====================================================================
=====================================================================
*/

typedef struct vec4_t
{
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

}vec4_t;

#define vec4_t_c(x, y, z, w) (vec4_t){{{x, y, z, w}}}

void vec4_t_add(vec4_t *r, vec4_t *a, vec4_t *b);

void vec4_t_sub(vec4_t *r, vec4_t *a, vec4_t *b);

void vec4_t_mul(vec4_t *r, vec4_t *v, float s);

void vec4_t_div(vec4_t *r, vec4_t *v, float s);

float vec4_t_length(vec4_t *v);

void vec4_t_normalize(vec4_t *r, vec4_t *v);

float vec4_t_dot(vec4_t *a, vec4_t *b);

void vec4_t_fabs(vec4_t *r, vec4_t *v);

/*
=====================================================================
=====================================================================
=====================================================================
*/

#ifdef DS_VECTOR_IMPLEMENTATION

void vec2_t_add(vec2_t *r, vec2_t *a, vec2_t *b)
{
    r->x = a->x + b->x;
    r->y = a->y + b->y;
}

void vec2_t_sub(vec2_t *r, vec2_t *a, vec2_t *b)
{
    r->x = a->x - b->x;
    r->y = a->y - b->y;
}

float vec2_t_dot(vec2_t *a, vec2_t *b)
{
    return a->x * b->x + a->y * b->y;
}

void vec2_t_mul(vec2_t *r, vec2_t *a, float s)
{
    r->x = a->x * s;
    r->y = a->y * s;
}

void vec2_t_normalize(vec2_t *r, vec2_t *v)
{
    float l = sqrt(v->x * v->x + v->y * v->y);
    r->x = v->x / l;
    r->y = v->y / l;
}

float vec2_t_length(vec2_t *v)
{
    return sqrt(v->x * v->x + v->y * v->y);
}

void vec3_t_add(vec3_t *r, vec3_t *a, vec3_t *b)
{
    r->x = a->x + b->x;
    r->y = a->y + b->y;
    r->z = a->z + b->z;
}

void vec3_t_sub(vec3_t *r, vec3_t *a, vec3_t *b)
{
    r->x = a->x - b->x;
    r->y = a->y - b->y;
    r->z = a->z - b->z;
}

void vec3_t_mul(vec3_t *r, vec3_t *v, float s)
{
    r->x = v->x * s;
    r->y = v->y * s;
    r->z = v->z * s;
}

void vec3_t_div(vec3_t *r, vec3_t *v, float s)
{
    r->x = v->x / s;
    r->y = v->y / s;
    r->z = v->z / s;
}

float vec3_t_length(vec3_t *v)
{
    return sqrt(v->x * v->x + v->y * v->y + v->z * v->z);
}

void vec3_t_normalize(vec3_t *r, vec3_t *v)
{
    float len = vec3_t_length(v);

    if(len)
    {
        vec3_t_div(r, v, len);
        return;
    }

    r->x = 0.0;
    r->y = 0.0;
    r->z = 0.0;
}

float vec3_t_dot(vec3_t *a, vec3_t *b)
{
    return a->x * b->x + a->y * b->y + a->z * b->z;
}

void vec3_t_cross(vec3_t *r, vec3_t *a, vec3_t *b)
{
    *r = (vec3_t){{.x = a->z * b->y - a->y * b->z, .y = a->x * b->z - a->z * b->x, .z = a->y * b->x - a->x * b->y}};
}

void vec3_t_fabs(vec3_t *r, vec3_t *v)
{
    r->x = fabsf(v->x);
    r->y = fabsf(v->y);
    r->z = fabsf(v->z);
}

void vec3_t_fmadd(vec3_t *r, vec3_t *a, vec3_t *b, float s)
{
    *r = (vec3_t)
    {
        .x = fmaf(a->x, b->x, s),
        .y = fmaf(a->y, b->y, s),
        .z = fmaf(a->z, b->z, s),
    };
}

void vec4_t_add(vec4_t *r, vec4_t *a, vec4_t *b)
{
    r->x = a->x + b->x;
    r->y = a->y + b->y;
    r->z = a->z + b->z;
    r->w = a->w + b->w;
}

void vec4_t_sub(vec4_t *r, vec4_t *a, vec4_t *b)
{
    r->x = a->x - b->x;
    r->y = a->y - b->y;
    r->z = a->z - b->z;
    r->w = a->w - b->w;
}

void vec4_t_mul(vec4_t *r, vec4_t *v, float s)
{
    r->x = v->x * s;
    r->y = v->y * s;
    r->z = v->z * s;
    r->w = v->w * s;
}

void vec4_t_div(vec4_t *r, vec4_t *v, float s)
{
    r->x = v->x / s;
    r->y = v->y / s;
    r->z = v->z / s;
    r->w = v->w / s;
}

float vec4_t_length(vec4_t *v)
{
    return sqrt(v->x * v->x + v->y * v->y + v->z * v->z + v->w * v->w);
}

void vec4_t_normalize(vec4_t *r, vec4_t *v)
{
    float len = vec4_t_length(v);

    if(len)
    {
        vec4_t_div(r, v, len);
        return;
    }

    r->x = 0.0;
    r->y = 0.0;
    r->z = 0.0;
    r->w = 0.0;
}

float vec4_t_dot(vec4_t *a, vec4_t *b)
{
    return a->x * b->x + a->y * b->y + a->z * b->z + a->w * b->w;
}

void vec4_t_fabs(vec4_t *r, vec4_t *v)
{
    r->x = fabsf(v->x);
    r->y = fabsf(v->y);
    r->z = fabsf(v->z);
    r->w = fabsf(v->w);
}

#endif








#endif // VECTOR_H










