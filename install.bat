@echo off


REM Compile the main executable with optimizations
gcc -O3 -march=native -mtune=native -funroll-loops -ffast-math -flto -o engine.exe engine/ENGINE.c engine/render/RENDER.c engine/camera/CAMERA.c engine/input/INPUT.c engine/shader/SHADER.c engine/rasterizer/RASTERIZER.c