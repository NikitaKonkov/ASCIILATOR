#ifndef DOT_ANIMATION_H
#define DOT_ANIMATION_H
#include "../shader/SHADER.h"   
#include <math.h>

void dot_wave_grid(dot *d){
    // Wave parameters
    static float time = 0.0f;
    time += 0.025f; // Animation speed
    float wave_amplitude = 1.0f; // Height of the waves
    float wave_frequency = 0.1f; // Frequency of the waves
    float wave_speed = 2.0f; // Speed of wave propagation

    for (int i = 0; i < 256; i++) {
        for (int j = 0; j < 256; j++) {
            float x = 256 - i*2; // X-coordinate
            float z = 256 - j*2; // Z-coordinate
            
            // Create multiple overlapping waves for complex patterns
            float wave1 = sin((x * wave_frequency) + (time * wave_speed)) * wave_amplitude;
            float wave2 = sin((z * wave_frequency) + (time * wave_speed * 0.7f)) * wave_amplitude * 0.6f;
            float wave3 = sin(((x + z) * wave_frequency * 0.5f) + (time * wave_speed * 1.3f)) * wave_amplitude * 0.4f;
            
            // Combine waves for more complex patterns
            float y = wave1 + wave2 + wave3;
            
            // Add circular wave from center
            float center_x = 4.0f;
            float center_z = 4.0f;
            float distance_from_center = sqrt((x - center_x) * (x - center_x) + (z - center_z) * (z - center_z));
            float circular_wave = sin((distance_from_center * wave_frequency * 128.0f) - (time * wave_speed * 128.0f)) * wave_amplitude * 0.3f;
            
            y += circular_wave;
            
            d[i * 256 + j] = dot_shader((vertex){x, y, z});
        }
    }
}

void dot_wave_cube(dot *d) {
    // Wave parameters
    static float time = 0.0f;
    time += 0.05f; // Animation speed
    float wave_amplitude = 2.0f; // Height of the waves
    float wave_frequency = 0.3f; // Frequency of the waves
    float wave_speed = 3.0f; // Speed of wave propagation
    
    int dot_index = 0;
    int cube_size = 20; // From -10 to 10 (same as edge cube)
    int resolution = 10; // Increased density of dots on each face
    int max_dots = 64 * 64; // Maximum dots in the array
    
    // Generate dots for each face of the cube
    // Front face (z = 10) - wave should move in Z direction (forward/backward)
    for (int i = 0; i <= resolution && dot_index < max_dots; i++) {
        for (int j = 0; j <= resolution && dot_index < max_dots; j++) {
            float x = -10.0f + (20.0f * i / resolution);
            float y = -10.0f + (20.0f * j / resolution);
            float z = 10.0f;
            
            // Add wave animation based on position and time - move in Z direction
            float wave = sin((x * wave_frequency) + (y * wave_frequency) + (time * wave_speed)) * wave_amplitude;
            z += wave * 0.3f; // Apply wave to z coordinate for front face
            
            d[dot_index++] = dot_shader((vertex){x, y - 20, z});
        }
    }
    
    // Back face (z = -10) - wave should move in Z direction (forward/backward)
    for (int i = 0; i <= resolution && dot_index < max_dots; i++) {
        for (int j = 0; j <= resolution && dot_index < max_dots; j++) {
            float x = -10.0f + (20.0f * i / resolution);
            float y = -10.0f + (20.0f * j / resolution);
            float z = -10.0f;
            
            float wave = sin((x * wave_frequency) + (y * wave_frequency) + (time * wave_speed * 1.2f)) * wave_amplitude;
            z += wave * 0.3f; // Apply wave to z coordinate for back face
            
            d[dot_index++] = dot_shader((vertex){x, y - 20, z});
        }
    }
    
    // Left face (x = -10) - wave should move in X direction (left/right)
    for (int i = 0; i <= resolution && dot_index < max_dots; i++) {
        for (int j = 0; j <= resolution && dot_index < max_dots; j++) {
            float x = -10.0f;
            float y = -10.0f + (20.0f * i / resolution);
            float z = -10.0f + (20.0f * j / resolution);
            
            float wave = sin((y * wave_frequency) + (z * wave_frequency) + (time * wave_speed * 0.8f)) * wave_amplitude;
            x += wave * 0.3f; // Apply wave to x coordinate for left face
            
            d[dot_index++] = dot_shader((vertex){x, y - 20, z});
        }
    }
    
    // Right face (x = 10) - wave should move in X direction (left/right)
    for (int i = 0; i <= resolution && dot_index < max_dots; i++) {
        for (int j = 0; j <= resolution && dot_index < max_dots; j++) {
            float x = 10.0f;
            float y = -10.0f + (20.0f * i / resolution);
            float z = -10.0f + (20.0f * j / resolution);
            
            float wave = sin((y * wave_frequency) + (z * wave_frequency) + (time * wave_speed * 1.5f)) * wave_amplitude;
            x += wave * 0.3f; // Apply wave to x coordinate for right face
            
            d[dot_index++] = dot_shader((vertex){x, y - 20, z});
        }
    }
    
    // Top face (y = 10) - wave should move in Y direction (up/down)
    for (int i = 0; i <= resolution && dot_index < max_dots; i++) {
        for (int j = 0; j <= resolution && dot_index < max_dots; j++) {
            float x = -10.0f + (20.0f * i / resolution);
            float y = 10.0f;
            float z = -10.0f + (20.0f * j / resolution);
            
            float wave = sin((x * wave_frequency) + (z * wave_frequency) + (time * wave_speed * 0.6f)) * wave_amplitude;
            y += wave * 0.3f; // Apply wave to y coordinate for top face
            
            d[dot_index++] = dot_shader((vertex){x, y - 20, z});
        }
    }
    
    // Bottom face (y = -10) - wave should move in Y direction (up/down)
    for (int i = 0; i <= resolution && dot_index < max_dots; i++) {
        for (int j = 0; j <= resolution && dot_index < max_dots; j++) {
            float x = -10.0f + (20.0f * i / resolution);
            float y = -10.0f;
            float z = -10.0f + (20.0f * j / resolution);
            
            float wave = sin((x * wave_frequency) + (z * wave_frequency) + (time * wave_speed * 1.8f)) * wave_amplitude;
            y += wave * 0.3f; // Apply wave to y coordinate for bottom face
            
            d[dot_index++] = dot_shader((vertex){x, y - 20, z});
        }
    }
    
    // Fill remaining dots with empty/invalid dots
    for (int i = dot_index; i < max_dots; i++) {
        d[i] = dot_shader((vertex){0, 0, -1000}); // Far away so they don't render
    }
}




#endif // DOT_ANIMATION_H