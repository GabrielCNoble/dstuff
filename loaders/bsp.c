#include "bsp.h"
#include "xchg.h"
#include "../file/file.h"
#include "../file/path.h"
#include <stdio.h>
#include <string.h>

void load_bsp(char *file_name, struct geometry_data_t *data)
{
    void *file_buffer;
    long file_size;
    FILE *file;
    struct bsp_header_t *header;
    struct bsp_vertex_t *bsp_vertices;
    struct bsp_vertex_t *bsp_face_vertices;
    struct bsp_vertex_t *bsp_vertex;
    struct bsp_face_t *bsp_faces;
    struct bsp_face_t *bsp_face;
    struct bsp_meshvert_t *bsp_meshverts;
    struct bsp_meshvert_t *bsp_face_meshverts;
    struct bsp_meshvert_t *bsp_meshvert;
    struct bsp_texture_t *bsp_textures;
    struct bsp_texture_t *bsp_texture;
    // struct geometry_data_t data;
    struct batch_data_t batch;
    struct batch_data_t *face_batch;
    vec3_t vec3;
    vec2_t vec2;
    char texture_name[64];

    // struct list_t vertices;
    // struct list_t normals;

    vec3_t *vertices;
    vec3_t *normals;
    vec2_t *tex_coords;
    struct batch_data_t *batches;

    uint32_t vert_count = 0;
    uint32_t face_count = 0;
    uint32_t vert_index;
    uint32_t texture_count = 0;
    file = fopen(file_name, "rb");

    if(file)
    {
        read_file(file, &file_buffer, &file_size);
        header = (struct bsp_header_t*)file_buffer;

        if(strncmp(header->magic, "IBSP", 4))
        {
            printf("%s is not a valid bsp file!\n", file_name);
        }

        // printf("entity string is %d bytes long\n", header->direntries[BSP_LUMP_ENTITIES].length);
        // printf("%d textures present\n", header->direntries[BSP_LUMP_TEXTURES].length / sizeof(struct bsp_texture_t));
        // printf("%d planes present\n", header->direntries[BSP_LUMP_PLANES].length / sizeof(struct bsp_plane_t));
        // printf("%d nodes present\n", header->direntries[BSP_LUMP_NODES].length / sizeof(struct bsp_node_t));
        // printf("%d leafs present\n", header->direntries[BSP_LUMP_LEAFS].length / sizeof(struct bsp_leaf_t));
        // printf("%d leaffaces present\n", header->direntries[BSP_LUMP_LEAFFACES].length / sizeof(struct bsp_leafface_t));
        // printf("%d models present\n", header->direntries[BSP_LUMP_MODELS].length / sizeof(struct bsp_model_t));
        // printf("%d brushes present\n", header->direntries[BSP_LUMP_BRUSHES].length / sizeof(struct bsp_brush_t));
        // printf("%d brushsides present\n", header->direntries[BSP_LUMP_BRUSHSIDES].length / sizeof(struct bsp_brushside_t));
        // printf("%d vertexes present\n", header->direntries[BSP_LUMP_VERTEXES].length / sizeof(struct bsp_vertex_t));
        // printf("%d meshverts present\n", header->direntries[BSP_LUMP_MESHVERTS].length / sizeof(struct bsp_meshvert_t));
        // printf("%d effects present\n", header->direntries[BSP_LUMP_EFFECTS].length / sizeof(struct bsp_effect_t));
        // printf("%d faces present\n", header->direntries[BSP_LUMP_FACES].length / sizeof(struct bsp_face_t));
        // printf("%d lightmaps present\n", header->direntries[BSP_LUMP_LIGHTMAPS].length / sizeof(struct bsp_lightmap_t));
        // printf("%d lightvols present\n", header->direntries[BSP_LUMP_LIGHTVOLS].length / sizeof(struct bsp_lightvol_t));

        // vert_count = header->direntries[BSP_LUMP_VERTEXES].length / sizeof(struct bsp_vertex_t);
        face_count = header->direntries[BSP_LUMP_FACES].length / sizeof(struct bsp_face_t);
        texture_count = header->direntries[BSP_LUMP_TEXTURES].length / sizeof(struct bsp_texture_t); 


        bsp_vertices = (struct bsp_vertex_t *)((char *)file_buffer + header->direntries[BSP_LUMP_VERTEXES].offset);
        bsp_faces = (struct bsp_face_t *)((char *)file_buffer + header->direntries[BSP_LUMP_FACES].offset);
        bsp_meshverts = (struct bsp_meshvert_t *)((char *)file_buffer + header->direntries[BSP_LUMP_MESHVERTS].offset);
        bsp_textures = (struct bsp_texture_t *)((char *)file_buffer + header->direntries[BSP_LUMP_TEXTURES].offset);


        data->batches = create_list(sizeof(struct batch_data_t), texture_count); 


        for(uint32_t i = 0; i < texture_count; i++)
        {
            /* gather all textures / materials */
            bsp_texture = bsp_textures + i;
            batch.start = 0;
            batch.count = 0;
            batch.normal_texture[0] = '\0';
            strcpy(batch.material, bsp_texture->name);

            strcpy(texture_name, bsp_texture->name);
            strcat(texture_name, ".jpg");

            if(!file_exists(texture_name))
            {
                strcpy(texture_name, bsp_texture->name);
                strcat(texture_name, ".tga");
            }

            // strcpy(batch.diffuse_texture, bsp_texture->name);
            // strcat(batch.diffuse_texture, ".jpg");
            strcpy(batch.diffuse_texture, texture_name);
            add_list_element(&data->batches, &batch);

            // printf("bsp texture: %s\n", batch.diffuse_texture);
        }

        // printf("0\n");

        for(uint32_t i = 0; i < face_count; i++)
        {
            bsp_face = bsp_faces + i;

            if(bsp_face->type == BSP_FACE_TYPE_POLYGON || bsp_face->type == BSP_FACE_TYPE_MESH)
            {
                /* find out how many vertices we have after triangulation */
                vert_count += bsp_face->n_meshverts;



                bsp_texture = bsp_textures + bsp_face->texture;
                face_batch = get_bsp_batch(bsp_texture->name, data);
                /* also, increment the number of vertices for the material 
                this face uses */
                face_batch->count += bsp_face->n_meshverts;
            }
        }

        
        // printf("1\n");

        /* now use the amount of vertices on each batch to
        compute where they start */
        batches = (struct batch_data_t*)data->batches.buffers[0];
        for(uint32_t i = 1; i < data->batches.cursor; i++)
        {
            batches[i].start = batches[i - 1].start + batches[i - 1].count;
            batches[i - 1].count = 0;   
        }
        batches[data->batches.cursor - 1].count = 0;
        
        // printf("2\n");

        data->vertices = create_list(sizeof(vec3_t), vert_count);
        data->normals = create_list(sizeof(vec3_t), vert_count);
        data->tex_coords = create_list(sizeof(vec2_t), vert_count);


        /* normally doing this would be unsafe, since each
        time the list resizes, a new buffer gets allocd and linked
        at the end. Here, however, we know exactly how many verts
        we have, so doing this makes things easier */
        vertices = (vec3_t*)data->vertices.buffers[0];
        normals = (vec3_t*)data->normals.buffers[0];
        tex_coords = (vec2_t*)data->tex_coords.buffers[0];

        for(uint32_t i = 0; i < face_count; i++)
        {
            bsp_face = bsp_faces + i;

            if(bsp_face->type == BSP_FACE_TYPE_POLYGON || bsp_face->type == BSP_FACE_TYPE_MESH)
            {
                bsp_face_meshverts = bsp_meshverts + bsp_face->meshvert;
                bsp_face_vertices = bsp_vertices + bsp_face->vertex;
                bsp_texture = bsp_textures + bsp_face->texture;

                for(uint32_t j = 0; j < bsp_face->n_meshverts; j++)
                {
                    bsp_vertex = bsp_face_vertices + bsp_face_meshverts[j].offset;
                    face_batch = get_bsp_batch(bsp_texture->name, data);
                    vert_index = face_batch->start + face_batch->count;

                    vertices[vert_index].comps[0] = bsp_vertex->position[0] * 0.1;
                    vertices[vert_index].comps[1] = bsp_vertex->position[2] * 0.1;
                    vertices[vert_index].comps[2] = bsp_vertex->position[1] * 0.1;

                    normals[vert_index].comps[0] = bsp_vertex->normal[0];
                    normals[vert_index].comps[1] = bsp_vertex->normal[2];
                    normals[vert_index].comps[2] = bsp_vertex->normal[1];

                    tex_coords[vert_index].comps[0] = bsp_vertex->texcoord[0][0];
                    tex_coords[vert_index].comps[1] = bsp_vertex->texcoord[0][1];

                    face_batch->count++;
                }    
            }
            else if(bsp_face->type == BSP_FACE_TYPE_PATCH)
            {
                // printf("fuck...\n");
            }
        }

        // for(uint32_t i = 0; i < data->batches.cursor; i++)
        // {
        //     printf("%d: start: %d -- count: %d\n", i, batches[i].start, batches[i].count);
        // }   

        data->vertices.cursor = vert_count;
        data->normals.cursor = vert_count;
        data->tex_coords.cursor = vert_count;

        // for(uint32_t i = 1; i < data->batches.cursor; i++)
        // {
        //     batches[0].count += batches[i].count;
        // }

        // data->batches.cursor = 1;

        
        // printf("3\n");
    }
}

struct batch_data_t *get_bsp_batch(char *name, struct geometry_data_t *data)
{
    struct batch_data_t *batch;

    for(uint32_t i = 0; i < data->batches.cursor; i++)
    {
        batch = (struct batch_data_t*)get_list_element(&data->batches, i);

        if(!strcmp(name, batch->material))
        {
            return batch;
        }
    }

    return NULL;
}