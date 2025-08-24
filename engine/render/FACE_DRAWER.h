#ifndef FACE_DRAWER_H
#define FACE_DRAWER_H

#include "../shader/SHADER.h"
#include "FACE_TEXTURE.h"



void test_face_drawer(face* test_faces) {
    // Triangle face example - create with vertices first, then apply shader
    vertex triangle_vertices[3] = {
        {.0f, .0f, .0f},
        {-50.0f, .0f, .0f},
        {.0f, 50.0f, 0.0f}
    };
    test_faces[0] = create_face_with_shader(triangle_vertices, 3, heart_texture, 16, 16);
    
    // Quad face example - create with vertices first, then apply shader  
    vertex quad_vertices[4] = {
        {30.0f, 10.0f, 60.0f},
        {20.0f, 10.0f, 50.0f},
        {30.0f, 30.0f, 60.0f},
        {30.0f, 20.0f, 40.0f}
    };
    test_faces[1] = create_face_with_shader(quad_vertices, 4, heart_texture, 16, 16);
}



void draw_smily_cube(face* faces, float scale, float x, float y, float z) {
    // Cube positioned at (x, y, z), size adjustable by scale parameter
    float size = 5.0f * scale;
    // Front face (z = z + size)
    vertex front[4] = {
        {x - size, y - size, z + size},
        {x + size, y - size, z + size},
        {x + size, y + size, z + size},
        {x - size, y + size, z + size}
    };
    faces[0] = create_face_with_shader(front, 4, heart_texture, 16, 16);

    // Back face (z = z - size)
    vertex back[4] = {
        {x + size, y - size, z - size},
        {x - size, y - size, z - size},
        {x - size, y + size, z - size},
        {x + size, y + size, z - size}
    };
    faces[1] = create_face_with_shader(back, 4, heart_texture, 16, 16);

    // Left face (x = x - size)
    vertex left[4] = {
        {x - size, y - size, z - size},
        {x - size, y - size, z + size},
        {x - size, y + size, z + size},
        {x - size, y + size, z - size}
    };
    faces[2] = create_face_with_shader(left, 4, heart_texture, 16, 16);

    // Right face (x = x + size)
    vertex right[4] = {
        {x + size, y - size, z + size},
        {x + size, y - size, z - size},
        {x + size, y + size, z - size},
        {x + size, y + size, z + size}
    };
    faces[3] = create_face_with_shader(right, 4, heart_texture, 16, 16);

    // Top face (y = y + size)
    vertex top[4] = {
        {x - size, y + size, z + size},
        {x + size, y + size, z + size},
        {x + size, y + size, z - size},
        {x - size, y + size, z - size}
    };
    faces[4] = create_face_with_shader(top, 4, heart_texture, 16, 16);

    // Bottom face (y = y - size)
    vertex bottom[4] = {
        {x - size, y - size, z - size},
        {x + size, y - size, z - size},
        {x + size, y - size, z + size},
        {x - size, y - size, z + size}
    };
    faces[5] = create_face_with_shader(bottom, 4, heart_texture, 16, 16);
}

#endif // FACE_DRAWER_H
