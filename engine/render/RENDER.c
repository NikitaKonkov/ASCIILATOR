// ANSI color palette (standard 16 colors)
// BLACK RED GRENN YELLOW BLUE MAGENTA CYAN GRAY 
// 30    31  32    33     34   35      36   37  DARKER
// 90    91  92    93     94   95      96   97  BRIGHTER

// BACKGROUND ANSI color palette (standard 16 colors)
// BLACK RED GRENN YELLOW BLUE MAGENTA CYAN GRAY 
// 40    41  42    43     44   45      46   47  DARKER
// 100   101 102   103    104  105     106  107 BRIGHTER

// FHD [1888 x 532]  [17.8 944 x 355]  [29.6 944 x 213] 
// 2k  [2528 x 712]  
// 4k  [3800 x 1070] 
#include <stdlib.h>
#include "windows.h"
#include "stdio.h"
#include <time.h>
#include "RENDER.h"
#include "../camera/CAMERA.h"
#include "../input/INPUT.h"
#include "../shader/SHADER.h"
#include "../rasterizer/RASTERIZER.h"



// Saves current console size to avoid infinite scrolling
unsigned int save_console_width = 0;
unsigned int save_console_height = 0;


// Initialize the rendering system (call once at startup)
void init_rendering_system() {
    // Clear screen once at startup
    system("cls");
    
    // Initialize buffers
    for (int y = 0; y < 2560; y++) {
        for (int x = 0; x < 2560; x++) {
            screen_buffer[y][x].valid = 0;
            screen_buffer[y][x].depth = 2560.0f;
            previous_screen_buffer[y][x].valid = 0;
            previous_screen_buffer[y][x].depth = 2560.0f;
        }
    }
}

// Initialize the new frame buffer system
void init_frame_buffer() {
    screen_width = cmd_buffer_width;
    screen_height = cmd_buffer_height;
    
    // Copy current screen buffer to previous buffer
    for (int y = 0; y < screen_height && y < 2560; y++) {
        for (int x = 0; x < screen_width && x < 2560; x++) {
            previous_screen_buffer[y][x] = screen_buffer[y][x];
        }
    }
    
    // Clear the current screen buffer
    for (int y = 0; y < screen_height && y < 2560; y++) {
        for (int x = 0; x < screen_width && x < 2560; x++) {
            screen_buffer[y][x].valid = 0;
            screen_buffer[y][x].depth = 2560.0f; // Far depth
        }
    }
    
    frame_buffer_pos = 0;
}

// Render the frame buffer to screen with differential updates
void render_frame_buffer() {
    frame_buffer_pos = 0;
    
    // Build output string with only changed pixels
    for (int y = 0; y < screen_height && y < 2560; y++) {
        for (int x = 0; x < screen_width && x < 2560; x++) {
            pixel current = screen_buffer[y][x];
            pixel previous = previous_screen_buffer[y][x];
            
            // Check if pixel has changed
            int pixel_changed = (current.valid != previous.valid) ||
                               (current.valid && (current.ascii != previous.ascii || 
                                                current.color != previous.color));
            
            if (pixel_changed) {
                if (frame_buffer_pos < sizeof(frame_buffer) - 50) {
                    if (current.valid) {
                        // Draw new pixel
                        frame_buffer_pos += snprintf(&frame_buffer[frame_buffer_pos], 
                            sizeof(frame_buffer) - frame_buffer_pos,
                            "\x1b[%d;%dH\x1b[%dm%c", 
                            y + 1, x + 1, 
                            current.color, 
                            current.ascii);
                    } else {
                        // Clear pixel (was valid before, now invalid)
                        frame_buffer_pos += snprintf(&frame_buffer[frame_buffer_pos], 
                            sizeof(frame_buffer) - frame_buffer_pos,
                            "\x1b[%d;%dH ", 
                            y + 1, x + 1);
                    }
                }
            }
        }
    }
    
    // Null terminate and output only if there are changes
    if (frame_buffer_pos > 0) {
        frame_buffer[frame_buffer_pos] = '\0';
        printf("%s", frame_buffer);
    }
}


