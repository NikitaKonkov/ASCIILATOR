#include "INPUT.h"
#include "../camera/CAMERA.h"
#include <windows.h>

// Mouse look variables
static int mouse_last_x = 0;
static int mouse_last_y = 0;
static int mouse_initialized = 0;

// Lock the mouse to the center of the screen
const int mouse_cursour_x = 200; // Replace with your desired center X coordinate
const int mouse_cursour_y = 200;  // Replace with your desired center Y coordinate

// Mouse sensitivity for camera rotation
const float mouse_sensitivity = 0.005f;

// Mouse input handling for camera rotation
void mouse_input() {
    // Camera rotation controls (Mouse look)
    POINT mouse_pos;
    GetCursorPos(&mouse_pos);

    if (!mouse_initialized) {
        // Initialize the mouse position
        mouse_last_x = mouse_cursour_x;
        mouse_last_y = mouse_cursour_y;
        SetCursorPos(mouse_cursour_x, mouse_cursour_y);
        mouse_initialized = 1;
    }

    // Calculate mouse movement delta
    int mouse_delta_x = mouse_pos.x - mouse_last_x;
    int mouse_delta_y = mouse_pos.y - mouse_last_y;

    // Update camera rotation based on mouse movement
    if (mouse_delta_x || mouse_delta_y) {
        camera.yaw -= mouse_delta_x * mouse_sensitivity;   // Horizontal mouse movement -> yaw
        camera.pitch -= mouse_delta_y * mouse_sensitivity; // Vertical mouse movement -> pitch
    }

    // Clamp pitch to prevent flipping
    const float MAX_PITCH = 1.5f; // About 85 degrees
    if (camera.pitch > MAX_PITCH) camera.pitch = MAX_PITCH;
    if (camera.pitch < -MAX_PITCH) camera.pitch = -MAX_PITCH;

    // Reset the mouse position to the center
    SetCursorPos(mouse_cursour_x, mouse_cursour_y);
}


// Keyboard input handling for object and camera movement
void keyboard_input() {
    if(GetAsyncKeyState(VK_SHIFT) & 0x8000) {
        // If Shift is held, increase camera speed
        camera_speed = 2.f ;
        camera_turn_speed = 0.2f;
    } else {
        // Otherwise, use normal speed
        camera_speed = 0.1f;
        camera_turn_speed = 0.1f;
    }
    if(GetAsyncKeyState('W') & 0x8000) { // Move forward (exactly where camera is looking)
        camera.x += diagonal_x * camera_speed;
        camera.y += diagonal_y * camera_speed;
        camera.z += diagonal_z * camera_speed;
    }
    if(GetAsyncKeyState('S') & 0x8000) { // Move backward (opposite to where camera is looking)
        camera.x -= diagonal_x * camera_speed;
        camera.y -= diagonal_y * camera_speed;
        camera.z -= diagonal_z * camera_speed;
    }
    if(GetAsyncKeyState('A') & 0x8000) { // Strafe left (perpendicular to look direction)
        camera.x -= horizontal_x * camera_speed;
        camera.y -= horizontal_y * camera_speed;
        camera.z -= horizontal_z * camera_speed;
    }
    if(GetAsyncKeyState('D') & 0x8000) { // Strafe right (perpendicular to look direction)
        camera.x += horizontal_x * camera_speed;
        camera.y += horizontal_y * camera_speed;
        camera.z += horizontal_z * camera_speed;
    }
    if(GetAsyncKeyState('Q') & 0x8000) { // Turn left
        camera.yaw += camera_turn_speed;
    }
    if(GetAsyncKeyState('E') & 0x8000) { // Turn right
        camera.yaw -= camera_turn_speed;
    }
    if(GetAsyncKeyState('R') & 0x8000) { // Look up
        camera.pitch += camera_turn_speed;
    }
    if(GetAsyncKeyState('F') & 0x8000) { // Look down
        camera.pitch -= camera_turn_speed;
    }
    if(GetAsyncKeyState(VK_SPACE) & 0x8000) { // Move up
        camera.y -= camera_speed; // Move up in Y direction
    }
    if(GetAsyncKeyState(VK_CONTROL) & 0x8000) { // Move down
        camera.y += camera_speed; // Move down in Y direction
    }
}


