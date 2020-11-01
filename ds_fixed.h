#ifndef DS_FIXED_H
#define DS_FIXED_H

#include <stdint.h>

#define DS_FIXED_32_FRAC_BITS 32
/* -2147483648 to +2147483647, in increments of 0,00000000023283064365386962890625  */
typedef uint64_t fixed32_t 


fixed32_t fixed32_from_imediate(uint32_t i, uint32_t f);

fixed32_t fixed32_from_float(float value);

float float_from_fixed32(fixed32_t);

fixed32_t fixed32_add(fixed32_t a, fixed32_t b);

fixed32_t fixed32_sub(fixed32_t a, fixed32_t b);

fixed32_t fixed32_mul(fixed32_t a, fixed32_t b);

fixed32_t fixed32_div(fixed32_t a, fixed32_t b);


#endif // DS_FIXED_H


#ifdef DS_FIXED_IMPLEMENTATION

fixed32_t fixed32_from_imediate(uint32_t i, uint32_t f)
{
    fixed32_t v;
    v = (i << DS_FIXED_32_FRAC_BITS) | f;
    return v;
}

fixed32_t fixed32_from_float(float value)
{
    
}

float float_from_fixed32(fixed32_t)
{
    
}

fixed32_t fixed32_add(fixed32_t a, fixed32_t b)
{
    
}

fixed32_t fixed32_sub(fixed32_t a, fixed32_t b)
{
    
}

fixed32_t fixed32_mul(fixed32_t a, fixed32_t b)
{
    
}

fixed32_t fixed32_div(fixed32_t a, fixed32_t b)
{
    
}


#endif // DS_FIXED_IMPLEMENTATION
