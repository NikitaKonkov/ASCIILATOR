#include <math.h>
#include <stdlib.h>
#include "SHADER.h"
#include "../camera/CAMERA.h"

float edge_distance_calc(vertex start, vertex end) {
    // Calculate Euclidean distance between two vertices
    float mid_x = (start.x + end.x) / 2.0f;
    float mid_y = (start.y + end.y) / 2.0f;
    float mid_z = (start.z + end.z) / 2.0f;
    float distance = sqrt((mid_x - camera.x) * (mid_x - camera.x) + 
                         (mid_y - camera.y) * (mid_y - camera.y) + 
                         (mid_z - camera.z) * (mid_z - camera.z));
}

float dot_distance_calc(vertex v) {
    // Calculate Euclidean distance from camera to vertex
    float dx = v.x - camera.x;
    float dy = v.y - camera.y;
    float dz = v.z - camera.z;

    return sqrt(dx * dx + dy * dy + dz * dz);
}

char edge_ascii_depth(vertex start, vertex end) {
    float distance = edge_distance_calc(start, end);
    distance *= 5; // Scale distance for more color variation

    // Map distance to discrete intervals
    int range = (int)(distance / 10);

    // Use a switch statement for faster lookup
    switch (range) {
        case 0: return '#';
        case 1: return '@';
        case 2: return '&';
        case 3: return '%';
        case 4: return 'M';
        case 5: return 'N';
        case 6: return '*';
        case 7: return '+';
        case 8: return '|';
        case 9: return '-';
        case 10: return ';';
        case 11: return ':';
        case 12: return '~';
        case 13: return '_';
        default: return '.'; // Far
    }
}

int edge_color_depth(vertex start, vertex end) {
    float distance = edge_distance_calc(start, end);
    distance *= 5; // Scale distance for more color variation

    // Map distance to discrete intervals
    int range = (int)(distance / 10);

    // Use a switch statement for faster lookup
    switch (range) {
        case 0: return 31;
        case 1: return 32;
        case 2: return 33;
        case 3: return 34;
        case 4: return 35;
        case 5: return 36;
        case 6: return 37;
        case 7: return 91;
        case 8: return 92;
        case 9: return 93;
        case 10: return 94;
        case 11: return 95;
        case 12: return 96;
        case 13: return 97;
        default: return 90; // Far
    }
}
edge edge_shader(vertex v1, vertex v2) {
    // Create an edge with depth and color based on the vertices
    edge e;
    e.start = v1;
    e.end = v2;
    e.ascii = edge_ascii_depth(v1, v2);
    e.color = edge_color_depth(v1, v2);
    return e;
}

char dot_ascii_depth(vertex v) {
    float distance = dot_distance_calc(v);
    distance *= 2; // Scale distance for more color variation

    // Map distance to discrete intervals
    int range = (int)(distance / 10);

    // Use a switch statement for faster lookup
    switch (range) {
        case 0: return '#';
        case 1: return '@';
        case 2: return '&';
        case 3: return '%';
        case 4: return 'M';
        case 5: return 'N';
        case 6: return '*';
        case 7: return '+';
        case 8: return '|';
        case 9: return '-';
        case 10: return ';';
        case 11: return ':';
        case 12: return '~';
        default: return '.'; // Far
    }
}

int dot_color_depth(vertex v) {
    float distance = dot_distance_calc(v);
    distance *= 2; // Scale distance for more color variation

    // Map distance to discrete intervals
    int range = (int)(distance / 10);

    // Use a switch statement for faster lookup
    switch (range) {
        case 0: return 31;
        case 1: return 32;
        case 2: return 33;
        case 3: return 34;
        case 4: return 35;
        case 5: return 36;
        case 6: return 37;
        case 7: return 91;
        case 8: return 92;
        case 9: return 93;
        case 10: return 94;
        case 11: return 95;
        case 12: return 96;
        case 13: return 97;
        default: return 90; // Far
    }
}

dot dot_shader(vertex v) {
    // Create a dot with depth and color based on the vertex
    dot d;
    d.position = v;
    d.ascii = dot_ascii_depth(v); // Use same vertex for depth calculation
    d.color = dot_color_depth(v); // Use same vertex for color calculation
    return d;
}

// Rotation shader - changes character based on viewing angle
char edge_rotation_shader(angle e) {
    // Calculate edge direction vector
    float edge_dx = e.a[1].x - e.a[0].x;
    float edge_dy = e.a[1].y - e.a[0].y;
    float edge_dz = e.a[1].z - e.a[0].z;
    
    // Calculate edge midpoint
    float mid_x = (e.a[0].x + e.a[1].x) * 0.5f;
    float mid_y = (e.a[0].y + e.a[1].y) * 0.5f;
    float mid_z = (e.a[0].z + e.a[1].z) * 0.5f;
    
    // Calculate view direction from camera to edge midpoint
    float view_dx = mid_x - camera.x;
    float view_dy = mid_y - camera.y;
    float view_dz = mid_z - camera.z;
    
    // Normalize vectors
    float edge_len = sqrt(edge_dx * edge_dx + edge_dy * edge_dy + edge_dz * edge_dz);
    float view_len = sqrt(view_dx * view_dx + view_dy * view_dy + view_dz * view_dz);
    
    if (edge_len < 0.001f || view_len < 0.001f) return '-'; // Fallback for zero-length vectors
    
    edge_dx /= edge_len;
    edge_dy /= edge_len;
    edge_dz /= edge_len;
    view_dx /= view_len;
    view_dy /= view_len;
    view_dz /= view_len;
    
    // Calculate dot product to get angle between edge and view direction
    float dot_product = edge_dx * view_dx + edge_dy * view_dy + edge_dz * view_dz;
    
    // Calculate cross product magnitude for perpendicular component
    float cross_x = edge_dy * view_dz - edge_dz * view_dy;
    float cross_y = edge_dz * view_dx - edge_dx * view_dz;
    float cross_z = edge_dx * view_dy - edge_dy * view_dx;
    float cross_magnitude = sqrt(cross_x * cross_x + cross_y * cross_y + cross_z * cross_z);
    
    // Calculate angle in radians
    float angle = atan2(cross_magnitude, fabs(dot_product));
    
    // Convert angle to degrees and normalize to 0-360
    float angle_degrees = angle * 180.0f / 3.14159265358979323846f;
    int angle_int = (int)(angle_degrees * 4) % 360; // Multiply by 4 for more sensitivity
    
    // Choose character based on angle ranges
    if (angle_int < 45) return '=';         // Nearly parallel
    else if (angle_int < 90) return '\\';   // Slight angle
    else if (angle_int < 135) return '|';   // Medium angle
    else if (angle_int < 180) return '/';   // Sharp angle
    else if (angle_int < 225) return '-';   // Perpendicular
    else if (angle_int < 270) return '\\';  // Sharp angle (other side)
    else if (angle_int < 315) return '|';   // Medium angle (other side)
    else return '/';                        // Slight angle (other side)
}


edge create_edge_with_shader(vertex start, vertex end) {
    edge edge;
    edge.start = start;
    edge.end = end;
    edge.ascii = edge_rotation_shader((angle){start, end}); // Use rotation shader
    edge.color = (rand() % 7) + 31; // Random color between 31 and 37
    return edge;
}

