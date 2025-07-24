#ifndef SHADER_H
#define SHADER_H

// Vertex structure for 3D coordinates
typedef struct {
    float x, y, z;
} vertex;

// Edge structure for drawing lines between vertices
typedef struct {
    vertex start, end;
    char ascii;
    int color;
} edge;

// Dot structure for drawing single points
typedef struct {
    vertex position;
    char ascii;
    int color;
} dot;

// Function to rotate a vertex around the camera's yaw and pitch
typedef struct {
    vertex a[2];
} angle;

char edge_ascii_depth(vertex start, vertex end);
int edge_color_depth(vertex start, vertex end);
char dot_ascii_depth(vertex v);
int dot_color_depth(vertex v);
char edge_rotation_shader(angle e);
float dot_distance_calc(vertex v);
float edge_distance_calc(vertex start, vertex end);
dot dot_shader(vertex v);
edge edge_shader(vertex start, vertex end);
edge create_edge_with_shader(vertex start, vertex end);

#endif // SHADER_H