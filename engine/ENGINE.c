#include "windows.h"
#include "stdio.h"
#include "render/RENDER.h"


int main(int argc, char const *argv[])
{
    DWORD lastTime = GetTickCount();
    DWORD frameCount = 0;
    float fps = 0.0f;
    int render_mode = 0; // 0 = ANSI pattern, 1 = 3D cube

    while (1)
    {
        DWORD currentTime = GetTickCount();
        frameCount++;
        
        // Calculate FPS every second
        if (currentTime - lastTime >= 250) {
            fps = frameCount * 250.0f / (currentTime - lastTime);
            frameCount = 0;
            lastTime = currentTime;
        }
        
        
        // Check for exit (Escape key)
        if (GetAsyncKeyState(VK_ESCAPE) & 0x8000) {
            break;
        }
        
        // output the frame buffer
        output_buffer();

        // Display the engine ticks
        printf("\x1B[2H%.2f Engine Ticks", fps);

    }
    return 0;
}
