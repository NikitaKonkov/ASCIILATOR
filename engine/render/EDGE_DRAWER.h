#ifndef EDGE_DRAWER_H
#define EDGE_DRAWER_H

#include "RENDER.h"
#include "../shader/SHADER.h"

void rgb_edge_cube(edge *e){
    // Draw edges of a cube with RGB colors
    e[0] = create_edge_with_shader((vertex){10.f,  10.f - 20, 10.f},   (vertex){-10.f, 10.f - 20, 10.f});
    e[1] = create_edge_with_shader((vertex){-10.f, 10.f - 20, 10.f},  (vertex){-10.f,  10.f - 20, -10.f});
    e[2] = create_edge_with_shader((vertex){-10.f, 10.f - 20, -10.f}, (vertex){10.f,   10.f - 20, -10.f});
    e[3] = create_edge_with_shader((vertex){10.f,  10.f - 20, -10.f},  (vertex){10.f,  10.f - 20, 10.f});
    e[4] = create_edge_with_shader((vertex){10.f, -10.f - 20, 10.f},  (vertex){-10.f, -10.f - 20, 10.f});
    e[5] = create_edge_with_shader((vertex){-10.f,-10.f - 20, 10.f}, (vertex){-10.f,  -10.f - 20, -10.f});
    e[6] = create_edge_with_shader((vertex){-10.f,-10.f - 20, -10.f},(vertex){10.f,   -10.f - 20, -10.f});
    e[7] = create_edge_with_shader((vertex){10.f, -10.f - 20, -10.f}, (vertex){10.f,  -10.f - 20, 10.f});
    e[8] = create_edge_with_shader((vertex){10.f,  10.f - 20, 10.f},   (vertex){10.f, -10.f - 20, 10.f});
    e[9] = create_edge_with_shader((vertex){-10.f, 10.f - 20, 10.f},  (vertex){-10.f, -10.f - 20, 10.f});
    e[10]= create_edge_with_shader((vertex){-10.f, 10.f - 20, -10.f}, (vertex){-10.f, -10.f - 20, -10.f});
    e[11]= create_edge_with_shader((vertex){10.f,  10.f - 20, -10.f},  (vertex){10.f, -10.f - 20, -10.f});
};

#endif // EDGE_DRAWER_H