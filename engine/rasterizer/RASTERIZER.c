#include "RASTERIZER.h"
#include "../shader/SHADER.h"
#include "../camera/CAMERA.h"
// Buffer positions for drawing
int frame_buffer_pos = 0;

pixel screen_buffer[2560][2560];
pixel previous_screen_buffer[2560][2560];  // Previous frame buffer for comparison
int screen_width = 0;
int screen_height = 0;

// Buffer dimensions
unsigned int cmd_buffer_width; 
unsigned int cmd_buffer_height;



// Set pixel in frame buffer with depth testing
int set_pixel(int x, int y, char ascii, int color, float depth) {
    // Bounds checking
    if (x < 1 || y < 1 || x > screen_width || y > screen_height || 
        x >= 2560 || y >= 2560) {
        return 0;
    }
    
    // Convert to 0-based indexing for buffer
    int buf_x = x - 1;
    int buf_y = y - 1;
    
    // Depth test - only draw if closer or position is empty
    if (!screen_buffer[buf_y][buf_x].valid || depth < screen_buffer[buf_y][buf_x].depth) {
        screen_buffer[buf_y][buf_x].ascii = ascii;
        screen_buffer[buf_y][buf_x].color = color;
        screen_buffer[buf_y][buf_x].depth = depth;
        screen_buffer[buf_y][buf_x].valid = 1;
        return 1;
    }
    
    return 0;
}

// Function to set aspect ratio correction
void set_aspect_ratio(float width_scale, float height_scale) {
    aspect_ratio_width = width_scale;
    aspect_ratio_height = height_scale;
}

// Function to get current aspect ratio settings
void get_aspect_ratio(float *width_scale, float *height_scale) {
    *width_scale = aspect_ratio_width;
    *height_scale = aspect_ratio_height;
}

// Calculate distance from camera to dot (for view distance culling)
float calculate_dot_distance(dot d) {
    // Calculate Euclidean distance from camera to dot
    float dx = d.position.x - camera.x;
    float dy = d.position.y - camera.y;
    float dz = d.position.z - camera.z;
    return sqrt(dx * dx + dy * dy + dz * dz);
}

// Calculate distance from camera to edge (for view distance culling)
float calculate_edge_distance(edge e) {
    // Calculate distance based on the average position of the edge
    float mid_x = (e.start.x + e.end.x) * 0.5f;
    float mid_y = (e.start.y + e.end.y) * 0.5f;
    float mid_z = (e.start.z + e.end.z) * 0.5f;
    // Calculate Euclidean distance from camera to edge midpoint
    float dx = mid_x - camera.x;
    float dy = mid_y - camera.y;
    float dz = mid_z - camera.z;
    
    return sqrt(dx * dx + dy * dy + dz * dz);
}

// Calculate distance from camera to face (for view distance culling)
float calculate_face_distance(face f) {
    // Calculate distance based on the average position of the face vertices
    float total_x = 0.0f, total_y = 0.0f, total_z = 0.0f;
    for (int i = 0; i < f.vertex_count; i++) {
        total_x += f.vertices[i].x;
        total_y += f.vertices[i].y;
        total_z += f.vertices[i].z;
    }
    float avg_x = total_x / f.vertex_count;
    float avg_y = total_y / f.vertex_count;
    float avg_z = total_z / f.vertex_count;
    
    // Calculate Euclidean distance from camera to face center
    float dx = avg_x - camera.x;
    float dy = avg_y - camera.y;
    float dz = avg_z - camera.z;
    
    return sqrt(dx * dx + dy * dy + dz * dz);
}

vertex project_vertex(vertex v, float cam_x, float cam_y, float cam_z, float cam_yaw, float cam_pitch, float fov, float aspect_ratio, float near_plane) {
    // Translate vertex relative to camera
    float dx = v.x - cached_transform.cam_x;
    float dy = v.y - cached_transform.cam_y;
    float dz = v.z - cached_transform.cam_z;

    // Rotate around yaw (Y-axis) using cached values
    float temp_x = dx * cached_transform.cos_yaw - dz * cached_transform.sin_yaw;
    float temp_z = dx * cached_transform.sin_yaw + dz * cached_transform.cos_yaw;
    dx = temp_x;
    dz = temp_z;

    // Rotate around pitch (X-axis) using cached values
    float temp_y = dy * cached_transform.cos_pitch - dz * cached_transform.sin_pitch;
    temp_z = dy * cached_transform.sin_pitch + dz * cached_transform.cos_pitch;
    dy = temp_y;
    dz = temp_z;

    // Perspective projection with aspect ratio correction
    if (dz <= 0) dz = near_plane; // Avoid division by zero
    
    // Apply aspect ratio correction to compensate for console character stretching
    float screen_x = (dx / dz) * (cmd_buffer_width / 2) / tan(fov * 0.5 * M_PI / 180.0f) * aspect_ratio_width + (cmd_buffer_width / 2);
    float screen_y = (dy / dz) * (cmd_buffer_height / 2) / tan(fov * 0.5 * M_PI / 180.0f) * aspect_ratio_height + (cmd_buffer_height / 2);

    vertex projected = {screen_x, screen_y, dz};
    return projected;
}

