@echo off


REM Compile the main executable
gcc -O3 -o engine.exe engine/ENGINE.c engine/render/RENDER.c engine/camera/CAMERA.c engine/input/INPUT.c engine/shader/SHADER.c engine/rasterizer/RASTERIZER.c