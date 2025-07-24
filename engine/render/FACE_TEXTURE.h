#ifndef TEXTURE_H
#define TEXTURE_H


// Create a simple texture for faces (smiley face)
int smily_texture[] = {
    33, 33, 33, 33, 33, 33, 33, 33,  // Yellow border (top)
    33, 35, 37, 37, 37, 37, 32, 33,  // White face with yellow border
    33, 37, 31, 37, 37, 31, 37, 33,  // Eyes (red dots)
    33, 37, 37, 37, 37, 37, 37, 33,  // White face
    33, 37, 37, 37, 37, 37, 37, 33,  // White face
    33, 37, 31, 37, 37, 31, 37, 33,  // Smile corners (red)
    33, 34, 37, 31, 31, 37, 31, 33,  // Smile mouth (red)
    33, 33, 33, 33, 33, 33, 33, 33   // Yellow border (bottom)
};
#endif // TEXTURE_H