#ifndef TEXTURE_H
#define TEXTURE_H


// Create a simple texture for faces (heart shape)
int heart_texture[] = {
    37, 37, 37, 37, 37, 37, 37, 37, 37, 37, 37, 37, 37, 37, 37, 37,  // White background (top)
    37, 37, 37, 37, 37, 37, 37, 37, 37, 37, 37, 37, 37, 37, 37, 37,  // White background
    37, 37, 31, 31, 31, 37, 37, 37, 37, 37, 37, 31, 31, 31, 37, 37,  // Heart top bumps (red)
    37, 31, 31, 31, 31, 31, 37, 37, 37, 37, 31, 31, 31, 31, 31, 37,  // Heart top bumps wider
    31, 31, 31, 31, 31, 31, 31, 37, 37, 31, 31, 31, 31, 31, 31, 31,  // Heart top connecting
    31, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31,  // Full red heart top
    31, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31,  // Full red heart
    31, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31,  // Full red heart
    37, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31, 37,  // Heart sides tapering
    37, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31, 37,  // Heart sides tapering
    37, 37, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31, 37, 37,  // Heart narrowing
    37, 37, 37, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31, 37, 37, 37,  // Heart narrowing more
    37, 37, 37, 37, 31, 31, 31, 31, 31, 31, 31, 31, 37, 37, 37, 37,  // Heart bottom narrowing
    37, 37, 37, 37, 37, 31, 31, 31, 31, 31, 31, 37, 37, 37, 37, 37,  // Heart bottom point
    37, 37, 37, 37, 37, 37, 31, 31, 31, 31, 37, 37, 37, 37, 37, 37,  // Heart tip
    37, 37, 37, 37, 37, 37, 37, 37, 37, 37, 37, 37, 37, 37, 37, 37   // White background (bottom)
};

// Red and dark gray checkerboard texture (16x16)
int checkerboard_texture[] = {
    31, 90, 31, 90, 31, 90, 31, 90, 31, 90, 31, 90, 31, 90, 31, 90,  // Row 1: Red, Dark Gray alternating
    90, 31, 90, 31, 90, 31, 90, 31, 90, 31, 90, 31, 90, 31, 90, 31,  // Row 2: Dark Gray, Red alternating
    31, 90, 31, 90, 31, 90, 31, 90, 31, 90, 31, 90, 31, 90, 31, 90,  // Row 3: Red, Dark Gray alternating
    90, 31, 90, 31, 90, 31, 90, 31, 90, 31, 90, 31, 90, 31, 90, 31,  // Row 4: Dark Gray, Red alternating
    31, 90, 31, 90, 31, 90, 31, 90, 31, 90, 31, 90, 31, 90, 31, 90,  // Row 5: Red, Dark Gray alternating
    90, 31, 90, 31, 90, 31, 90, 31, 90, 31, 90, 31, 90, 31, 90, 31,  // Row 6: Dark Gray, Red alternating
    31, 90, 31, 90, 31, 90, 31, 90, 31, 90, 31, 90, 31, 90, 31, 90,  // Row 7: Red, Dark Gray alternating
    90, 31, 90, 31, 90, 31, 90, 31, 90, 31, 90, 31, 90, 31, 90, 31,  // Row 8: Dark Gray, Red alternating
    31, 90, 31, 90, 31, 90, 31, 90, 31, 90, 31, 90, 31, 90, 31, 90,  // Row 9: Red, Dark Gray alternating
    90, 31, 90, 31, 90, 31, 90, 31, 90, 31, 90, 31, 90, 31, 90, 31,  // Row 10: Dark Gray, Red alternating
    31, 90, 31, 90, 31, 90, 31, 90, 31, 90, 31, 90, 31, 90, 31, 90,  // Row 11: Red, Dark Gray alternating
    90, 31, 90, 31, 90, 31, 90, 31, 90, 31, 90, 31, 90, 31, 90, 31,  // Row 12: Dark Gray, Red alternating
    31, 90, 31, 90, 31, 90, 31, 90, 31, 90, 31, 90, 31, 90, 31, 90,  // Row 13: Red, Dark Gray alternating
    90, 31, 90, 31, 90, 31, 90, 31, 90, 31, 90, 31, 90, 31, 90, 31,  // Row 14: Dark Gray, Red alternating
    31, 90, 31, 90, 31, 90, 31, 90, 31, 90, 31, 90, 31, 90, 31, 90,  // Row 15: Red, Dark Gray alternating
    90, 31, 90, 31, 90, 31, 90, 31, 90, 31, 90, 31, 90, 31, 90, 31   // Row 16: Dark Gray, Red alternating
};
#endif // TEXTURE_H