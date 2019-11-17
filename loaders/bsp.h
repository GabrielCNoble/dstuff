#ifndef BSP_H
#define BSP_H

#include <stdint.h>
#include "xchg.h"

enum BSP_LUMP
{
    BSP_LUMP_ENTITIES = 0,
    BSP_LUMP_TEXTURES,
    BSP_LUMP_PLANES,
    BSP_LUMP_NODES,
    BSP_LUMP_LEAFS,
    BSP_LUMP_LEAFFACES,
    BSP_LUMP_LEAFBRUSHES,
    BSP_LUMP_MODELS,
    BSP_LUMP_BRUSHES,
    BSP_LUMP_BRUSHSIDES,
    BSP_LUMP_VERTEXES,
    BSP_LUMP_MESHVERTS,
    BSP_LUMP_EFFECTS,
    BSP_LUMP_FACES,
    BSP_LUMP_LIGHTMAPS,
    BSP_LUMP_LIGHTVOLS,
    BSP_LUMP_VISDATA,
};

struct direntry_t
{
    int32_t offset;
    int32_t length;
};

struct bsp_header_t
{
    char magic[4];
    int32_t version;
    struct direntry_t direntries[17];
};

struct bsp_texture_t
{
    char name[64];
    int32_t flags;
    int32_t contents;
};

struct bsp_plane_t
{
    float normal[3];
    float dist;
};

struct bsp_node_t
{
    int32_t plane;
    int32_t children[2];
    int32_t mins[3];
    int32_t maxs[3];
};

struct bsp_leaf_t
{
    int32_t cluster;
    int32_t area;
    int32_t mins[3];
    int32_t maxs[3];
    int32_t leafface;
    int32_t n_leaffaces;
    int32_t leafbrush;
    int32_t n_leafbrushes;
};

struct bsp_leafface_t
{
    int32_t face;
};

struct bsp_leafbrush_t
{
    int32_t brush;
};

struct bsp_model_t
{
    int32_t brushside;
    int32_t n_brushsides;
    int32_t texture;
};

struct bsp_brush_t
{
    int32_t brushside;
    int32_t n_brushsides;
    int32_t texture;
};

struct bsp_brushside_t
{
    int32_t plane;
    int32_t texture;
};

struct bsp_vertex_t
{
    float position[3];
    float texcoord[2][2];
    float normal[3];
    uint8_t color[4];
};

struct bsp_meshvert_t
{
    int32_t offset;
};

struct bsp_effect_t
{
    char name[64];
    int32_t brush;
    int32_t unknown;
};

enum BSP_FACE_TYPE
{
    BSP_FACE_TYPE_POLYGON = 1,
    BSP_FACE_TYPE_PATCH = 2,
    BSP_FACE_TYPE_MESH = 3,
    BSP_FACE_TYPE_BILLBOARD = 4,
};

struct bsp_face_t
{
    int32_t texture;
    int32_t effect;
    int32_t type;
    int32_t vertex;
    int32_t n_vertexes;
    int32_t meshvert;
    int32_t n_meshverts;
    int32_t lm_index;
    int32_t lm_start[2];
    int32_t lm_size[2];
    float lm_origin[3];
    float lm_vercs[2][3];
    float normal[3];
    int32_t size[2];
};

struct bsp_lightmap_t
{
    uint8_t map[128][128][3];
};

struct bsp_lightvol_t
{
    uint8_t ambient[3];
    uint8_t directional[3];
    uint8_t dir[2];
};


void load_bsp(char *file_name, struct geometry_data_t *data);

struct batch_data_t *get_bsp_batch(char *name, struct geometry_data_t *data);


#endif