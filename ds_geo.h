#ifndef DS_GEO
#define DS_GEO

#include "ds_vector.h"
//#include "utils.h"

// vec3_t support_vertex_upright_capsule(const vec3_t &dir, float height, float radius, vec3_t &normal);

// vec3_t support_vertex_triangle(const vec3_t &dir, const vec3_t &a, const vec3_t &b, const vec3_t &c);


float project_point_on_line(vec3_t *p, vec3_t *line_a, vec3_t *line_vec);

float dist_to_plane(vec3_t *point, vec3_t *plane_point, vec3_t *plane_normal);

void project_point_on_plane(vec3_t *r, vec3_t *point, vec3_t *plane_point, vec3_t *plane_normal);

vec3_t closest_point_on_triangle(vec3_t *p, vec3_t *v0, vec3_t *v1, vec3_t *v2);

int closest_point_line_triangle(vec3_t *v0, vec3_t *v1, vec3_t *v2, vec3_t *a, vec3_t *b, vec3_t *point_on_tri, vec3_t *point_on_line, float *line_t);

#ifdef DS_GEOMETRY_IMPLEMENTATION

float project_point_on_line(vec3_t *p, vec3_t *line_a, vec3_t *line_vec)
{
    vec3_t line_a_to_p;
    vec3_t_sub(&line_a_to_p, p, line_a);
    return vec3_t_dot(&line_a_to_p, line_vec) / vec3_t_dot(line_vec, line_vec);
}

float dist_to_plane(vec3_t *point, vec3_t *plane_point, vec3_t *plane_normal)
{
    vec3_t plane_point_to_point;
    vec3_t_sub(&plane_point_to_point, point, plane_point);
    return vec3_t_dot(&plane_point_to_point, plane_normal);
}

void project_point_on_plane(vec3_t *r, vec3_t *point, vec3_t *plane_point, vec3_t *plane_normal)
{
    vec3_t plane_point_to_point;
    vec3_t_sub(&plane_point_to_point, point, plane_point);
    vec3_t_fmadd(r, point, plane_normal, -vec3_t_dot(plane_normal, &plane_point_to_point));
}

vec3_t closest_point_on_triangle(vec3_t *p, vec3_t *v0, vec3_t *v1, vec3_t *v2)
{
    float edge_t;
    float tri_dist;
    int j;

    vec3_t tri_vec;
    vec3_t projected;
    vec3_t planes[3];
    vec3_t *edge;
    vec3_t *vert;
    vec3_t normal;
    float dists[3];

    vec3_t v0_to_v1;
    vec3_t v1_to_v2;
    vec3_t v2_to_v0;

    vec3_t_sub(&v0_to_v1, v1, v0);
    vec3_t_sub(&v1_to_v2, v2, v1);
    vec3_t_sub(&v2_to_v0, v0, v2);

    // normal = vec3_t_normalize(vec3_t_cross(c - b, b - a));
    vec3_t_cross(&normal, &v1_to_v2, &v0_to_v1);
    vec3_t_normalize(&normal, &normal);

    // projected = point - normal * vec3_t_dot(normal, point - a);
    project_point_on_plane(&projected, p, v0, &normal);

    /* build the edge planes' normals... */
    vec3_t_cross(&planes[0], &normal, &v0_to_v1);
    vec3_t_cross(&planes[1], &normal, &v1_to_v2);
    vec3_t_cross(&planes[2], &normal, &v2_to_v0);

    /* calculate the distance between the projected point and the edge planes... */
    dists[0] = dist_to_plane(&projected, v0, &planes[0]);
    dists[1] = dist_to_plane(&projected, v1, &planes[1]);
    dists[2] = dist_to_plane(&projected, v2, &planes[2]);


    if(dists[0] > 0.0 || dists[1] > 0.0 || dists[2] > 0.0)
    {
        /* the closest point on the triangle's plane has a positive distance
        to one of the edge planes, which means the point on the plane lies
        outside the triangle... */

        /* find to which edge the point on the plane is closer to...*/
        if(dists[0] > dists[1])
        {
            if(dists[0] > dists[2])
            {
                vert = v0;
                // edge = b - a;
                edge = &v0_to_v1;
            }
            else
            {
                vert = v2;
                // edge = a - c;
                edge = &v2_to_v0;
            }
        }
        else
        {
            if(dists[1] > dists[2])
            {
                vert = v1;
                // edge = c - b;
                edge = &v1_to_v2;
            }
            else
            {
                vert = v2;
                // edge = a - c;
                edge = &v2_to_v0;
            }
        }

        {
            /* project the point in the plane onto the closest edge... */
            // edge_t = vec3_t_dot(projected - vert, edge) / vec3_t_dot(edge, edge);
            vec3_t vert_to_projected;
            vec3_t_sub(&vert_to_projected, &projected, vert);
            edge_t = vec3_t_dot(&vert_to_projected, edge) / vec3_t_dot(edge, edge);
        }

        /* make sure the projection lies within the edge... */
        edge_t = clamp(0.0, edge_t, 1.0);
        vec3_t_fmadd(&projected, vert, edge, edge_t);
    }

    return projected;
}

