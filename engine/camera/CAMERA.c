#include <stdlib.h>
#include <math.h>
#include "windows.h"
#include "stdio.h"
#include <time.h>
#include "../render/RENDER.h"
#include "CAMERA.h"

// Start camera back from origin
camera3d camera = {100.0f, -2.5f, 100.0f, 0.0f, -1.5f}; 

// Aspect ratio correction for console character stretching
float aspect_ratio_width = 1.0f;  // Width scaling factor (1.0 = no scaling)
float aspect_ratio_height = 2.0f; // Height scaling factor (2.0 = compensate for tall characters)

// Culling distance for 3D objects
const float culling_distance = .5f; // Distance behind camera to start culling
const float view_distance = 1000.0f; // Maximum view distance - objects beyond this distance are culled

// Diagonal vector for camera movement (forward/backward)
float diagonal_x, diagonal_y, diagonal_z;

// Horizontal vector for camera movement (left/right)
float horizontal_x, horizontal_y, horizontal_z;

// Camera movement controls (WASD) - aligned with mouse look direction
float camera_speed = 0.1f;

// Camera turning speed (Q/E for left/right, R/F for up/down)
float camera_turn_speed = 0.1f;


camera_cache cached_transform = {0};

// Update camera transformation cache
void update_camera_cache() {
    cached_transform.cam_x = camera.x;
    cached_transform.cam_y = camera.y;
    cached_transform.cam_z = camera.z;
    cached_transform.cam_yaw = camera.yaw;
    cached_transform.cam_pitch = camera.pitch;
    cached_transform.cos_yaw = cos(-camera.yaw);
    cached_transform.sin_yaw = sin(-camera.yaw);
    cached_transform.cos_pitch = cos(-camera.pitch);
    cached_transform.sin_pitch = sin(-camera.pitch);
    cached_transform.valid = 1;
}

// Check if camera cache is valid
int is_camera_cache_valid() {
    return cached_transform.valid &&
           cached_transform.cam_x == camera.x &&
           cached_transform.cam_y == camera.y &&
           cached_transform.cam_z == camera.z &&
           cached_transform.cam_yaw == camera.yaw &&
           cached_transform.cam_pitch == camera.pitch;
}


void camera_update() {
    // Calculate movement directions based on camera orientation (mouse look direction)
    // Forward direction: exactly where the camera is looking (includes pitch)
    float cos_yaw = cos(camera.yaw);
    float sin_yaw = sin(camera.yaw);
    float cos_pitch = cos(camera.pitch);
    float sin_pitch = sin(camera.pitch);
    
    // Forward vector: direction the camera is actually looking (with pitch)
    // Fixed: Match the camera view matrix coordinate system (uses -camera.yaw)
    diagonal_x = -sin_yaw * cos_pitch;  // Negative sin to match view matrix
    diagonal_y = -sin_pitch; // Negative because we want W to move toward where we're looking
    diagonal_z = cos_yaw * cos_pitch;   // Positive cos to match view matrix
    
    // Right vector: perpendicular to forward, always horizontal (no pitch component)
    // Fixed: Use standard 3D camera right vector calculation
    horizontal_x = cos_yaw;    // Keep positive cos for right direction
    horizontal_y = 0.0f;       // Keep horizontal strafe movement
    horizontal_z = sin_yaw;    // Positive sin for correct right direction
    
    
    // Clamp pitch to prevent camera flipping
    const float MAX_PITCH = 1.5f; // About 85 degrees
    if (camera.pitch > MAX_PITCH) camera.pitch = MAX_PITCH;
    if (camera.pitch < -MAX_PITCH) camera.pitch = -MAX_PITCH;
}