void cmd_init() {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    GetConsoleScreenBufferInfo(hConsole, &csbi);
    
    cmd_buffer_width  = csbi.dwSize.X;
    cmd_buffer_height = csbi.dwSize.Y;
    
    // Check if the console buffer size matches the desired dimensions if console is resized
    if (cmd_buffer_width != save_console_width || cmd_buffer_height != save_console_height) {
        // Resize console buffer if necessary
        system("cls"); // Clear the console
    }
    
    // Save current console size, to avoid infinite scrolling
    save_console_width = cmd_buffer_width;
    save_console_height = cmd_buffer_height;

}

void clock_tick() {

    static clock_t last_tick = 0;
    clock_t current_tick = clock();

    // Check if 16ms (CLOCKS_PER_SEC / 62.5) have passed since the last tick
    if ((current_tick - last_tick) < (CLOCKS_PER_SEC / 62.5)) {
        return; // Skip this tick
    }
    geometry_draw(); // Draw the test mesh with horizontal line optimization
    keyboard_input(); // Handle keyboard input for object and camera movement

    if (!is_camera_cache_valid()) update_camera_cache(); // Update camera cache if necessary
    
    last_tick = current_tick; // Update the last tick time
}

void debug_output() {
    // Corner markers for the console buffer
    printf("\x1B[31m\x1b[0H#\x1b[0;%dH#\x1b[%d;%dH#\x1b[%d;0H#", cmd_buffer_width, cmd_buffer_height, cmd_buffer_width, cmd_buffer_height);

    // Display buffer dimensions and edge markers
    printf("\x1B[31m\x1b[3H%dx%d", cmd_buffer_width, cmd_buffer_height);

    // Display camera position and orientation
    printf("\x1B[31m\x1b[4HCam: X:%.1f Y:%.1f Z:%.1f Yaw:%.2f Pitch:%.2f", camera.x, camera.y, camera.z, camera.yaw, camera.pitch);
    
}

void output_buffer() { // CPU based output

    clock_tick(); // Handle clock tick for frame rate control first

    cmd_init(); // Prepare the console for rendering

    debug_output(); // Output debug information

    camera_update(); // Update camera movement based on current orientation

    mouse_input(); // Handle mouse input for camera rotation
    
}

// New simplified unified drawing function using frame buffer
void draw_unified(edge *edges, int edge_count, dot *dots, int dot_count, face *faces, int face_count) {
    // Initialize frame buffer for this frame
    init_frame_buffer();
    
    // Draw all faces first (back to front)
    for (int i = 0; i < face_count; i++) {
        draw_face(faces[i]);
    }
    
    // Draw all edges
    for (int i = 0; i < edge_count; i++) {
        draw_edge(edges[i]);
    }
    
    // Draw all dots
    for (int i = 0; i < dot_count; i++) {
        draw_dot(dots[i]);
    }
    
    // Render the complete frame to screen
    render_frame_buffer();
}