int closest_point_line_triangle(vec3_t *v0, vec3_t *v1, vec3_t *v2, vec3_t *a, vec3_t *b, vec3_t *point_on_tri, vec3_t *point_on_line, float *line_t)
{
    float edge_time;
    float a_dist;
    float b_dist;

    vec3_t planes[3];
    vec3_t *edge;
    vec3_t *edge_vert;
    vec3_t *edge_normal;
//    vec3_t vert;
    vec3_t triangle_normal;
    vec3_t dists;
    vec3_t closest;
    vec3_t projected;
    vec3_t line_vec;

    vec3_t v0_to_v1;
    vec3_t v1_to_v2;
    vec3_t v2_to_v0;
    vec3_t v0_to_closest;

    vec3_t_sub(&v0_to_v1, v1, v0);
    vec3_t_sub(&v1_to_v2, v2, v1);
    vec3_t_sub(&v2_to_v0, v0, v2);
    vec3_t_sub(&line_vec, b, a);


    vec3_t_cross(&triangle_normal, &v1_to_v2, &v0_to_v1);
    vec3_t_normalize(&triangle_normal, &triangle_normal);

    /* build the edge planes. The normals point outwards,
    which means that a point that is inside the triangle
    has negative distance to all planes... */
    vec3_t_cross(&planes[0], &triangle_normal, &v0_to_v1);
    vec3_t_cross(&planes[1], &triangle_normal, &v1_to_v2);
    vec3_t_cross(&planes[2], &triangle_normal, &v2_to_v0);

    /* compute the line endpoints distances to the
    triangle plane */
    a_dist = dist_to_plane(a, v0, &triangle_normal);
    b_dist = dist_to_plane(b, v0, &triangle_normal);

    if(a_dist != b_dist)
    {
        /* line intersects the plane, so find
        the intersection time. */
        edge_time = clamp(0.0, a_dist / (a_dist - b_dist), 1.0);
    }
    else
    {
        /* line is parallel to plane. */
        edge_time = 0.0;
    }

    /* this handles the case of endpoints being inside the
    triangle. If the line intersects the triangles' plane,
    'closest' will be the intersection point. Otherwise, it'll
    be one of the endpoints. */
    vec3_t_fmadd(&closest, a, &line_vec, edge_time);

    /* compute the distance between 'closest' and
    the edge planes... */
    dists.x = dist_to_plane(&closest, v0, &planes[0]);
    dists.y = dist_to_plane(&closest, v1, &planes[1]);
    dists.z = dist_to_plane(&closest, v2, &planes[2]);

    /* project the closest point onto the triangle's plane. The point
    may be outside the triangle... */
    project_point_on_plane(&projected, &closest, v0, &triangle_normal);

    if(dists.x < 0.0 && dists.y < 0.0 && dists.z < 0.0)
    {
        /* closest point projects inside the triangle. */
        *point_on_tri = projected;
        *point_on_line = closest;
        *line_t = edge_time;
        return a_dist * b_dist < 0.0;
    }

    /* find which edge the projected closest point is closer to. */
    if(dists.x > dists.y)
    {
        if(dists.x > dists.z)
        {
            edge_vert = v0;
            edge = &v0_to_v1;
            edge_normal = &planes[0];
        }
        else
        {
            edge_vert = v2;
            edge = &v2_to_v0;
            edge_normal = &planes[2];
        }
    }
    else
    {
        if(dists.y > dists.z)
        {
            edge_vert = v1;
            edge = &v1_to_v2;
            edge_normal = &planes[1];
        }
        else
        {
            edge_vert = v2;
            edge = &v2_to_v0;
            edge_normal = &planes[2];
        }
    }

    /* compute the endpoints' distances to the
    edge plane... */
    a_dist = dist_to_plane(a, edge_vert, edge_normal);
    b_dist = dist_to_plane(b, edge_vert, edge_normal);

    if(a_dist * b_dist < 0.0)
    {
        /* line segment intersects the edge plane... */
        edge_time = a_dist / (a_dist - b_dist);
        vec3_t_fmadd(&closest, a, &line_vec, edge_time);
    }

    {
        // edge_t = vec3_t_dot(closest - edge_vert, edge) / vec3_t_dot(edge, edge);
        vec3_t edge_vert_to_closest;
        vec3_t_sub(&edge_vert_to_closest, &closest, edge_vert);
        edge_time = clamp(0.0, vec3_t_dot(&edge_vert_to_closest, edge) / vec3_t_dot(edge, edge), 1.0);
    }

    vec3_t_fmadd(point_on_tri, edge_vert, edge, edge_time);

    b_dist = vec3_t_dot(&line_vec, &line_vec);

    if(b_dist != 0.0)
    {
        // edge_t = vec3_t_dot(*point_on_tri - a, &line_vec) / b_dist;
        vec3_t point_on_tri_to_a;
        vec3_t_sub(&point_on_tri_to_a, point_on_tri, a);
        edge_time = clamp(0.0, vec3_t_dot(&point_on_tri_to_a, &line_vec) / b_dist, 1.0);
    }
    else
    {
        edge_time = 0.0;
    }

    vec3_t_fmadd(point_on_line, a, &line_vec, edge_time);
    *line_t = edge_time;

    return 0;
}

#endif

#endif // GEOMETRY_H
