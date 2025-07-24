#ifndef RASTERIZER_H
#define RASTERIZER_H
#include "../shader/SHADER.h"
#include <math.h>
#define M_PI 3.14159265358979323846
#define HASH_MAP_SIZE 256*256  // 512^2 Distribution for hash map

// Buffer positions for drawing
extern int frame_buffer_pos;

// Frame buffer pixel structure
typedef struct {
    char ascii;
    int color;
    float depth;
    int valid;  // 1 if pixel is drawn, 0 if empty
} pixel;

// 2D frame buffer for pixel-perfect rendering
extern pixel screen_buffer[2560][2560];
extern pixel previous_screen_buffer[2560][2560];  // Previous frame buffer for comparison
extern int screen_width;
extern int screen_height;

// Buffer dimensions
extern unsigned int cmd_buffer_width; 
extern unsigned int cmd_buffer_height;



// Unified renderable object for depth sorting
typedef struct {
    int type; // 0 = edge, 1 = dot, 2 = face
    union {
        edge e;
        dot d;
        face f;
    } object;
    float depth;
} renderable;


int set_pixel(int x, int y, char ascii, int color, float depth);
void set_aspect_ratio(float width_scale, float height_scale);
void get_aspect_ratio(float *width_scale, float *height_scale);
float calculate_dot_distance(dot d);
float calculate_edge_distance(edge e);
float calculate_face_distance(face f);
vertex project_vertex(vertex v, float cam_x, float cam_y, float cam_z, float cam_yaw, float cam_pitch, float fov, float aspect_ratio, float near_plane);
float calculate_edge_depth(edge e);
float calculate_renderable_depth(renderable r);
int compare_renderables_by_depth(const void *a, const void *b);
void draw_dot(dot d);
void draw_edge(edge e);
void draw_face(face f);

#endif // RASTERIZER_H