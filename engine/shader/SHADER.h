#ifndef SHADER_H
#define SHADER_H

// Vertex structure for 3D coordinates
typedef struct {
    float x, y, z;
} vertex;

// Face structure for triangles and quads with texture support
typedef struct {
    vertex vertices[4]; // Up to 4 vertices (3 for triangle, 4 for quad)
    int vertex_count;   // 3 for triangle, 4 for quad
    int *texture;       // Pointer to texture array
    int texture_width;  // Width of texture
    int texture_height; // Height of texture
    int color;          // Base color if no texture
    char ascii;         // ASCII character for face
} face;

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

// Face shader functions
float face_distance_calc(face f);
char face_ascii_depth(face f);
int face_color_depth(face f);
void calculate_face_normal(face f, float *nx, float *ny, float *nz);
char face_rotation_shader(face f);
face face_shader(face f);
face face_rotation_shader_face(face f);
face create_face_with_shader(vertex *vertices, int vertex_count, int *texture, int texture_width, int texture_height);

#endif // SHADER_H