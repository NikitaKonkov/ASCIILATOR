#ifndef DOT_ANIMATION_H
#define DOT_ANIMATION_H
#include "../shader/SHADER.h"   
#include <math.h>

void dot_wave_grid(dot *d){
    // Wave parameters
    static float time = 0.0f;
    time += 0.02f; // Slower animation speed to reduce vibration
    float wave_amplitude = 8.0f; // Larger wave height
    float wave_frequency = 0.005f; // Much lower frequency for smoother large waves
    float wave_speed = 1.5f; // Slower wave propagation

    for (int i = 0; i < 256; i++) {
        for (int j = 0; j < 256; j++) {
            float x = 100 + (i - 128) * 5; // X-coordinate centered around camera position (100)
            float z = 100 + (j - 128) * 5; // Z-coordinate centered around camera position (100)
            
            // Create multiple overlapping waves for complex patterns
            float wave1 = sin((x * wave_frequency) + (time * wave_speed)) * wave_amplitude;
            float wave2 = sin((z * wave_frequency) + (time * wave_speed * 0.8f)) * wave_amplitude * 0.7f;
            float wave3 = sin(((x + z) * wave_frequency * 0.3f) + (time * wave_speed * 1.1f)) * wave_amplitude * 0.5f;
            
            // Combine waves for more complex patterns
            float y = wave1 + wave2 + wave3;
            
            // Add circular wave from center (centered at camera position)
            float center_x = 100.0f; // Center at camera X position
            float center_z = 100.0f; // Center at camera Z position
            float distance_from_center = sqrt((x - center_x) * (x - center_x) + (z - center_z) * (z - center_z));
            float circular_wave = sin((distance_from_center * wave_frequency * 2.0f) - (time * wave_speed * 2.0f)) * wave_amplitude * 0.4f;
            
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
    int resolution = 10; // Density of dots on each face
    int max_dots = 64 * 64; // Maximum dots in the array
    
    // Face data: [base_x, base_y, base_z, axis_to_wave (0=x,1=y,2=z), time_multiplier]
    float face_data[6][5] = {
        {-10.0f, -10.0f,  10.0f, 2, 1.0f},  // Front face (wave Z)
        {-10.0f, -10.0f, -10.0f, 2, 1.2f},  // Back face (wave Z)
        {-10.0f, -10.0f, -10.0f, 0, 0.8f},  // Left face (wave X)
        { 10.0f, -10.0f, -10.0f, 0, 1.5f},  // Right face (wave X)
        {-10.0f,  10.0f, -10.0f, 1, 0.6f},  // Top face (wave Y)
        {-10.0f, -10.0f, -10.0f, 1, 1.8f}   // Bottom face (wave Y)
    };
    
    // Which coordinates use i/j for each face: [x_uses_i, y_uses_i, z_uses_i, x_uses_j, y_uses_j, z_uses_j]
    int coord_mapping[6][6] = {
        {1, 0, 0, 0, 1, 0},  // Front: x=i, y=j, z=fixed
        {1, 0, 0, 0, 1, 0},  // Back: x=i, y=j, z=fixed
        {0, 1, 0, 0, 0, 1},  // Left: x=fixed, y=i, z=j
        {0, 1, 0, 0, 0, 1},  // Right: x=fixed, y=i, z=j
        {1, 0, 0, 0, 0, 1},  // Top: x=i, y=fixed, z=j
        {1, 0, 0, 0, 0, 1}   // Bottom: x=i, y=fixed, z=j
    };
    
    // Generate all cube faces using data-driven approach
    for (int face = 0; face < 6 && dot_index < max_dots; face++) {
        for (int i = 0; i <= resolution && dot_index < max_dots; i++) {
            for (int j = 0; j <= resolution && dot_index < max_dots; j++) {
                float i_val = 20.0f * i / resolution;
                float j_val = 20.0f * j / resolution;
                
                // Calculate position based on mapping
                float x = face_data[face][0] + (coord_mapping[face][0] * i_val) + (coord_mapping[face][3] * j_val);
                float y = face_data[face][1] + (coord_mapping[face][1] * i_val) + (coord_mapping[face][4] * j_val);
                float z = face_data[face][2] + (coord_mapping[face][2] * i_val) + (coord_mapping[face][5] * j_val);
                
                // Calculate wave based on axis
                float wave_input = (face_data[face][3] == 0) ? (y * wave_frequency) + (z * wave_frequency) :
                                  (face_data[face][3] == 1) ? (x * wave_frequency) + (z * wave_frequency) :
                                                              (x * wave_frequency) + (y * wave_frequency);
                                                              
                float wave = sin(wave_input + (time * wave_speed * face_data[face][4])) * wave_amplitude * 0.3f;
                
                // Apply wave to correct axis
                if (face_data[face][3] == 0) x += wave;
                else if (face_data[face][3] == 1) y += wave;
                else z += wave;
                
                d[dot_index++] = dot_shader((vertex){x + 60.0f, y - 20, z}); // Offset to the far right
            }
        }
    }
    
    // Fill remaining dots with empty/invalid dots
    for (int i = dot_index; i < max_dots; i++) {
        d[i] = dot_shader((vertex){0, 0, -1000}); // Far away so they don't render
    }
}




#endif // DOT_ANIMATION_H