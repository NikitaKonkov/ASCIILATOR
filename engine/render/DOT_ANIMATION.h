#ifndef DOT_ANIMATION_H
#define DOT_ANIMATION_H
#include "../shader/SHADER.h"   
#include <math.h>

void dot_wave_grid(dot *d){
    // Wave parameters
    static float time = 0.0f;
    time += 0.025f; // Animation speed
    float wave_amplitude = 1.0f; // Height of the waves
    float wave_frequency = 0.8f; // Frequency of the waves
    float wave_speed = 2.0f; // Speed of wave propagation

    for (int i = 0; i < 256; i++) {
        for (int j = 0; j < 256; j++) {
            float x = i*2; // X-coordinate
            float z = j*2; // Z-coordinate
            
            // Create multiple overlapping waves for complex patterns
            float wave1 = sin((x * wave_frequency) + (time * wave_speed)) * wave_amplitude;
            float wave2 = sin((z * wave_frequency) + (time * wave_speed * 0.7f)) * wave_amplitude * 0.6f;
            float wave3 = sin(((x + z) * wave_frequency * 0.5f) + (time * wave_speed * 1.3f)) * wave_amplitude * 0.4f;
            
            // Combine waves for more complex patterns
            float y = wave1 + wave2 + wave3;
            
            // Add circular wave from center
            float center_x = 128.0f;
            float center_z = 128.0f;
            float distance_from_center = sqrt((x - center_x) * (x - center_x) + (z - center_z) * (z - center_z));
            float circular_wave = sin((distance_from_center * wave_frequency * 2.0f) - (time * wave_speed * 2.0f)) * wave_amplitude * 0.3f;
            
            y += circular_wave;
            
            d[i * 256 + j] = dot_shader((vertex){x, y, z});
        }
    }
}




#endif // DOT_ANIMATION_H