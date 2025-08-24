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



void draw_hearth_cube(face* faces, float scale, float x, float y, float z) {
    // Cube positioned at (x, y, z), size adjustable by scale parameter
    float size = 5.0f * scale;
    
    // Face vertex offsets: [x1,y1,z1, x2,y2,z2, x3,y3,z3, x4,y4,z4] relative to size
    float face_offsets[6][12] = {
        // Front face (z = z + size)
        {-1,-1, 1,  1,-1, 1,  1, 1, 1, -1, 1, 1},
        // Back face (z = z - size) 
        { 1,-1,-1, -1,-1,-1, -1, 1,-1,  1, 1,-1},
        // Left face (x = x - size)
        {-1,-1,-1, -1,-1, 1, -1, 1, 1, -1, 1,-1},
        // Right face (x = x + size)
        { 1,-1, 1,  1,-1,-1,  1, 1,-1,  1, 1, 1},
        // Top face (y = y + size)
        {-1, 1, 1,  1, 1, 1,  1, 1,-1, -1, 1,-1},
        // Bottom face (y = y - size)
        {-1,-1,-1,  1,-1,-1,  1,-1, 1, -1,-1, 1}
    };
    
    // Generate all faces using data-driven approach
    for (int face_idx = 0; face_idx < 6; face_idx++) {
        vertex face_vertices[4];
        for (int vert = 0; vert < 4; vert++) {
            int offset_idx = vert * 3;
            face_vertices[vert].x = x + size * face_offsets[face_idx][offset_idx];
            face_vertices[vert].y = y + size * face_offsets[face_idx][offset_idx + 1];
            face_vertices[vert].z = z + size * face_offsets[face_idx][offset_idx + 2];
        }
        faces[face_idx] = create_face_with_shader(face_vertices, 4, heart_texture, 16, 16);
    }
}

void draw_pyramid(face* faces, float scale, float x, float y, float z) {
    float size = 5.0f * scale;
    
    // Pyramid vertices (5 vertices total)
    vertex pyramid_verts[5] = {
        // Base vertices (square base)
        {x - size, y - size, z - size}, // 0: bottom-left-back
        {x + size, y - size, z - size}, // 1: bottom-right-back
        {x + size, y - size, z + size}, // 2: bottom-right-front
        {x - size, y - size, z + size}, // 3: bottom-left-front
        {x, y + size, z}                // 4: apex (top point)
    };
    
    // Create 5 faces: 1 square base + 4 triangular sides
    
    // Base face (square) - looking up from below
    vertex base[4] = {pyramid_verts[0], pyramid_verts[1], pyramid_verts[2], pyramid_verts[3]};
    faces[0] = create_face_with_shader(base, 4, checkerboard_texture, 16, 16);
    
    // Front triangular face
    vertex front[3] = {pyramid_verts[3], pyramid_verts[2], pyramid_verts[4]};
    faces[1] = create_face_with_shader(front, 3, checkerboard_texture, 16, 16);
    
    // Right triangular face  
    vertex right[3] = {pyramid_verts[2], pyramid_verts[1], pyramid_verts[4]};
    faces[2] = create_face_with_shader(right, 3, checkerboard_texture, 16, 16);
    
    // Back triangular face
    vertex back[3] = {pyramid_verts[1], pyramid_verts[0], pyramid_verts[4]};
    faces[3] = create_face_with_shader(back, 3, checkerboard_texture, 16, 16);
    
    // Left triangular face
    vertex left[3] = {pyramid_verts[0], pyramid_verts[3], pyramid_verts[4]};
    faces[4] = create_face_with_shader(left, 3, checkerboard_texture, 16, 16);
}

#endif // FACE_DRAWER_H
