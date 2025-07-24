#ifndef CAMERA_H
#define CAMERA_H

// 3D Camera system
typedef struct {
    float x, y, z;        // Camera position
    float yaw, pitch;     // Camera rotation (yaw = left/right, pitch = up/down)
} camera3d;

// Start camera back from origin
extern camera3d camera; 

// Aspect ratio correction for console character stretching
extern float aspect_ratio_width;  // Width scaling factor (1.0 = no scaling)
extern float aspect_ratio_height; // Height scaling factor (2.0 = compensate for tall characters)

// Culling distance for 3D objects
extern const float culling_distance; // Distance behind camera to start culling
extern const float view_distance; // Maximum view distance - objects beyond this distance are culled

// Diagonal vector for camera movement (forward/backward)
extern float diagonal_x, diagonal_y, diagonal_z;

// Horizontal vector for camera movement (left/right)
extern float horizontal_x, horizontal_y, horizontal_z;

// Camera movement controls (WASD) - aligned with mouse look direction
extern float camera_speed;

// Camera turning speed (Q/E for left/right, R/F for up/down)
extern float camera_turn_speed;

// Camera transformation caching for optimization
typedef struct {
    float cos_yaw, sin_yaw, cos_pitch, sin_pitch;
    float cam_x, cam_y, cam_z, cam_yaw, cam_pitch;
    int valid;
} camera_cache;

extern camera_cache cached_transform;

// Update camera transformation cache
void update_camera_cache();
// Check if camera cache is valid
int is_camera_cache_valid();
// Update camera position and rotation based on input
void camera_update();

#endif // CAMERA_H