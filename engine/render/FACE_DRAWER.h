#ifndef FACE_DRAWER_H
#define FACE_DRAWER_H

#include "../shader/SHADER.h"
#include "FACE_TEXTURE.h"



void test_face_drawer(face* test_faces) {
    // Triangle face example
    test_faces[0].vertex_count = 3;
    test_faces[0].vertices[0] = (vertex){.0f, .0f, .0f};
    test_faces[0].vertices[1] = (vertex){-50.0f, .0f, .0f};
    test_faces[0].vertices[2] = (vertex){.0f, 50.0f, 0.0f};
    test_faces[0].texture = smily_texture;
    test_faces[0].texture_width = 8;
    test_faces[0].texture_height = 8;
    test_faces[0].color = 93; // Bright yellow fallback
    test_faces[0].ascii = '#';
    
    // Quad face example
    test_faces[1].vertex_count = 4;
    test_faces[1].vertices[0] = (vertex){30.0f, 10.0f, 60.0f};
    test_faces[1].vertices[1] = (vertex){20.0f, 10.0f, 50.0f};
    test_faces[1].vertices[2] = (vertex){30.0f, 30.0f, 60.0f};
    test_faces[1].vertices[3] = (vertex){30.0f, 20.0f, 40.0f};
    test_faces[1].texture = smily_texture;
    test_faces[1].texture_width = 8;
    test_faces[1].texture_height = 8;
    test_faces[1].color = 96; // Bright cyan fallback
    test_faces[1].ascii = '@';
}

#endif // FACE_DRAWER_H