// Calculate depth of an edge for sorting
float calculate_edge_depth(edge e) {
    // Calculate depth based on the average Z value of the edge after camera transformation
    float mid_x = (e.start.x + e.end.x) * 0.5f;  // Use multiplication instead of division
    float mid_y = (e.start.y + e.end.y) * 0.5f;
    float mid_z = (e.start.z + e.end.z) * 0.5f;
    
    // Transform relative to camera position
    float dx = mid_x - camera.x;
    float dy = mid_y - camera.y;
    float dz = mid_z - camera.z;
    
    // Apply camera rotation (yaw and pitch) to get depth in camera space
    // Rotate around yaw (Y-axis)
    float temp_x = dx * cached_transform.cos_yaw - dz * cached_transform.sin_yaw;
    float temp_z = dx * cached_transform.sin_yaw + dz * cached_transform.cos_yaw;
    dx = temp_x;
    dz = temp_z;
    
    // Rotate around pitch (X-axis)
    float temp_y = dy * cached_transform.cos_pitch - dz * cached_transform.sin_pitch;
    temp_z = dy * cached_transform.sin_pitch + dz * cached_transform.cos_pitch;
    dy = temp_y;
    dz = temp_z;
    
    return dz; // Return depth in camera space (positive = in front of camera)
}

// Calculate depth for any renderable object
float calculate_renderable_depth(renderable r) {
    if (r.type == 0) { // edge
        return calculate_edge_depth(r.object.e);
    } else if (r.type == 1) { // dot
        // Calculate depth for dot similar to edge depth calculation
        vertex pos = r.object.d.position;
        float dx = pos.x - camera.x;
        float dy = pos.y - camera.y;
        float dz = pos.z - camera.z;
        
        // Apply camera rotation to get depth in camera space
        float temp_x = dx * cached_transform.cos_yaw - dz * cached_transform.sin_yaw;
        float temp_z = dx * cached_transform.sin_yaw + dz * cached_transform.cos_yaw;
        dx = temp_x;
        dz = temp_z;
        
        float temp_y = dy * cached_transform.cos_pitch - dz * cached_transform.sin_pitch;
        temp_z = dy * cached_transform.sin_pitch + dz * cached_transform.cos_pitch;
        dy = temp_y;
        dz = temp_z;
        
        return dz;
    } else { // face
        // Calculate average depth of face vertices
        face f = r.object.f;
        float total_depth = 0.0f;
        for (int i = 0; i < f.vertex_count; i++) {
            vertex pos = f.vertices[i];
            float dx = pos.x - camera.x;
            float dy = pos.y - camera.y;
            float dz = pos.z - camera.z;
            
            // Apply camera rotation to get depth in camera space
            float temp_x = dx * cached_transform.cos_yaw - dz * cached_transform.sin_yaw;
            float temp_z = dx * cached_transform.sin_yaw + dz * cached_transform.cos_yaw;
            dx = temp_x;
            dz = temp_z;
            
            float temp_y = dy * cached_transform.cos_pitch - dz * cached_transform.sin_pitch;
            temp_z = dy * cached_transform.sin_pitch + dz * cached_transform.cos_pitch;
            dy = temp_y;
            dz = temp_z;
            
            total_depth += dz;
        }
        return total_depth / f.vertex_count; // Average depth
    }
}

// Comparison function for sorting renderables by depth (back to front)
int compare_renderables_by_depth(const void *a, const void *b) {
    renderable *r_a = (renderable *)a;
    renderable *r_b = (renderable *)b;
    
    float depth_a = r_a->depth;
    float depth_b = r_b->depth;
    
    // Sort from farthest to nearest (back to front for painter's algorithm)
    if (depth_a > depth_b) return -1;  // a is farther, should come first
    if (depth_a < depth_b) return 1;   // b is farther, should come first
    return 0;
}

