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
    test_faces[0] = create_face_with_shader(triangle_vertices, 3, smily_texture, 8, 8);
    
    // Quad face example - create with vertices first, then apply shader  
    vertex quad_vertices[4] = {
        {30.0f, 10.0f, 60.0f},
        {20.0f, 10.0f, 50.0f},
        {30.0f, 30.0f, 60.0f},
        {30.0f, 20.0f, 40.0f}
    };
    test_faces[1] = create_face_with_shader(quad_vertices, 4, smily_texture, 8, 8);
}



void draw_ship(face* faces) {
    // Base quad of the ship - create with vertices first, then apply shader
    vertex base_vertices[4] = {
        {-10.0f,-5.0f - 20, 0.0f + 20},
        {10.0f, -5.0f - 20, 0.0f + 20},
        {10.0f,  5.0f - 20, 0.0f + 20},
        {-10.0f, 5.0f - 20, 0.0f + 20}
    };
    faces[0] = create_face_with_shader(base_vertices, 4, smily_texture, 8, 8);

    // Folded quad for the ship's front - create with vertices first, then apply shader
    vertex front_vertices[4] = {
        {-10.0f, -5.0f - 20, 0.0f  + 20},
        {0.0f,   -5.0f - 20, 10.0f + 20},
        {0.0f,    5.0f - 20, 10.0f + 20},
        {-10.0f,  5.0f - 20, 0.0f  + 20}
    };
    faces[1] = create_face_with_shader(front_vertices, 4, smily_texture, 8, 8);

    // Folded quad for the ship's back - create with vertices first, then apply shader
    vertex back_vertices[4] = {
        {10.0f, -5.0f - 20, 0.0f  + 20},
        {0.0f,  -5.0f - 20, 10.0f + 20},
        {0.0f,   5.0f - 20, 10.0f + 20},
        {10.0f,  5.0f - 20, 0.0f  + 20}
    };
    faces[2] = create_face_with_shader(back_vertices, 4, smily_texture, 8, 8);

    // Left wing quad
    vertex left_wing_vertices[4] = {
        {-20.0f, -2.5f - 20, 5.0f + 20},
        {-10.0f, -2.5f - 20, 5.0f + 20},
        {-10.0f,  2.5f - 20, 5.0f + 20},
        {-20.0f,  2.5f - 20, 5.0f + 20}
    };
    faces[3] = create_face_with_shader(left_wing_vertices, 4, smily_texture, 8, 8);

    // Right wing quad
    vertex right_wing_vertices[4] = {
        {10.0f, -2.5f - 20, 5.0f + 20},
        {20.0f, -2.5f - 20, 5.0f + 20},
        {20.0f,  2.5f - 20, 5.0f + 20},
        {10.0f,  2.5f - 20, 5.0f + 20}
    };
    faces[4] = create_face_with_shader(right_wing_vertices, 4, smily_texture, 8, 8);

    // Tail quad
    vertex tail_vertices[4] = {
        {-5.0f, -5.0f - 20, -10.0f + 20},
        {5.0f,  -5.0f - 20, -10.0f + 20},
        {5.0f,   5.0f - 20, -10.0f + 20},
        {-5.0f,  5.0f - 20, -10.0f + 20}
    };
    faces[5] = create_face_with_shader(tail_vertices, 4, smily_texture, 8, 8);
}

void draw_abstract_model(face* faces) {
    // Central quad
    vertex central_vertices[4] = {
        {-5.0f, -5.0f, 0.0f},
        {5.0f, -5.0f, 0.0f},
        {5.0f, 5.0f, 0.0f},
        {-5.0f, 5.0f, 0.0f}
    };
    faces[0] = create_face_with_shader(central_vertices, 4, smily_texture, 8, 8);

    // Diagonal quad 1
    vertex diagonal1_vertices[4] = {
        {0.0f, 0.0f, 0.0f},
        {10.0f, 0.0f, 10.0f},
        {0.0f, 10.0f, 10.0f},
        {-10.0f, 0.0f, 0.0f}
    };
    faces[1] = create_face_with_shader(diagonal1_vertices, 4, smily_texture, 8, 8);

    // Diagonal quad 2
    vertex diagonal2_vertices[4] = {
        {0.0f, 0.0f, 0.0f},
        {0.0f, -10.0f, 10.0f},
        {10.0f, 0.0f, 10.0f},
        {0.0f, 10.0f, 0.0f}
    };
    faces[2] = create_face_with_shader(diagonal2_vertices, 4, smily_texture, 8, 8);

    // Vertical quad
    vertex vertical_vertices[4] = {
        {-2.0f, -10.0f, 5.0f},
        {2.0f, -10.0f, 5.0f},
        {2.0f, 10.0f, 5.0f},
        {-2.0f, 10.0f, 5.0f}
    };
    faces[3] = create_face_with_shader(vertical_vertices, 4, smily_texture, 8, 8);

    // Horizontal quad
    vertex horizontal_vertices[4] = {
        {-10.0f, -2.0f, 5.0f},
        {10.0f, -2.0f, 5.0f},
        {10.0f, 2.0f, 5.0f},
        {-10.0f, 2.0f, 5.0f}
    };
    faces[4] = create_face_with_shader(horizontal_vertices, 4, smily_texture, 8, 8);

    // Rotated quad
    vertex rotated_vertices[4] = {
        {-5.0f, -5.0f, 5.0f},
        {5.0f, -5.0f, 10.0f},
        {5.0f, 5.0f, 15.0f},
        {-5.0f, 5.0f, 10.0f}
    };
    faces[5] = create_face_with_shader(rotated_vertices, 4, smily_texture, 8, 8);
}

#endif // FACE_DRAWER_H
