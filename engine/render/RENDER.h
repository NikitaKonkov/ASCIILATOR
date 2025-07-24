#ifndef RENDER_H
#define RENDER_H


// Saves current console size to avoid infinite scrolling
extern unsigned int save_console_width;
extern unsigned int save_console_height;

// Buffers for drawing - New frame buffer system
static char frame_buffer[2560*2560];     // Main frame buffer for ANSI codes
static float depth_buffer[2560*2560];    // Depth buffer for Z-testing
static int buffer_width, buffer_height;  // Buffer dimensions


// Function declarations
void output_buffer();
void geometry_draw();
#endif // RENDER_H