// Dot drawing function
void draw_dot(dot d) {
    // Camera parameters
    float cam_x = camera.x, cam_y = camera.y, cam_z = camera.z;
    float cam_yaw = camera.yaw, cam_pitch = camera.pitch;
    float fov = 90.0f, aspect_ratio = (float)cmd_buffer_width / (float)cmd_buffer_height;
    float near_plane = 0.1f;

    // Project the dot position
    vertex projected = project_vertex(d.position, cam_x, cam_y, cam_z, cam_yaw, cam_pitch, fov, aspect_ratio, near_plane);

    // Skip if behind camera
    if (projected.z < culling_distance) return;
    
    // View distance culling
    float dot_distance = calculate_dot_distance(d);
    if (dot_distance > view_distance) return;

    // Check if dot is within screen bounds
    int screen_x = (int)projected.x + 1;
    int screen_y = (int)projected.y + 1;
    
    if (screen_x >= 1 && screen_x <= (int)cmd_buffer_width && 
        screen_y >= 1 && screen_y <= (int)cmd_buffer_height) {
        
        // Calculate dot depth for Z-buffering
        vertex pos = d.position;
        float dx = pos.x - camera.x;
        float dy = pos.y - camera.y;
        float dz = pos.z - camera.z;
        
        // Apply camera rotation to get depth in camera space
        float temp_x = dx * cached_transform.cos_yaw - dz * cached_transform.sin_yaw;
        float temp_z = dx * cached_transform.sin_yaw + dz * cached_transform.cos_yaw;
        dx = temp_x;
        dz = temp_z;
        
        float temp_y = dy * cached_transform.cos_pitch - dz * cached_transform.sin_pitch;
        temp_z = dy * cached_transform.sin_pitch + dz * cached_transform.cos_pitch;
        dy = temp_y;
        dz = temp_z;
        
        float current_depth = dz;
        set_pixel(screen_x, screen_y, d.ascii, d.color, current_depth);
    }
}

// Edge drawing function
void draw_edge(edge e) {
    // Camera parameters
    float cam_x = camera.x, cam_y = camera.y, cam_z = camera.z;
    float cam_yaw = camera.yaw, cam_pitch = camera.pitch;
    float fov = 90.0f, aspect_ratio = (float)cmd_buffer_width / (float)cmd_buffer_height;
    float near_plane = 0.1f;

    // Project vertices
    vertex start_proj = project_vertex(e.start, cam_x, cam_y, cam_z, cam_yaw, cam_pitch, fov, aspect_ratio, near_plane);
    vertex end_proj = project_vertex(e.end, cam_x, cam_y, cam_z, cam_yaw, cam_pitch, fov, aspect_ratio, near_plane);

    // Skip if behind camera
    if (start_proj.z < culling_distance && end_proj.z < culling_distance) return;
    
    // View distance culling
    float edge_distance = calculate_edge_distance(e);
    if (edge_distance > view_distance) return;

    // Draw the edge using Bresenham's algorithm
    int x1 = (int)start_proj.x, y1 = (int)start_proj.y;
    int x2 = (int)end_proj.x, y2 = (int)end_proj.y;
    int dx = abs(x2 - x1), sx = x1 < x2 ? 1 : -1;
    int dy = -abs(y2 - y1), sy = y1 < y2 ? 1 : -1;
    int err = dx + dy, e2;

    while (1) {
        if (x1 >= 0 && x1 < (int)cmd_buffer_width && y1 >= 0 && y1 < (int)cmd_buffer_height) {
            int screen_x = x1 + 1, screen_y = y1 + 1;
            float current_depth = calculate_edge_depth(e);
            set_pixel(screen_x, screen_y, e.ascii, e.color, current_depth);
        }
        if (x1 == x2 && y1 == y2) break;
        e2 = 2 * err;
        if (e2 >= dy) { err += dy; x1 += sx; }
        if (e2 <= dx) { err += dx; y1 += sy; }
    }
}

