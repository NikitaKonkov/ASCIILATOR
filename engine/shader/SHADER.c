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

// Calculate distance from camera to face center for depth calculation
float face_distance_calc(face f) {
    // Calculate face center
    float center_x = 0.0f, center_y = 0.0f, center_z = 0.0f;
    for (int i = 0; i < f.vertex_count; i++) {
        center_x += f.vertices[i].x;
        center_y += f.vertices[i].y;
        center_z += f.vertices[i].z;
    }
    center_x /= f.vertex_count;
    center_y /= f.vertex_count;
    center_z /= f.vertex_count;
    
    // Calculate distance from camera to face center
    float dx = center_x - camera.x;
    float dy = center_y - camera.y;
    float dz = center_z - camera.z;
    
    return sqrt(dx * dx + dy * dy + dz * dz);
}

// Depth-based ASCII character selection for faces
char face_ascii_depth(face f) {
    float distance = face_distance_calc(f);
    distance *= 3; // Scale distance for more variation
    
    // Map distance to discrete intervals
    int range = (int)(distance / 10);
    
    // Use a switch statement for faster lookup
    switch (range) {
        case 0: return '#';  // Very close - solid block
        case 1: return '@';  // Close - dense pattern
        case 2: return '&';  // Medium close - medium dense
        case 3: return '%';  // Medium - pattern
        case 4: return 'M';  // Medium far - letters
        case 5: return 'N';  // Far - letters
        case 6: return '*';  // Far - symbols
        case 7: return '+';  // Very far - light symbols
        case 8: return '|';  // Very far - lines
        case 9: return '-';  // Very far - dashes
        case 10: return ';'; // Far away - punctuation
        case 11: return ':'; // Far away - dots
        case 12: return '~'; // Very far - tilde
        case 13: return '_'; // Very far - underscore
        default: return '.'; // Extremely far - dot
    }
}

// Depth-based color selection for faces
int face_color_depth(face f) {
    float distance = face_distance_calc(f);
    distance *= 3; // Scale distance for more color variation
    
    // Map distance to discrete intervals
    int range = (int)(distance / 10);
    
    // Use a switch statement for faster lookup
    switch (range) {
        case 0: return 31;  // Red - very close
        case 1: return 32;  // Green - close
        case 2: return 33;  // Yellow - medium close
        case 3: return 34;  // Blue - medium
        case 4: return 35;  // Magenta - medium far
        case 5: return 36;  // Cyan - far
        case 6: return 37;  // White - far
        case 7: return 91;  // Bright red - very far
        case 8: return 92;  // Bright green - very far
        case 9: return 93;  // Bright yellow - very far
        case 10: return 94; // Bright blue - very far
        case 11: return 95; // Bright magenta - very far
        case 12: return 96; // Bright cyan - very far
        case 13: return 97; // Bright white - very far
        default: return 90; // Dark gray - extremely far
    }
}

// Calculate face normal vector
void calculate_face_normal(face f, float *nx, float *ny, float *nz) {
    if (f.vertex_count < 3) {
        *nx = *ny = *nz = 0.0f;
        return;
    }
    
    // Use first three vertices to calculate normal
    vertex v0 = f.vertices[0];
    vertex v1 = f.vertices[1]; 
    vertex v2 = f.vertices[2];
    
    // Calculate two edge vectors
    float edge1_x = v1.x - v0.x;
    float edge1_y = v1.y - v0.y;
    float edge1_z = v1.z - v0.z;
    
    float edge2_x = v2.x - v0.x;
    float edge2_y = v2.y - v0.y;
    float edge2_z = v2.z - v0.z;
    
    // Calculate cross product (normal)
    *nx = edge1_y * edge2_z - edge1_z * edge2_y;
    *ny = edge1_z * edge2_x - edge1_x * edge2_z;
    *nz = edge1_x * edge2_y - edge1_y * edge2_x;
    
    // Normalize the normal vector
    float length = sqrt((*nx) * (*nx) + (*ny) * (*ny) + (*nz) * (*nz));
    if (length > 0.001f) {
        *nx /= length;
        *ny /= length;
        *nz /= length;
    }
}

// Rotation shader for faces - changes character based on viewing angle
char face_rotation_shader(face f) {
    // Calculate face normal
    float nx, ny, nz;
    calculate_face_normal(f, &nx, &ny, &nz);
    
    // Calculate face center
    float center_x = 0.0f, center_y = 0.0f, center_z = 0.0f;
    for (int i = 0; i < f.vertex_count; i++) {
        center_x += f.vertices[i].x;
        center_y += f.vertices[i].y;
        center_z += f.vertices[i].z;
    }
    center_x /= f.vertex_count;
    center_y /= f.vertex_count;
    center_z /= f.vertex_count;
    
    // Calculate view direction from camera to face center
    float view_dx = center_x - camera.x;
    float view_dy = center_y - camera.y;
    float view_dz = center_z - camera.z;
    
    // Normalize view direction
    float view_len = sqrt(view_dx * view_dx + view_dy * view_dy + view_dz * view_dz);
    if (view_len < 0.001f) return '#'; // Fallback for zero-length vector
    
    view_dx /= view_len;
    view_dy /= view_len;
    view_dz /= view_len;
    
    // Calculate dot product between face normal and view direction
    float dot_product = nx * view_dx + ny * view_dy + nz * view_dz;
    
    // Calculate angle between normal and view direction
    float angle = acos(fabs(dot_product)); // Use absolute value for angle
    float angle_degrees = angle * 180.0f / 3.14159265358979323846f;
    
    // Choose character based on viewing angle
    if (angle_degrees < 15.0f) {
        return '#';     // Face-on view - solid block
    } else if (angle_degrees < 30.0f) {
        return '@';     // Slight angle - dense pattern
    } else if (angle_degrees < 45.0f) {
        return '&';     // Medium angle - medium pattern
    } else if (angle_degrees < 60.0f) {
        return '%';     // Sharp angle - sparse pattern
    } else if (angle_degrees < 75.0f) {
        return '*';     // Very sharp angle - stars
    } else {
        return '.';     // Edge-on view - dots
    }
}

// Combined face shader using both depth and rotation
face face_shader(face f) {
    // Create a new face with shaded properties
    face shaded_face = f; // Copy original face structure
    
    // Apply depth-based shading
    shaded_face.ascii = face_ascii_depth(f);
    shaded_face.color = face_color_depth(f);
    
    return shaded_face;
}

// Face shader using rotation-based character selection
face face_rotation_shader_face(face f) {
    // Create a new face with rotation-based shading
    face shaded_face = f; // Copy original face structure
    
    // Apply rotation-based character selection
    shaded_face.ascii = face_rotation_shader(f);
    
    // Use distance-based color but keep rotation-based character
    shaded_face.color = face_color_depth(f);
    
    return shaded_face;
}

// Create face with combined depth and rotation shading
face create_face_with_shader(vertex *vertices, int vertex_count, int *texture, int texture_width, int texture_height) {
    face f;
    f.vertex_count = vertex_count;
    
    // Copy vertices
    for (int i = 0; i < vertex_count && i < 4; i++) {
        f.vertices[i] = vertices[i];
    }
    
    // Set texture properties
    f.texture = texture;
    f.texture_width = texture_width;
    f.texture_height = texture_height;
    
    // Apply rotation-based shading for character
    f.ascii = face_rotation_shader(f);
    
    // Apply depth-based shading for color
    f.color = face_color_depth(f);
    
    return f;
}

