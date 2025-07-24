#ifndef INPUT_H
#define INPUT_H

// Lock the mouse to the center of the screen
extern const int center_x;
extern const int center_y;

// Mouse sensitivity for camera rotation
extern const float mouse_sensitivity;

void keyboard_input();
void mouse_input();

#endif // INPUT_H