// Face drawing function with perspective-correct rasterization "brain damage version"
void draw_face(face f) {
    // Camera parameters
    float cam_x = camera.x, cam_y = camera.y, cam_z = camera.z;
    float cam_yaw = camera.yaw, cam_pitch = camera.pitch;
    float fov = 90.0f, aspect_ratio = (float)cmd_buffer_width / (float)cmd_buffer_height;
    float near_plane = 0.1f;

    // View distance culling
    float face_distance = calculate_face_distance(f);
    if (face_distance > view_distance) return;

    // Project all vertices
    vertex projected[4];
    int valid_vertices = 0;
    
    // Check if face has valid vertices
    for (int i = 0; i < f.vertex_count; i++) {
        projected[i] = project_vertex(f.vertices[i], cam_x, cam_y, cam_z, cam_yaw, cam_pitch, fov, aspect_ratio, near_plane);
        if (projected[i].z >= culling_distance) {
            valid_vertices++;
        }
    }
    
    // Skip if all vertices are behind camera
    if (valid_vertices == 0) return;
    
    // Perspective-correct scanline rasterization for triangles and quads
    if (f.vertex_count == 3) {
        // Triangle rasterization with perspective correction
        vertex v0 = projected[0], v1 = projected[1], v2 = projected[2];
        
        // Store original world-space depths for perspective correction
        float w0 = 1.0f / v0.z, w1 = 1.0f / v1.z, w2 = 1.0f / v2.z;
        
        // Find bounding box
        int min_x = (int)fmin(fmin(v0.x, v1.x), v2.x);
        int max_x = (int)fmax(fmax(v0.x, v1.x), v2.x);
        int min_y = (int)fmin(fmin(v0.y, v1.y), v2.y);
        int max_y = (int)fmax(fmax(v0.y, v1.y), v2.y);
        
        // Clamp to screen bounds
        min_x = fmax(0, min_x);
        max_x = fmin(cmd_buffer_width - 1, max_x);
        min_y = fmax(0, min_y);
        max_y = fmin(cmd_buffer_height - 1, max_y);
        
        // Precompute triangle area for barycentric coordinates
        float area = (v1.x - v0.x) * (v2.y - v0.y) - (v2.x - v0.x) * (v1.y - v0.y);
        if (fabs(area) < 0.001f) return; // Degenerate triangle
        
        // Rasterize triangle with perspective correction
        for (int y = min_y; y <= max_y; y++) {
            for (int x = min_x; x <= max_x; x++) {
                // Calculate barycentric coordinates more efficiently
                float lambda1 = ((v2.y - v0.y) * (x - v0.x) + (v0.x - v2.x) * (y - v0.y)) / area;
                float lambda2 = ((v0.y - v1.y) * (x - v0.x) + (v1.x - v0.x) * (y - v0.y)) / area;
                float lambda0 = 1.0f - lambda1 - lambda2;
                
                // Check if point is inside triangle
                if (lambda0 >= 0 && lambda1 >= 0 && lambda2 >= 0) {
                    // Perspective-correct interpolation using reciprocal depth
                    float w_interp = lambda0 * w0 + lambda1 * w1 + lambda2 * w2;
                    float depth = 1.0f / w_interp;
                    
                    // Perspective-correct barycentric coordinates
                    // float a_correct = (lambda0 * w0) / w_interp;
                    float b_correct = (lambda1 * w1) / w_interp;
                    float c_correct = (lambda2 * w2) / w_interp;
                    
                    int screen_x = x + 1, screen_y = y + 1;
                    char pixel_char = f.ascii;
                    int pixel_color = f.color;
                    
                    // Perspective-correct texture mapping
                    if (f.texture && f.texture_width > 0 && f.texture_height > 0) {
                        // Use perspective-corrected coordinates for texture mapping
                        float u =  b_correct;
                        float v =  c_correct;
                        
                        // Clamp UV coordinates
                        u = fmax(0.0f, fmin(1.0f, u));
                        v = fmax(0.0f, fmin(1.0f, v));
                        
                        int tex_x = (int)(u * (f.texture_width ));
                        int tex_y = (int)(v * (f.texture_height ));
                        
                        // Bounds check for texture access
                        if (tex_x >= 0 && tex_x < f.texture_width && tex_y >= 0 && tex_y < f.texture_height) {
                            pixel_color = f.texture[tex_y * f.texture_width + tex_x];
                        }
                    }
                    set_pixel(screen_x, screen_y, pixel_char, pixel_color, depth);
                }
            }
        }
    } else if (f.vertex_count == 4) {
        // Treat quad as two separate triangles with proper UV mapping
        vertex v0 = projected[0], v1 = projected[1], v2 = projected[2], v3 = projected[3];
        
        // Store original world-space depths for perspective correction
        float w0 = 1.0f / v0.z, w1 = 1.0f / v1.z, w2 = 1.0f / v2.z, w3 = 1.0f / v3.z;
        
        // Process both triangles: (v0,v1,v2) and (v0,v2,v3)
        for (int triangle = 0; triangle < 2; triangle++) {
            vertex tri_v0, tri_v1, tri_v2;
            float tri_w0, tri_w1, tri_w2;
            float uv_coords[6]; // u0,v0, u1,v1, u2,v2
            
            if (triangle == 0) {
                // First triangle (v0, v1, v2) with UV: v0=(0,0), v1=(1,0), v2=(1,1)
                tri_v0 = v0; tri_v1 = v1; tri_v2 = v2;
                tri_w0 = w0; tri_w1 = w1; tri_w2 = w2;
                uv_coords[0] = 0.0f; uv_coords[1] = 0.0f; // v0 UV
                uv_coords[2] = 1.0f; uv_coords[3] = 0.0f; // v1 UV
                uv_coords[4] = 1.0f; uv_coords[5] = 1.0f; // v2 UV
            } else {
                // Second triangle (v0, v2, v3) with UV: v0=(0,0), v2=(1,1), v3=(0,1)
                tri_v0 = v0; tri_v1 = v2; tri_v2 = v3;
                tri_w0 = w0; tri_w1 = w2; tri_w2 = w3;
                uv_coords[0] = 0.0f; uv_coords[1] = 0.0f; // v0 UV
                uv_coords[2] = 1.0f; uv_coords[3] = 1.0f; // v2 UV
                uv_coords[4] = 0.0f; uv_coords[5] = 1.0f; // v3 UV
            }
            
            // Find bounding box for current triangle
            int min_x = (int)fmin(fmin(tri_v0.x, tri_v1.x), tri_v2.x);
            int max_x = (int)fmax(fmax(tri_v0.x, tri_v1.x), tri_v2.x);
            int min_y = (int)fmin(fmin(tri_v0.y, tri_v1.y), tri_v2.y);
            int max_y = (int)fmax(fmax(tri_v0.y, tri_v1.y), tri_v2.y);
            
            // Clamp to screen bounds
            min_x = fmax(0, min_x);
            max_x = fmin(cmd_buffer_width - 1, max_x);
            min_y = fmax(0, min_y);
            max_y = fmin(cmd_buffer_height - 1, max_y);
            
            // Precompute triangle area
            float area = (tri_v1.x - tri_v0.x) * (tri_v2.y - tri_v0.y) - (tri_v2.x - tri_v0.x) * (tri_v1.y - tri_v0.y);
            if (fabs(area) < 0.001f) continue; // Skip degenerate triangle
            
            // Rasterize triangle with perspective correction
            for (int y = min_y; y <= max_y; y++) {
                for (int x = min_x; x <= max_x; x++) {
                    float lambda1 = ((tri_v2.y - tri_v0.y) * (x - tri_v0.x) + (tri_v0.x - tri_v2.x) * (y - tri_v0.y)) / area;
                    float lambda2 = ((tri_v0.y - tri_v1.y) * (x - tri_v0.x) + (tri_v1.x - tri_v0.x) * (y - tri_v0.y)) / area;
                    float lambda0 = 1.0f - lambda1 - lambda2;
                    
                    if (lambda0 >= 0 && lambda1 >= 0 && lambda2 >= 0) {
                        float w_interp = lambda0 * tri_w0 + lambda1 * tri_w1 + lambda2 * tri_w2;
                        float depth = 1.0f / w_interp;
                        
                        // Perspective-correct UV interpolation
                        float w0_correct = (lambda0 * tri_w0) / w_interp;
                        float w1_correct = (lambda1 * tri_w1) / w_interp;
                        float w2_correct = (lambda2 * tri_w2) / w_interp;
                        
                        float u = w0_correct * uv_coords[0] + w1_correct * uv_coords[2] + w2_correct * uv_coords[4];
                        float v = w0_correct * uv_coords[1] + w1_correct * uv_coords[3] + w2_correct * uv_coords[5];
                        
                        int screen_x = x + 1, screen_y = y + 1;
                        char pixel_char = f.ascii;
                        int pixel_color = f.color;
                        
                        if (f.texture && f.texture_width > 0 && f.texture_height > 0) {
                            u = fmax(0.0f, fmin(1.0f, u));
                            v = fmax(0.0f, fmin(1.0f, v));
                            
                            int tex_x = (int)(u * f.texture_width);
                            int tex_y = (int)(v * f.texture_height);
                            
                            if (tex_x >= 0 && tex_x < f.texture_width && tex_y >= 0 && tex_y < f.texture_height) {
                                pixel_color = f.texture[tex_y * f.texture_width + tex_x];
                            }
                        }
                        set_pixel(screen_x, screen_y, pixel_char, pixel_color, depth);
                    }
                }
            }
        }
    }
}