// Test function to draw a complex shapes
void geometry_draw() {
    // Initialize rendering system on first call
    static int first_call = 1;
    if (first_call) {
        init_rendering_system();
        first_call = 0;
    }
    


    // Cube edges
    edge test_edges[] = {
        // Top square
        create_edge_with_shader((vertex){10.f, 10.f, 10.f},   (vertex){-10.f, 10.f, 10.f}),  // Top edge
        create_edge_with_shader((vertex){-10.f, 10.f, 10.f},  (vertex){-10.f, 10.f, -10.f}), // Left edge
        create_edge_with_shader((vertex){-10.f, 10.f, -10.f}, (vertex){10.f, 10.f, -10.f}), // Bottom edge
        create_edge_with_shader((vertex){10.f, 10.f, -10.f},  (vertex){10.f, 10.f, 10.f}),   // Right edge
        create_edge_with_shader((vertex){10.f, -10.f, 10.f},  (vertex){-10.f, -10.f, 10.f}),   // Top edge
        create_edge_with_shader((vertex){-10.f, -10.f, 10.f}, (vertex){-10.f, -10.f, -10.f}),   // Left edge
        create_edge_with_shader((vertex){-10.f, -10.f, -10.f},(vertex){10.f, -10.f, -10.f}),   // Bottom edge
        create_edge_with_shader((vertex){10.f, -10.f, -10.f}, (vertex){10.f, -10.f, 10.f}),    // Right edge
        create_edge_with_shader((vertex){10.f, 10.f, 10.f},   (vertex){10.f, -10.f, 10.f}),   // Front right vertical edge
        create_edge_with_shader((vertex){-10.f, 10.f, 10.f},  (vertex){-10.f, -10.f, 10.f}),   // Front left vertical edge
        create_edge_with_shader((vertex){-10.f, 10.f, -10.f}, (vertex){-10.f, -10.f, -10.f}),   // Back left vertical edge
        create_edge_with_shader((vertex){10.f, 10.f, -10.f},  (vertex){10.f, -10.f, -10.f}),    // Back right vertical edge
    };

    // Create a set of test dots with wave algorithm
    dot test_dots[256 * 256];
    
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
            
            test_dots[i * 256 + j] = dot_shader((vertex){x, y, z});
        }
    }
    
    // Calculate number of edges and dots
    int num_edges = sizeof(test_edges) / sizeof(test_edges[0]);
    int num_dots = sizeof(test_dots) / sizeof(test_dots[0]);

    // Create a simple texture for faces (smiley face)
    static int simple_texture[] = {
                                    33, 33, 33, 33, 33, 33, 33, 33,  // Yellow border (top)
                                    33, 35, 37, 37, 37, 37, 32, 33,  // White face with yellow border
                                    33, 37, 31, 37, 37, 31, 37, 33,  // Eyes (red dots)
                                    33, 37, 37, 37, 37, 37, 37, 33,  // White face
                                    33, 37, 37, 37, 37, 37, 37, 33,  // White face
                                    33, 37, 31, 37, 37, 31, 37, 33,  // Smile corners (red)
                                    33, 34, 37, 31, 31, 37, 31, 33,  // Smile mouth (red)
                                    33, 33, 33, 33, 33, 33, 33, 33   // Yellow border (bottom)
                                };

    // Create face examples
    face test_faces[2];
    
    // Triangle face example
    test_faces[0].vertex_count = 3;
    test_faces[0].vertices[0] = (vertex){.0f, .0f, .0f};
    test_faces[0].vertices[1] = (vertex){-50.0f, .0f, .0f};
    test_faces[0].vertices[2] = (vertex){.0f, 50.0f, 0.0f};
    test_faces[0].texture = simple_texture;
    test_faces[0].texture_width = 8;
    test_faces[0].texture_height = 8;
    test_faces[0].color = 93; // Bright yellow fallback
    test_faces[0].ascii = '#';
    
    // Quad face example
    test_faces[1].vertex_count = 4;
    test_faces[1].vertices[0] = (vertex){30.0f, 10.0f, 60.0f};
    test_faces[1].vertices[1] = (vertex){20.0f, 10.0f, 50.0f};
    test_faces[1].vertices[2] = (vertex){30.0f, 30.0f, 60.0f};
    test_faces[1].vertices[3] = (vertex){30.0f, 20.0f, 40.0f};
    test_faces[1].texture = simple_texture;
    test_faces[1].texture_width = 8;
    test_faces[1].texture_height = 8;
    test_faces[1].color = 96; // Bright cyan fallback
    test_faces[1].ascii = '@';
    
    int num_faces = sizeof(test_faces) / sizeof(test_faces[0]);
    
    // Use unified rendering with new frame buffer system for proper depth sorting
    draw_unified(test_edges, num_edges, test_dots, num_dots, test_faces, num_faces);
}